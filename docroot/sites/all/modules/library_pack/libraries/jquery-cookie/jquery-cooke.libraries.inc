<?php

$libraries['jquery-cookie'] = array(
    'name' => 'jQuery Cookie',
    'vendor url' => 'https://github.com/carhartl/jquery-cookie',
    'download url' => 'https://github.com/carhartl/jquery-cookie/archive/master.zip',
    'version arguments' => array(
        'file' => 'src/jquery.cookie.js',
        'pattern' => '/Cookie Plugin v([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'src/jquery.cookie.js',
        )
    )
);