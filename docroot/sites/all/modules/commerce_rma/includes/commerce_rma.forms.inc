<?php

/**
 * @file
 * Exposes the various form callbacks used by commerce_rma
 */

/**
 * Implements hook_form().
 */
function commerce_rma_return_form($form, &$form_state, CommerceReturn $return) {
  global $user;
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_rma') . '/includes/commerce_rma.forms.inc';

  // Ensure the owner name is accessible if the uid is set.
  if (!empty($return->uid) && $owner = user_load($return->uid)) {
    $return->name = $owner->name;

    if (empty($return->mail)) {
      $return->mail = $owner->mail;
    }
  } else {
    $return->name = $user->name;
    $return->mail = $user->mail;
  }

  // Add the field related form elements.
  $form_state['commerce_return'] = $return;
  field_attach_form('commerce_return', $return, $form, $form_state);

  // Hide the return total and order reference fields from direct editing.
  $form['commerce_return_total']['#access'] = FALSE;
  $form['commerce_order_reference']['#access'] = FALSE;

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-return-actions')),
    '#weight' => 100,
  );

  // Expose a cancel button.
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel return'),
    // We don't validate the fields widget.
    '#limit_validation_errors' => array(),
    '#submit' => array('commerce_rma_cancel_return_form'),
  );

  // Expose a submit button.
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Next', array(), array('context' => 'a drupal commerce return')),
  );

  return $form;
}

/**
 * Implements hook_form_validate().
 */
function commerce_rma_return_form_validate($form, &$form_state) {
  field_attach_form_validate('commerce_return', $form_state['commerce_return'], $form, $form_state);
}

/**
 * Implements hook_form_submit().
 */
function commerce_rma_return_form_submit($form, &$form_state) {
  // Notify field widgets.
  field_attach_submit('commerce_return', $form_state['commerce_return'], $form, $form_state);

  // Ensure the attached customer profile is associated with the order owner
  // if it does not have a uid yet and the order does.
  $return_wrapper = entity_metadata_wrapper('commerce_return', $form_state['commerce_return']);

  // Ensure the customer profile is correctly attached to entity.
  if (!is_null($return_wrapper->commerce_customer_rma->value()) && $return_wrapper->commerce_customer_rma->uid->value() == 0) {
    $return_wrapper->commerce_customer_rma->uid = $form_state['commerce_return']->uid;
    $return_wrapper->commerce_customer_rma->save();
  }

  // We are not saving the return presently, caching it using ctools.
  ctools_include('object-cache');
  ctools_object_cache_set('CommerceReturn', 'rma', $return_wrapper->value());

  // Redirect the customer.
  $form_state['redirect'] = 'user/' . $return_wrapper->uid->value() . '/orders/' . $return_wrapper->commerce_order_reference->value()->order_id . '/return/review';
}

/**
 * Implements hook_form_submit().
 */
function commerce_rma_cancel_return_form($form, &$form_state) {
  // Remove the cached object.
  ctools_include('object-cache');
  ctools_object_cache_clear('CommerceReturn', 'rma');
  $form_state['redirect'] = 'user/' . $form_state['commerce_return']->uid . '/orders';
}

/**
 * Implements hook_form().
 */
function commerce_rma_return_review_form($form, &$form_state, CommerceReturn $return) {

  // Store the return object into the form_state array.
  $form_state['commerce_return'] = $return;

  $form['rendered_return'] = array(
    entity_view('commerce_return', array($return)),
  );

  // Hiding commerce_order_reference, useless for the customer.
  $form['rendered_return'][0]['commerce_return'][NULL]['commerce_order_reference']['#access'] = FALSE;

  // Adding a confirmation checkbox.
  $form['confirmation'] = array(
    '#type' => 'checkbox',
    '#title' => t('By checking this you are submitting a new return to us. Please, make sure the correct products have been selected.'),
    '#default_value' => FALSE,
    '#return_value' => TRUE,
  );

  $form['action'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-return-action-back')),
    '#weight' => 101,
  );

  $form['action']['cancel'] = array(
    '#type' => 'submit',
    '#executes_submit_callback' => TRUE,
    '#value' => t('Back'),
    '#limit_validation_errors' => array(),
    '#submit' => array('commerce_rma_return_review_back_form'),
  );

  $form['action']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm', array(), array('context' => 'a drupal commerce return')),
  );

  return $form;
}

/**
 * Implements hook_form().
 *
 * Redirect customer to previous step.
 */
function commerce_rma_return_review_back_form($form, &$form_state) {
  $order = entity_metadata_wrapper('commerce_return', $form_state['commerce_return'])->commerce_order_reference->value();
  $form_state['redirect'] = 'user/' . $form_state['commerce_return']->uid . '/orders/' . $order->order_id . '/return/add';
}

/**
 * Implements hook_form_validate().
 */
function commerce_rma_return_review_form_validate($form, &$form_state) {
  if (empty($form_state['values']['confirmation'])) {
    form_set_error('confirmation', t('You must check the confirmation checkbox to get through.'));
  }
}

/**
 * Implements hook_form_submit().
 */
function commerce_rma_return_review_form_submit($form, &$form_state) {
  $return = $form_state['commerce_return'];
  // Finally, save the return object.
  if (commerce_return_save($return)) {
    // Send a message to customer.
    drupal_set_message(t('- Your request for return @number is confirmed.', array('@number' => $return->return_number)));
  } else {
    drupal_set_message(t('Something wrong happened, please contact us'), 'error');
  }

  // Redirect to user returns.
  $form_state['redirect'] = 'user/' . $return->uid . '/returns/' . $return->return_id;
}
