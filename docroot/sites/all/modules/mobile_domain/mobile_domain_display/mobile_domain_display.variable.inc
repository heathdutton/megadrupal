<?php
/**
 * @file
 * Mobile domain display - Variable definition
 */

/**
 * Implements hook_variable_info_alter().
 *
 * Only variables which have the "multidomain" property set to TRUE, can be
 * used as domain specific variables. Here we set this property for a set of
 * default system variables.
 *
 * @see _domain_variable_variable_realm_list()
 */
function mobile_domain_display_variable_info_alter(&$variables, $options) {
  $list = array(
    // Number of posts in main page.
    'default_nodes_main',
    // Hide content translation links.
    'i18n_hide_translation_links',
  );
  foreach ($list as $name) {
    if (isset($variables[$name])) {
      $variables[$name]['multidomain'] = TRUE;
    }
  }
}

