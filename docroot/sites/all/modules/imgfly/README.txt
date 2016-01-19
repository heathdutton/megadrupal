Description
-----------
This module generates dynamic images on-the-fly and serves them without saving
them to the disk. This module is useful only when you have a reverse-proxy 
caching service like Varnish or Squid on your sites so the images are served 
from cache.

It also serves images in exactly the right dimensions you need so you 
avoid downloading slightly larger images and scaling them down like with most 
reponsive image solutions.


Prerequisites
-------------
Its advisable to have a reverse proxy solution like Varnish to cache all the
images you serve.

You also need to style your images so that they scale to fit within a parent 
container by adding img { max-width: 100%; } to your stylesheet.
This way the module will figure out the right width necessary and deliver 
exactly the right size of image you need on various layouts.


Installation
------------
To install, copy the imgfly directory and all its contents to your modules
directory.

To enable this module, visit Administration -> Modules, and enable imgfly.


Configuration
-------------
To configure the module go to admin/config/media/imgfly

Specify a suffix that you wish to use for your responsive image styles. The 
default is '_imgfly'. All styles that have this configured suffix in their 
style name will be handled by imgfly and made responsive.

You can also specify a maximum image width and height dimension. This way you 
restrict the maximum dimension generated. This is a security measure
to prevent malicious requests to generate extremely large sizes and thus choke
your server.

When you create your image styles, you just need to make sure you name the style
with the suffix '_imgfly' (or whatever you chose in the configuration) and then 
choose the "Scale" or "Scale and crop" action to scale them to a mobile size 
(eg. 280px width) as the module uses this as the fallback image for users who 
do not have javascript. Thus a mobile version of the image is served on devices
that do not have javascript.

The module automatically figures the width and height necessary for each device
based on the parent container elements dimension and generates the image based
on these parameters.

Troubleshooting
---------------
After installing this module and creating image styles with the right suffix, 
if your images do not show up its most probably to do with your CSS. 
Set all your images to have 100% width in your CSS, and the parent element of 
the image should have a width setting to display images in the sizes you 
prefer for your layout (desktop/tablet/mobile).

Example:
If your markup looks like:
<div class="article">
  My article goes here....
  <img src="someimage.jpg" />
</div>

Then, your css should probably contain something like this:
.article{
  width: 600px;
}
.article img{
  width: 100%;
}

The .article css style definition will probably have different width 
parameters for your desktop.css, tablet.css and mobile.css.
