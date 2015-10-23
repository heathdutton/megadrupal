<?php
$libraries['chosen'] = array(
    'name' => 'jQuery Chosen',
    'vendor url' => 'http://harvesthq.github.com/chosen/',
    'download url' => 'https://github.com/harvesthq/chosen/releases/latest',
    'version arguments' => array(
        'file' => 'chosen.jquery.js',
        'pattern' => '/Version ([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'chosen.jquery.js'
        ),
        'css' => array(
            'chosen.css'
        ),
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'chosen.jquery.min.js'
                ),
                'css' => array(
                    'chosen.css'
                ),
            ),
        ),
    ),
);
