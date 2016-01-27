CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Usage

INTRODUCTION
------------

Current Maintainer: Kristofer Tengström <kristofer@storleden.se>

The site logo variable is used by many contrib modules but the way it is
rendered by default leaves little room for customization. Usually it is 
rendered by the theme in a pre-defined region, linking to the home page and 
displaying the full image without any chance of using image styles. Logo Block 
solves these problems by creating a block that you can put wherever you want, 
with a configuration form where you can select which page (if any) you want 
the image to link to, as well as an optional image style that you want to use 
for the rendering of the logo.

INSTALLATION
------------

This module is installed just like any other Drupal module, for example through
the interface at yoursite.com/admin/modules.

USAGE
-----

Configure the block at
yoursite.com/admin/structure/block/manage/logo_block/logo/configure. Follow 
the instructions for the link field to optionally add a link and an image 
style for the logo.

Note: Due to limitations in Image API, image styles only works for logos
uploaded to the Drupal files directory or a subfolder.
