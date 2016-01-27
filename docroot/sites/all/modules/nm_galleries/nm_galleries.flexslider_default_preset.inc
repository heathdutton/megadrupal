<?php
/**
 * @file
 * fuck.flexslider_default_preset.inc
 */

/**
 * Implements hook_flexslider_default_presets().
 */
function nm_galleries_flexslider_default_presets() {
  $export = array();

  $preset = new stdClass();
  $preset->disabled = FALSE; /* Edit this to true to make a default preset disabled initially */
  $preset->api_version = 1;
  $preset->name = 'nodemaker_full';
  $preset->title = 'NodeMaker Full';
  $preset->theme = 'classic';
  $preset->imagestyle_normal = 'nodemaker_gallery_large';
  $preset->options = array(
    'animation' => 'fade',
    'animationDuration' => 500,
    'slideDirection' => 'horizontal',
    'slideshow' => 1,
    'slideshowSpeed' => 5000,
    'animationLoop' => 1,
    'randomize' => 0,
    'slideToStart' => 0,
    'directionNav' => 1,
    'controlNav' => 0,
    'keyboardNav' => 1,
    'mousewheel' => 0,
    'prevText' => 'Previous',
    'nextText' => 'Next',
    'pausePlay' => 0,
    'pauseText' => 'Pause',
    'playText' => 'Play',
    'pauseOnAction' => 1,
    'controlsContainer' => '.flex-nav-container',
    'manualControls' => '',
  );
  $export['nodemaker_full'] = $preset;
  
  $preset = new stdClass();
  $preset->disabled = FALSE; /* Edit this to true to make a default preset disabled initially */
  $preset->api_version = 1;
  $preset->name = 'nodemaker_narrow';
  $preset->title = 'NodeMaker Narrow';
  $preset->theme = 'classic';
  $preset->imagestyle_normal = 'nodemaker_gallery_narrow';
  $preset->options = array(
    'animation' => 'fade',
    'animationDuration' => 500,
    'slideDirection' => 'horizontal',
    'slideshow' => 1,
    'slideshowSpeed' => 5000,
    'animationLoop' => 1,
    'randomize' => 0,
    'slideToStart' => 0,
    'directionNav' => 1,
    'controlNav' => 0,
    'keyboardNav' => 1,
    'mousewheel' => 0,
    'prevText' => 'Previous',
    'nextText' => 'Next',
    'pausePlay' => 0,
    'pauseText' => 'Pause',
    'playText' => 'Play',
    'pauseOnAction' => 1,
    'controlsContainer' => '.flex-nav-container',
    'manualControls' => '',
  );
  $export['nodemaker_narrow'] = $preset;
  
  $preset = new stdClass();
  $preset->disabled = FALSE; /* Edit this to true to make a default preset disabled initially */
  $preset->api_version = 1;
  $preset->name = 'nodemaker_square';
  $preset->title = 'NodeMaker Square';
  $preset->theme = 'classic';
  $preset->imagestyle_normal = 'nodemaker_gallery_square';
  $preset->options = array(
    'animation' => 'fade',
    'animationDuration' => 500,
    'slideDirection' => 'horizontal',
    'slideshow' => 1,
    'slideshowSpeed' => 5000,
    'animationLoop' => 1,
    'randomize' => 0,
    'slideToStart' => 0,
    'directionNav' => 1,
    'controlNav' => 0,
    'keyboardNav' => 1,
    'mousewheel' => 0,
    'prevText' => 'Previous',
    'nextText' => 'Next',
    'pausePlay' => 0,
    'pauseText' => 'Pause',
    'playText' => 'Play',
    'pauseOnAction' => 1,
    'controlsContainer' => '.flex-nav-container',
    'manualControls' => '',
  );
  $export['nodemaker_square'] = $preset;

  return $export;
}
