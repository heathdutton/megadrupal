The LightGallery module integrates the jQuery lightgallery plugin with drupal. 
jQuery lightgallery is a customizable, modular, responsive, lightbox gallery plugin for jQuery. 
This module integrates with the views module.

## Dependencies

The LightGallery module currently requires the [Libraries API]
(http://drupal.org/project/libraries) module, the [Views](http://drupal.org/project/views) module 
as well as the jquery update module (http://drupal.org/project/jquery_update). 

## Installation

1. If not already installed, download and install the dependencies above. 
2. Recent versions of the LightGallery have broken backwards compatibility 
with jQuery 1.4.x, so you will have to install the jquery update module and use at least jQuery 1.7.
3. Download the [LightGallery plugin](http://sachinchoolur.github.io/lightGallery/) 
and place the resulting directory into the sites/all/libraries directory. 
Make sure sites/all/libraries/lightgallery/dist/js/lightgallery.min.js is available.
4. Download the LightGallery module and follow the instruction for [installing
contributed modules](http://drupal.org/node/895232).

## Usage

1. When creating a view, select the *LightGallery* format.
2. Click on the *Settings* link, under the **Format** section.
3. Scroll down to the *LightGallery Settings* section.
4. Fill out the settings and apply them to your display.

## Ideas for Contributions 

Patches are always welcome. Some particular features 
that will be implemented in the near feature are:
- Support more Slider LightGallery
- Make cck formatter for LightGallery