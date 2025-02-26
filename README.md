# Ray-Tracing-Renderer
This is a tiny renderer based on three famous books.
We use .ppm, a pixel map format to generate images.

## Ray Tracing in One Weekend
Basic classes:
1. vec3: a simple way to express colors, locations and directions
2. ray: shooting from one point to another
3. camera: a positionable camera, allowing one to change its focus
4. hitable: an object that can be hit by a ray and returns hit infomation
    - hitable_list: store various hitables
    - sphere: a sphere hitable object
5. material: descibes how light reflects, scatters and refracts
    - lambertian: light reflects in all directions
    - metal: light obeys standard reflection
    - dielectric: light is allowed to refract

Features:
1. support calculating the process when light hits a hitbale object
2. antialising: by rasing sample rate ro denoise
3. support three ways to deal with light by changing the material of a hitable
4. defoucus blur: by changing the depth of field

## Ray Tracing: The Next Weekend


## Ray Tracing: The Rest of Your Life

