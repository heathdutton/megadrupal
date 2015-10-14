<?php

/**
 * @file
 * Webform authcode module functions for admin sections.
 */

 /**
  * Validation callback handler for the webform configure form.
  */
function webform_authcode_webform_configure_validate($form, $form_state) {
  // Retrieve values from form_state.
  $values = $form_state['values'];

  // Check for emtpty pass phrase.
  if ($values['authcode_protect'] == TRUE && empty($values['authcode_pass_phrase'])) {
    form_set_error('authcode_pass_phrase', t("The field pass phrase can't be empty."));
  }
}

/**
 * Submit callback handler for the webform configure form.
 */
function webform_authcode_webform_configure_submit($form, $form_state) {
  // Retrieve values from form_state.
  $values = $form_state['values'];

  // Retrieve the node id.
  $nid = $form['#node']->nid;

  // Retrieve the stored pass phrases from the variable store.
  $authcode_phrases = variable_get('authtcode_pass_phrases', array());

  // If the authcode protect checkbox was not checked, remove existing phrase.
  // Otherwise add/update it.
  if ($values['authcode_protect'] != TRUE && isset($authcode_phrases[$nid])) {
    // Delete the pass phrase.
    unset($authcode_phrases[$nid]);
  }
  else {
    // Add / update the pass phrase.
    $authcode_phrases[$nid] = $values['authcode_pass_phrase'];
  }

  // Store the pass phrase in the variable store.
  variable_set('authtcode_pass_phrases', $authcode_phrases);
}
