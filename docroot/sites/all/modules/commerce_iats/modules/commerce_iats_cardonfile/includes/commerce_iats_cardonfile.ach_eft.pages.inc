<?php
/**
 * @file
 * User page callbacks and forms for ACH/EFT payment processing
 * with Card on File.
 */

/**
 * Gets a title describing a card on file.
 *
 * @see commerce_cardonfile_card_title()
 */
function commerce_iats_cardonfile_ach_eft_card_title($card_data) {
  return t('@type bank account ending in @number',
    array('@type' => $card_data->card_type, '@number' => $card_data->card_number));
}

/**
 * Displays form elements for a card on file.
 *
 * @see commerce_cardonfile_card_form_page()
 */
function commerce_iats_cardonfile_ach_eft_card_form_page($op, $card_data, $account) {
  $payment_method = commerce_payment_method_instance_load($card_data->instance_id);

  if ($op == 'update') {
    drupal_set_title(t('Update your @type bank account ending in @number', array('@type' => $card_data->card_type, '@number' => $card_data->card_number)));

    // This is not likely to happen, but if the payment method doesn't implement
    // the update callback, redirect the user back to the card listing page and
    // inform them about the error.
    if (!isset($payment_method['cardonfile']['update callback'])) {
      drupal_set_message(t('We encountered an error attempting to update your bank account information. Please try again and contact us if this error persists.'), 'error');
      drupal_goto('user/' . $card_data->uid . '/cards');
    }
  }
  else {
    drupal_set_title(t('Add new bank account'));
    // Card data was initialized with the anonymous user as its owner. Set the
    // owner here to the user from the menu item, so that the form will receive
    // the complete information that is needed to save the card.
    $card_data->uid = $account->uid;
  }

  if ($form_callback = commerce_cardonfile_payment_method_callback($payment_method, $op . ' form callback')) {
    return drupal_get_form($form_callback, $op, $card_data);
  }
  else {
    return drupal_get_form('commerce_iats_cardonfile_card_form', $op, $card_data);
  }
}

/**
 * Creates form elements for updating a card on file.
 *
 * @see commerce_cardonfile_card_form()
 */
function commerce_iats_cardonfile_ach_eft_card_form($form, &$form_state, $op, $card_data) {
  module_load_include('inc', 'commerce_iats', 'includes/commerce_iats.ach_eft.forms');

  // Pass along information to the validate and submit handlers.
  $form_state['card'] = $card_data;
  $form_state['op'] = $op;

  $account_types = commerce_iats_ach_eft_account_types();

  $fields = array(
    'owner' => '',
    'type' => array_keys($account_types),
  );
  $defaults = array();
  if ($op == 'update') {
    $defaults = array(
      'type' => $card_data->card_type,
      'owner' => $card_data->card_name,
    );
  }

  $payment_form = commerce_iats_ach_eft_payment_form($fields, $defaults);
  $form = array_merge($form, $payment_form);

  if ($op == 'update') {
    $form['bank_account']['number']['#type'] = 'item';
    $form['bank_account']['number']['#markup'] = 'XXXXXXXXXXXX' . $card_data->card_number;
  }

  $payment_method = commerce_payment_method_load($card_data->payment_method);
  $form['bank_account']['cardonfile_instance_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use as default bank account for payments with %method', array('%method' => $payment_method['display_title'])),
    '#default_value' => FALSE,
  );

  // Disable the checkbox if we are adding a new card and the user doesn't have
  // any other active cards with the same instance ID. Also disable it, if we
  // are updating the current default card, so the user can't uncheck the
  // checkbox.
  $existing_cards = commerce_cardonfile_load_multiple_by_uid($card_data->uid, $card_data->instance_id, TRUE);
  $default_card = FALSE;

  if ($op == 'update' && !empty($existing_cards)) {
    foreach ($existing_cards as $card) {
      if ($card->instance_default) {
        if ($card->card_id == $card_data->card_id) {
          $default_card = TRUE;
        }
        break;
      }
    }
  }
  if (($op == 'create' && !$existing_cards) || ($op == 'update' && $default_card)) {
    $form['bank_account']['cardonfile_instance_default']['#default_value'] = TRUE;
    $form['bank_account']['cardonfile_instance_default']['#disabled'] = TRUE;
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ($op == 'create') ? t('Add bank account') : t('Update bank account information'),
    '#suffix' => l(t('Cancel'), 'user/' . $card_data->uid . '/cards'),
  );

  return $form;
}

/**
 * Validates a submitted card on file.
 *
 * @see commerce_cardonfile_card_form_validate()
 */
function commerce_iats_cardonfile_ach_eft_card_form_validate($form, &$form_state) {
  module_load_include('inc', 'commerce_iats', 'includes/commerce_iats.ach_eft.forms');

  $settings = array(
    'form_parents' => array(
      'bank_account',
    ),
  );
  commerce_iats_ach_eft_payment_form_validate($form_state['values']['bank_account'], $settings);
}

/**
 * Creates or updates a card on file.
 *
 * @see commerce_cardonfile_card_form_submit()
 */
function commerce_iats_cardonfile_ach_eft_card_form_submit($form, &$form_state) {
  $op = $form_state['op'];
  $card_data = $form_state['card'];

  $card_data->card_name = $form_state['values']['bank_account']['owner'];
  if ($op == 'create') {
    $card_data->card_number = substr($form_state['values']['bank_account']['number'], -4);
  }

  $card_data->card_type = $form_state['values']['bank_account']['type'];

  // Invoke the payment method's card create/update callback.
  $payment_method = commerce_payment_method_instance_load($card_data->instance_id);
  $callback = $payment_method['cardonfile'][$op . ' callback'];
  $success = FALSE;
  if (function_exists($callback)) {
    $callback_return = $callback($form, $form_state, $payment_method, $card_data);
    if ($callback_return) {
      if ($op == 'create') {
        $card_data_save = $callback_return;
        $confirm_message = t('A new bank account has been added.');
      }
      else {
        $card_data_save = $card_data;
        $confirm_message = t('The stored bank account information has been updated.');
      }
      commerce_cardonfile_save($card_data_save);
      if (!empty($form_state['values']['bank_account']['cardonfile_instance_default'])) {
        commerce_cardonfile_set_default_card($card_data_save->card_id);
      }
      drupal_set_message($confirm_message);
      $success = TRUE;
    }
  }

  if (!$success) {
    if ($op == 'create') {
      drupal_set_message(t('We encountered an error attempting to save your bank account information. Please try again and contact us if this error persists.'), 'error');
    }
    else {
      drupal_set_message(t('We encountered an error attempting to update your bank account information. Please try again and contact us if this error persists.'), 'error');
    }
  }
  $form_state['redirect'] = 'user/' . $card_data->uid . '/cards';
}

/**
 * Displays a delete form for a card on file.
 *
 * @see commerce_cardonfile_delete_form()
 */
function commerce_iats_cardonfile_ach_eft_delete_form($form, &$form_state, $card_data) {
  module_load_include('inc', 'commerce_iats', 'includes/commerce_iats.ach_eft.forms');

  $account_types = commerce_iats_ach_eft_account_types();

  $form['account_type'] = array(
    '#type' => 'item',
    '#title' => t('Card type'),
    '#markup' => !empty($account_types[$card_data->card_type]) ? $account_types[$card_data->card_type] : $card_data->card_type,
  );

  $form['account_number'] = array(
    '#type' => 'item',
    '#title' => t('Account number'),
    '#markup' => 'XXXXXXXXXXXX' . $card_data->card_number,
  );

  $form['account_owner'] = array(
    '#type' => 'item',
    '#title' => t('Account owner'),
    '#markup' => $card_data->card_name,
  );

  $form_state['card'] = $card_data;

  return confirm_form(
    $form,
    t('Are you sure you want to delete this bank account information?'),
    'user/' . $card_data->uid . '/cards'
  );
}

/**
 * Deletes a card on file.
 *
 * @see commerce_cardonfile_delete_form_submit()
 */
function commerce_iats_cardonfile_ach_eft_delete_form_submit($form, &$form_state) {
  $card_data = $form_state['card'];

  // Invoke the payment method's card delete callback.
  $payment_method = commerce_payment_method_instance_load($card_data->instance_id);
  $callback = $payment_method['cardonfile']['delete callback'];

  if (function_exists($callback)) {
    if (!$callback($form, $form_state, $payment_method, $card_data)) {
      // Display a message if we failed to communicate properly with the payment
      // gateway in the Card on File delete callback.
      drupal_set_message(t('We encountered an error attempting to delete your bank account information. Please try again and contact us if this error persists.'), 'error');
      $form_state['redirect'] = 'user/' . $card_data->uid . '/cards';
      return;
    }
  }

  // Disable the card data but retain the record.
  $card_data->status = 0;
  commerce_cardonfile_save($card_data);

  drupal_set_message(t('The stored card information has been deleted.'));

  // Redirect to the payment methods tab if the user has other stored payment.
  $stored_cards = commerce_cardonfile_load_multiple_by_uid($card_data->uid);

  if (!empty($stored_cards)) {
    $form_state['redirect'] = 'user/' . $card_data->uid . '/cards';
  }
  else {
    $form_state['redirect'] = 'user/' . $card_data->uid;
  }
}
