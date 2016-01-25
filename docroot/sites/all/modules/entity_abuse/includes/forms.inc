<?php

/**
 * @file
 * Contains forms for the Entity Abuse module.
 */

/**
 * Abuse submit form.
 */
function entity_abuse_submit_form($form, &$form_state, $entity_type, $entity_id) {

  form_load_include($form_state, 'inc', 'entity_abuse', 'includes/forms');

  $form_id = drupal_html_id('entity-abuse-submit-form-wrapper');
  $form['#attributes']['id'] = $form_id;

  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $entity_type,
  );

  $form['entity_id'] = array(
    '#type' => 'value',
    '#value' => $entity_id,
  );

  $reason_field_status = variable_get('entity_abuse_reason_field', ENTITY_ABUSE_REASON_FIELD_OPTIONAL);
  $form['reason'] = array(
    '#type' => 'textarea',
    '#title' => t('Reason'),
    '#description' => t('Please, enter the reason of your complain.'),
    '#resizable' => FALSE,
    '#required' => $reason_field_status == ENTITY_ABUSE_REASON_FIELD_REQUIRED,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit complain'),
    '#ajax' => array(
      'callback' => 'entity_abuse_submit_form_ajax_callback',
      'wrapper' => $form_id
    )
  );

  return $form;
}

/**
 * Validate callback for abuse submit form.
 */
function entity_abuse_submit_form_validate($form, &$form_state) {

  $values = $form_state['values'];

  if (drupal_strlen($values['reason']) > 255) {
    form_set_error('reason', t('Reason text should not exceed the limit of 250 symbols.'));
  }

  entity_abuse_include('api');
  $user_has_complain = entity_abuse_user_has_complain($values['entity_id'], $values['entity_type']);

  if ($user_has_complain)  {
    form_set_error('entity_id', t('You already complained on this content.'));
  }
}

/**
 * Submit callback for abuse submit form.
 */
function entity_abuse_submit_form_submit($form, &$form_state) {

  // Remove extra data from the values.
  form_state_values_clean($form_state);
  $entity_abuse = $form_state['values'];

  // Add abuse.
  entity_abuse_include('api');
  $result = entity_abuse_add($entity_abuse['entity_id'], $entity_abuse['entity_type'], $entity_abuse['reason']);

  // Set message about results of adding abuse.
  if ($result) {
    drupal_set_message(t('Your complain received. Thank you!'));
  }
  else {
    drupal_set_message(t('You complain was not added. Probably, you already complained for this content.'), 'warning');
  }
}

/**
 * Ajax callback for abuse submit form.
 */
function entity_abuse_submit_form_ajax_callback($form, &$form_state) {

  if (!form_get_errors()) {

    $commands = array();
    $values = $form_state['values'];

    // Replace all links with abuse to this entity with link to cancel abuse.
    $abuse_link = entity_abuse_get_abuse_link($values['entity_id'], $values['entity_type'], TRUE);
    $abuse_link_selector = '.entity-abuse-' . $values['entity_type'] . '-' . $values['entity_id'];
    $commands[] = ajax_command_invoke($abuse_link_selector, 'replaceWith', array(render($abuse_link)));

    // Display abuse result message in the modal dialog.
    entity_abuse_include('ajax');
    $commands[] = entity_abuse_ajax_command_open_dialog(theme('status_messages'));

    return array('#type' => 'ajax', '#commands' => $commands);
  }

  return $form;
}
