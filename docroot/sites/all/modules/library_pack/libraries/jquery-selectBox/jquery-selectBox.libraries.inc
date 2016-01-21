<?php

$libraries['jquery-selectBox'] = array(
    'name' => 'jquery-selectBox',
    'vendor url' => 'https://github.com/marcj/jquery-selectBox',
    'download url' => 'https://github.com/marcj/jquery-selectBox',
    'version arguments' => array(
        'file' => 'jquery.selectBox.js',
        'pattern' => '/\* v([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'jquery.selectBox.js'
        ),
        'css' => array(
          'jquery.selectBox.css'
        )
    )
);