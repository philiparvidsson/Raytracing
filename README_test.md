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

![Eq. 1](https://cdn.rawgit.com/philiparvidsson/Raytracing/master/images/eq1.svg "Eq. 1")

Let **c** and r be the center and radius of the sphere, respectively. For every point in space **q** where the equation holds true, **q** is a point on the sphere:

![Eq. 2a](https://cdn.rawgit.com/philiparvidsson/Raytracing/master/images/eq2a.svg "Eq. 2a")

Or, equivalently with vector notation:

![Eq. 2b](https://cdn.rawgit.com/philiparvidsson/Raytracing/master/images/eq2b.svg "Eq. 2b")

Solving the equation—finding if any point on the line is also a point on the sphere surface:

![Eq. 3a](https://cdn.rawgit.com/philiparvidsson/Raytracing/master/images/eq3a.svg "Eq. 3a")

![Eq. 3b](https://cdn.rawgit.com/philiparvidsson/Raytracing/master/images/eq3b.svg "Eq. 3b")

Simplified final form with vector notation:

![Eq. 4](https://cdn.rawgit.com/philiparvidsson/Raytracing/master/images/eq4.svg "Eq. 4")
