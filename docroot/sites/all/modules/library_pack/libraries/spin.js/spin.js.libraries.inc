<?php

$libraries['spin.js'] = array(
  'name' => 'Spin JS',
  'vendor url' => 'https://github.com/fgnass/spin.js',
  'download url' => 'https://github.com/fgnass/spin.js',
  'version arguments' => array(
    'file' => 'component.json',
    'pattern' => '/"version": "([@0-9a-zA-Z\.-]+)/',
  ),
  'files' => array(
    'js' => array(
      'spin.js'
    ),
  ),
  'variants' => array(
    'minified' => array(
      'files' => array(
        'js' => array(
          'spin.min.js'
        ),
      ),
    ),
    'jquery' => array(
      'files' => array(
        'js' => array(
          'spin.min.js',
          'jquery.spin.js'
        ),
      ),
    ),
  ),
);