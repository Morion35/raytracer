#include "type.hh"
#include "scene.hh"
#include "materials.hh"
#include "objects.hh"
#include "image.hh"
#include "iostream"

using namespace raytracing;

int main()
{
    // Vector3 vect(1.5f, 1.0f, 1.0f);
    // std::cout << vect;

    // std::cout << (vect - Vector3(1.0f, 1.0f, 1.0f));
    // std::cout << (vect * 2.0f);
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto light = std::make_shared<Point_Light>(p3(0, 80, 15), 70);
    auto light2 = std::make_shared<Point_Light>(p3(0, -10, 50), 10);
    lights.push_back(light);
    lights.push_back(light2);



    auto mirror = std::make_shared<Plane>(vec3(0, 0, -1), p3(0, 0, 30), std::make_shared<Uniform_Material>(.4, 0.9, 200, color(150, 150, 150)));
    auto mirror2 = std::make_shared<Plane>(vec3(0, 0, 1), p3(0, 0, -5), std::make_shared<Uniform_Material>(.4, 0.9, 200, color(150, 155, 155)));

    //objects.push_back(mirror);
    //objects.push_back(mirror2);

    auto wall = std::make_shared<Plane>(vec3(0, 0, -1), p3(0, 0, 500), std::make_shared<Uniform_Material>(1, 0.1, 0.8, color(55, 100, 120)));
    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Uniform_Material>(1, 0.1, .8, color(222, 184,136)));
    //objects.push_back(wall);
    objects.push_back(floor);

    auto ball = std::make_shared<Sphere>(p3(-10, 10, 80), 20., std::make_shared<Uniform_Material>(1, .7, 10, color(100, 100, 100)));
    auto ball2 = std::make_shared<Sphere>(p3(12, -10, 40), 5., std::make_shared<Uniform_Material>(0.8, 0.3, 0.5, color(250, 80, 0)));
    auto sky = std::make_shared<Sphere>(p3(0, 0, 0), 500., std::make_shared<Uniform_Material>(1, 0, 0, color(30, 120, 180)));

    objects.push_back(ball);
    objects.push_back(ball2);
    objects.push_back(sky);


    auto box = std::make_shared<Box>(p3(-15, -10, 40), 10., std::make_shared<Uniform_Material>(0.8, 0.3, 0.5, color(0, 180, 0)));
    //auto triangle1 = std::make_shared<Triangle>(p3(0, -15, 25), p3(-10, 0, 25), p3(10, 0, 25), std::make_shared<Uniform_Material>(1, 0.3, 0.8, color(255, 0, 100)));

    objects.push_back(box);

    auto blob = std::make_shared<Blob>(18, 0.5, 0.072, p3(0, -10, 20), std::vector<p3>({p3(-5, -10, 20), p3(5, -10, 20)}), std::make_shared<Uniform_Material>(1, 0.8, 1.3, color(250, 250, 250)));

    //objects.push_back(blob);


    auto camera = Camera(p3(0, 0, 0), p3(-10, 20, 80), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,10);
    auto scene = Scene(objects, lights, camera);

    auto image = scene.compute_image(1024, 1024, 2, false);
    std::cout << "Width: " << image.width() << "\n";
    std::cout << "Height: " << image.height() << "\n";

    image.to_ppm("/home/simon/Documents/Scene.test.ppm");
}
