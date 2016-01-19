<?php

// Picture fill
$libraries['picturefill'] = array(
  'name' => 'picturefill',
  'vendor url' => 'https://github.com/scottjehl/picturefill',
  'download url' => 'https://github.com/scottjehl/picturefill',
  'version arguments' => array(
    'file' => 'dist/picturefill.js',
    'pattern' => '/Picturefill - v([0-9a-zA-Z\.-]+)/',
  ),
  'files' => array(
    'js' => array(
      'dist/picturefill.js',
    ),
  ),
  'variants' => array(
    'minified' => array(
      'files' => array(
        'js' => array(
          'dist/picturefill.min.js',
        ),
      ),
    ),
  ),
);
