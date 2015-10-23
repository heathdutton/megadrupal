<?php

$libraries['cycle2'] = array(
    'name' => 'jQuery Cycle2',
    'vendor url' => 'http://jquery.malsup.com/cycle2',
    'download url' => 'http://jquery.malsup.com/cycle2/download/',
    'version arguments' => array(
        'file' => 'build/jquery.cycle2.js',
        'pattern' => '/version: ([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'build/jquery.cycle2.js',
        ),
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'build/jquery.cycle2.min.js'
                ),
            ),
        ),
    ),
);
