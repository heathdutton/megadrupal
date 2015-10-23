<?php
$libraries['Respond'] = array(
    'name' => 'Respond',
    'vendor url' => 'https://github.com/scottjehl/Respond',
    'download url' => 'https://github.com/scottjehl/Respond',
    'version callback' => '_library_pack_force_version',
    'version arguments' => array(
        'file' => 'src/respond.js',
        'force' => '1'
    ),
    'files' => array(
        'js' => array(
            'src/respond.js',
        ),
    )
);