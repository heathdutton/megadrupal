<?php
// Matchmedia
$libraries['matchMedia.js'] = array(

  'name' => 'matchMedia.js',
  'vendor url' => 'https://github.com/paulirish/matchMedia.js',
  'download url' => 'https://github.com/paulirish/matchMedia.js',
  'version arguments' => array(
    'file' => 'bower.json',
    'pattern' => '/version": "([0-9a-zA-Z\.-]+)/',
  ),
  'files' => array(
    'js' =>  array(
      'matchMedia.js'=> array(
        'every_page' => TRUE,
        'group' => JS_LIBRARY,
        'preprocess' => 0,
        'scope' => 'header',
        'type' => 'file',
        'weight' => -9999,
      ),
    )
  ),
  'variants' => array(
    'addListener' => array(
      'files' => array(
        'js' => array(
          'matchMedia.js'=> array(
            'every_page' => TRUE,
            'group' => JS_LIBRARY,
            'preprocess' => 0,
            'scope' => 'header',
            'type' => 'file',
            'weight' => -9999,
          ),
          'matchMedia.addListener.js'=> array(
            'every_page' => TRUE,
            'group' => JS_LIBRARY,
            'preprocess' => 0,
            'scope' => 'header',
            'type' => 'file',
            'weight' => -9998,
          ),
        )
      ),
    ),
  ),
);