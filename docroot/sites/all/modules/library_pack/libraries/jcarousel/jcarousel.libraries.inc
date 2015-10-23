<?php
$libraries['jcarousel'] = array(
    'name' => 'jQuery jcarousel',
    'vendor url' => 'http://sorgalla.com/jcarousel/',
    'download url' => 'https://github.com/jsor/jcarousel',
    'version callback' => '_library_pack_force_version',
    'version arguments' => array(
        'file' => 'dist/jquery.jcarousel.js',
        'force' => '0.3.0'
    ),
    'files' => array(
        'js' => array(
            'dist/jquery.jcarousel.js',
            'dist/jquery.jcarousel-control.js',
        ),
    ),
);