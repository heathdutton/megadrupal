<?php
$libraries['newsticker'] = array(
    // Only used in administrative UI of Libraries API.
    'name' => 'jquery.newsticker',
    'vendor url' => 'http://makemineatriple.com/newsticker#download',
    'download url' => 'http://makemineatriple.com/newsticker#download',
    'version arguments' => array(
        'file' => 'jquery.newsTicker-2.3.6.js',
        'pattern' => '/version ([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'jquery.newsTicker-2.3.6.js',
        ),
    )
);