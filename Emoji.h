#pragma once
#include <filesystem>
#include <list>
#include <string>

namespace fs = std::filesystem;
namespace emojitool {
	class Directory;
	class Emoji
	{
	private:
		std::string mFileName;
		std::string mEmojiName;
		Directory* mParentDir;
		std::list<unsigned int> mTags;
	public:
		Emoji(Directory& _ParentDir, fs::path _path);
		~Emoji();

		void AddTag(const unsigned int _tag);
		void RemoveTag(const unsigned int _tag);

		void SetFileName(const std::string _name);
		void SetName(const std::string _name);
		void SetParentDirectory(Directory& _dir);
		
		Directory& GetParentDirectory(void);
		fs::path GetFileName(void) const;
		std::list<unsigned int> GetTags(void) const;
	};

}