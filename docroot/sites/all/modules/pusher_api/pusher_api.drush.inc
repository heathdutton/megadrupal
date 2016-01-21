<?php

/**
 * @file
 * Drush integration for Pusher API.
 */

function pusher_api_drush_command() {
  $items = array();

  $items['pusher_download'] = array(
    'description' => 'Download Pusher API to libraries.',
    'aliases' => array('pudl', 'pusherdl'),
    'examples' => array(
      'drush pusher-download' =>
        'Downloads pusherapi to sites/all/libraries',
    ),
  );

  return $items;
}

function drush_pusher_api_pusher_download() {
  // generate libraries path to pusher_api
  $path = libraries_get_path('pusher_api');

  // check to see if api already exists.
  if (file_exists($path . '/Pusher.php')) {
    drush_print('Pusher API already exists');
    return 1;
  }

  // Check to see if directory exists.. create if necessary
  if (!file_exists($path)) {
    $mkdir = mkdir($path, 0755, TRUE);
  }

  // download latest
  $file = 'https://github.com/squeeks/Pusher-PHP/blob/master/lib/Pusher.php';
  $f = file_get_contents($file);
  $saved = file_put_contents($path . '/Pusher.php', $f);
  if ($saved) {
    drush_print('Pusher API downloaded.');
  }
  else {
    drush_print('There was some saving Pusher API.');
  }
}