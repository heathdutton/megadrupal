<?php

$libraries['on-media-query'] = array(

  'name' => 'On Media Query',
  'vendor url' => 'https://github.com/JoshBarr/on-media-query',
  'download url' => 'https://github.com/JoshBarr/on-media-query',
  'version callback' => '_library_pack_force_version',
  'version arguments' => array(
    'file' => 'js/onmediaquery.js',
    'force' => '1',
  ),
  'files' => array(
    'js' => array(
      'js/onmediaquery.js',
    ),
  ),
  'variants' => array(
    'minified' => array(
      'files' => array(
        'js' => array(
          'js/onmediaquery.min.js',
        ),
      ),
    ),
  ),
);