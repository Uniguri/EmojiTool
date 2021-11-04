#include "EmojiTool.h"

namespace emojitool {
	EmojiTool::EmojiTool() {
		mDir = nullptr;
	}
	EmojiTool::EmojiTool(fs::path _path) {
		mPath = _path;
		if (fs::directory_entry(mPath).exists()) {
			mDir = new Directory(mPath);
		}
	}
	EmojiTool::~EmojiTool() {
		for (const auto& list : mTagedEmojiContainers) {
			delete list.second;
		}
		if (mDir) {
			delete mDir;
		}
	}
	bool EmojiTool::HasTag(std::string _tag) {
		auto HashedTag = std::hash<std::string>{}(_tag);
		if (mTagList.find(HashedTag) == mTagList.end()) {
			return false;
		}
		return true;
	}

	void EmojiTool::RemoveDirectory(fs::path _path) {
		Directory& Dir = GetDirectoryByPath(_path);
		Directory& ParentDir = GetDirectoryByPath(_path.parent_path());

		//Remove Dir from mDirectorys of Parent Dir
		for (auto itr = ParentDir.GetDirectorys().begin(); itr != ParentDir.GetDirectorys().end(); itr++) {
			if ((*itr)->GetPath() == Dir.GetPath()) {
				ParentDir.GetDirectorys().erase(itr);
				break;
			}
		}

		//Remove Emoji from mTagedEmojiContainer
		bool EachEmojisHasTag = false;
		for (const auto& emoji : Dir.GetEmojis()) {
			EachEmojisHasTag = EachEmojisHasTag || !emoji->GetTags().empty();
		}
		if (EachEmojisHasTag) {
			for (auto emoji = Dir.GetEmojis().begin(); emoji != Dir.GetEmojis().end(); emoji++) {
				for (auto& tag : (*emoji)->GetTags()) {
					RemoveTagFromEmojiByHashedTag(**emoji, tag);
				}
			}
		}
		
		delete &Dir;
	}
	void EmojiTool::RemoveDirectoryReally(fs::path _path) {
		RemoveDirectory(_path);
		fs::remove_all(_path);
	}
	void EmojiTool::RemoveEmoji(fs::path _path) {
		Emoji& emoji = GetEmojiByPath(_path);
		Directory& Dir = GetDirectoryByPath(_path.parent_path());

		//Remove Emoji from mTagedEmojiContainer
		const auto& Tags = emoji.GetTags();
		if (!Tags.empty()) {
			for (const auto& tag : Tags) {
				RemoveTagFromEmojiByHashedTag(emoji, tag);
			}
		}

		//Remove Emoji form mEmojis
		for (auto itr = Dir.GetEmojis().begin(); itr != Dir.GetEmojis().end(); itr++) {
			if ((*itr)->GetFileName() == _path.filename()) {
				delete *itr;
				Dir.GetEmojis().erase(itr);
				break;
			}
		}

		delete& emoji;
	}
	void EmojiTool::RemoveEmojiReally(fs::path _path) {
		RemoveEmoji(_path);
		fs::remove(_path);
	}

	void EmojiTool::AddTagToEmoji(Emoji& _emoji, const std::string _tag) {
		unsigned int HashedTag = std::hash<std::string>{}(_tag);

		//if Tag is undefined, Add Tag and HashedTag to mTagList
		if (mTagList.find(HashedTag) == mTagList.end()) {
			mTagList.insert(std::make_pair(HashedTag, _tag));
			mTagedEmojiContainers.insert(std::make_pair(HashedTag, new EmojiContainerByTag(HashedTag)));
		}

		_emoji.AddTag(HashedTag);
		mTagedEmojiContainers.find(HashedTag)->second->AddEmoji(_emoji);
	}
	void EmojiTool::AddTagToEmojiByPath(const fs::path& _emoji, const std::string _tag) {
		Emoji& emoji = GetEmojiByPath(_emoji);
		AddTagToEmoji(emoji, _tag);
	}
	void EmojiTool::RemoveTagFromEmojiByStrTag(Emoji& _emoji, const std::string _tag) {
		unsigned int HashedTag = std::hash<std::string>{}(_tag);
		RemoveTagFromEmojiByHashedTag(_emoji, HashedTag);
	}
	void EmojiTool::RemoveTagFromEmojiByHashedTag(Emoji& _emoji, const unsigned int _HashedTag) {
		auto Contaieritr = mTagedEmojiContainers.find(_HashedTag);
		if (Contaieritr != mTagedEmojiContainers.end()) {
			auto& emojis = Contaieritr->second->GetEmojis();

			for (auto Emojiitr = emojis.begin(); Emojiitr != emojis.end(); Emojiitr++) {
				if (*Emojiitr == &_emoji) {
					emojis.erase(Emojiitr);
					break;
				}
			}

			if (emojis.empty()) {
				delete Contaieritr->second;

				mTagedEmojiContainers.erase(Contaieritr);
				mTagList.erase(_HashedTag);
			}
		}
	}
	void EmojiTool::RemoveTagFromEmojiByPath(const fs::path& _emoji, const std::string _tag) {
		Emoji& emoji = GetEmojiByPath(_emoji);
		RemoveTagFromEmojiByStrTag(emoji, _tag);
	}

	std::list<std::string> EmojiTool::GetTagList(void) {
		std::list<std::string> ret;
		for (const auto& tag : mTagList) {
			ret.push_back(tag.second);
		}

		return ret;
	}
	Directory& EmojiTool::GetDirectory(void) const {
		return *mDir;
	}
	Directory& EmojiTool::GetDirectoryByPath(const fs::path& _path) const {
		std::stack<fs::path> TempStack;
		TempStack.push(_path);
		while (mDir->GetPath() != TempStack.top()) {
			if (!TempStack.top().has_parent_path()) break;
			TempStack.push(TempStack.top().parent_path());
		}
		TempStack.pop();

		Directory* TempDir = mDir;
		while (!TempStack.empty()) {
			for (auto& dir : TempDir->GetDirectorys()) {
				if (dir->GetPath() == TempStack.top()) {
					TempDir = dir;
					TempStack.pop();
					break;
				}
			}
		}
		return *TempDir;
	}
	Emoji& EmojiTool::GetEmojiByPath(fs::path _path) const {
		Directory& TempDir = GetDirectoryByPath(_path.parent_path());
		Emoji* TempEmoji = nullptr;
		for (auto& emoji : TempDir.GetEmojis()) {
			if ( (TempDir.GetPath() / emoji->GetFileName()) == _path) {
				TempEmoji = emoji;
			}
		}

		return *TempEmoji;
	}
	std::list<Emoji*>& EmojiTool::GetEmojisByTag(std::string _tag) const {
		unsigned int HashedTag = std::hash<std::string>{}(_tag);
		return (mTagedEmojiContainers.find(HashedTag)->second->GetEmojis());
	}

	EmojiTool& EmojiTool::operator=(EmojiTool&& et) noexcept{
		mPath = et.mPath;
		mDir = et.mDir;
		et.mDir = nullptr;

		return *this;
	}
}