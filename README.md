# 2DAnimator
This program allows creating and animating simple scenes of basic geometric shapes. 
Main goal was to create program that animates 2D objects without causing deformations that could occur because of accumulation of computational inaccuracies.

You can create scene by specifying its elements in scene.txt.
Possible scene elements (with correct syntax) are:
background: path="PATH TO BACKGROUND"
polygon: name="polygon_name" points={{x1, y1}, {x2, y2}, ..., {x_n, y_n}}
rectangle: name="rectangle_name" x=x_val y=y_val width=width_val height=height_val
ellipse: name="ellipse_name" x=x_val y=y_val xRadius=xRad_val yRadius=yRad_val

{0, 0} is top left corner of canvas.
Every value is measured in pixels.

You can create animations in transforms.txt by specifying type of transformation (rotation, translation or scaling), 
target and apropriate parameters.
Possible transformations (with correnct syntax) are:
rotate: target="target_name" angle=angle
translate: target="target_name" x=x_val y=y_val
scale: target="target_name" x=x_scale y=y_scale

Specified parameters are targets of animation so for example translation will end when target translates by x_val and y_val.
Animations are created by applying small transforms untill target is reached.
Angle is measured in radians.
Translation values are measured in pixels.
Scale values are multipliers.
Every aniimation is executed at the same time, it is not possible to queue them up.
