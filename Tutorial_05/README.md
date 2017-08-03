# Tutorial 5 - Shading

<br>
<br>

## About

In this tutorial we shade a Sphere, using two methods, Gouraud Shading and PerPixel shading

<br>
<br>

## Running the code

If you have a driver supporting OpenGL 3.2+, you can run the gouraud
shading code as 
```bash
./05_gouraud 30 
```
here e the parameter 30 specifies the amount of subdivisions to the sphere,
we’ll explain that later. The code will run even without this parameter too.
The value of subdivisions may vary from 10 to 80. The code will run with
less than 10 subdivisions but it will not be a sphere.

<br>
<br>

## Understanding the code

To perform the Shading we would need to pass the normals to the various
vertices in the sphere too. As the color depends on the orientation of vertex
compared to light. Where the vertex lies compared to light. So we add
normals to attributes of our shaders. In order to pass the normals, we create
a normal matrix and add normals of the vertices to it. Now since the sphere
is centered at origin, the normals will be same as the coordinates of the
vertex. So, for every point(x, y, z) we do the following,

```cpp

```
Also, we create a uniform normal matrix, which is used in order to transform
normals. We add the required code for initializing the normalMatrix,
getting its position in shader, and do the following,
```cpp

```
We set the value of normal matrix as transpose of inverse of modelview matrix. Why we do this? To transform normal. Since, vertices have transformed from model space to world space, in doing so they could have been rotated, translated and scaled(depending upon modelview matrix). But the normals are still in model space. We multiply the normals to the above calculated matrix in shader. To ensure that the finally transformed normal is still a unit vector, we use the normalize() function in the shader.

<br>
<br>
### Drawing Sphere
The piece of code for drawing sphere basically uses the polar coordinates of
a sphere to draw the points. If you recall polar coordinates, we can express
x,y,z on surface of a sphere as,

x = radius ∗ cosθ ∗ sinφ
y = radius ∗ sinθ ∗ sinφ
z = radius ∗ cosφ

The variable slices specify the number of different planes on the sphere(including poles) while sectors, specify number of different points on each plane.

How we draw the sphere is simple, in one iteration we plot one point on
one plane and next point on the following plane. Hence making a zig-zag
pattern between two planes. We iterate the value of θ from 0 to π and φ
from 0 to 2π.

```cpp

```
Note that there is an addtional loop at the end, what this loop does is
basically draw circles for the wireframe. That loop draws a circle for each
plane on the sphere.

## Shaders

I highly recommended that you read up the slides on shading and get your
head around the basics of computing the colors. Essentially, in gouraud
shading the color computation is on vertices of polygons, while in per-pixel
shading, the color for each pixel is computed individually, hence a lot more
computation compared to Gouraud shading.

### Gouraud Shading
#### Vertex Shaders

In the main function we start with defining the materials for the surface.
Note that, since we have only one object so we are defining the material
in the shader itself. If you want to create more materials, you can define
them in cpp, and pass them to shader, you can pass different components
separately, or you can pass them as single structure. Same goes for the
lighting also. So the first few lines specify the material diffuse, ambient,
specular and shininess. Essentially the final color is a combination of these
components.

```cpp

```
In the following lines we specify the light position to be at 1.0, 1.0, 1.0.
So, we will cast the light from the point (1.0, 1.0, 1.0). We follow it up by
multiplying the light position to view matrix. We do this so that when we
move the object, the light doesn’t move. As, it is supposed to be stationary.

```cpp

```
The next line is the same as we have been doing in the past, assigning the
vertex positions. The following steps, we compute the diffuse component of
light,
```cpp

```
The first line is already explained before. In the next line we compute the
intensity(in variable dotProduct) of diffuse component of color which is essentially L.N (L is light position and N is Normal). Nextly we make sure that
the intensity is not negative, as if the light doesn’t fall on a vertex we just
set the intensity at that vertex to 0(Black).

Now, we need to compute the specular component only if light falls on
the vertex, otherwise, we can just ignore specular compuation. The following
lines do, the specular computation.

```cpp

```
Now finally we assign the final color of pixel simply as sum of diffuse component(intensity*diffuse) and specular component (spec). We can also add the ambient component to this. Note that we are assigning it to max((intensity * diffuse + spec)*vColor, ambient). This is to make sure that if a pixel is on
screen it gets some color. Next thing to note is that we multiply the vColor
to the computed color, this is a simple blending, we can assign weights of
vColor and get different blending.

```cpp

```

### Per-Pixel Shading
Now the computation of color remains same in per-pixel shading, except the
fact that here it is done in the fragment shader. i.e. We compute color of
each pixel individually

#### Vertex Shader
In the vertex shader we compute the modified normal, and eye as previously
and pass these to fragment shader.

#### Fragment Shaders

Here, initially, we create materials and lights as previously. We follow it by
the diffuse and specular computation. And finally we assign the computed
colors and blend it with input color to the frag shader. We, don’t do any
additional computation here. We just do the same computation as before
and get much better results

<br>
<br>

## Output

You can see the difference in the output of both shading methods. Note that
the specular in the output of gouraud shading seems peculiar. But to if you
see it with the wireframe on, you can see that the specular is being applied
to complete polygons as can be seen in the following output.


// Add Images
