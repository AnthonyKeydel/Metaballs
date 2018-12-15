
Anthony Keydel - Final Project Checkpoint
 *********************************************************************************************************
 |
 |
 | Marching Cubes + Metaballs
 |
 | based on the methods described
 | (and tables provided) by
 | Paul Bourke at
 | http://paulbourke.net/geometry/polygonise/
 |
 |
 *********************************************************************************************************
Compiling:
~~~~~~~~~~~~~~
	A simple 'make' command should compile my code, I have run into problems where I have to
	do a 'make clean' beforehand.
	
	I changed the variable 'time' to 'gTime' so hopefully that won't colide with any other libraries again

**********************************************************************************************************
Controls:
~~~~~~~~~~~~~~

	*  't'      |  Toggle time-based animation
	*  'l'		|  Toggle between lighting enabled or disabled
	*  'o'      |  Toggle debug overlay (axis, camera, and grid information
	*  'v'      |  Toggle between fill and vvireframe 
	*  'c'		|  Switch between color modes. There are 4 colors, plus a rainbow effect
				|		that gets color value from the surface normals
	*  '+/-' 	|  Increase/decrease the density of the 3-d grid (ranges between 5 and 100)
 		    	
	*'w/a/s/d'	|  Move the controllable ball in the x/z plane
	*  'z/x' 	|  Move the controllable ball up/down the y axis

	*  '7/1' 	|  Increase/decrease the size of the Metaballs
		or   	|
	*  'u/j  	| 

	*  '8/2'	|	Increase/decrease the surface threshold
		or		|
	*  'y/h'	|
					- I wrote my code with a numberpad in mind, but added these alternatives

	* Arrow keys|  Rotate the camera around the box
	* PgUp/PgDwn|  Zoom in or out (change world dim)
	*  '9/3'    |  Zoom in or out (change fov)


