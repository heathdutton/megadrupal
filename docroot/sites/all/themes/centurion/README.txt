/*=======================================================
	CENTURION FRAMEWORK for Drupal 7
	Written by: Justin Hough
=======================================================*/

Centurion is a Drupal template that is built using HTML5,
and CSS3. It was initially designed to allow a website to
auto-size to the browser. Using CSS3 media queries Centurion
can auto-size from a 960px width for a desktop down to 320px
for a mobile device. It was built to be a framework, and allows
for customization outside of the layout.

The basic Centurion Framework is available for download on
Github (https://github.com/jhough10/Centurion)

/*=======================================================
	Features
=======================================================*/

- Framework Grid (960, 768, 480, 320)
- Prebuilt Menu
- Buttons (small, medium, large, includes 6 colors)
- Image Handling
- Text reset, and Heading resizing based on screen size
(currently only works for 320 screen width)

/*=======================================================
	Customization
=======================================================*/

The Centurion Framework is built using CSS3 media queries,
and certain features are customizable based on the desktop
size. For screen sizes smaller than 480, the grid will layout
in a scrollable format from top to bottom of the page.
(Basically, all grids no matter what size will get resized
to a set width, and will stack on top of each other.) This
is done to allow for mobile devices to easily scroll through
the site without having to zoom-in or out to see the content. 

** The example below works off a simple feature that detects
if certain areas of the page are turned on or off depending on
if blocks are present. So if the right bar has no blocks in it,
then the content area will resize to fit the rest of the width.
You can see this by looking at the variable that is being set
in the first portion of the class attribute. Then it it is un
true tenth "centurion_chclass" gets set to null and the extra
space is passed to the content area. (This script is adapted
from the 960 grid theme, which essentially works the same way,
but ideally this only allows the content area to grow not the
left sidebar. Eventually this will feature will expand to
include more options, but for now this is what is available.)

/*=======================================================
	Upcoming Features
=======================================================*/
- Fluid resizing of grid based on active areas
- Right-to-Left language support
- Changeable container size (currently only supports a 12
column layout)

/*=======================================================
	Submit Features
=======================================================*/
If you would like to see new features in the next release of
this theme, please "Create an Issue" and assign the "Category"
to feature request.

Though if you have questions feel free to contact me @
ignotus.scriptor at gmail dot com.
