<?php

$libraries['jScrollPane'] = array(
    'name' => 'jScrollPane',
    'vendor url' => 'https://github.com/vitch/jScrollPane',
    'download url' => 'https://github.com/vitch/jScrollPane',
    'version arguments' => array(
        'file' => 'script/jquery.jscrollpane.js',
        'pattern' => '/jScrollPane - v([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'script/jquery.jscrollpane.js',
        ),
        'css' => array(
            'script/jquery.jscrollpane.css',
        )
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'script/jquery.jscrollpane.min.js'
                ),
                'css' => array(
                    'script/jquery.jscrollpane.css'
                ),
            ),
        ),
    ),
);
