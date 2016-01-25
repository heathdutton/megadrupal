<?php

$libraries['cycle2.carousel'] = array(

  'name' => 'jQuery Cycle2',
  'vendor url' => 'http://jquery.malsup.com/cycle2',
  'download url' => 'http://jquery.malsup.com/cycle2/download/',
  'version arguments' => array(
    'file' => 'jquery.cycle2.carousel.js',
    'pattern' => '/version: ([0-9a-zA-Z\.-]+)/',
  ),
  'files' => array(
    'js' => array(
      'jquery.cycle2.carousel.js',
    ),
  ),
  'variants' => array(
    'minified' => array(
      'files' => array('js' => array(
        'jquery.cycle2.carousel.min.js'
      ),
      ),
    ),
  ),
);