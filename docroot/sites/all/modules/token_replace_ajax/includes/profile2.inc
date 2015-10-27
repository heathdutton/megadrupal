<?php

/**
 * @file
 * Profile2 module integration.
 */

/**
 * Implements hook_token_replace_ajax_form_entity() on behalf of
 * profile2.module.
 */
function profile2_token_replace_ajax_form_entity($entity_type, $form, $form_state) {
  if (isset($form['#user_category']) && isset($form_state['profiles'][$form['#user_category']])) {
    $profile = (object) array_merge((array) $form_state['profiles'][$form['#user_category']], isset($form_state['values']["profile_{$form['#user_category']}"]) ? $form_state['values']["profile_{$form['#user_category']}"] : array());
    return $profile;
  }
  return FALSE;
}
