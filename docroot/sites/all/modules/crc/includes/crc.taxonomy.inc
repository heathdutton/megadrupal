<?php

/**
 * @file
 * This handles the crc pseudo-hooks for the following modules:
 *  taxonomy
 *
 * It implement the following pseudo-hooks
 *
 *  hook_crc_node_prepopulate(&$node, $settings, $values, $base_node)
 *  hook_crc_type_settings($type, $settings, $org_form, $form_state)
 *  hook_crc_settings($settings)
 */

/**
 * Helper function
 *
 * Sets the default values to use.
 *
 * @param $settings
 * @return void
 */
function taxonomy_crc_settings(&$settings = array()) {
  $settings += array(
    'taxonomy' => 1,
  );
}

/**
 * Implementation of hook_crc_node_prepopulate().
 *
 * This prepopulates the node object before it is passed
 * to the node_form.
 *
 * @param $node
 * @param $settings
 * @param $values
 * @param $base_node
 * @return void
 */
function taxonomy_crc_node_prepopulate(&$node, $settings, $values, $base_node) {
  global $user;
  switch ($settings['taxonomy']) {
    case 2: // Ignore - empty
      $node->taxonomy = array();
      break;
    case 1: // Clone
    default:
      $node->taxonomy = $base_node->taxonomy;
      break;
  }
}

/**
 * Implementation of hook_crc_type_settings().
 *
 * This provides the form elements to show on the edit content
 * type form. These are used to set the defaults for the
 * content type.
 *
 * @param $type
 * @param $settings
 * @param $org_form
 * @param $form_state
 * @return $form
 */
function taxonomy_crc_type_settings($type, $settings, $org_form, $form_state) {
  $form = array();
  $taxonomy_options = array(1 => t('Clone'), 2 => t('Empty'));
  $form['taxonomy'] = array(
    '#title' => t('Taxonomy settings'),
    '#type' => 'radios',
    '#options' =>  $taxonomy_options,
    '#default_value' => $settings['taxonomy'],
  );

  return $form;
}
