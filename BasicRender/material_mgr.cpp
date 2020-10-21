#include "material_mgr.h"

namespace sparrow {
	template<> shared_ptr<MaterialMgr> Singleton<MaterialMgr>::_instance = nullptr;

	shared_ptr<MaterialMgr> MaterialMgr::getSingleton()
	{
		if (_instance == nullptr)
			return _instance = std::make_shared<MaterialMgr>();
		return _instance;
	}
}

