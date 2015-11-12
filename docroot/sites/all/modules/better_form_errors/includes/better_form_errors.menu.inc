<?php

/**
 * @file
 * Part of the "Better Form Errors" module that will be included for
 * registering pathes.
 */

/**
 * Implements hook_menu().
 *
 * https://api.drupal.org/
 *  api/drupal/modules!system!system.api.php/function/hook_menu/7
 */
function better_form_errors_menu() {
  $module_info = _better_form_errors_module_info();
  $items = array();
  // Item 1:
  $items['admin/' . $module_info['MODULE_NAME'] . '/readme'] = array(
    'file' => 'includes/' . $module_info['MODULE_NAME'] . '.menu.inc',
    'page callback' => '_better_form_errors_readme',
    'access arguments' => array('administer modules'),
    'type' => MENU_CALLBACK,
    'title' => $module_info['MODULE_FULLNAME'] . ': ' . $module_info['MODULE_README'],
    'title callback' => FALSE,
  );
  return $items;
}

/**
 * Page callback function for Item 1 of function above.
 */
function _better_form_errors_readme() {
  $module_info = _better_form_errors_module_info();
  $link_help = l(
    $module_info['MODULE_FULLNAME'],
    'admin/help/' . $module_info['MODULE_NAME'],
    array(
      'attributes' => array(
        'title' => t('Help') . ' ' . t('for the module') . ' "' . $module_info['MODULE_FULLNAME'] . '"',
      ),
    )
  );

  $placeholders = array(
    '!link_help' => $link_help,
  );

  $return = "\n" . '<p><ul><li>' . t('Help') . ' ' . t('for the module') . ' ' . $link_help . '</li></ul></p>';
  return $return;
}
