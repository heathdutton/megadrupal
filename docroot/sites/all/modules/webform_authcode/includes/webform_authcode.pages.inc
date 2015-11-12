<?php

/**
 * @file
 * Webform authcode module functions for frontend.
 */

 /**
  * Callback function to build pass phrase form.
  */
function webform_authcode_pass_phrase_form($form, &$form_state) {

  $form['description'] = array(
    '#prefix' => '<div class="introduction-text">',
    '#suffix' => '</div>',
    '#markup' => t('This form is protected by a pass phrase. Please enter the the pass phrase provided to you to gain access to the form.'),
  );

  $form['authcode_pass_phrase'] = array(
    '#type' => 'textfield',
    '#title' => t('Pass phrase'),
    '#maxlength' => 255,
    '#size' => 50,
    '#description' => t('Please enter the pass phrase provided to you.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue'),
  );

  // Add validation handler.
  $form['#validate'][] = 'webform_authcode_pass_phrase_form_validate';
  // Add submit handler.
  $form['#submit'][] = 'webform_authcode_pass_phrase_form_submit';

  return $form;
}

/**
 * Validation handler for webform_authcode_pass_phrase_form.
 */
function webform_authcode_pass_phrase_form_validate($form, $form_state) {
  // Retrieve values from form_state.
  $values = $form_state['values'];

  // Retrieve the node id.
  $node = menu_get_object();
  $nid = $node->nid;

  // Retrieve the stored pass phrases from the variable store.
  $authcode_phrases = variable_get('authtcode_pass_phrases', array());

  // Check for emtpty pass phrase.
  if (empty($values['authcode_pass_phrase'])) {
    form_set_error('authcode_pass_phrase', t("The field pass phrase can't be empty."));
  }

  // Check if the pass phrase is correct.
  if (!empty($values['authcode_pass_phrase']) && $values['authcode_pass_phrase'] !== $authcode_phrases[$nid]) {
    form_set_error('authcode_pass_phrase', t('The pass phrase you entered is incorrect. Please note that the pass phrase is case sensitive.'));
  }
}

/**
 * Submit handler for webform_authcode_pass_phrase_form.
 */
function webform_authcode_pass_phrase_form_submit($form, $form_state) {
  // Retrieve values from form_state.
  $values = $form_state['values'];

  // Retrieve the node id.
  $node = menu_get_object();
  $nid = $node->nid;

  // Store the authcode in a cookie.
  $cookie_name = 'webform_authcode_' . $nid;
  user_cookie_save(array($cookie_name => check_plain($values['authcode_pass_phrase'])));
}
