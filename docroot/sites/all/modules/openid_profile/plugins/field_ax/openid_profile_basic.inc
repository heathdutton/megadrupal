<?php

/**
 * The default plugin exists only to provide the base class. Other plugins
 * which do not provide a class will get this class instead. Any classes
 * provided should use this class as their parent:
 *
 * @code
 *   'handler' => array(
 *     'class' => 'ctools_export_ui_mine',
 *     'parent' => 'ctools_export_ui',
 *   ),
 * @endcode
 *
 * Using the above notation will ensure that this plugin's is loaded before
 * the child plugin's class and avoid whitescreens.
 */
$plugin = array(
  'field_types' => array('name', 'mail'),
  'handler' => array(
    'name' => 'Basic text',
    'class' => 'openid_profile_basic',
  ),
  'file' => 'openid_profile_basic.class.php',
);
