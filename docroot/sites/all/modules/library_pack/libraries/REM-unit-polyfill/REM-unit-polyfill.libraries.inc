
<?php

$libraries['REM-unit-polyfill'] = array(
  'name' => 'REM-unit-polyfill',
  'vendor url' => 'https://github.com/chuckcarpenter/REM-unit-polyfill.git',
  'download url' => 'https://github.com/chuckcarpenter/REM-unit-polyfill.git',
  'version arguments' => array(
    'file' => 'bower.json',
    'pattern' => '/"version": "([0-9a-zA-Z\.-]+)/',
  ),
  'files' => array(
    'js' => array(
      'js/rem.js'
    )
  ),
  'variants' => array(
    'minified' => array(
      'files' => array(
        'js' => array(
          'js/rem.min.js'
        ),
      ),
    ),
  ),
);



