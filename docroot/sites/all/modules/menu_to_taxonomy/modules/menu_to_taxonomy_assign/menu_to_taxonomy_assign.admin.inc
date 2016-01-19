<?php

/**
 * @file
 * Admin functionality for the Menu to Taxonomy Assign module.
 */

/**
 * Form callback for bulk "Re-assign term references".
 */
function menu_to_taxonomy_assign_reassign_form($form, $form_state, $bundle, $field_name) {
  $form['bundle'] = array(
    '#type' => 'value',
    '#value' => $bundle,
  );
  $form['field_name'] = array(
    '#type' => 'value',
    '#value' => $field_name,
  );
  // Protect against CSRF.
  if (!drupal_valid_token($_GET['token'], current_path())) {
    return drupal_access_denied();
  }

  $path = drupal_get_destination();
  return confirm_form($form, t('Are you sure you want to reassign all %field_name term references for node type %bundle?', array(
    '%field_name' => $field_name,
    '%bundle' => $bundle,
  )), $path['destination']);
}


/**
 * Submit callback for 'menu_to_taxonomy_assign_reassign_form' form.
 */
function menu_to_taxonomy_assign_reassign_form_submit($form, &$form_state) {
  $field_name = $form_state['values']['field_name'];
  $bundle = $form_state['values']['bundle'];
  _menu_to_taxonomy_assign_reassign_batch($field_name, $bundle);
  $path = drupal_get_destination();
  $form_state['redirect'] = $path['destination'];
}
