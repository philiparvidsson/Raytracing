# Experimenting with Raytracing

The original idea was (and still is—when I have the time!) to create a photorealistic [RAL color](https://en.wikipedia.org/wiki/RAL_colour_standard) visualizer for various objects (specifically bikes, but others as well).

Starting this project, I had no prior experience with implementing raytracing, but merely a concept understanding of it. Turns out the math is not too complex (e.g. rearranging and solving quadratic polynomials for sphere surfaces). Everything was learned and implemented over the course of a (pretty intense) weekend.

## Screenshots

The raytracer is multi-threaded and makes use of all available CPU cores to minimize rendering time since the process is immensely computationally intensive. See screenshot below (if I remember correctly, the rendering time is about 40 hours on my dual-core [plus Hyper-Threading] Intel i7-4500U).

<img alt="" src="ral-viz/images/rt0.png"/>

Below is a simple [Cornell box](https://en.wikipedia.org/wiki/Cornell_box) render containing a few spheres to show global illumination and color bleeding effects, taking about 90 minutes to render on the same hardware; much faster since there are no recursive reflections!

<img alt="" src="ral-viz/images/rt1.png"/>

## Features

* Global illumination
* Color bleeding
* Recursive reflections
* Antialiasing (using a box filter)
* Depth-of-field (using a physically based [aperture](https://en.wikipedia.org/wiki/Aperture) model)
* Phong shading model
* Monte Carlo integration

## How does it work?

<img alt="" src="images/raytrace.png"/>

Raytracing is a matter of finding the point-of-intersection in 3-space (and, after that, calculating normals, light and shadow rays, reflections etc—not illustrated above!) closest to the eye. In the illustration above, we need to solve an equation for the two functions. Since we're interested in where, in space, the intersection between the line and sphere occurs, and since we start with a ray (each pixel rendered starts with a ray being cast in that direction from the eye/camera!), we want to solve the equation for the scalar t. This tells us how far along the ray we need to travel to end up at the intersection!

### Math

Let **o** be the origin (eye) and **d** the direction of the ray:

![Eq. 1](https://latex.codecogs.com/svg.latex?%5Cvec%7Bp%7D%3D%5Cvec%7Bo%7D&plus;%5Cvec%7Bd%7D%20%5Ccdot%20t "Eq. 1")

Let **c** and r be the center and radius of the sphere, respectively. For every point in space **q** where the equation holds true, **q** is a point on the sphere:

![Eq. 2a](https://latex.codecogs.com/svg.latex?%28q_x-c_x%29%5E2&plus;%28q_y-c_y%29%5E2&plus;%28q_z-c_z%29%5E2%3Dr%5E2 "Eq. 2a")

Or, equivalently with vector notation:

![Eq. 2b](https://latex.codecogs.com/svg.latex?%28%5Cvec%7Bq%7D-%5Cvec%7Bc%7D%29%20%5Ccdot%20%28%5Cvec%7Bq%7D-%5Cvec%7Bc%7D%29%3Dr%5E2 "Eq. 2b")

Solving the equation—finding if any point on the line is also a point on the sphere surface:

![Eq. 3](https://latex.codecogs.com/svg.latex?%5Cbegin%7Balign*%7D%20%26%28o_x&plus;d_xt-c_x%29%5E2%26%20%26&plus;%20%26%28o_y&plus;d_yt-c_y%29%5E2%26%20%26&plus;%20%26%5C%20%5C%20%28o_z&plus;d_zt-c_z%29%5E2%26%20%26%3D%20r%5E2%20%5C%5C%20%26%28o_x-c_x&plus;d_xt%29%5E2%26%20%26&plus;%20%26%28o_y-c_y&plus;d_yt%29%5E2%26%20%26&plus;%20%26%5C%20%5C%20%28o_z-c_z&plus;d_zt%29%5E2%20-r%5E2%26%20%26%3D%200%20%5Cend%7Balign*%7D%20%5C%5C%20%5C%5C%20%5Cbegin%7Balign*%7D%20%26%28o_x-c_x%29%5E2%20%26%20%26&plus;%20%26%202t%28o_x-c_x%29d_x%20%26%20%26&plus;%20%26%5C%20%5C%20t%5E2d_x%5E2%20%26&plus;%20%26%20%5C%5C%20%26%28o_y-c_y%29%5E2%20%26%20%26&plus;%20%26%202t%28o_y-c_y%29d_y%20%26%20%26&plus;%20%26%5C%20%5C%20t%5E2d_y%5E2%20%26&plus;%20%26%5C%5C%20%26%28o_z-c_z%29%5E2%20%26%20%26&plus;%20%26%202t%28o_z-c_z%29d_z%20%26%20%26&plus;%20%26%5C%20%5C%20t%5E2d_z%5E2%20%26-%20%26%20%26%20r%5E2%20%3D%20%5C%200%20%26%20%5Cend%7Balign*%7D%20%5C%5C%20%5C%5C%20%5Cbegin%7Balign*%7D%20%26t%5E2%20%28%26%26%5Cphantom%7B%28o_x-c_x%29%7Dd_x%5E2%26%20%26&plus;%20%26%26%5Cphantom%7B%28o_y-c_y%29%7Dd_y%5E2%26%20%26&plus;%20%26%26%5Cphantom%7B%28o_z-c_z%29%7Dd_z%5E2%26%29%20%26%26&plus;%20%5C%5C%20%262t%20%28%26%26%28o_x-c_x%29d_x%26%20%26&plus;%20%26%26%28o_y-c_y%29d_y%26%20%26&plus;%20%26%26%28o_z-c_z%29d_z%26%29%20%26%26&plus;%20%5C%5C%20%26%5C%20%26%26%28o_x-c_x%29%5E2%26%20%26&plus;%20%26%26%28o_y-c_y%29%5E2%26%20%26&plus;%20%26%26%28o_z-c_z%29%5E2%26%20%26%26-%20%26%26r%5E2%20%3D%200%20%5Cend%7Balign*%7D%20%5C%5C%20%5C%5C%20%5Cbegin%7Balign*%7D%20a%26%3D%28d_x%5E2&plus;d_y%5E2&plus;d_z%5E2%29%20%5C%5C%20b%26%3D2%28%28o_x-c_x%29d_x&plus;%28o_y-c_y%29d_y&plus;%28o_z-c_z%29d_z%29%20%5C%5C%20c%26%3D%28o_x-c_x%29%5E2&plus;%28o_y-c_y%29%5E2&plus;%28o_z-c_z%29%5E2-r%5E2%20%5Cend%7Balign*%7D "Eq. 3")

![Eq. 3 cont.](https://latex.codecogs.com/svg.latex?%5Cbegin%7Balign*%7D%20t%5E2&plus;%20%5Cleft%28%20%5Cdfrac%7Bb%7D%7Ba%7D%20%5Cright%29t&plus;%5Cfrac%7Bc%7D%7Ba%7D%20%26%3D%200%20%5C%5C%20t%5E2&plus;%20%5Cleft%28%20%5Cdfrac%7Bb%7D%7Ba%7D%20%5Cright%29t%20%26%3D%20-%5Cdfrac%7Bc%7D%7Ba%7D%20%5C%5C%20%5Cleft%28%20t&plus;%5Cfrac%7Bb%7D%7B2a%7D%20%5Cright%29%5E2%20%26%3D%20-%5Cfrac%7Bc%7D%7Ba%7D&plus;%5Cleft%28%5Cfrac%7Bb%7D%7B2a%7D%5Cright%29%5E2%20%5C%5C%20t&plus;%5Cfrac%7Bb%7D%7B2a%7D%20%26%3D%20%5Cpm%20%5Csqrt%7B-%5Cfrac%7Bc%7D%7Ba%7D&plus;%5Cleft%28%20%5Cfrac%7Bb%7D%7B2a%7D%20%5Cright%29%5E2%7D%20%5C%5C%20t%20%26%3D%20%5Cpm%20%5Csqrt%7B-%5Cdfrac%7Bc%7D%7Ba%7D&plus;%5Cleft%28%20%5Cdfrac%7Bb%7D%7B2a%7D%20%5Cright%29%5E2%7D%20-%20%5Cdfrac%7Bb%7D%7B2a%7D%20%5Cend%7Balign*%7D "Eq. 3 cont.")

Simplified final form with vector notation:

![Eq. 4](https://latex.codecogs.com/svg.latex?%5C%5C%20i%3D%5Cvec%7Bd%7D%20%5Ccdot%20%5Cvec%7Bd%7D%20%5C%5C%20j%3D%5Cvec%7Bd%7D%20%5Ccdot%20%5Cvec%7Bo%7D-%5Cvec%7Bd%7D%20%5Ccdot%20%5Cvec%7Bc%7D%20%5C%5C%20k%3D%5Cvec%7Bo%7D%20%5Ccdot%20%5Cvec%7Bo%7D%20&plus;%20%5Cvec%7Bc%7D%20%5Ccdot%20%5Cvec%7Bc%7D-2%5Cvec%7Bo%7D%20%5Ccdot%20%5Cvec%7Bc%7D-r%5E2%20%5C%5C%20%5C%5C%20t%3D-%5Cdfrac%7Bj%7D%7Bi%7D%20%5Cpm%20%5Csqrt%7B-%5Cdfrac%7Bk%7D%7Bi%7D%20&plus;%20%5Cleft%28%5Cdfrac%7Bj%7D%7Bi%7D%5Cright%29%5E2%7D "Eq. 4")
