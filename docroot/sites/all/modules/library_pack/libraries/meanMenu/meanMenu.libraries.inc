<?php

// meanMenu.
$libraries['meanMenu'] = array(
  'name' => 'meanMenu',
  'vendor url' => 'https://github.com/meanthemes/meanMenu',
  'download url' => 'https://github.com/meanthemes/meanMenu',
  'version arguments' => array(
    'file' => 'jquery.meanmenu.js',
    'pattern' => '/v([0-9a-zA-Z\.-]+)/',
  ),
  'files' => array(
    'js' => array(
      'jquery.meanmenu.js',
    ),
    'css' => array(
      'meanmenu.css',
    ),
  ),
  'variants' => array(
    'minified' => array(
      'files' => array(
        'js' => array(
          'jquery.meanmenu.min.js',
        ),
        'css' => array(
          'meanmenu.min.css',
        )
      ),
    ),
  ),
);
