<?php

/**
 * @file
 * Defines settings form and checkbox for checkout pane.
 */

/**
 * Settings form callback for the pane.
 */
function commerce_agree_terms_pane_settings_form($checkout_pane) {
  $form = array();

  $form['commerce_agree_terms_pane_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Pane title'),
    '#default_value' => variable_get('commerce_agree_terms_pane_title', t('Terms and Conditions')),
    '#required' => TRUE,
  );
  $form['commerce_agree_terms_checkbox_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Checkbox label'),
    '#default_value' => variable_get('commerce_agree_terms_checkbox_label', t('I agree with the %terms')),
    '#description' => t('%terms is the link with the pane title as the link text'),
    '#required' => TRUE,
  );
  $form['commerce_agree_terms_pane_node'] = array(
    '#type' => 'textfield',
    '#title' => t('Terms and Conditions path'),
    '#default_value' => variable_get('commerce_agree_terms_pane_node', ''),
    '#description' => t('The internal Drupal path to your terms and conditions node (or an external URL)'),
    '#required' => TRUE,
  );
  $form['commerce_agree_terms_new_window'] = array(
    '#type' => 'checkbox',
    '#title' => t('Open the link in a new window'),
    '#default_value' => variable_get('commerce_agree_terms_new_window', 1),
  );
  $form['commerce_agree_terms_checkbox_error'] = array(
    '#type' => 'textfield',
    '#title' => t('Error message'),
    '#default_value' => variable_get('commerce_agree_terms_checkbox_error', t('You must agree with the %terms before continuing')),
    '#description' => t('User friendly message to show if the customer fails to check the checkbox'),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Implements base_checkout_form().
 */
function commerce_agree_terms_pane_checkout_form($form, $form_state, $checkout_pane, $order) {

  $title = variable_get('commerce_agree_terms_pane_title', t('Terms and Conditions'));
  $link_url = variable_get('commerce_agree_terms_pane_node', '');

  // Build the link, in the same window or a new one.
  if (variable_get('commerce_agree_terms_new_window', 1) == 0) {
    $link = l($title, $link_url);
  }
  else {
    $link = l($title, $link_url, array(
      'attributes' => array(
        'target' => '_blank',
      ),
    ));
  }
  // Position the link in the label using the %terms placeholder.
  $label = strtr(variable_get('commerce_agree_terms_checkbox_label', t('I agree with the %terms')), array(
    '%terms' => $link,
  ));

  // Create the checkbox.
  $checkout_form['commerce_agree_terms_pane_field'] = array(
    '#type' => 'checkbox',
    '#title' => $label,
    '#required' => TRUE,
  );
  return $checkout_form;
}

/**
 * Implements base_checkout_form_validate().
 */
function commerce_agree_terms_pane_checkout_form_validate($form, &$form_state) {

  $title = variable_get('commerce_agree_terms_pane_title', t('Terms and Conditions'));
  $error_message = filter_xss_admin(strtr(variable_get('commerce_agree_terms_checkbox_error', t('You must agree with the %terms before continuing')), array(
    '%terms' => $title,
  )));

  if (form_get_errors()) {
    // Clear the fields default error message because we are adding a custom one below.
    // If we don't clear it we will get two errors on the field.
    $form_errors = form_get_errors();
    $drupal_errors = drupal_get_messages('error');

    form_clear_error();
    if (!empty($drupal_errors['error'])) {
      foreach ($drupal_errors['error'] as $key => $error) {
        if (in_array($error, $form_errors)) {
          unset($drupal_errors['error'][$key]);
        }
      }
    }

    // Set our custom error message.
    if ($form_state['values']['terms_conditions']['commerce_agree_terms_pane_field'] === 0) {
      form_set_error('commerce_agree_terms_pane_field', $error_message);
      return FALSE;
    }
  }
  else {
    return TRUE;
  }
}
