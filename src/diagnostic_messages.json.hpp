//
// Created by ruarq on 18/07/2024.
//

#ifndef DIAGNOSTIC_MESSAGES_JSON_HPP
#define DIAGNOSTIC_MESSAGES_JSON_HPP

#include <nlohmann/json.hpp>

#define LEXER_ERR(x) (0xA000 | (x))
#define PARSE_ERR(x) (0xB000 | (x))
#define SEMA_ERR(x) (0xC000 | (x))

#define EXTRA(x) (0xD000 | (x))

namespace iris {
	static const nlohmann::json diagnostic_messages_json = {
	/**
		 * Lexer errors (0xAXXX)
		 */
	// UnknownToken
		{
			{ "name", "UnknownToken" },
			{ "id", LEXER_ERR(0) },
			{ "kind", "Error" },
			{ "fmt", "Unknown token '{}'" },
			{ "description", "" },
		 },

 /**
		 * Parser errors (0xBXXX)
		 */
	// UnexpectedToken
		{ { "name", "UnexpectedToken" },
			{ "id", PARSE_ERR(0) },
			{ "kind", "Error" },
			{ "fmt", "Unexpected token '{}'" },
			{ "description", "" } },

 // UnexpectedTokenExpected
		{ { "name", "UnexpectedTokenExpected" },
			{ "id", PARSE_ERR(1) },
			{ "kind", "Error" },
			{ "fmt", "Unexpected token '{}', expected '{}'" },
			{ "description", "" } },

 // UnexpectedTokenExpectedAnyOf
		{ { "name", "UnexpectedTokenExpectedAnyOf" },
			{ "id", PARSE_ERR(2) },
			{ "kind", "Error" },
			{ "fmt", "Unexpected token '{}', expected any of {}" },
			{ "description", "" } },

 /**
		 * Semantic errors (0xCXXX)
		 */
	// UnknownIdentifier
		{ { "name", "UnknownIdentifier" },
			{ "id", SEMA_ERR(0) },
			{ "kind", "Error" },
			{ "fmt", "Unknown identifier '{}'" },
			{ "description", "" } },
 // TypesDontMatch
		{ { "name", "TypesDontMatch" },
			{ "id", SEMA_ERR(1) },
			{ "kind", "Error" },
			{ "fmt", "Types {} and {} don't match" },
			{ "description", "" } },
 // ExpectedFunction
		{ { "name", "ExpectedFunction" },
			{ "id", SEMA_ERR(2) },
			{ "kind", "Error" },
			{ "fmt", "Expected function type, but found {}" },
			{ "description", "" } },
 // MismatchedArgCount
		{ { "name", "MismatchedArgCount" },
			{ "id", SEMA_ERR(3) },
			{ "kind", "Error" },
			{ "fmt", "Expected {} arguments but got {}" },
			{ "description", "" } },
 // ExpectedType
		{ { "name", "ExpectedType" },
			{ "id", SEMA_ERR(4) },
			{ "kind", "Error" },
			{ "fmt", "Expected type {}, but found {}" },
			{ "description", "" } },
		{ { "name", "CantAssignToReadOnly" },
			{ "id", SEMA_ERR(5) },
			{ "kind", "Error" },
			{ "fmt", "Can't assign to read-only value" },
			{ "description", "" } },
 // PartiallyNamedArgs
		{ { "name", "PartiallyNamedArgs" },
			{ "id", SEMA_ERR(6) },
			{ "kind", "Error" },
			{ "fmt", "Expected all arguments to be either named or positional." },
			{ "description", "" } },
 // UnmatchedNamedArgument
		{ { "name", "UnmatchedNamedArgument" },
			{ "id", SEMA_ERR(7) },
			{ "kind", "Error" },
			{ "fmt", "Unmatched argument '{}'" },
			{ "description", "" } },
 // CCompiler.NamedArgsNotSupported
		{ { "name", "CCompiler.NamedArgsNotSupported" },
			{ "id", EXTRA(0) },
			{ "kind", "Warning" },
			{ "fmt", "Compiling to C with named arguments is not supported." },
			{ "description", "" } },
		{ { "name", "ClosuresNotSupported" },
			{ "id", EXTRA(1) },
			{ "kind", "Warning" },
			{ "fmt", "Closures are not supported." },
			{ "description", "" } },
	};
}

#endif	  //DIAGNOSTIC_MESSAGES_JSON_HPP
