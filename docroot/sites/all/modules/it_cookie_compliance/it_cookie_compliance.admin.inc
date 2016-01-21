<?php

/**
 * @file
 * It Cookie Compliance configuration functions.
 */

/**
 * Implements hook_admin_form().
 */
function it_cookie_compliance_admin_settings() {

  $form = array();
  $form['it_cookie_compliance_javascript'] = array(
    '#type' => 'textarea',
    '#title' => t('Javascript to exclude'),
    '#default_value' => variable_get('it_cookie_compliance_javascript', ''),
    '#description' => t('Set the javascript names to remove, separated by commas or one for every line. Names are checked by containment. E.g., "google" removes from the page all javascripts that contain "google".'),
  );

  $form['it_cookie_compliance_blocks'] = array(
    '#type' => 'textarea',
    '#title' => t('Blocks to exclude'),
    '#default_value' => variable_get('it_cookie_compliance_blocks', ''),
    '#description' => t('Set block machine names to remove, separated by commas or one for every line. Names are checked by containment. E.g., "google" removes from the page all blocks that contain "google", "block_2" excludes a block that in the html has id "block-block-2".'),
  );
  
  $form['it_cookie_compliance_fields'] = array(
    '#type' => 'textarea',
    '#title' => t('Fields to exclude'),
    '#default_value' => variable_get('it_cookie_compliance_fields', ''),
    '#description' => t('Set the exact field machine names to remove from node view, separated by commas or one for every line. E.g. field_image removes the field named exactly "field_image".'),
  );
  return system_settings_form($form);
}
