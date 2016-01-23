<?php // $Id$

/**
 * @file
 * Creating the admin pages for the Github widget pages.
 */

/**
 * Form building callback for the github widget settings page.
 */
function _github_widget_admin_settings() {
  $types = node_type_get_types();
  $final_types = array();
  foreach ($types as $key => $value) {
    $final_types[$key] = $value->name;
  }

  $form['github_widget_type_filters'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Content types available'),
    '#default_value' => variable_get('github_widget_type_filters', array()),
    '#options'       => $final_types,
    '#description'   => t('Select the node types where the github widget should be shown.'),
  );

  $form['github_widget_allowed_releases'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Show only stable releases'),
    '#default_value' => variable_get('github_widget_allowed_releases', 1),
    '#description'   => t('Check this if you want to show the allowed releases of projects. This way only the project that apply to the naming conventions of Drupal will be shown. More information can be found here http://drupal.org/node/1015226'),
  );

  $form['github_widget_releases_only'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use only the Github widget releases'),
    '#default_value' => variable_get('github_widget_releases_only', 0),
    '#description'   => t('Check this if you want to use the releases and don\'t want github project information on the end of a page.'),
  );

  $form['github_widget_block_only'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use only the Github widget block'),
    '#default_value' => variable_get('github_widget_block_only', 0),
    '#description'   => t('Check this if you want to use the block and don\'t want github information on the end of a page.'),
  );

  return system_settings_form($form);
}