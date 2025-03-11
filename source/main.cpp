#include <iostream>
#include <fstream>
#include <getopt.h>
#include "float.h"
#include "camera.h"
#include "hitables.h"
#include "materials.h"
#include "textures.h"


const vec3 color(const ray& r, hitable *world, int depth, int maxDepth) {
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < maxDepth && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth+1, maxDepth);
        }
        else {
            return vec3(0,0,0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

hitable *two_spheres() {
    texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
    int n = 50;
    hitable **list = new hitable*[n + 1];
    list[0] = new sphere(vec3(0, -10, 0), 10, new lambertian(checker));
    list[1] = new sphere(vec3(0, 10, 0), 10, new lambertian(checker));

    return new hitable_list(list, 2);
}

hitable *random_scene() {
    int n = 50000;
    hitable **list = new hitable*[n+1];

    checker_texture *checker = new checker_texture( new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
    list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(checker));
    int i = 1;
    for (int a = -10; a < 10; a++) {
        for (int b = -10; b < 10; b++) {
            float choose_mat = drand48();
            vec3 center(a+0.9*drand48(),0.2,b+0.9*drand48());
            if ((center - vec3(4,0.2,0)).length() > 0.9) {
                if (choose_mat < 0.8) {  // diffuse
                    list[i++] = new moving_sphere(center, center+vec3(0,0.5*drand48(),0), 0, 1.0, 0.2, 
                                                new lambertian(new constant_texture(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))));
                }
                else if (choose_mat < 0.95) {  // metal
                    list[i++] = new sphere(center, 0.2, 
                                            new metal(new constant_texture(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48()))), 0.5*drand48()));
                }
                else {  // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(new constant_texture(vec3(0.7, 0.6, 0.5)), 0.0));
    return new hitable_list(list,i);
}

int parse(int argc, char* argv[], std::string& ppmFileName, int& nx, int& ny, int& ns, int& maxDepth) {
    static struct option long_options[] = {
        {"name", required_argument, 0, 'n'},
        {"x-pixels", required_argument, 0, 'x'},
        {"y-pixels", required_argument, 0, 'y'},
        {"sample-rate", required_argument, 0, 's'},
        {"recursive-depth", required_argument, 0, 'd'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "n:x:y:s:d:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'n':
                ppmFileName = optarg;
                break;
            case 'x':
                nx = std::atoi(optarg);
                break;
            case 'y':
                ny = std::atoi(optarg);
                break;
            case 's':
                ns = std::atoi(optarg);
                break;
            case 'd':
                maxDepth = std::atoi(optarg);
                break;
            case '?':
                std::cerr << "unknown option" << std::endl;
                return 1;
            default:
                std::cerr << "error occurred when parsing argument" << std::endl;
                return 1;
        }
    }

    if (ppmFileName.empty() || nx == -1 || ny == -1 || ns == -1 || maxDepth == -1) {
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    std::string ppmFileName;
    int nx = -1;
    int ny = -1;
    int ns = -1;
    int maxDepth = -1;  // 初始化允许递归深度，默认值为 50

    if (parse(argc, argv, ppmFileName, nx, ny, ns, maxDepth) != 0) {
        std::cerr << "usage: " << argv[0] << \
            " [-n|--name <output.ppm>] \
            [-x|--x-pixels <x pixel>] \
            [-y|--y-pixels <y pixcel>] \
            [-s|--sample-rate <sampleRate>] \
            [-d|--recursive-depth <maxRecursiveDepth>]" << std::endl;
        return 1;
    }

    std::ofstream ppmFile(ppmFileName);
    if (!ppmFile) {
        std::cerr << "unable to open " << ppmFileName << std::endl;
        return 1;
    }

    ppmFile << "P3\n" << nx << " " << ny << "\n255\n";

    const int totalPixels = nx * ny;
    int completedPixels = 0;

    // 开始时间，用于计算剩余时间
    std::clock_t startTime = std::clock();

    hitable *world = random_scene();

    vec3 lookfrom(8, 2, 3);
    vec3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    float aspect = float(nx) / float(ny);
    float dist_to_focus = 10.0;
    float aperture = 0.0;

    camera cam(lookfrom, lookat, vup, 90, aspect, aperture, dist_to_focus, 0.0, 1.0);
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s=0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                // vec3 p = r.point_at_parameter(2.0);
                col += color(r, world, 0, maxDepth);
            }
            col /= float(ns);
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            ppmFile << ir << " " << ig << " " << ib << "\n";

            // 计算进度百分比
            float progress = static_cast<float>(++completedPixels) / totalPixels;

            // 计算已用时间和剩余时间
            std::clock_t currentTime = std::clock();
            double elapsedTime = static_cast<double>(currentTime - startTime) / CLOCKS_PER_SEC;
            double remainingTime = (elapsedTime / progress) - elapsedTime;

            // 进度条长度
            const int barWidth = 50;

            // 输出进度条
            std::cout << "[";
            int pos = barWidth * progress;
            for (int i = 0; i < barWidth; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else std::cout << " ";
            }
            std::cout << "] " << std::fixed << std::setprecision(2) << progress * 100.0 << "% ";
            std::cout << "Elapsed: " << elapsedTime << "s ";
            std::cout << "Remaining: " << remainingTime << "s\r";
            std::cout.flush();
        }
    }
}