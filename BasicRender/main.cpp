#include "tracer.h"
#include "lambertian.h"
#include "dielectric.h"
#include "sphere.h"
#include "aarect.h"
#include "box.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
using namespace sparrow;

Tracer tracer;

void CornellBox() {
    TextureMgr::ptr texMgr = tracer.getTextureMgr();
    unsigned int white = texMgr->loadTexture(new SolidColor(0.73, 0.73, 0.73));
    unsigned int red = texMgr->loadTexture(new SolidColor(0.65, 0.05, 0.05));
    unsigned int green = texMgr->loadTexture(new SolidColor(.12, .45, .15));
    unsigned int light = texMgr->loadTexture(new SolidColor(15, 15, 15));

    MaterialMgr::ptr matMgr = tracer.getMaterialMgr();
    unsigned int whiteMat = matMgr->loadMaterial(new Lambertian(white));
    unsigned int redMat = matMgr->loadMaterial(new Lambertian(red));
    unsigned int greenMat = matMgr->loadMaterial(new Lambertian(green));
    unsigned int lightMat = matMgr->loadMaterial(new Lambertian(light));

    tracer.addObjects(make_shared<YZRect>(0, 555, 0, 555, 555, greenMat));
    tracer.addObjects(make_shared<YZRect>(0, 555, 0, 555, 0, redMat));
    tracer.addObjects(make_shared<XZRect>(0, 555, 0, 555, 0, whiteMat));
    tracer.addObjects(make_shared<FlipFace>(make_shared<XZRect>(213, 343, 227, 332, 554, lightMat)));
    tracer.addObjects(make_shared<XZRect>(0, 555, 0, 555, 555, whiteMat));
    tracer.addObjects(make_shared<XYRect>(0, 555, 0, 555, 555, whiteMat));

    shared_ptr<Hittable> box1 = make_shared<Box>(Point3f(0, 0, 0), Point3f(165, 330, 165), whiteMat);
    shared_ptr<Hittable> box2 = make_shared<Box>(Point3f(0, 0, 0), Point3f(165, 165, 165), whiteMat);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vector3f(265, 0, 295));
    tracer.addObjects(box1);

    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vector3f(130, 0, 65));
    tracer.addObjects(box2);

    BCamera* camera = tracer.getCamera();
    camera->setPosition(Point3f(278, 278, -800));
    camera->setTarget(Point3f(278, 278, 0));
    camera->setFovy(39);
}

int main() {

    tracer.initialize(800, 800, 3);
    tracer.setMaxDepth(1);
    tracer.setSpp(20);

    //twoSpheresScene();
    CornellBox();

    Float totalTime = 0.0f;
    unsigned char* ret = nullptr;
    ret = tracer.render(totalTime);

    stbi_flip_vertically_on_write(1);
    stbi_write_png("./result.png",
        tracer.getWidth(),
        tracer.getHeight(),
        4,
        static_cast<void*>(tracer.getImage()),
        tracer.getWidth() * 4);
    std::cout << "Over\n";
    return 0;
}