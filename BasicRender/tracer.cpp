#include "tracer.h"

namespace sparrow {
	unsigned char* Tracer::render(double& totalTime) {
		config.startFrame = clock();
		if (config.parrelForCpu == 1) {
			rawSerialRender(scene);
		}
		else {
			rawSerialRender(scene);
		}
		config.endFrame = clock();
		config.totalFrameTime = static_cast<Float>(config.endFrame - config.startFrame) / CLOCKS_PER_SEC;
		totalTime = config.totalFrameTime;
		return image;
	}

	void Tracer::rawSerialRender(Hittable& scene) {
		std::cout << "P3\n" << config.width << ' ' << config.height << "\n255\n";
		for (auto j = config.height - 1; j >= 0; --j) {
			for (auto i = 0; i < config.width; ++i) {
				Color color(0, 0, 0);
				for (auto s = 0; s < config.spp; ++s) {
					Float u = (i + RandomFloat()) / (config.width - 1);
					Float v = (j + RandomFloat()) / (config.height - 1);
					RRay ray = config.camera->getRay(u, v);
					Color tmp= tracing(ray, scene, config.maxDepth);
					color += tmp;
				}
				drawPixel(i, j, color, config.spp);
			}
		}
	}

	Color Tracer::tracing(const RRay& r, const Hittable& world, int depth) {
		HitRecord rec;
		/*if (depth <= 0)
			return Color(0.1, 0.1, 0.1);
		if (world.hit(r, 0.001, Infinity, rec)) {
			RRay scattered;
			Color attenuation;
			Material::ptr mat = manager.m_materialMgr->getMaterial(rec.material);
			if (mat->scatter(r, rec, attenuation, scattered)) {
				Color tmpColor = EleDot<Float>(attenuation, tracing(scattered, world, depth - 1));
				return tmpColor;
			}
			return Color(0, 0, 0);
		}
		return Color(1, 1, 1);*/

		if (depth <= 0) return Color(0, 0, 0);
		if (!world.hit(r, 0.001, Infinity, rec)) {
			Color ret;
			switch (config.background)
			{
			case PURE:
				ret = Color(1, 1, 1);
				break;
			case LERP:
				ret = Color(1, 1, 1);
				break;
			default:
				break;
			}
			return ret;
		}
		else {
			RRay scattered;
			Color attenuation;
			shared_ptr<Material> mat = manager.m_materialMgr->getMaterial(rec.material);
			Color emitted = mat->emitted(rec.u, rec.v, rec.p);

			Float pdf;
			Color albedo;
			
			if (!mat->scatter(r, rec, albedo, scattered, pdf))
				return emitted;

			return emitted
				+ EleDot<Float>(albedo * mat->scatteringPDF(r, rec, scattered)
					, tracing(scattered, world, depth - 1)) / pdf;

					//	//return emitted + EleDot<Float>(albedo * mat->scatteringPDF(r, rec, scattered), tracing(scattered, world, depth - 1)) / pdf;
					//	//std::cout << "albedo1:" << albedo << std::endl;
					//	//return EleDot(albedo * mat->scatteringPDF(r, rec, scattered), tracing(scattered, world, depth - 1)) / pdf;
		}
	}

	void Tracer::drawPixel(unsigned int x,unsigned int y, Color pixelColor, int spp) {
		Float r = pixelColor.x;
		Float g = pixelColor.y;
		Float b = pixelColor.z;
		auto scale = 1.0 / spp;
		//Gamma correction for gamma=2.0
		r = sqrt(scale * r);
		g = sqrt(scale * g);
		b = sqrt(scale * b);

		unsigned int idx = (y * config.width + x) * config.channel;
		image[idx + 0] = static_cast<unsigned char>(256 * Clamp(r, 0.0, 0.999));
		image[idx + 1] = static_cast<unsigned char>(256 * Clamp(g, 0.0, 0.999));
		image[idx + 2] = static_cast<unsigned char>(256 * Clamp(b, 0.0, 0.999));

		std::cout << static_cast<int>(256 * Clamp(r, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * Clamp(g, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * Clamp(b, 0.0, 0.999)) << '\n';
	}
	
	void Tracer::initialize(int w, int h, int c) {
		config.width = w;
		config.height = h;
		config.channel = c;
		if (image != nullptr) delete image;
		if (config.camera != nullptr) delete config.camera;

		Point3f lookfrom(0, 6, 21);
		Point3f lookat(0, 0, 0);
		Float distToFocus = 10.0f;
		Float vfov = 45.0;
		Float aperture = 0.1f;
		Float aspectRatio = static_cast<Float>(w / h);
		config.camera=new BCamera(lookfrom, lookat, vfov, aspectRatio, aperture, distToFocus);

		image = new unsigned char[config.width * config.height * config.channel];
		if (manager.m_textureMgr == nullptr)
			manager.m_textureMgr = TextureMgr::getSingleton();
		if (manager.m_materialMgr == nullptr)
			manager.m_materialMgr = MaterialMgr::getSingleton();
	}
}