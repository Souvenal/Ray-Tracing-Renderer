#include "camera.h"

vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),0) - vec3(1,1,0);
    } while (dot(p,p) >= 1.0);
    return p;
}

camera::camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist,
	float t0, float t1) {  // vfov is top to bottom in degrees
	time0 = t0;
	time1 = t1;
	lens_radius = aperture / 2;
	float theta = vfov * M_PI / 180;
	float half_height = tan(theta / 2);
	float half_width = aspect * half_height;
	origin = lookfrom;
	w = unit_vector(lookfrom - lookat);
	u = unit_vector(cross(vup, w));
	v = cross(w, u);
	lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
	horizontal = 2 * half_width * focus_dist * u;
	vertical = 2 * half_height * focus_dist * v;
}

ray camera::get_ray(float s, float t) {
    vec3 rd = lens_radius * random_in_unit_disk();
    vec3 offset = u * rd.x() + v * rd.y();
    // random time
    float time = time0 + drand48() * (time1 - time0);
    return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, time);
}