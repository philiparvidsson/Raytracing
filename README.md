# Learning Raytracing

The original was (and still is - when I have the time!) to create a photorealistic [RAL color](https://en.wikipedia.org/wiki/RAL_colour_standard) visualizer for various objects (specifically bikes, but others as well). 

Starting this project, I had no prior experience with implementing raytracing, but merely a concept understanding of it. Turns out the math is not too complex (e.g. rearranging and solving quadratic polynomials for sphere surfaces). Everything was learned and implemented over the course of a (pretty intense) weekend.

I also made sure the raytracer makes use of multiple CPU cores to minimize renderning time since the process is immenesly computationally intensive. See screenshot below (if I remember correctly, the rendering time is about 40 hours on my dual-core [plus Hyper-Threading] Intel i7-4500U).

**Features**:

* Global illumination
* Color bleeding
* Recursive reflections
* Antialiasing
* Depth-of-field
<img alt="" src="ral-viz/images/rt0.png"/>
