<?php
/**
 * @file
 * Check that all webforms on the site have an attached email address.
 */

/**
 * Implements hook_drush_command().
 */
function drush_webform_health_drush_command() {
  $items = array();
  $items['webform-email-check'] = array(
    'description' => 'Webform emails check.',
    'callback' => 'drush_webform_email_check',
    'options' => array(),
    'examples' => array(
      'drush drugtest' => 'Check to see if a webform has an email setup.',
    ),
    'aliases' => array('wec'),
  );
  return $items;
}

/**
 * Check if all webforms on a website have an email address attached to them.
 */
function drush_webform_email_check() {
  if (version_compare(VERSION, '7.0', '<=') || version_compare(VERSION, '8.0', '>=')) {
    drush_log('Site is not Drupal 7.', 'ok');
    return;
  }
  if (!module_exists('webform')) {
    drush_log(t('Webform is not installed on this site.'), 'ok');
    return;
  }

  // Loop through all nodes which have webforms attached to them.
  foreach (webform_variable_get('webform_node_types') as $node_type) {
    foreach (drush_webform_load_all_nodes($node_type) as $node) {
      if (count($node->webform['emails']) == 0) {
        drush_log(t('Webform Health Error: "@name" on "@site_name" had no email address entry.', array('@name' => $node->title,'@site_name' => variable_get('site_name', ''))), 'ok');
      }
    }
  }
}

/**
 * Load all nodes of a particular type.
 *
 * @param string $type
 *   The content type to load all nodes for.
 *
 * @return array
 *   An array of loaded nodes.
 */
function drush_webform_load_all_nodes($type) {
  $nodes = node_load_multiple(array(), array('type' => $type));
  return array_values($nodes);
}
