<?php

$libraries['jquery-placeholder'] = array(
    'name' => 'jQuery Placeholder',
    'vendor url' => 'http://mths.be/placeholder',
    'download url' => 'https://github.com/mathiasbynens/jquery-placeholder',
    'version arguments' => array(
        'file' => 'jquery.placeholder.js',
        'pattern' => '/placeholder v([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'jquery.placeholder.js',
        )
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'jquery.placeholder.min.js',
                )
            ),
        ),
    ),
);