<?php

$libraries['lessphp'] = array(
  'name' => 'lessphp',
  'vendor url' => 'http://leafo.net/lessphp/',
  'download url' => 'http://leafo.net/lessphp/',
  'version arguments' => array(
    'file' => 'lessc.inc.php',
    'pattern' => '/lessphp v([0-9a-zA-Z\.-]+)/',
    'lines' => 50,
  ),
  'files' => array(
    'php' => array(
      'lessc.inc.php',
    ),
  ),
);