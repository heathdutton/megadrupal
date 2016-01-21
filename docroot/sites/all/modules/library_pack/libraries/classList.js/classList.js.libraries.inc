<?php

// classList.js.
$libraries['classList.js'] = array(
  'name' => 'classList.js',
  'vendor url' => 'https://github.com/eligrey/classList.js',
  'download url' => 'https://github.com/eligrey/classList.js',
  'version callback' => '_library_pack_force_version',
  'version arguments' => array(
    'file' => 'classList.js',
    'force' => '1'
  ),
  'files' => array(
    'js' => array(
      'classList.js',
    )
  ),
  'variants' => array(
    'minified' => array(
      'files' => array(
        'js' => array(
          'classList.min.js',
        )
      ),
    ),
  ),
);


