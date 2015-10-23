<?php

$libraries['colorpicker'] = array(
    'name' => 'colorpicker',
    'vendor url' => 'http://www.eyecon.ro/colorpicker',
    'download url' => 'http://www.eyecon.ro/colorpicker/colorpicker.zip',
    'version callback' => '_library_pack_force_version',
    'version arguments' => array(
        'file' => 'js/colorpicker.js',
        'force' => '1'
    ),

    'files' => array(
        'css' => array(
            'css/colorpicker.css',
        ),
        'js' => array(
            'js/colorpicker.js',
        ),
    )
);