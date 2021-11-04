#include "Directory.h"

namespace emojitool {
	Directory::Directory() {
		misCopied = false;
	}
	Directory::Directory(const fs::directory_entry _dir) {
		mPath = _dir.path();
		misCopied = false;
		for (const auto& file : fs::directory_iterator(_dir)) {
				if (file.is_directory()) {
					mDirectorys.push_back(new Directory(file));
				}
				else {
					mEmojis.push_back(new Emoji(*this, file.path()));
				}
		}
	}
	Directory::Directory(const fs::path _dir) : Directory(fs::directory_entry(_dir)) {}
	Directory::Directory(const std::string _dir) : Directory(fs::directory_entry(_dir)) {}
	Directory::Directory(const Directory& _dir) {
		misCopied = true;
		mPath = _dir.mPath;
		mDirectorys = _dir.mDirectorys;
		mEmojis = _dir.mEmojis;
	}

	Directory::~Directory() {
		if (!misCopied) {
			for (const auto& emoji : mEmojis) {
				delete emoji;
			}
			for (const auto& dir : mDirectorys) {
				delete dir;
			}
		}
	}

	void Directory::CreateDirectory(const std::string _name) {
		if (fs::create_directory(_name)) {
			mDirectorys.push_back(new Directory(mPath / _name));
		}
	}
	void Directory::RenameEmoji(Emoji& _emoji, std::string _name) {
		fs::directory_entry(mPath / _emoji.GetFileName()).replace_filename(_name);
		_emoji.SetFileName(_name);
	}
	void Directory::RenameDirectory(std::string _name) {
		std::string temp = mPath.string();

		fs::directory_entry(mPath).replace_filename(_name);
		mPath = fs::path(temp.erase(temp.find_last_of('/'))) / _name;

	}
	void Directory::RemoveEmoji(Emoji& _emoji) {
		delete &_emoji;

		for (auto itr = mEmojis.begin(); itr != mEmojis.end(); itr++) {
			if (*itr == &_emoji) {
				mEmojis.erase(itr);
				break;
			}
		}

		fs::remove(mPath / _emoji.GetFileName());
	}
	void Directory::RemoveDirectory(void) {
		for (const auto& emoji : mEmojis) {
			delete emoji;
		}
		mEmojis.clear();

		for (const auto& dir : mDirectorys) {
			dir->RemoveDirectory();
			delete dir;
		}
		mDirectorys.clear();
		fs::remove_all(mPath);
	}
	void Directory::MoveDirectoryOfEmoji(Emoji& _emoji, Directory& _dest) {
		fs::copy_file(mPath / _emoji.GetFileName(), _dest.GetPath());
		for (auto itr = mEmojis.begin(); itr != mEmojis.end(); itr++) {
			if (*itr == &_emoji) {
				mEmojis.erase(itr);
				break;
			}
		}
		_emoji.SetParentDirectory(_dest);
	}

	fs::path Directory::GetPath(void) const {
		return mPath;
	}
	std::vector<Directory*>& Directory::GetDirectorys(void) {
		return mDirectorys;
	}
	std::vector<Emoji*>& Directory::GetEmojis(void) {
		return mEmojis;
	}
}