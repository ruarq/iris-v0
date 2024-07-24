cmake_minimum_required(VERSION 3.28)
project(irisc)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)

add_executable(irisc src/main.cpp
        src/Token.cpp
        src/Token.hpp
        src/Span.cpp
        src/Span.hpp
        src/common.hpp
        src/File.cpp
        src/File.hpp
        src/Scanner.cpp
        src/Scanner.hpp
        src/Tokenizer.cpp
        src/Tokenizer.hpp
        src/Lexer.cpp
        src/Lexer.hpp
        src/Diagnostics.cpp
        src/Diagnostics.hpp
        src/diagnostic_messages.json.hpp
        src/MessageTemplate.cpp
        src/MessageTemplate.hpp
        src/Message.cpp
        src/Message.hpp
        src/Parser.cpp
        src/Parser.hpp
        src/ast/Node.cpp
        src/ast/Node.hpp
        src/ast/Visitor.hpp
        src/ast/Stmt.cpp
        src/ast/Stmt.hpp
        src/ast/ReturnStmt.cpp
        src/ast/ReturnStmt.hpp
        src/ast/Expr.cpp
        src/ast/Expr.hpp
        src/ast/Decl.cpp
        src/ast/Decl.hpp
        src/ast/TranslationUnit.cpp
        src/ast/TranslationUnit.hpp
        src/ast/FnDecl.cpp
        src/ast/FnDecl.hpp
        src/ast/Identifier.cpp
        src/ast/Identifier.hpp
        src/ast/TypeName.hpp
        src/ast/Block.cpp
        src/ast/Block.hpp
        src/ast/ExprStmt.cpp
        src/ast/ExprStmt.hpp
        src/ast/LetStmt.cpp
        src/ast/LetStmt.hpp
        src/ast/MutStmt.cpp
        src/ast/MutStmt.hpp
        src/ast/NameExpr.cpp
        src/ast/NameExpr.hpp
        src/ast/LiteralExpr.cpp
        src/ast/LiteralExpr.hpp
        src/ast/Value.cpp
        src/ast/Value.hpp
        src/ast/BinaryExpr.cpp
        src/ast/BinaryExpr.hpp
        src/ast/ast.hpp
        src/ast/AssignExpr.cpp
        src/ast/AssignExpr.hpp
        src/ast/GroupExpr.cpp
        src/ast/GroupExpr.hpp
        src/ast/CallExpr.cpp
        src/ast/CallExpr.hpp
        src/AstJsonSerializer.cpp
        src/AstJsonSerializer.hpp
        src/TypeChecker.cpp
        src/TypeChecker.hpp
        src/Environment.cpp
        src/Environment.hpp
        src/types/Type.cpp
        src/types/Type.hpp
        src/types/SimpleType.cpp
        src/types/SimpleType.hpp
        src/ast/TypeName.cpp
        src/types/FnType.cpp
        src/types/FnType.hpp
        src/types/types.hpp
        src/types/ErrorType.cpp
        src/types/ErrorType.hpp
        src/CCompiler.cpp
        src/CCompiler.hpp
        src/ast/IfStmt.cpp
        src/ast/IfStmt.hpp
        src/types/UnitType.cpp
        src/types/UnitType.hpp
        src/ast/PipeExpr.cpp
        src/ast/PipeExpr.hpp
        src/ast/WhileStmt.cpp
        src/ast/WhileStmt.hpp
)

target_include_directories(irisc PRIVATE src)

# nlohmann/json
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3)
FetchContent_MakeAvailable(json)
target_link_libraries(irisc PRIVATE nlohmann_json::nlohmann_json)

# fmtlib/fmt
FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 11.0.1)
FetchContent_MakeAvailable(fmt)
target_link_libraries(irisc PRIVATE fmt::fmt)