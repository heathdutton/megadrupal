<?php

$libraries['bootstrap'] = array(
    'name' => 'Twitter Bootstrap',
    'vendor url' => 'http://twitter.github.com',
    'download url' => 'http://twitter.github.com/bootstrap/assets/bootstrap.zip',
    'version arguments' => array(
        'file' => 'dist/css/bootstrap.css',
        'pattern' => '/Bootstrap v([0-9a-zA-Z\.-]+)/',
        'lines' => 3,
        'cols' => 50,
    ),
    'files' => array(
        'js' => array(
            'dist/js/bootstrap.js',
        ),
        'css' => array(
            'dist/css/bootstrap.css',
        ),
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'dist/js/bootstrap.min.js',
                ),
                'css' => array(
                    'dist/css/bootstrap.min.css',
                ),
            ),
        ),
    ),
);
