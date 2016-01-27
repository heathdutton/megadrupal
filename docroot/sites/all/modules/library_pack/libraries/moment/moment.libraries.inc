<?php

// Moment JS
$libraries['moment'] = array(
  'name' => 'Moment JS',
  'vendor url' => 'http://momentjs.com/',
  'download url' => 'https://github.com/moment/moment/',
  'version arguments' => array(
    'file' => 'moment.js',
    'pattern' => '/version : ([0-9a-zA-Z\.-]+)/',
  ),
  'files' => array(
    'js' => array(
      'moment.js',
    ),
  ),
);