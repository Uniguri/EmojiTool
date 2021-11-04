#include "EmojiContainerByTag.h"

namespace emojitool {
	EmojiContainerByTag::EmojiContainerByTag() : mTag(0) {}
	EmojiContainerByTag::EmojiContainerByTag(unsigned int _tag) : mTag(_tag) {}
	EmojiContainerByTag::~EmojiContainerByTag() {}

	void EmojiContainerByTag::AddEmoji(Emoji& _emoji) {
		mEmojis.push_back(&_emoji);
	}

	void EmojiContainerByTag::RemoveEmoji(Emoji& _emoji) {
		for (auto itr = mEmojis.begin(); itr != mEmojis.end(); itr++) {
			if (*itr == &_emoji) {
				mEmojis.erase(itr);
			}
		}
	}

	unsigned int EmojiContainerByTag::GetTag(void) const {
		return mTag;
	}
	std::list<Emoji*>& EmojiContainerByTag::GetEmojis(void){
		return mEmojis;
	}
}