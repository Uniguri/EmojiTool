#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "Emoji.h"

namespace fs = std::filesystem;
namespace emojitool {
	class Directory
	{
	private:
		bool misCopied;
		fs::path mPath;
		std::vector<Directory*> mDirectorys;
		std::vector<Emoji*> mEmojis;
	public:
		Directory();
		Directory(const fs::directory_entry _dir);
		Directory(const fs::path _dir);
		Directory(const std::string _dir);
		Directory(const Directory& _dir);
		~Directory();

		void CreateDirectory(const std::string _name);
		void RenameEmoji(Emoji& _emoji, std::string _name);
		void RenameDirectory(std::string _name);
		void RemoveEmoji(Emoji& _emoji);
		void RemoveDirectory(void);
		void MoveDirectoryOfEmoji(Emoji& _emoji, Directory& _dest);

		fs::path GetPath(void) const;
		std::vector<Directory*>& GetDirectorys(void);
		std::vector<Emoji*>& GetEmojis(void);
	};
}