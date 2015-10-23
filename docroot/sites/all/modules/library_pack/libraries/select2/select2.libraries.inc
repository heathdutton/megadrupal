<?php

$libraries['select2'] = array(
    'name' => 'Select 2',
    'vendor url' => 'http://ivaynberg.github.com/select2/',
    'download url' => 'https://github.com/ivaynberg/select2/zipball/master',
    'version arguments' => array(
        'file' => 'dist/js/select2.js',
        'pattern' => '/Select2 ([@0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'dist/js/select2.js'
        ),
        'css' => array(
            'dist/css/select2.css'
        ),
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'dist/js/select2.min.js'
                ),
                'css' => array(
                    'dist/css/select2.css'
                ),
            ),
        ),
    ),
);