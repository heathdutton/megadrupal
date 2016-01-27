<?php

$libraries['HTML5-History-API'] = array(
    'name' => 'HTML5-History-API',
    'vendor url' => 'https://github.com/devote/HTML5-History-API',
    'download url' => 'https://github.com/devote/HTML5-History-API/zipball/master',
    'version arguments' => array(
        'file' => 'package.json',
        'pattern' => '/"version": "([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'history.js',
        )
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'history.min.js',
                )
            ),
        ),
    ),
);