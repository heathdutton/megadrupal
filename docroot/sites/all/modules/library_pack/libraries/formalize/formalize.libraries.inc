<?php

$libraries['formalize'] = array(
    'name' => 'Formalize',
    'vendor url' => 'https://github.com/nathansmith/formalize',
    'download url' => 'https://github.com/nathansmith/formalize/zipball/master',
    'version arguments' => array(
        'file' => 'assets/js/jquery.formalize.js',
        'pattern' => '/Formalize - version ([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'css' => array(
            'assets/css/formalize.css',
        ),
        'js' => array(
            'assets/js/jquery.formalize.js'
        ),
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'assets/js/jquery.formalize.min.js'
                ),
                'css' => array(
                    'assets/css/formalize.css',
                ),
            ),
        ),
    ),
);
