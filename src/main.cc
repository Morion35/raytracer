#include "type.hh"
#include "scene.hh"
#include "materials.hh"
#include "textures.hh"
#include "objects.hh"
#include "image.hh"
#include "iostream"

using namespace raytracing;

Scene create_scene_1()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto light = std::make_shared<Point_Light>(p3(0, 80, 15), 70);
    auto light2 = std::make_shared<Point_Light>(p3(0, -10, 50), 20);
    lights.push_back(light);
    lights.push_back(light2);

    auto wall = std::make_shared<Plane>(vec3(0, 0, -1), p3(0, 0, 150),std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(30, 120, 180), 1, 0, 0)));
    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Plane_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10),1, 0, 0, 0), 10));
    objects.push_back(wall);
    objects.push_back(floor);

    auto ball = std::make_shared<Sphere>(p3(-10, 10, 80), 20., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(100, 100, 100), .2, .8, 3)));
    objects.push_back(ball);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_2()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto smooth_light = std::make_shared<Square_Light>(p3(0, 30, 40), 1, 3, 50, color(1., 1., 1.));
    lights.push_back(smooth_light);

    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Plane_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10),1, 0, 0, 0), 10));
    objects.push_back(floor);

    auto ball = std::make_shared<Sphere>(p3(-10, 10, 80), 20., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(100, 100, 100), .2, .8, 3)));
    auto ball2 = std::make_shared<Sphere>(p3(10, -10, 50), 5., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 0, 0), 0.1, .9, 9.9, 1, 1.5)));
    auto sky = std::make_shared<Sphere>(p3(0, 0, 0), 500., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(30, 120, 180))));
    objects.push_back(ball);
    objects.push_back(ball2);
    objects.push_back(sky);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_3()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto smooth_light = std::make_shared<Square_Light>(p3(0, 30, 40), 1, 3, 50, color(1., 1., 1.));
    lights.push_back(smooth_light);

    auto bulb = std::make_shared<Sphere>(p3(0, 30, 40), 3., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(1., 1., 1.), 0.0, 1.0, 1, 1, 1.5)));
    objects.push_back(bulb);

    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Plane_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10),1, 0, 0, 0), 10));
    objects.push_back(floor);

    auto ball = std::make_shared<Sphere>(p3(-10, 10, 80), 20., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(100, 100, 100), .2, .8, 3)));
    auto ball2 = std::make_shared<Sphere>(p3(10, -10, 50), 5., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 0, 0), 0.1, .9, 9.9, 1, 1.5)));
    auto sky = std::make_shared<Sphere>(p3(0, 0, 0), 500., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(30, 120, 180))));
    objects.push_back(ball);
    objects.push_back(ball2);
    objects.push_back(sky);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_4()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto smooth_light = std::make_shared<Square_Light>(p3(0, 30, 40), 1, 3, 50, color(1., 1., 1.));
    lights.push_back(smooth_light);

    auto bulb = std::make_shared<Sphere>(p3(0, 30, 40), 3., std::make_shared<Sphere_Texture>(std::make_shared<Uniform_Material>(color(1., 1., 1.), 0.0, 1.0, 1, 1, 1.5), 10));
    objects.push_back(bulb);

    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Plane_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10),1, 0, 0, 0), 10));
    objects.push_back(floor);

    auto sky = std::make_shared<Sphere>(p3(0, 0, 0), 500., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(30, 120, 180))));
    objects.push_back(sky);

    auto ball = std::make_shared<Sphere>(p3(-10, 10, 80), 20., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(100, 100, 100), .2, .8, 3)));
    auto ball2 = std::make_shared<Sphere>(p3(10, -10, 50), 5., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 0, 0), 0.1, .9, 9.9, 1, 1.5)));
    objects.push_back(ball);
    objects.push_back(ball2);

    auto box = std::make_shared<Box>(p3(-15, -9.9, 30), 10, vec3(0, 1, 0), vec3(1, 0, 0), std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 180, 180), 1, 0., 0.3, 0.0, 1.0)));
    objects.push_back(box);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_5()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto smooth_light = std::make_shared<Square_Light>(p3(0, 30, 40), 1, 3, 500, color(1., 1., 1.));
    lights.push_back(smooth_light);

    auto bulb = std::make_shared<Sphere>(p3(0, 30, 40), 3., std::make_shared<Sphere_Texture>(std::make_shared<Uniform_Material>(color(1., 1., 1.), 0.0, 1.0, 1, 1, 1.5), 10));
    objects.push_back(bulb);

    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Plane_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10),1, 0, 0, 0), 10));
    objects.push_back(floor);

    auto cylinder = std::make_shared<Cylinder>(5, p3(1, 0, 50), p3(1, 10, 70), std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(150, 150,150), 1, 0, 0, 0)));
    objects.push_back(cylinder);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_6()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto smooth_light = std::make_shared<Square_Light>(p3(0, 30, 40), 1, 3, 500, color(1., 1., 1.));
    lights.push_back(smooth_light);

    auto bulb = std::make_shared<Sphere>(p3(0, 30, 40), 3., std::make_shared<Sphere_Texture>(std::make_shared<Uniform_Material>(color(1., 1., 1.), 0.0, 1.0, 1, 1, 1.5), 10));
    objects.push_back(bulb);

    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Plane_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10),1, 0, 0, 0), 10));
    objects.push_back(floor);

    auto box = std::make_shared<Box>(p3(0, 0, 50), 10, vec3(0, 1, 1), vec3(1, 1, -.5), std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 180, 180), 1, 0., 0.3, 0.0, 1.0)));
    objects.push_back(box);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_mirror()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto light = std::make_shared<Point_Light>(p3(0, 80, 15), 70);
    lights.push_back(light);

    auto mirror = std::make_shared<Plane>(vec3(0, 0, -1), p3(0, 0, 30), std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(150, 150, 150), .4, 0.9, 200)));
    auto mirror2 = std::make_shared<Plane>(vec3(0, 0, 1), p3(0, 0, -5), std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(150, 155, 155), .4, 0.9, 200)));
    objects.push_back(mirror);
    objects.push_back(mirror2);

    auto ball = std::make_shared<Sphere>(p3(-10, 10, 80), 20., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(100, 100, 100), .2, .8, 3)));
    objects.push_back(ball);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_blob()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto light = std::make_shared<Point_Light>(p3(0, 80, 15), 70);
    auto light2 = std::make_shared<Point_Light>(p3(0, -10, 50), 20);
    lights.push_back(light);
    lights.push_back(light2);

    auto wall = std::make_shared<Plane>(vec3(0, 0, -1), p3(0, 0, 150), std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(30, 120, 180), 1, 0, 0)));
    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Plane_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10),1, 0, 0, 0), 10));
    objects.push_back(wall);
    objects.push_back(floor);

    auto ball = std::make_shared<Sphere>(p3(-10, 10, 80), 20., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(100, 100, 100), .2, .8, 3)));
    objects.push_back(ball);

    auto blob = std::make_shared<Blob>(18, 0.5, 0.072, p3(0, -10, 20), std::vector<p3>({p3(-5, -10, 20), p3(5, -10, 20)}), std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(250, 250, 250), 1, 0.8, 1.3)));
    objects.push_back(blob);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_texture_1()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto smooth_light = std::make_shared<Square_Light>(p3(0, 30, 40), 1, 3, 50, color(1., 1., 1.));
    lights.push_back(smooth_light);

    auto bulb = std::make_shared<Sphere>(p3(0, 30, 40), 3., std::make_shared<Sphere_Texture>(std::make_shared<Uniform_Material>(color(1., 1., 1.), 0.0, 1.0, 1, 1, 1.5), 10));
    objects.push_back(bulb);

    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Plane_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10),1, 0, 0, 0), 10));
    objects.push_back(floor);

    auto sky = std::make_shared<Sphere>(p3(0, 0, 0), 500., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(30, 120, 180))));
    objects.push_back(sky);

    auto ball_paved = std::make_shared<Sphere>(p3(-10, 10, 80), 20, std::make_shared<Sphere_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10), 1, 0, 0, 0), 1));
    auto ball2 = std::make_shared<Sphere>(p3(10, -10, 50), 5., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 0, 0), 0.1, .9, 9.9, 1, 1.5)));
    objects.push_back(ball_paved);
    objects.push_back(ball2);

    auto box = std::make_shared<Box>(p3(-15, -9.9, 30), 10., vec3(0, 1, 0), vec3(1, 0, 0), std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 180, 180), 1, 0., 0.3, 0.0, 1.0)));
    objects.push_back(box);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_texture_2()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto smooth_light = std::make_shared<Square_Light>(p3(0, 30, 40), 1, 3, 50, color(1., 1., 1.));
    lights.push_back(smooth_light);

    auto bulb = std::make_shared<Sphere>(p3(0, 30, 40), 3., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(1., 1., 1.), 0.0, 1.0, 1, 1, 1.5)));
    objects.push_back(bulb);

    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Plane_Texture>(std::make_shared<Sin_Paved_Material>(color(255, 255,255), color(60, 20,10),1, 0, 0, 0), 10));
    objects.push_back(floor);

    auto sky = std::make_shared<Sphere>(p3(0, 0, 0), 500., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(30, 120, 180))));
    objects.push_back(sky);

    auto earth = std::make_shared<Sphere>(p3(-10, 10, 80), 20, std::make_shared<Sphere_Texture>(std::make_shared<Ppm_Material>("../image_test/texture_earth.ppm", 1, 0, 0, 0), 45));
    auto ball2 = std::make_shared<Sphere>(p3(10, -10, 50), 5., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 0, 0), 0.1, .9, 9.9, 1, 1.5)));
    objects.push_back(earth);
    objects.push_back(ball2);

    auto box = std::make_shared<Box>(p3(-15, -9.9, 30), 10., vec3(0, 1, 0), vec3(1, 0, 0), std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 180, 180), 1, 0., 0.3, 0.0, 1.0)));
    objects.push_back(box);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_texture_3()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto smooth_light = std::make_shared<Square_Light>(p3(0, 30, 40), 1, 3, 500, color(1., 1., 1.));
    lights.push_back(smooth_light);

    auto bulb = std::make_shared<Sphere>(p3(0, 30, 40), 3., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(1., 1., 1.), 0.0, 1.0, 1, 1, 1.5)));
    objects.push_back(bulb);

    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Plane_Texture>(std::make_shared<Ppm_Material>("../image_test/texture_water.ppm", 1, 0, 0, 0), 100));
    // auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Cube_Material>(color(255, 255, 255), color(255, 0, 0), color(255, 255, 0), color(0, 255, 0), color(0, 255, 255), 1, 1, 0, 0));
    objects.push_back(floor);

    auto camera = Camera(p3(0, 10, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_texture_4()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto smooth_light = std::make_shared<Square_Light>(p3(0, 30, 40), 1, 3, 50, color(1., 1., 1.));
    lights.push_back(smooth_light);

    auto bulb = std::make_shared<Sphere>(p3(0, 30, 40), 3., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(1., 1., 1.), 0.0, 1.0, 1, 1, 1.5)));
    objects.push_back(bulb);

    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Plane_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10), 1, 0, 0, 0), 10));
    objects.push_back(floor);

    auto sky = std::make_shared<Sphere>(p3(0, 0, 0), 500., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(30, 120, 180))));
    objects.push_back(sky);

    auto ball = std::make_shared<Sphere>(p3(-10, 10, 80), 20, std::make_shared<Sphere_Texture>(std::make_shared<Ring_Material>(color(255, 255,255), color(60, 20,10), 1, 0, 0, 0), 1));
    auto ball2 = std::make_shared<Sphere>(p3(10, -10, 50), 5., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 0, 0), 0.1, .9, 9.9, 1, 1.5)));
    objects.push_back(ball);
    objects.push_back(ball2);

    auto box = std::make_shared<Box>(p3(-15, -9.9, 30), 10., vec3(0, 1, 0), vec3(1, 0, 0), std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 180, 180), 1, 0., 0.3, 0.0, 1.0)));
    objects.push_back(box);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_texture_5()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto smooth_light = std::make_shared<Square_Light>(p3(0, 30, 40), 1, 3, 500, color(1., 1., 1.));
    auto smooth_light2 = std::make_shared<Square_Light>(p3(0, 30, 0), 1, 3, 500, color(1., 1., 1.));
    lights.push_back(smooth_light);
    lights.push_back(smooth_light2);

    auto bulb = std::make_shared<Sphere>(p3(0, 30, 40), 3., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(1., 1., 1.), 0.0, 1.0, 1, 1, 1.5)));
    objects.push_back(bulb);

    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Plane_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10),1, 0, 0, 0), 10));
    objects.push_back(floor);

    // auto box = std::make_shared<Box>(p3(-15, -9.9, 30), 10., vec3(0, 1, 0), vec3(1, 0, 0), std::make_shared<Box_Texture>(std::make_shared<Cube_Material>(color(255, 255, 255), color(255, 0, 0), color(255, 255, 0), color(0, 255, 0), color(0, 255, 255), 1, 0, 0), 10));
    auto box = std::make_shared<Box>(p3(-15, -9.9, 30), 10., vec3(0, 1, 0), vec3(1, 0, 0), std::make_shared<Box_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(0, 255,255),1, 0, 0, 0), 5));
    objects.push_back(box);

    auto ball_paved = std::make_shared<Sphere>(p3(-10, 10, 80), 20, std::make_shared<Sphere_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10), 1, 0, 0, 0), 3));
    auto ball2 = std::make_shared<Sphere>(p3(10, -10, 50), 5., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 0, 0), 0.1, .9, 9.9, 1, 1.5)));
    objects.push_back(ball_paved);
    objects.push_back(ball2);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_texture_6()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto smooth_light = std::make_shared<Square_Light>(p3(0, 30, 40), 1, 3, 500, color(1., 1., 1.));
    auto smooth_light2 = std::make_shared<Square_Light>(p3(0, 30, 0), 1, 3, 500, color(1., 1., 1.));
    lights.push_back(smooth_light);
    lights.push_back(smooth_light2);

    auto bulb = std::make_shared<Sphere>(p3(0, 30, 40), 3., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(1., 1., 1.), 0.0, 1.0, 1, 1, 1.5)));
    objects.push_back(bulb);

    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Plane_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10),1, 0, 0, 0), 10));
    objects.push_back(floor);

    auto box = std::make_shared<Box>(p3(-15, -9.9, 30), 10., vec3(0, 1, 0), vec3(1, 0, 0), std::make_shared<Box_Texture>(std::make_shared<Ppm_Material>("../image_test/texture_earth.ppm", 1, 0, 0, 0, 0), 10));
    objects.push_back(box);

    auto ball_paved = std::make_shared<Sphere>(p3(-10, 10, 80), 20, std::make_shared<Sphere_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10), 1, 0, 0, 0), 2));
    auto ball2 = std::make_shared<Sphere>(p3(10, -10, 50), 5., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 0, 0), 0.1, .9, 9.9, 1, 1.5)));
    objects.push_back(ball_paved);
    objects.push_back(ball2);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_texture_7()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto smooth_light = std::make_shared<Square_Light>(p3(0, 30, 40), 1, 3, 500, color(1., 1., 1.));
    lights.push_back(smooth_light);

    auto bulb = std::make_shared<Sphere>(p3(0, 30, 40), 3., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(1., 1., 1.), 0.0, 1.0, 1, 1, 1.5)));
    objects.push_back(bulb);

    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Plane_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10), 1, 0, 0, 0), 10));
    objects.push_back(floor);

    auto cylinder = std::make_shared<Cylinder>(3, p3(0, -14.5, 20), p3(0, -5, 20), std::make_shared<Cylindre_Texture>(std::make_shared<Paved_Material>(color(255, 255,255), color(60, 20,10), 1, 0, 0, 0), 5));
    objects.push_back(cylinder);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

Scene create_scene_skybox_1()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto box = std::make_shared<Box>(p3(0, 0, 0), 100., vec3(0, 1, 0), vec3(1, 0, 0), std::make_shared<Skybox_Texture>(
            std::make_shared<Ppm_Material>("../skybox/field-skyboxes/FishPond/negx.ppm", 1, 0, 0, 0, 0),
            std::make_shared<Ppm_Material>("../skybox/field-skyboxes/FishPond/posx.ppm", 1, 0, 0, 0, 0),
            std::make_shared<Ppm_Material>("../skybox/field-skyboxes/FishPond/posz.ppm", 1, 0, 0, 0, 0),
            std::make_shared<Ppm_Material>("../skybox/field-skyboxes/FishPond/negz.ppm", 1, 0, 0, 0, 0),
            std::make_shared<Ppm_Material>("../skybox/field-skyboxes/FishPond/posy.ppm", 1, 0, 0, 0, 0),
            std::make_shared<Ppm_Material>("../skybox/field-skyboxes/FishPond/negy.ppm", 1, 0, 0, 0, 0),
                    100));
    objects.push_back(box);

    auto ball = std::make_shared<Sphere>(p3(0, 0, 20), 5., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 0, 0), 0.1, .9, 9.9 )));
    objects.push_back(ball);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera, 1);
}

Scene create_scene_shadow_1()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    auto smooth_light = std::make_shared<Square_Light>(p3(0, 30, 40), 1, 3, 500, color(1., 1., 1.));
    lights.push_back(smooth_light);

    auto bulb = std::make_shared<Sphere>(p3(0, 30, 40), 3., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(0, 255, 255), .3, 0, 0, 1, 1.5)));
    objects.push_back(bulb);

    auto floor = std::make_shared<Plane>(vec3(0, 1, 0), p3(0, -15, 0), std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(255, 255,255), 1, 0, 0, 0)));
    objects.push_back(floor);

    auto ball = std::make_shared<Sphere>(p3(0, -10, 40), 5., std::make_shared<Uniform_Texture>(std::make_shared<Uniform_Material>(color(255, 255, 0), .5, 0, 0, 0.7, 1.5)));
    objects.push_back(ball);

    auto camera = Camera(p3(0, 0, 0), p3(0, 0, 15), vec3(0, 1, 0), M_PI_4 * 1, M_PI_4 * 1,15, 0, 0);
    return Scene(objects, lights, camera);
}

int main()
{
    auto scene = create_scene_texture_7();

    auto image = scene.compute_image(1024, 1024, 3, false);
    std::cout << "Width: " << image.width() << "\n";
    std::cout << "Height: " << image.height() << "\n";

    image.to_ppm("../image_test/Scene.test.ppm");
}