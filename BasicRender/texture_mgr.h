#pragma once

#include "texture.h"
#include "singleton.h"
#include <vector>

using std::shared_ptr;

namespace sparrow{
	class Texture;

	class TextureMgr :public Singleton<TextureMgr> {
	private:
		std::vector<shared_ptr<Texture>> tUnits;
	public:
		using ptr = std::shared_ptr<TextureMgr>;
		TextureMgr() = default;
		~TextureMgr() = default;
		static shared_ptr<TextureMgr> getSingleton();

		unsigned int loadTexture(Texture* t) {
			shared_ptr<Texture> newPtr(t);
			tUnits.push_back(newPtr);
			return tUnits.size() - 1;
		}

		shared_ptr<Texture> getTexture(unsigned int idx) {
			if (idx >= tUnits.size())
				return nullptr;
			return tUnits[idx];
		}
	};
}