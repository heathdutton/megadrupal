The Slider Pro module integrates the slider pro library with drupal. 
Slider Pro is a responsive and touch-enabled jQuery slider 
that allows you to create elegant and professionally looking slider.

## Dependencies

The Slider Pro module currently requires the [Libraries API]
(http://drupal.org/project/libraries) module, the [Views](http://drupal.org/project/views) module 
as well as the jquery update module (http://drupal.org/project/jquery_update). 

## Installation

1. If not already installed, download and install the dependencies above. 
2. Recent versions of the Slider Pro plugin have broken backwards compatibility 
with jQuery 1.4.x, so you will have to install the jquery update module and use at least jQuery 1.8.
3. Download the [Slider Pro jquery plugin](https://github.com/bqworks/slider-pro) 
and place the resulting directory into the sites/all/libraries directory. 
Make sure sites/all/libraries/slider-pro/dist/js/jquery.sliderPro.min.js is available.
4. Download the Slider Pro module and follow the instruction for [installing
contributed modules](http://drupal.org/node/895232).

## Usage

1. When creating a view, select the *Slider Pro* format.
2. Click on the *Settings* link, under the **Format** section.
3. Scroll down to the *Slider Pro Settings* section.
4. Fill out the settings and apply them to your display.

## Ideas for Contributions 

Patches are always welcome. Some particular features 
that will be implemented in the near feature are:
- Support layers
- Support more Slider Pro options
- Make cck field for Slider Pro

