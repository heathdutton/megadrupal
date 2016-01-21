<?php

$libraries['flexnav'] = array(
    'name' => 'jQuery flexnav',
    'vendor url' => 'https://github.com/indyplanets/flexnav',
    'download url' => 'https://github.com/indyplanets/flexnav',
    'version arguments' => array(
        'file' => 'js/jquery.flexnav.js',
        'pattern' => '/FlexNav.js ([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'js/jquery.flexnav.js',
        ),
        'css' => array(
            'css/flexnav.css',
        ),
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'js/jquery.flexnav.min.js',
                ),
                'css' => array(
                    'css/flexnav.css',
                )
            ),
        ),
    ),
);