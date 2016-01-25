<?php

$libraries['qTip2'] = array(
    'name' => 'jQuery Qtip2',
    'vendor url' => 'https://github.com/Craga89/qTip2',
    'download url' => 'https://github.com/Craga89/qTip2/zipball/master',
    'version arguments' => array(
        'file' => 'jquery.qtip.js',
        'pattern' => '/Pretty powerful tooltips - v([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'jquery.qtip.js'
        ),
        'css' => array(
            'jquery.qtip.css'
        ),
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'jquery.qtip.min.js'
                ),
                'css' => array(
                    'jquery.qtip.min.css'
                ),
            ),
        ),
    ),
);