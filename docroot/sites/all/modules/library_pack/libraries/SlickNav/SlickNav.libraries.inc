<?php

$libraries['SlickNav'] = array(
    'name' => 'jQuery slicknav',
    'vendor url' => 'https://github.com/ComputerWolf/SlickNav',
    'download url' => 'https://github.com/ComputerWolf/SlickNav',
    'version callback' => '_library_pack_force_version',
    'version arguments' => array(
        'file' => 'jquery.slicknav.js',
        'force' => '0.0.1'
    ),
    'files' => array(
        'js' => array(
            'jquery.slicknav.js',
        ),
        'css' => array(
            'slicknav.css',
        ),
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'jquery.slicknav.min.js',
                ),
                'css' => array(
                    'slicknav.css',
                )
            ),
        ),
    ),
);