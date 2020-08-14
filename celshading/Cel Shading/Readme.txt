**************************************************************************
**	Cel Shading
**
**	www.paulsprojects.net
**
**	paul@paulsprojects.net
**************************************************************************

Description:

This is a Direct3D conversion of my OpenGL cel shading project. It displays a torus, which is lit by a single light.

The cartoon effect is acheived by using a vertex shader to calculate l dot n, where l is the vector to the light, and n is the normal. This information is then used as a texture lookup into a "sharp lighting" texture. A similar lookup into a "border" texture is done using the vector to the camera. The border texture has black when the dot product is less than 0.25, and white otherwise. The torus is then drawn, modulating the torus color with these two textures. This will produce black around the edges, and the cartoon lighting in the middle.

There are two major differences between this project and my OpenGL version.

Firstly, the OpenGL version provides an option to calculate the l dot n dot product on the CPU, for when vertex programs (shaders) are not supported. In Direct3D, vertex shaders are supported (at least via emulation) on all systems, so only a vertex shader path was provided.

Secondly, the OpenGL version draws the border by drawing the back of the torus in wireframe with thick lines after rendering the front. This produces a much nicer border as there is no "leakage" of the border into the centre of the torus, as can be seen when the torus is at a sharp angle to the viewer. This method does require an extra pass, and is not possible in Direct3D as there is no way to alter the thickness of lines.


Requirements:

2 texture units
Vertex shader 1.0 support


References:

Cel-Shading by Sami "MENTAL" Hamlaoui. From www.gamedev.net


Keys:

F1	-	Take a screenshot
Escape	-	Quit

Use the mouse to spin the object.
