#include <iostream>
#include <fstream>
#include <getopt.h>
#include "float.h"
#include "camera.h"
#include "scenario.h"
#include "omp.h"

const vec3 color(const ray& r, hitable *world, int depth, int maxDepth) {
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if (depth < maxDepth && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return emitted + attenuation*color(scattered, world, depth+1, maxDepth);
        }
        else {
            return emitted;
        }
    }
    // else {
    //     vec3 unit_direction = unit_vector(r.direction());
    //     float t = 0.5*(unit_direction.y() + 1.0);
    //     return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    // }
    else {
        return vec3(0, 0, 0); // Return black if no hit
    }
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

    // hitable *world = random_scene();
    // hitable *world = two_spheres();
    // hitable *world = two_perlin_spheres();
    // hitable *world = simple_light();
    hitable *world = cornell_box();

    // vec3 lookfrom(13, 2, 3);
    // vec3 lookat(0, 0, 0);
    // vec3 vup(0, 1, 0);
    // float vfov = 20.0;
    // float dist_to_focus = 10.0; 
    // float aperture = 0.0;
    vec3 lookfrom(278, 278, -800);
    vec3 lookat(278,278,0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    vec3 vup(0, 1, 0);
    float aspect = float(nx) / float(ny);

    camera cam(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus, 0.0, 1.0);
    // std::cout << "Using " << omp_get_max_threads() << " threads." << std::endl;
    // #pragma omp parallel for schedule(dynamic)
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

            // #pragma omp critical
            // {
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
        // }
        }
    }
}