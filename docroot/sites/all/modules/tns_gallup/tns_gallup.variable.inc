<?php

/**
 * @file
 * TNS Gallup module integration for the variable module.
 */

/**
 * Implements hook_variable_group_info().
 */
function tns_gallup_variable_group_info() {
  $groups['tns_gallup'] = array(
    'title' => t('TNS Gallup'),
    'description' => t('TNS Gallup'),
    'access' => 'administer tns gallup',
    'path' => array('admin/config/system/tns-gallup'),
  );

  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function tns_gallup_variable_info($options) {
  $variables = array();

  $variables['tns_gallup_site_id'] = array(
    'type' => 'string',
    'title' => t('TNS Gallup Site ID', array(), $options),
    'description' => t('Your site ID as provided by TNS Gallup.', array(), $options),
    'default' => FALSE,
    'group' => 'tns_gallup',
  );

  $variables['tns_gallup_roles'] = array(
    'type' => 'options',
    'title' => t('TNS Gallup Role specific visibility', array(), $options),
    'description' => t('Include script only for the selected role(s). If you select none of the roles, then all roles will see the script. If a user has any of the roles checked, the script will be included for the user..', array(), $options),
    'options callback' => '_tns_gallup_user_roles',
    'default' => array(),
    'group' => 'tns_gallup',
  );

  $variables['tns_gallup_visibility'] = array(
    'type' => 'select',
    'title' => t('Page specific visibility settings', array(), $options),
    'description' => t('Add script to specific pages', array(), $options),
    'default' => FALSE,
    'options callback' => '_tns_gallup_visibility',
    'group' => 'tns_gallup',
  );

  $variables['tns_gallup_pages'] = array(
    'type' => 'string',
    'element' => array(
      '#type' => 'textarea',
    ),
    'title' => t('Page specific visibility pages', array(), $options),
    'description' => t("Enter one page per line as Drupal paths. The ''*' character is a wildcard. Example paths are blog for the blog page and blog/* for every personal blog. <front> is the front page.", array(), $options),
    'default' => FALSE,
    'group' => 'tns_gallup',
  );

  return $variables;
}

/**
 * Get user roles options.
 */
function _tns_gallup_user_roles() {
  return user_roles();
}

/**
 * Get visibility settings options.
 */
function _tns_gallup_visibility() {
  $options = array(
    t('Add to every page except the listed pages.'),
    t('Add to the listed pages only.'),
  );

  if (module_exists('php') && user_access('use PHP for TNS Gallup visibility')) {
    $options[] = t('Add if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
  }

  return $options;
}
