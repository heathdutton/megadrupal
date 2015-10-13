<?php

$libraries['jquery.appear'] = array(
    'name' => 'jQuery Appear',
    'vendor url' => 'https://github.com/morr/jquery.appear',
    'download url' => 'https://github.com/morr/jquery.appear/archive/master.zip',
    'version arguments' => array(
        'file' => 'jquery.appear.js',
        'pattern' => '/Version: ([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'jquery.appear.js',
        )
    )
);