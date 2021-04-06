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
    auto light2 = std::make_shared<Point_Light>(p3(0, -10, 50), 20);
    // lights.push_back(light);
    // lights.push_back(light2);

    auto smooth_light = std::make_shared<Square_Light>(p3(0, 80, 15), 1, 3, 70);
    auto smooth_light2 = std::make_shared<Square_Light>(p3(0, -10, 50), 1, 3, 20);
    lights.push_back(smooth_light);
    lights.push_back(smooth_light2);

    auto mirror = std::make_shared<Plane>(vec3(0, 0, -1), p3(0, 0, 30), std::make_shared<Uniform_Material>(color(150, 150, 150), .4, 0.9, 200));
    auto mirror2 = std::make_shared<Plane>(vec3(0, 0, 1), p3(0, 0, -5), std::make_shared<Uniform_Material>(color(150, 155, 155), .4, 0.9, 200));

    // objects.push_back(mirror);
    // objects.push_back(mirror2);

    auto wall = std::make_shared<Plane>(vec3(0, 0, -1), p3(0, 0, 500), std::make_shared<Uniform_Material>(color(55, 100, 120), 1, 0.1, 0.8));
    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Uniform_Material>(color(222, 184,136), 1, 0.1, .8));
    // objects.push_back(wall);
    objects.push_back(floor);

    auto ball = std::make_shared<Sphere>(p3(-10, 10, 80), 20., std::make_shared<Uniform_Material>(color(100, 100, 100), 1, .7, 10));
    auto ball2 = std::make_shared<Sphere>(p3(10, -10, 30), 5., std::make_shared<Uniform_Material>(color(250, 80, 0), 0, 0, 0.5));
    auto sky = std::make_shared<Sphere>(p3(0, 0, 0), 500., std::make_shared<Uniform_Material>(color(30, 120, 180)));

    objects.push_back(ball);
    objects.push_back(ball2);
    objects.push_back(sky);

    auto box = std::make_shared<Box>(p3(-15, -10, 40), 10., std::make_shared<Uniform_Material>(color(0, 180, 0), 0.8, 0.3, 0.5));
    //auto triangle1 = std::make_shared<Triangle>(p3(0, -15, 25), p3(-10, 0, 25), p3(10, 0, 25), std::make_shared<Uniform_Material>(color(255, 0, 100), 1, 0.3, 0.8));

    // objects.push_back(box);

    auto blob = std::make_shared<Blob>(18, 0.5, 0.072, p3(0, -10, 20), std::vector<p3>({p3(-5, -10, 20), p3(5, -10, 20)}), std::make_shared<Uniform_Material>(color(250, 250, 250), 1, 0.8, 1.3));

    //objects.push_back(blob);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0.3, 0);
    auto scene = Scene(objects, lights, camera);

    auto image = scene.compute_image(1024, 1024, 2, false);
    std::cout << "Width: " << image.width() << "\n";
    std::cout << "Height: " << image.height() << "\n";

    image.to_ppm("../image_test/Scene.test.ppm");
}