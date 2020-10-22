#pragma once

#include "color.h"
#include "ray.h"
#include "material.h"
#include "camera.h"
#include "material_mgr.h"
#include "texture_mgr.h"
#include "geometry.h"
#include "hittable_list.h"
#include <time.h>

namespace sparrow {
	class BCamera;
	class Hittable;

	enum BackGround {
		PURE = 0, LERP = 1
	};

	class Tracer {
	private:
		class Manager {
		public:
			shared_ptr<TextureMgr> m_textureMgr;
			shared_ptr<MaterialMgr> m_materialMgr;
		};

		class Setting {
		public:
			BCamera* camera;
			int parrelForCpu;
			BackGround background;
			unsigned int maxDepth;
			unsigned int spp;
			int width, height, channel;
			double startFrame, endFrame, totalFrameTime;

			Setting() :maxDepth(50), spp(10), channel(3), camera(nullptr), parrelForCpu(1), background(PURE) {}
			~Setting() {
				if (camera) delete camera;
				camera = nullptr;
			}
		};

		Setting config;
		Manager manager;
		unsigned char* image;
		HittableList scene;
		//std::vector<Hittable*> objects;

	public:
		Tracer() :image(nullptr) {}
		~Tracer() {
			if (image) delete image;
			image = nullptr;
		}

		void setParrelThread(int num) {
			config.parrelForCpu = num;
		}

		void setSpp(unsigned int num) {
			config.spp = num;
		}

		void setMaxDepth(unsigned int depth) {
			config.maxDepth = depth;
		}

		int getWidth() const {
			return config.width;
		}

		int getHeight() const {
			return config.height;
		}

		int getChannel() const {
			return config.channel;
		}

		Float getTotalFrameTime() const { return config.totalFrameTime; }
		unsigned int getSpp() const { return config.spp; }
		unsigned int getMaxDepth() const { return config.maxDepth; }
		unsigned char* getImage() const { return image; }
		BCamera* getCamera() const { return config.camera; }
		shared_ptr<TextureMgr> getTextureMgr() const { return manager.m_textureMgr; }
		shared_ptr<MaterialMgr> getMaterialMgr() const { return manager.m_materialMgr; }

		void addObjects(shared_ptr<Hittable> object) { scene.add(object); }

		void initialize(int w, int h, int c = 3);
		void beginFrame() {}
		void endFrame() {}

		unsigned char* render(double& totaltime);

	private:
		Color tracing(const RRay& r, const Hittable& scene, int depth);
		void drawPixel(unsigned int x, unsigned int y, const Color& color);
		void rawSerialRender(Hittable& scene);
		void parallelThreadRender(Hittable& scene);

		Vector3f deNan(const Vector3f& c);
	};
}