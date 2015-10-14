<?php
/**
 * @file
 */


/**
 * Implements hook_views_plugins().
 */
function slate_views_plugins() {
  return array(
    'module' => 'slate',
    'row' => array(
      'slate' => array(
        'title' => 'Slate',
        'help' => 'Slate.',
        'handler' => 'slate_views_plugin_row_slate',
        'uses fields' => FALSE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
