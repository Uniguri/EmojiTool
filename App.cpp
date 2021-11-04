#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include "EmojiTool.h"

namespace fs = std::filesystem;
class EmojiToolApp {
private:
	fs::path mPath;
	emojitool::EmojiTool mEt;

	std::string MakeLower(const std::string& str) {
		std::string ret = str;
		for (int i = 0; i < (int)str.length(); i++) {
			ret[i] = std::tolower(str[i]);
		}
		return ret;
	}
	std::vector<std::string> GetCommand(void) {
		std::string input;
		std::string temp;
		std::vector<std::string> args;

		std::getline(std::cin, input);
		//when input contains continuous whitespace ex) "command      parma1 ..."
		int NumberOfRemovedWhitespace = 0;
		bool isAdded = false;
		if (input.back() == ' ') {
			input.push_back('a');
			isAdded = true;
		}
		for (int i = 0; i < (int)input.length() - 1; i++) {
			if (input[i] == ' ' && input[i + 1] == ' ') {
				NumberOfRemovedWhitespace++;
				for (int j = i; j < (int)input.length() - 1; j++) {
					input[j] = input[j + 1];
				}
				i--;
			}
		}
		if (isAdded) {
			input.pop_back();
		}
		input.erase(input.length() - NumberOfRemovedWhitespace, NumberOfRemovedWhitespace);

		//when input starts with whitespace ex) " command ..."
		if (input[0] == ' ') {
			input.erase(1);
		}

		//if input does not ends with whitespace, add it at end of input
		//   for ease of algorithm
		if (input.back() != ' ') {
			input.push_back(' ');
		}

		int start = 0;
		int end = 0;
		bool quote = false;
		for (int i = start; i < (int)input.length(); i++) {
			if (input[i] == ' ' && quote == false) {
				temp = input.substr(start, i - start);
				if (temp[0] == ' ') {
					temp.erase(0, 1);
				}
				args.push_back(std::move(temp));
				start = i;
				continue;
			}
			if (input[i] == '\'') {
				quote = !quote;
				if (quote == false) {
					temp = input.substr(start + 1, i - start - 1);
					args.push_back(std::move(temp));
					i += 2;
				}
				start = i;
				continue;
			}
		}

		return args;
	}
public:
	EmojiToolApp(fs::path _path) {
		mPath = _path;
		mEt = std::move(emojitool::EmojiTool(_path));
	}

	void StartApp(void) {
		std::cout << "Usage: Command pram1 param2 ..." << std::endl;
		std::cout << "If you want to use white space in parm, You must write \"Command \'param 1\' param2 ...\"" << std::endl;
		std::cout << std::endl;

		while (1) {
			std::vector<std::string> args( std::move(GetCommand()) );
			std::string command( std::move(MakeLower(args[0])) );
			if (command == "exit") {
				break;
			}
			if (command == "help") {
				std::cout << "Command list:" << std::endl;
				std::cout << "    AddTag [Emoji path] [Tag]           Add tag to Emoji" << std::endl;
				std::cout << "    RemoveTag [File path] [Tag]         Remove tag from File" << std::endl;
				std::cout << "    GetEmojisByTag [Tag]                Get Emojis having that tag" << std::endl;
				std::cout << "    Exit                                Exit" << std::endl;
				continue;
			}
			if (command == "addtag") {
				if (args.size() < 3) {
					std::cout << "Emoji Path and Tag are required" << std::endl;
					continue;
				}

				auto de = fs::directory_entry(fs::absolute(args[1]));
				if (!de.exists()) {
					std::cout << "Invailed Path" << std::endl;
					continue;
				}
				if (de.is_directory()) {
					std::cout << "Path point out directory, enter emoji path" << std::endl;
					continue;
				}
				
				mEt.AddTagToEmojiByPath(de.path(), args[2]);
				continue;
			}
			if (command == "getemojisbytag") {
				if (args.size() < 2) {
					std::cout << "Tag is required" << std::endl;
					continue;
				}
				if (!mEt.HasTag(args[1])) {
					std::cout << "Tag does not exist" << std::endl;
					std::cout << "Tag list: ";
					for (const auto& tag : mEt.GetTagList()) {
						std::cout << tag << ", ";
					}
					std::cout << std::endl;
					continue;
				}
				for (const auto& emoji : mEt.GetEmojisByTag(args[1])) {
					std::cout << emoji->GetParentDirectory().GetPath() / emoji->GetFileName() << std::endl;
				}
				continue;
			}
			if (command == "removetag") {
				if (args.size() < 3) {
					std::cout << "Emoji Path and Tag are required" << std::endl;
					continue;
				}

				auto de = fs::directory_entry(fs::absolute(args[1]));
				if (!de.exists()) {
					std::cout << "Invailed Path" << std::endl;
					continue;
				}
				if (de.is_directory()) {
					std::cout << "Path point out directory, enter emoji path" << std::endl;
					continue;
				}

				mEt.RemoveTagFromEmojiByPath(de.path(), args[2]);
				continue;
			}
		}
	}

};