<?php

$libraries['history.js'] = array(
    'name' => 'History.js',
    'vendor url' => 'https://github.com/browserstate/history.js',
    'download url' => 'https://github.com/browserstate/history.js/zipball/master',
    'version arguments' => array(
        'file' => 'bower.js',
        'pattern' => '/"version": "([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'scripts/bundled-uncompressed/html4+html5/jquery.history.js',
        )
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'scripts/bundled/html4+html5/jquery.history.js',
                )
            ),
        ),
    ),
);