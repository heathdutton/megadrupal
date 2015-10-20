<?php

$libraries['isotope'] = array(
    // Only used in administrative UI of Libraries API.
    'name' => 'Isotope',
    'vendor url' => 'https://github.com/desandro/isotope',
    'download url' => 'https://github.com/desandro/isotope/zipball/master',
    'version arguments' => array(
        'file' => 'dist/isotope.pkgd.js',
        'pattern' => '/PACKAGED v([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'dist/isotope.pkgd.js',
        ),
    )
);