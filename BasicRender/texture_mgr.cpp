#include "texture_mgr.h"

namespace sparrow {

	template<> shared_ptr<TextureMgr> Singleton<TextureMgr>::_instance = nullptr;

	shared_ptr<TextureMgr> TextureMgr::getSingleton()
	{
		if (_instance == nullptr)
			return _instance = std::make_shared<TextureMgr>();
		return _instance;
	}
}
