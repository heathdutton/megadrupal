<?php

/**
 * @file
 * User page callbacks and forms for Commerce Card on File.
 */

/**
 * Menu item title callback: returns the card title.
 */
function commerce_cardonfile_card_title($card) {
  return $card->label();
}

/**
 * Menu callback: displays the add card page.
 *
 * If there are multiple CoF payment methods available, show a list and
 * require the user to select one. Otherwise redirect to the add form.
 */
function commerce_cardonfile_add_page() {
  $account = menu_get_object('user');
  $create_implements = commerce_cardonfile_payment_method_implements('create callback');
  $content = array();
  foreach ($create_implements as $method_id => $method_function) {
    $payment_method_instances = _commerce_cardonfile_payment_method_instances($method_id);
    foreach ($payment_method_instances as $instance_id => $payment_method) {
      $path = 'user/' . $account->uid . '/cards/add/' . drupal_hash_base64($instance_id);
      $item = menu_get_item($path);
      if ($item['access']) {
        $content[] = $item;
      }
    }
  }

  // Bypass the user/%user/cards/add listing if only one payment method is
  // available.
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }

  return theme('card_add_list', array('content' => $content));
}

/**
 * Displays the add card list.
 *
 * @ingroup themeable
 */
function theme_card_add_list($variables) {
  $output = '<dl class="commerce-cardonfile-add-card-list">';
  foreach ($variables['content'] as $item) {
    $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
  }
  $output .= '</dl>';

  return $output;
}

/**
 * Returns the appropriate card form.
 */
function commerce_cardonfile_card_form_page($op, $card, $account) {
  $payment_method = commerce_payment_method_instance_load($card->instance_id);

  if ($op == 'update') {
    // This is not likely to happen, but if the payment method doesn't implement
    // the update callback, redirect the user back to the card listing page and
    // inform them about the error.
    if (!isset($payment_method['cardonfile']['update callback'])) {
      drupal_set_message(t('We encountered an error attempting to update your card data. Please try again and contact us if this error persists.'), 'error');
      drupal_goto('user/' . $card->uid . '/cards');
    }
  }
  else {
    drupal_set_title(t('Add a card'));
    // Card data was initialized with the anonymous user as its owner. Set the
    // owner here to the user from the menu item, so that the form will receive
    // the complete information that is needed to save the card.
    $card->uid = $account->uid;
  }

  if ($form_callback = commerce_cardonfile_payment_method_callback($payment_method, $op . ' form callback')) {
    return drupal_get_form($form_callback, $op, $card);
  }
  else {
    return drupal_get_form('commerce_cardonfile_card_form', $op, $card);
  }
}

/**
 * Form callback: create or edit a card.
 */
function commerce_cardonfile_card_form($form, &$form_state, $op, $card) {
  // Load the credit card helper functions from the Payment module.
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  // Pass along information to the validate and submit handlers.
  $form_state['card'] = $card;
  $form_state['op'] = $op;

  $fields = array(
    'owner' => '',
    'code' => '',
    'type' => array(
      'visa',
      'mastercard',
      'amex',
    ),
  );
  $defaults = array();
  if ($op == 'update') {
    unset($fields['code']);
    $defaults = array(
      'type' => $card->card_type,
      'owner' => $card->card_name,
      'exp_month' => $card->card_exp_month,
      'exp_year' => $card->card_exp_year,
    );
  }
  $form += commerce_payment_credit_card_form($fields, $defaults);
  if ($op == 'update') {
    $form['credit_card']['type']['#access'] = FALSE;
    $form['credit_card']['number']['#access'] = FALSE;
  }

  $payment_method = commerce_payment_method_load($card->payment_method);
  $form['credit_card']['cardonfile_instance_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use as default card for payments with %method', array('%method' => $payment_method['display_title'])),
    '#default_value' => FALSE,
  );
  // Disable the checkbox if we are adding a new card and the user doesn't have
  // any other active cards with the same instance ID. Also disable it, if we
  // are updating the current default card, so the user can't uncheck the
  // checkbox.
  $existing_cards = commerce_cardonfile_load_multiple_by_uid($card->uid, $card->instance_id, TRUE);
  if (($op == 'create' && !$existing_cards) || ($op == 'update' && $card->instance_default)) {
    $form['credit_card']['cardonfile_instance_default']['#default_value'] = TRUE;
    $form['credit_card']['cardonfile_instance_default']['#access'] = FALSE;
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ($op == 'create') ? t('Add card') : t('Update card'),
    '#suffix' => l(t('Cancel'), 'user/' . $card->uid . '/cards'),
  );

  return $form;
}

/**
 * Validation callback for commerce_cardonfile_card_form().
 */
function commerce_cardonfile_card_form_validate($form, &$form_state) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  if ($form_state['op'] == 'create') {
    commerce_payment_credit_card_validate($form_state['values']['credit_card'], array('form_parents' => array('credit_card')));
  }
  else {
    // Ensure the expiration date is not being updated to a past date.
    $exp_valid = commerce_payment_validate_credit_card_exp_date($form_state['values']['credit_card']['exp_month'], $form_state['values']['credit_card']['exp_year']);
    if ($exp_valid !== TRUE) {
      form_set_error('credit_card][exp_' . $exp_valid, t('You have specified an expired credit card.'));
    }
  }
}

/**
 * Submit callback for commerce_cardonfile_card_form().
 */
function commerce_cardonfile_card_form_submit($form, &$form_state) {
  $op = $form_state['op'];
  $card = $form_state['card'];

  $card->card_name = $form_state['values']['credit_card']['owner'];
  $card->card_exp_month = $form_state['values']['credit_card']['exp_month'];
  $card->card_exp_year = $form_state['values']['credit_card']['exp_year'];
  if ($op == 'create') {
    $card->card_number = substr($form_state['values']['credit_card']['number'], -4);
    $card->card_type = $form_state['values']['credit_card']['type'];
  }
  $card->instance_default = $form_state['values']['credit_card']['cardonfile_instance_default'];

  // Invoke the payment method's card create/update callback.
  $payment_method = commerce_payment_method_instance_load($card->instance_id);
  $callback = $payment_method['cardonfile'][$op . ' callback'];
  $success = FALSE;
  if (function_exists($callback)) {
    $callback_return = $callback($form, $form_state, $payment_method, $card);
    if ($callback_return) {
      if ($op == 'create') {
        $card_save = $callback_return;
        $confirm_message = t('A new card has been added.');
      }
      else {
        $card_save = $card;
        $confirm_message = t('The card has been updated.');
      }
      commerce_cardonfile_save($card_save);
      drupal_set_message($confirm_message);
      $success = TRUE;
    }
  }

  if (!$success) {
    if ($op == 'create') {
      drupal_set_message(t('We encountered an error attempting to save your card data. Please try again and contact us if this error persists.'), 'error');
    }
    else {
      drupal_set_message(t('We encountered an error attempting to update your card data. Please try again and contact us if this error persists.'), 'error');
    }
  }
  $form_state['redirect'] = 'user/' . $card->uid . '/cards';
}

/**
 * Builds the form for deleting cardonfile data.
 */
function commerce_cardonfile_delete_form($form, &$form_state, $card) {
  $entity_view = entity_view('commerce_cardonfile', array($card), 'customer', NULL, TRUE);
  $rendered_card = drupal_render($entity_view);

  return confirm_form(
    $form,
    t('Are you sure you want to delete this card?'),
    'user/' . $card->uid . '/cards',
    $rendered_card,
    t('Delete')
  );
}

/**
 * Form submit handler: delete stored card data.
 */
function commerce_cardonfile_delete_form_submit($form, &$form_state) {
  $card = $form_state['build_info']['args'][0];

  // Invoke the payment method's card delete callback.
  $payment_method = commerce_payment_method_instance_load($card->instance_id);
  $callback = $payment_method['cardonfile']['delete callback'];
  if (function_exists($callback)) {
    if (!$callback($form, $form_state, $payment_method, $card)) {
      // Display a message if we failed to communicate properly with the payment
      // gateway in the Card on File delete callback.
      drupal_set_message(t('We encountered an error attempting to delete your card data. Please try again and contact us if this error persists.'), 'error');
      $form_state['redirect'] = 'user/' . $card->uid . '/cards';
      return;
    }
  }

  // Delete the card.
  commerce_cardonfile_delete($card->card_id);

  drupal_set_message(t('The card has been deleted.'));
  $form_state['redirect'] = 'user/' . $card->uid . '/cards';
}
