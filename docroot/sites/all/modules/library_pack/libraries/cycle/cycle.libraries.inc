<?php

$libraries['cycle'] = array(
    // Only used in administrative UI of Libraries API.
    'name' => 'jQuery Cycle',
    'vendor url' => 'https://github.com/malsup/cycle',
    'download url' => 'https://github.com/malsup/cycle',
    'version arguments' => array(
        'file' => 'jquery.cycle.all.js',
        'pattern' => '/Version: ([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'jquery.cycle.all.js',
        ),
    )
);
