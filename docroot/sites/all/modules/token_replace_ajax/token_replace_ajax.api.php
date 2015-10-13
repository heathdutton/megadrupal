<?php

/**
 * @file
 * Hooks related to Token replace AJAX.
 */

/**
 * Allows modules to retrieve their entity form the provided form/form_state.
 *
 * @param $entity_type
 *   The entity type of the token being processed.
 * @param $form
 *   The form posted to Token replace AJAX callback.
 * @param $form_state
 *   The processed form state of the Token replace AJAX form submission.
 *
 * @return bool|object
 */
function hook_token_replace_ajax_form_entity($entity_type, $form, $form_state) {
  if (isset($form['#my_entity'])) {
    $my_entity = (object) array_merge((array) $form_state['my_entity'], isset($form_state['values']['my_entity']) ? $form_state['values']['my_entity'] : array());

    return $my_entity;
  }

  return FALSE;
}

/**
 * Allows modules to alter the response value of the Token replace AJAX callback..
 *
 * @param $value
 *   The processed token result.
 * @param $token
 *   The unprocessed token.
 * @param $data
 *   The data used for the token replacement.
 */
function hook_token_replace_ajax_response_alter(&$value, $token, $data) {
  if ($value == $token) {
    $value = '';
  }
}
