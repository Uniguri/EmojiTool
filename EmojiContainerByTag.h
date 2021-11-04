#pragma once
#include <list>
#include "Emoji.h"

namespace emojitool {
	class EmojiContainerByTag
	{
	private:
		unsigned int mTag;
		std::list<Emoji*> mEmojis;
	public:
		EmojiContainerByTag();
		EmojiContainerByTag(unsigned int _tag);
		~EmojiContainerByTag();

		void AddEmoji(Emoji& _emoji);

		void RemoveEmoji(Emoji& _emoji);

		unsigned int GetTag(void) const;
		std::list<Emoji*>& GetEmojis(void);
	};
}