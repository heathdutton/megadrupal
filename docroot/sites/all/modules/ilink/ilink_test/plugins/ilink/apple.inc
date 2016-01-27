<?php

/**
 * Implements hook_ilink_info().
 */
function ilink_test_apple_ilink_info() {
  return array(
    'title' => t('Justice!'),
    'description' => t('It will be served!'),
    'href' => 'justice/%person/%gender/!age',
    'parameters' => array(
      'person' => array(
        'name' => 'uid',
        'keyword' => 'person',
        'identifier' => t('Person'),
        'settings' => array(),
      ),
      'gender' => array(
        'name' => 'string',
        'keyword' => 'gender',
        'identifier' => t('Gender'),
        'settings' => array(),
      ),
      'age' => array(
        'name' => 'string',
        'keyword' => 'age',
        'identifier' => t('Age'),
        'settings' => array(),
      ),
    ),
  );
}

// function ilink_test_justice_link_theme($plugin, $arguments) {
//   return array(
//     'ilink_test_justice_link' => array(
//       'path' => $plugin['path'],
//       'file' => $plugin['file'],
//     ),
//   );
// }

function ilink_test_justice_link_label() {
  return "Bar Baz";
}

function ilink_test_justice_link_disable() {
  return FALSE;
}
