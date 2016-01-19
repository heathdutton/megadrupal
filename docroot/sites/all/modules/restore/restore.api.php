<?php
/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Allows modules to provide restore script configs.
 *
 * For more information on how the data should be structured for
 * each operation, view the operations class file.
 *
 * @return array
 *   An array containing the modules restore config.
 */
function hook_restore_scripts() {
  $items = array();

  $items['example_restore_script'] = array(
    // The title of the script.
    'title' => t('Example restore script'),
    // The description of the script.
    'description' => t('This is an example restore script.'),
    // Which group to place the script in.
    'group' => 'Examples',
    // Determine if the script can be edited.
    'locked' => FALSE,
    // An array of operations.
    'operations' => array(
      array(
        // The operation type.
        'type' => 'variables',
        // The operation details.
        'details' => array(
          'variables' => array(
            'site_name' => 'example7.dev',
            'site_slogan' => 'Blahhh',
            'site_mail' => 'info@example7.dev',
          ),
        ),
      ),
      array(
        // The operation type.
        'type' => 'modules',
        // The operation details.
        'details' => array(
          'enabled' => array(),
          'disabled' => array('overlay' => 'overlay', 'toolbar' => 'toolbar'),
        ),
      ),
    ),
  );

  return $items;
}

/**
 * Allow adding or altering scripts.
 *
 * @param array &$scripts
 *   A reference to the scripts array.
 */
function hook_restore_scripts_alter(&$scripts) {
  $scripts['example_restore_script']['title'] = t('A different script title');
}

/**
 * Register additional restore operations.
 *
 * The module's info file must declare the class inc file
 * ie. "files[] = operations/my-operation.class.inc".
 *
 * @return array
 *   An array of operations.
 */
function hook_restore_operations() {
  $items = array();

  $items['modules'] = array(
    // The operation title.
    'title' => t('Modules'),
    // The operation description.
    'description' => t('Enable or Disable modules.'),
    // The operation classname.
    'class' => 'RestoreOperationModules',
  );

  return $items;
}

/**
 * Allow adding or altering operations.
 *
 * @param array &$operations
 *   A reference to the operations array.
 */
function hook_restore_operations_alter(&$operations) {
  $operations['modules']['title'] = t('A different operation title');
}
