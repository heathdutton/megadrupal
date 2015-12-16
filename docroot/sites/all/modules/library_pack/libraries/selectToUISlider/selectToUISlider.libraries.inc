<?php


// jQuery select-to-ui-slider.
$libraries['selectToUISlider'] = array(
  'name' => 'jQuery Select To UI Slider',
  'vendor url' => 'http://www.filamentgroup.com/examples/slider_v2/FilamentGroup_selectToUISlider.zip',
  'download url' => 'http://www.filamentgroup.com/examples/slider_v2/FilamentGroup_selectToUISlider.zip',
  'version callback' => '_library_pack_force_version',
  'version arguments' => array(
    'file' => 'js/selectToUISlider.jQuery.js',
    'force' => '1.0'
  ),
  'files' => array(
    'js' => array(
      'js/selectToUISlider.jQuery.js',
    ),
    'css' => array(
      'css/ui.slider.extras.css'
    )
  ),

);
