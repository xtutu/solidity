/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0

#include <libevmasm/Ethdebug.h>

using namespace solidity;
using namespace solidity::evmasm;
using namespace solidity::evmasm::ethdebug;

namespace
{

Json programInstructions(Assembly const* _assembly, LinkerObject const* _linkerObject, unsigned _sourceId)
{
	// e.g. interfaces don't have a valid assembly object.
	if (_assembly)
	{
		solUnimplementedAssert(_assembly->eofVersion() == std::nullopt, "ethdebug does not yet support EOF.");
		solUnimplementedAssert(_assembly->codeSections().size() == 1, "ethdebug does not yet support multiple code-sections.");
	}

	solAssert(_linkerObject->offsetAfterLastInstruction <= _linkerObject->bytecode.size());
	Json instructions = Json::array();
	for (size_t i = 0; i < _linkerObject->instructionOffsets.size(); ++i)
	{
		solAssert(_assembly);
		size_t offset = _linkerObject->instructionOffsets[i].offset;
		size_t instructionIndex = _linkerObject->instructionOffsets[i].index;
		size_t nextOffset = offset;
		if (i + 1 < _linkerObject->instructionOffsets.size())
			nextOffset = _linkerObject->instructionOffsets[i + 1].offset;
		if (i == _linkerObject->instructionOffsets.size() - 1)
			nextOffset = _linkerObject->offsetAfterLastInstruction;
		solAssert(offset < _linkerObject->bytecode.size());
		solAssert(nextOffset <= _linkerObject->bytecode.size());
		solAssert(offset < nextOffset);
		Json operation = Json::object();
		operation["mnemonic"] = instructionInfo(static_cast<Instruction>(_linkerObject->bytecode[offset]), _assembly->evmVersion()).name;
		static size_t constexpr instructionSize = 1;
		bytes const argumentData(
			_linkerObject->bytecode.begin() + static_cast<std::ptrdiff_t>(offset) + instructionSize,
			_linkerObject->bytecode.begin() + static_cast<std::ptrdiff_t>(nextOffset)
		);
		if (!argumentData.empty())
			operation["arguments"] = Json::array({ "0x" + util::toHex(argumentData) });
		solAssert(instructionIndex < _assembly->codeSections().at(0).items.size());
		langutil::SourceLocation const& location = _assembly->codeSections().at(0).items.at(instructionIndex).location();
		Json instruction = Json::object();
		instruction["offset"] = offset;
		instruction["operation"] = operation;

		instruction["context"] = Json::object();
		instruction["context"]["code"] = Json::object();
		instruction["context"]["code"]["source"] = Json::object();
		instruction["context"]["code"]["source"]["id"] = static_cast<int>(_sourceId);

		instruction["context"]["code"]["range"] = Json::object();
		instruction["context"]["code"]["range"]["offset"] = location.start;
		instruction["context"]["code"]["range"]["length"] = location.end - location.start;
		instructions.emplace_back(instruction);
	}

	return instructions;
}

} // anonymous namespace

Json ethdebug::program(std::string_view _name, unsigned _sourceId, Assembly const* _assembly, LinkerObject const* _linkerObject)
{
	Json result = Json::object();
	result["contract"] = Json::object();
	result["contract"]["name"] = _name;
	result["contract"]["definition"] = Json::object();
	result["contract"]["definition"]["source"] = Json::object();
	result["contract"]["definition"]["source"]["id"] = _sourceId;
	result["environment"] = (!_assembly || _assembly->isCreation()) ? "create" : "call";
	result["instructions"] = programInstructions(_assembly, _linkerObject, _sourceId);
	return result;
}

Json ethdebug::resources(std::vector<std::string> const& _sources, std::string const& _version)
{
	Json sources = Json::array();
	for (size_t id = 0; id < _sources.size(); ++id)
	{
		Json source = Json::object();
		source["id"] = id;
		source["path"] = _sources[id];
		sources.push_back(source);
	}
	Json result = Json::object();
	result["compilation"] = Json::object();
	result["compilation"]["compiler"] = Json::object();
	result["compilation"]["compiler"]["name"] = "solc";
	result["compilation"]["compiler"]["version"] = _version;
	result["compilation"]["sources"] = sources;
	return result;
}
