#include "Emoji.h"

namespace emojitool {
	Emoji::Emoji(Directory& _ParentDir, fs::path _path) {
		mParentDir = &_ParentDir;
		mFileName = _path.filename().string();
		mEmojiName = mFileName;
	}
	Emoji::~Emoji() {}

	void Emoji::AddTag(const unsigned int _tag) {
		mTags.push_back(_tag);
	}
	void Emoji::RemoveTag(const unsigned int _tag) {
		mTags.remove(_tag);
	}

	void Emoji::SetFileName(const std::string _name) {
		mFileName = _name;
	}
	void Emoji::SetName(const std::string _name) {
		mEmojiName = _name;
	}
	void Emoji::SetParentDirectory(Directory& _dir) {
		mParentDir = &_dir;
	}

	Directory& Emoji::GetParentDirectory(void){
		return *mParentDir;
	}
	fs::path Emoji::GetFileName(void) const {
		return fs::path(mFileName);
	}
	std::list<unsigned int> Emoji::GetTags(void) const {
		return mTags;
	}
}