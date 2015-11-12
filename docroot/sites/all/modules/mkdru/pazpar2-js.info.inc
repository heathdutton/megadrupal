<?php
/**
 * @file
 * Library definitions for Pazpar2 JS tools
 */
function mkdru_libraries_info() {
  $libraries['pazpar2-js'] = array(
    'name' => 'Pazpar2/Service Proxy JS Tools',
    'vendor url' => 'http://www.indexdata.com',
    'download url' => 'http://www.indexdata.com/pazpar2-js',
    'version arguments' => array(
      'file' => 'VERSION',
      'pattern' => '/^(.*)$/',
    ),
  );
  return $libraries;
}
function mkdru_library() {
  $libinfo = libraries_detect('pazpar2-js');
  $version = $libinfo['version'];
  $libraries['pz2'] = array(
    'version' => $version,
    'title' => 'Pazpar2/Service Proxy API',
    'js' => array(
      libraries_get_path('pazpar2-js') . '/pz2.js' => array(
        'scope' => 'footer',
        'defer' => FALSE,
        'preprocess' => TRUE,
      ),
    ),
  );
  return $libraries;
}
