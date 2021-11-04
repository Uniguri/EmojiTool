#pragma once
#include <map>
#include <stack>
#include <filesystem>
#include <string>
#include "Directory.h"
#include "Emoji.h"
#include "EmojiContainerByTag.h"

namespace fs = std::filesystem;
namespace emojitool {
	class EmojiTool
	{
	private:
		fs::path mPath;
		Directory* mDir;
		std::map<unsigned int, EmojiContainerByTag*> mTagedEmojiContainers;
		std::map<unsigned int, std::string> mTagList;
	public:
		EmojiTool();
		EmojiTool(fs::path _path);
		~EmojiTool();

		bool HasTag(std::string _tag);

		void RemoveDirectory(fs::path _path);
		void RemoveDirectoryReally(fs::path _path);
		void RemoveEmoji(fs::path _path);
		void RemoveEmojiReally(fs::path _path);
		void AddTagToEmoji(Emoji& _emoji, const std::string _tag);
		void AddTagToEmojiByPath(const fs::path& _emoji, const std::string _tag);
		void RemoveTagFromEmojiByStrTag(Emoji& _emoji, const std::string _tag);
		void RemoveTagFromEmojiByHashedTag(Emoji& _emoji, const unsigned int _HashedTag);
		void RemoveTagFromEmojiByPath(const fs::path& _emoji, const std::string _tag);

		std::list<std::string> GetTagList(void);
		Directory& GetDirectory(void) const;
		Directory& GetDirectoryByPath(const fs::path& _path) const;
		Emoji& GetEmojiByPath(fs::path _path) const;
		std::list<Emoji*>& GetEmojisByTag(std::string _tag) const;

		EmojiTool& operator=(EmojiTool&& et) noexcept;
	};
}