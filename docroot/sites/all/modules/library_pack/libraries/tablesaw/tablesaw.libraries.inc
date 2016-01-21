<?php

// Tablesaw
$libraries['tablesaw'] = array(

  'name' => 'Tablesaw',
  'vendor url' => 'https://github.com/filamentgroup/tablesaw',
  'download url' => 'https://github.com/filamentgroup/tablesaw',
  'version arguments' => array(
    'file' => 'dist/tablesaw.js',
    'pattern' => '/Tablesaw - v([0-9a-zA-Z\.-]+)/',
  ),
  'files' => array(
    'js' => array(
      'dist/tablesaw.js',
    ),
    'css' => array(
      'dist/tablesaw.css',
    ),
  ),
  'variants' => array(
    'stackonly' => array(
      'files' => array(
        'js' => array(
          'dist/tablesaw.stackonly.js',
        ),
        'css' => array(
          'dist/tablesaw.stackonly.css',
        ),
      ),
    ),
  ),
);