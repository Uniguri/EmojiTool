#include <iostream>
#include <filesystem>
#include "App.cpp"


int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "Usage: EmojiTool [Directory Path]" << std::endl;
		return 0;
	}
	if (argc >= 2) {
		const auto e = fs::directory_entry(fs::absolute(argv[1]));
		std::cout << e.path() << std::endl;
		if (!e.exists() || !e.is_directory()) {
			std::cout << "Directory is not exist" << std::endl;
			return 0;
		}
		
		EmojiToolApp eta(e.path());
		eta.StartApp();
	}
}