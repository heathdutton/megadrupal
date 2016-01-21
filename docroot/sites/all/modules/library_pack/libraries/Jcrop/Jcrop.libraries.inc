<?php
$libraries['Jcrop'] = array(
    'name' => 'Jcrop',
    'vendor url' => 'https://github.com/tapmodo/Jcrop',
    'download url' => 'https://github.com/tapmodo/Jcrop',
    'version arguments' => array(
        'file' => 'js/jquery.Jcrop.js',
        'pattern' => '/jquery.Jcrop.js v([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'js/jquery.Jcrop.js',
        ),
    ),
    'variants' => array(
      'minified' => array(
        'files' => array(
          'js' => array(
            'js/jquery.Jcrop.min.js'
          ),
        ),
      ),
    ),
);