<?php

$libraries['JSON-js'] = array(
    'name' => 'JSON-js',
    'vendor url' => 'http://github.com/douglascrockford/JSON-js',
    'download url' => 'http://github.com/douglascrockford/JSON-js',
    'version callback' => '_library_pack_force_version',
    'version arguments' => array(
      'file' => 'json2.js',
      'force' => '1'
    ),
    'files' => array(
        'js' => array(
            'json2.js'
        ),
    )
);