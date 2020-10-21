#pragma once
#include "material.h"
#include "singleton.h"

namespace sparrow {
	class MaterialMgr :public Singleton<MaterialMgr> {
	private:
		std::vector<shared_ptr<Material>> mUnits;
	public:
		using ptr = std::shared_ptr<MaterialMgr>;
		MaterialMgr() = default;
		~MaterialMgr() = default;
		static shared_ptr<MaterialMgr> getSingleton();
		unsigned int loadMaterial(Material* mat) {
			shared_ptr<Material> newPtr(mat);
			mUnits.push_back(newPtr);
			return mUnits.size() - 1;
		}

		shared_ptr<Material> getMaterial(unsigned int idx) {
			if (idx >= mUnits.size()) {
				return nullptr;
			}
			return mUnits[idx];
		}
	};
}