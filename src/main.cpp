#include <fstream>

#include "CCompiler.hpp"
#include "Diagnostics.hpp"
#include "File.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Scanner.hpp"
#include "Tokenizer.hpp"
#include "TypeChecker.hpp"
#include "diagnostic_messages.json.hpp"

auto create_diagnostics(iris::File const &file) -> iris::Diagnostics {
	iris::Diagnostics diagnostics{ file };
	auto const templates = iris::parse_message_templates(iris::diagnostic_messages_json);
	for (auto const &[name, message_template] : templates) {
		diagnostics.add_template(name, message_template);
	}
	return diagnostics;
}

auto compile(iris::File const &file, std::filesystem::path const &out) -> int {
	auto diagnostics = create_diagnostics(file);

	iris::Scanner scanner{ file };
	iris::Tokenizer tokenizer{ scanner, diagnostics };
	iris::Lexer lexer{ tokenizer };
	iris::Parser parser{ lexer, diagnostics };
	auto translation_unit = parser.parse();

	iris::TypeChecker type_checker{ file, diagnostics };
	translation_unit.accept(type_checker);

	if (diagnostics.had_errors()) {
		fmt::print("{}\n", diagnostics.get_report());
		return EXIT_SUCCESS;
	}

	iris::CCompiler compiler{ file, diagnostics };
	translation_unit.accept(compiler);

	if (diagnostics.had_messages()) {
		fmt::print("{}\n", diagnostics.get_report());
	}
	if (!diagnostics.had_errors()) {
		std::ofstream ofile{ out };
		if (!ofile.is_open()) {
			fmt::print(stderr, "{}\n", strerror(errno));
			return EXIT_FAILURE;
		}

		ofile << compiler.result();
	}

	return EXIT_SUCCESS;
}

auto main(int const argc, char **argv) -> int {
	if (argc < 2) {
		fmt::print("Usage: {} FILE...", argv[0]);
		return EXIT_SUCCESS;
	}

	char **arg = argv + 1;
	while (*arg) {
		std::string_view const filename = *arg;
		iris::File file;

		try {
			file = iris::File::open(filename);
		} catch (std::exception &e) {
			fmt::print(stderr, "{}", e.what());
			return EXIT_FAILURE;
		}

		fmt::print("compiling {} => {}\n", file.path().string(), file.path().string() + ".c");
		try {
			auto const result = compile(file, file.path().string() + ".c");
			if (result != 0) {
				return EXIT_FAILURE;
			}
		} catch (std::exception &e) {
			fmt::print("|- compilation failed: {}\n", e.what());
		}

		++arg;
	}

	return EXIT_SUCCESS;
}
