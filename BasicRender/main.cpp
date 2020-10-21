#include "tracer.h"
#include "lambertian.h"
#include "dielectric.h"
#include "sphere.h"
using namespace sparrow;

Tracer tracer;

void twoSpheresScene()
{
    TextureMgr::ptr texMgr = tracer.getTextureMgr();
    unsigned int ground = texMgr->loadTexture(new SolidColor(1, 0.0, 0.0));
    unsigned int mid = texMgr->loadTexture(new SolidColor(0.0, 1, 0.0));
    unsigned int earth_unit = texMgr->loadTexture(new ImageTexture("D:\\Sparrow\\Texture\\earthmap.jpg"));

    shared_ptr<MaterialMgr> matMgr = tracer.getMaterialMgr();
    unsigned int ground_mat = matMgr->loadMaterial(new Lambertian(ground));
    unsigned int mid_mat = matMgr->loadMaterial(new Lambertian(mid));
    unsigned int checker_mat = matMgr->loadMaterial(new Lambertian(ground));
    unsigned int earth_mat = matMgr->loadMaterial(new Lambertian(ground));
    //unsigned int bottle_mat = matMgr->loadMaterial(new Dielectric(1.5f));

    tracer.addObjects(make_shared<Sphere>(Point3f(0, -100.5, -1.0), 100, ground_mat));
    tracer.addObjects(make_shared<Sphere>(Point3f(0, 0, -1), 0.5, mid));
    tracer.addObjects(make_shared<Sphere>(Point3f(-1, 0.0, -1), 0.5, mid));
    BCamera* camera = tracer.getCamera();
    camera->setPosition(Point3f(-2, 2, 1));
    camera->setTarget(Point3f(0, 0, -1));
    camera->setFovy(90);
}


//HittableList SimpleLight() {
//    auto ground = make_shared<Lambertian>(Color(0.2, 0.3, 0.1));
//    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
//    HittableList objects;
//    auto pertext = make_shared<NoiseTexture>(4);
//    objects.add(make_shared<Sphere>(Point3f(0, -1000, 0), 1000, ground));
//    objects.add(make_shared<Sphere>(Point3f(0, 2, 0), 2, red));
//
//    auto diffLight = make_shared<DiffuseLight>(Color(4, 4, 4));
//    objects.add(make_shared<XYRect>(3, 5, 1, 3, -2, diffLight));
//    return objects;
//}
//
//HittableList CornellBox() {
//    HittableList objects;
//    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
//    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
//    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
//    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));
//
//    objects.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
//    objects.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
//    objects.add(make_shared<FlipFace>(make_shared<XZRect>(213, 343, 227, 332, 554, light)));
//    objects.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
//    objects.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
//    objects.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));
//
//    objects.add(make_shared<Box>(Point3f(130, 0, 65), Point3f(295, 165, 230), white));
//    objects.add(make_shared<Box>(Point3f(265, 0, 295), Point3f(430, 330, 460), white));
//
//    return objects;
//}

void randomScene()
{
	TextureMgr::ptr texMgr = tracer.getTextureMgr();
	MaterialMgr::ptr matMgr = tracer.getMaterialMgr();
	//    unsigned int checker_unit = texMgr->loadTexture(new CheckerTexture(
	//        new ConstantTexture(Vector3D(0.2f, 0.3f, 0.1f)),
	//        new ConstantTexture(Vector3D(0.9f, 0.9f, 0.9f))));
	unsigned int bottle_mat = matMgr->loadMaterial(new Dielectric(1.5f));
	//    tracer.addObjects(new Sphere(Vector3D(0, -1000.0, 0), 1000,
	//        matMgr->loadMaterial(new Metal(texMgr->loadTexture(new ConstantTexture(Vector3D(1.0,1.0,1.0))),
	//                                       0.1f))));
    unsigned int checker_unit = texMgr->loadTexture(new CheckerTexture(
        make_shared<SolidColor>(Vector3f(0.2f, 0.3f, 0.1f)),
        make_shared<SolidColor>(Vector3f(0.9f, 0.9f, 0.9f))));
    unsigned int earth_unit = texMgr->loadTexture(new ImageTexture("D:\\Sparrow\\Texture\\earthmap.jpg"));

    unsigned int checker_mat = matMgr->loadMaterial(new Lambertian(checker_unit));
    unsigned int earth_mat = matMgr->loadMaterial(new Lambertian(earth_unit));
	for (int a = -11; a < 11; ++a)
	{
		for (int b = -11; b < 11; ++b)
		{
			Float choose_mat = RandomFloat();
            Point3f center(a + 0.9* RandomFloat(), 0.2, b + 0.9* RandomFloat());
			if ((center - Point3f(4, 0.2, 0)).Length() > 0.9)
			{
				// diffuse.
				if (choose_mat < 0.8f)
					tracer.addObjects(make_shared<Sphere>(center, 0.2,
                        checker_mat));
				// metal
				else if (choose_mat < 0.95f)
					tracer.addObjects(make_shared<Sphere>(center, 0.2,
                        checker_mat));
				// glass
				else
					tracer.addObjects(make_shared<Sphere>(center, 0.2, bottle_mat));
			}
		}
	}
	tracer.addObjects(make_shared<Sphere>(Point3f(0, 1, 0), 1.0, bottle_mat));
	tracer.addObjects(make_shared<Sphere>(Point3f(-4, 1, 0), 1.0,
        checker_mat));
	tracer.addObjects(make_shared<Sphere>(Point3f(4, 1, 0), 3.0,
        checker_mat));

	BCamera *camera = tracer.getCamera();
    camera->setPosition(Point3f(1, 1, 1));
    camera->setTarget(Point3f(0, 0, 0));
    camera->setFovy(45.0f);
}

int main() {
    //// Image
    //Float aspectRatio = 16.0 / 9.0;
    //Float imageWidth = 800;
    //int spp = 5;
    //int maxDepth = 3;
    //Float vfov = 40.0;
    //
    //// Right up zom in
    ////Point3f lookfrom(3, 3, 2);

    //Point3f lookfrom(3, 3, 2);
    //Point3f lookat(0, 0, 0);
    //Vector3f vup(0, 1, 0);
    //Float aperture = 0.0;

    ////auto matLambertian=make_shared<Lambertian>(Color(0, 0.53, 0.73));
    ////auto matCenter=make_shared<Dielectric>(1.5);
    ////auto matLeft=make_shared<Metal>(Color(0.8, 0.8, 0.8),0.3);
    ////auto matDielectric=make_shared<Dielectric>(1.5);
    ////auto matMetal=make_shared<Metal>(Color(0.7, 0.3, 0.3),0.2);
    ////auto checker = make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    ////auto pertext = make_shared<NoiseTexture>(4);
    ////auto perlinMat = make_shared<Lambertian>(pertext);
    ////auto matChecker = make_shared<Lambertian>(checker);

    ////auto earthTexture = make_shared<ImageTexture>("D:\\Sparrow\\Texture\\earthmap.jpg");
    ////auto earthMat = make_shared<Lambertian>(earthTexture);

    //Color background{ 0,0,0 };

    //HittableList world;
    //switch (0)
    //{
    //case 0:
    //    world = CornellBox();
    //    aspectRatio = 1.0;
    //    imageWidth = 800;
    //    spp = 400;
    //    background = Color(0, 0, 0);
    //    lookfrom = Point3f(278, 278, -800);
    //    lookat = Point3f(278, 278, 0);
    //    vfov = 40.0;
    //    break;
    //case 1:
    //    world = SimpleLight();
    //    spp = 400;
    //    lookfrom = Point3f(3, 3, 2);
    //    lookat = Point3f(0, 0, 1);
    //    vfov = 20.0;
    //    maxDepth = 10;
    //    break;
    //default:
    //    break;
    //}

    //auto distToFocus = (lookfrom - lookat).Length();
    //int imageHeight = static_cast<int>(imageWidth / aspectRatio);

    //BCamera cam(lookfrom, lookat, vup, vfov, aspectRatio, aperture, distToFocus, 0.0, 2.0);
    ////ground
    ////world.add(make_shared<Sphere>(Point3f(0.0, -100.5, -1.0), 100.0, matChecker));
    ////center
    ////world.add(make_shared<Sphere>(Point3f(0.0, 0.0, -1.0), 0.5, matMetal));
    ////right
    ////world.add(make_shared<Sphere>(Point3f(1.0, 0.0, -1.0), 0.5, earthMat));
    ////left
    ////world.add(make_shared<Sphere>(Point3f(-1.0, 0.0, -1.0), 0.5, matDielectric));
    ////world.add(make_shared<MovingSphere>(cen, cen2, 0.0, 1.0, 0.2, matLeft));

    //std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    //for (int j = imageHeight - 1; j >= 0; --j) {
    //    for (int i = 0; i < imageWidth; ++i) {
    //        Color p(0, 0, 0);
    //        for (int s = 0; s < spp; ++s) {
    //            auto u = (i + RandomFloat()) / (imageWidth - 1);
    //            auto v = (j + RandomFloat()) / (imageHeight - 1);
    //            RRay r = cam.getRay(u, v);
    //            p += RayColor(r, background, world, maxDepth);
    //        }
    //        WriteColor(std::cout, p, spp);
    //    }
    //}
    //std::cerr << "\nDone!\n";

    tracer.initialize(800, 600);
    tracer.setMaxDepth(3);
    tracer.setSpp(30);

    twoSpheresScene();

    Float totalTime = 0.0f;
    unsigned char* ret = nullptr;
    ret = tracer.render(totalTime);

}