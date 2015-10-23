<?php

$libraries['normalize.css'] = array(
    'name' => 'Normalize CSS',
    'vendor url' => 'https://github.com/necolas/normalize.css/',
    'download url' => 'https://github.com/necolas/normalize.css/',
    'version arguments' => array(
        'file' => 'normalize.css',
        'pattern' => '/normalize.css v([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'css' => array(
            'normalize.css',
        ),
    )
);