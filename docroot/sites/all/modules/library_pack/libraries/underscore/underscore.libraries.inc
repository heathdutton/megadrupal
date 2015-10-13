<?php

$libraries['underscore'] = array(
    'name' => 'Underscore',
    'vendor url' => 'https://github.com/documentcloud/underscore',
    'download url' => 'https://github.com/documentcloud/underscore/zipball/master',
    'version arguments' => array(
        'file' => 'underscore.js',
        'pattern' => '/Underscore.js ([0-9a-zA-Z\.-]+)/',
        'lines' => 1,
    ),
    'files' => array(
        'js' => array(
            'underscore.js',
        )
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'underscore-min.js',
                )
            ),
        ),
    ),
);

