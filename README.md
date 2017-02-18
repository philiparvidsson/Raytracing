# Experimenting with Raytracing

The original idea was (and still is—when I have the time!) to create a photorealistic [RAL color](https://en.wikipedia.org/wiki/RAL_colour_standard) visualizer for various objects (specifically bikes, but others as well). 

Starting this project, I had no prior experience with implementing raytracing, but merely a concept understanding of it. Turns out the math is not too complex (e.g. rearranging and solving quadratic polynomials for sphere surfaces). Everything was learned and implemented over the course of a (pretty intense) weekend.

## Screenshot

The raytracer is multi-threaded makes use of all available CPU cores to minimize rendering time since the process is immenesly computationally intensive. See screenshot below (if I remember correctly, the rendering time is about 40 hours on my dual-core [plus Hyper-Threading] Intel i7-4500U).

<img alt="" src="ral-viz/images/rt0.png"/>

**Features**:

* Global illumination
* Color bleeding
* Recursive reflections
* Antialiasing
* Depth-of-field

## Math

<img alt="" src="images/raytrace.png"/>

Raytracing is a matter of finding the point-of-intersection in 3-space (and, after that, calculating normals, light and shadow rays, reflections etc—not illustrated above!) closes to the eye. In the illustration above, we need to solve an equation for the two functions. Since we're interested in where, in space, the intersection occurs, and since we start with a ray (each pixel rendered starts with a ray being cast in that direction from the camera!), we want then solve the equation for the scalar t. This tells how far along the ray we need to travel to end up at the intersection!

### Solving for t

Let o be the origin (eye) and d the direction of the ray:

![Eq. 1](https://latex.codecogs.com/gif.latex?%5Cvec%7Bp%7D%3D%5Cvec%7Bo%7D&plus;%5Cvec%7Bd%7D%20%5Ccdot%20t "Eq. 1")

Let c and r be the center and radius of the sphere, respectively. For every point in space q where the equation holds true, q is a point on the sphere:

![Eq. 2a](https://latex.codecogs.com/gif.latex?%28q_x-c_x%29%5E2&plus;%28q_y-c_y%29%5E2&plus;%28q_z-c_z%29%5E2%3Dr%5E2 "Eq. 2a")

Or, equivalently with vector notation:

![Eq. 2b](https://latex.codecogs.com/gif.latex?%28%5Cvec%7Bq%7D-%5Cvec%7Bc%7D%29%20%5Ccdot%20%28%5Cvec%7Bq%7D-%5Cvec%7Bc%7D%29%3Dr%5E2 "Eq. 2b")

Solving the equation—finding if any point on the line is also a point on the sphere surface:

![Eq. 3](https://latex.codecogs.com/gif.latex?%5C%5C%20%28o_x&plus;d_xt-c_x%29%5E2&plus;%28o_y&plus;d_yt-c_y%29%5E2&plus;%28o_z&plus;d_zt-c_z%29%5E2%3Dr%5E2%20%5C%5C%20%28%28o_x-c_x%29&plus;d_xt%29%5E2&plus;%28%28o_y-c_y%29&plus;d_yt%29%5E2&plus;%28%28o_z-c_z%29&plus;d_zt%29%5E2-r%5E2%3D0%20%5C%5C%20%5C%5C%20%28o_x-c_x%29%5E2&plus;2%28o_x-c_x%29d_xt&plus;t%5E2d_x%5E2%20&plus;%20%5C%5C%20%28o_y-c_y%29%5E2&plus;2%28o_y-c_y%29d_yt&plus;t%5E2d_y%5E2%20&plus;%20%5C%5C%20%28o_z-c_z%29%5E2&plus;2%28o_z-c_z%29d_yt&plus;t%5E2d_y%5E2%20-%20r%5E2%20%3D%200%20%5C%5C%20%5C%5C%20%28o_x-c_x%29%5E2&plus;%28o_y-c_y%29%5E2&plus;%28o_z-c_z%29%5E2%20&plus;%20%5C%5C%20%28%28o_x-c_x%29d_x&plus;%28o_y-c_y%29d_y&plus;%28o_z-c_z%29d_y%292t%20&plus;%20%5C%5C%20%28d_x%5E2&plus;d_y%5E2&plus;d_z%5E2%29t%5E2%20-%20r%5E2%20%3D%200%20%5C%5C%20%5C%5C%20a%3D%28d_x%5E2&plus;d_y%5E2&plus;d_z%5E2%29%20%5C%5C%20b%3D2%28%28o_x-c_x%29d_x&plus;%28o_y-c_y%29d_y&plus;%28o_z-c_z%29d_y%29%20%5C%5C%20c%3D%28o_x-c_x%29%5E2&plus;%28o_y-c_y%29%5E2&plus;%28o_z-c_z%29%5E2-r%20%5C%5C%20%5C%5C%20t%5E2&plus;%28%5Cdfrac%7Bb%7D%7Ba%7D%29t&plus;%28%5Cdfrac%7Bc%7D%7Ba%7D%29%20%3D%200%20%5C%5C%20%5C%5C%20t%5E2&plus;%28%5Cdfrac%7Bb%7D%7Ba%7D%29t%20%3D%20-%5Cdfrac%7Bc%7D%7Ba%7D%20%5C%5C%20%28t&plus;0.5%5Cdfrac%7Bb%7D%7Ba%7D%29%5E2%20%3D%20-%5Cdfrac%7Bc%7D%7Ba%7D&plus;0.25%28%5Cdfrac%7Bb%7D%7Ba%7D%29%5E2%20%5C%5C%20t&plus;0.5%5Cdfrac%7Bb%7D%7Ba%7D%20%3D%20%5Cpm%20sqrt%28-%5Cdfrac%7Bc%7D%7Ba%7D&plus;0.25%28%5Cdfrac%7Bb%7D%7Ba%7D%29%5E2%29%20%5C%5C%20t%20%3D%20%5Cpm%20sqrt%28-%5Cdfrac%7Bc%7D%7Ba%7D&plus;0.25%28%5Cdfrac%7Bb%7D%7Ba%7D%29%5E2%29%20-%200.5%5Cdfrac%7Bb%7D%7Ba%7D%20%5C%5C "Eq. 3")

Simplified final form with vector notation:

![Eq. 4](https://latex.codecogs.com/gif.latex?%5C%5C%20a%3D%5Cvec%7Bd%7D%20%5Ccdot%20%5Cvec%7Bd%7D%20%5C%5C%20b%3D%5Cvec%7Bd%7D%20%5Ccdot%20%5Cvec%7Bo%7D-%5Cvec%7Bd%7D%20%5Ccdot%20%5Cvec%7Bp%7D%20%5C%5C%20c%3D%5Cvec%7Bo%7D%20%5Ccdot%20%5Cvec%7Bo%7D&plus;%5Cvec%7Bp%7D%20%5Ccdot%20%5Cvec%7Bp%7D-2%28%5Cvec%7Bo%7D%20%5Ccdot%20%5Cvec%7Bp%7D%29-r%5E2%20%5C%5C%20%5C%5C%20t%3D-%5Cdfrac%7Bb%7D%7Ba%7D%20%5Cpm%20%28-%5Cdfrac%7Bc%7D%7Ba%7D&plus;%28%5Cdfrac%7Bb%7D%7Ba%7D%29%5E2%29 "Eq. 4")
