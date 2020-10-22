#include "tracer.h"
#include "tbb/parallel_for.h"
#include "stb_image_write.h"

using namespace tbb;

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
		//std::cout << "P3\n" << config.width << ' ' << config.height << "\n255\n";
		for (int row = config.height - 1; row >= 0; --row) {
			for (int col = 0; col < config.width; ++col) {
				Color color(0, 0, 0);
				for (unsigned int s = 0; s < config.spp; ++s) {
					Float u = static_cast<Float>(col + RandomFloat()) / static_cast<Float>(config.width);
					Float v = static_cast<Float>(row + RandomFloat()) / static_cast<Float>(config.height);
					RRay ray = config.camera->getRay(u, v);
					Color tmp= deNan(tracing(ray, scene, config.maxDepth));
					color += tmp;
				}
				color /= static_cast<Float>(config.spp);
				color = Vector3f(sqrt(color.x), sqrt(color.y), sqrt(color.z));
				if (color.x > 1.0f) color.x = 1.0f;
				if (color.y > 1.0f) color.y = 1.0f;
				if (color.z > 1.0f) color.z = 1.0f;
				drawPixel(col, row, color);

				/*std::cout << static_cast<int>(256 * Clamp(color.x, 0.0, 0.999)) << ' '
					<< static_cast<int>(256 * Clamp(color.y, 0.0, 0.999)) << ' '
					<< static_cast<int>(256 * Clamp(color.z, 0.0, 0.999)) << '\n';*/
			}
		}
	}

	Vector3f Tracer::deNan(const Vector3f& c) {
		Vector3f tmp = c;
		if (!(tmp.x == tmp.x)) tmp.x = 0;
		if (!(tmp.y == tmp.y)) tmp.y = 0;
		if (!(tmp.z == tmp.z)) tmp.z = 0;
		return tmp;
	}

	void Tracer::parallelThreadRender(Hittable& scene) {
		std::cout << "P3\n" << config.width << ' ' << config.height << "\n255\n";
		parallel_for(blocked_range<size_t>(0, config.height * config.width, 10000),
			[&](blocked_range<size_t>& r) {
				for (size_t i = r.begin(); i != r.end(); ++i) {
					Color color(0, 0, 0);
					size_t col = i % config.width;
					size_t row = i / config.width;
					for (unsigned int x = 0; x < config.spp; ++x) {
						Float u = (col + RandomFloat()) / (config.width - 1);
						Float v = (row + RandomFloat()) / (config.height - 1);
						RRay ray = config.camera->getRay(u, v);
						Color tmp = tracing(ray, scene, config.maxDepth);
						color += tmp;
					}
					drawPixel(col, row, color);
				}
			}, auto_partitioner());
	}

	Color Tracer::tracing(const RRay& r, const Hittable& world, int depth) {
		HitRecord rec;

		if (depth <= 0) return Color(0, 0, 0);
		if (!world.hit(r, 0.001, Infinity, rec)) {
			Color ret;
			switch (config.background)
			{
			case PURE:
				ret = Color(0.5, 0.5, 0.5);
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
		}
	}

	void Tracer::drawPixel(unsigned int col,unsigned int row,const Color& color) {
		if (col < 0 || col >= config.width || row < 0 || row >= config.height) return;
		unsigned int idx = (row * config.width + col) * config.channel;
		image[idx + 0] = static_cast<unsigned char>(255.99 * color.x);
		image[idx + 1] = static_cast<unsigned char>(255.99 * color.y);
		image[idx + 2] = static_cast<unsigned char>(255.99 * color.z);
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