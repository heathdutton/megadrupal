<?php

$libraries['enquire.js'] = array(
    'name' => 'Enquire JS',
    'vendor url' => 'https://github.com/WickyNilliams/enquire.js',
    'download url' => 'https://github.com/WickyNilliams/enquire.js',
    'version arguments' => array(
        'file' => 'dist/enquire.js',
        'pattern' => '/enquire.js v([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(

            'dist/enquire.js'=> array(
                'every_page' => TRUE,
                'group' => JS_LIBRARY,
                'preprocess' => 0,
                'scope' => 'header',
                'type' => 'file',
                'weight' => -9999,
            )
        ),
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'dist/enquire.min.js'
                ),
            ),
        ),
    ),
);