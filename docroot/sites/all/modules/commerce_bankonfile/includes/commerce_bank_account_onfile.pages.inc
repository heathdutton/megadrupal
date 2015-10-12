<?php

/**
 * Menu callback: displays the add bank account page.
 *
 * If there are multiple Bank Account on File payment methods available, show a list and
 * require the user to select one. Otherwise redirect to the add form.
 */
function commerce_bank_account_onfile_add_page() {
  $account = menu_get_object('user');
  $create_implements = commerce_bank_account_file_payment_method_implements('create callback');
  $content = array();
  foreach ($create_implements as $method_id => $method_function) {
    $payment_method_instances = _commerce_bank_account_onfile_payment_method_instances($method_id);
    foreach ($payment_method_instances as $instance_id => $payment_method) {
      $path = 'user/' . $account->uid . '/bank-accounts/add/' . drupal_hash_base64($instance_id);
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

  return theme('bank_account_add_list', array('content' => $content));
}

/**
 * Displays the add bank account  list.
 *
 * @ingroup themeable
 */
function theme_bank_account_add_list($variables) {
  $output = '<dl class="commerce-bank-account-onfile-add-bank-account-list">';
  foreach ($variables['content'] as $item) {
    $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
  }
  $output .= '</dl>';

  return $output;
}

/**
 * Returns the appropriate bank account form.
 */
function commerce_bank_account_onfile_bank_account_form_page($op, $bank_account, $account) {
  $payment_method = commerce_payment_method_instance_load($bank_account->instance_id);

  if ($op == 'update') {
    // This is not likely to happen, but if the payment method doesn't implement
    // the update callback, redirect the user back to the bank account listing page and
    // inform them about the error.
    if (!isset($payment_method['bank_account_onfile']['update callback'])) {
      drupal_set_message(t('We encountered an error attempting to update your bank account data. Please try again and contact us if this error persists.'), 'error');
      drupal_goto('user/' . $bank_account->uid . '/bank-accounts');
    }
  }
  else {
    drupal_set_title(t('Add a bank account'));
    // Bank account data was initialized with the anonymous user as its owner. Set the
    // owner here to the user from the menu item, so that the form will receive
    // the complete information that is needed to save the bank account.
    $bank_account->uid = $account->uid;
  }

  if ($form_callback = commerce_bank_account_onfile_payment_method_callback($payment_method, $op . ' form callback')) {
    return drupal_get_form($form_callback, $op, $bank_account);
  }
  else {
    return drupal_get_form('commerce_cardonfile_card_form', $op, $bank_account);
  }
}

/**
 * Form callback: create or edit a bank account.
 */
/*
function commerce_cardonfile_card_form($form, &$form_state, $op, $bank_account) {
  // Load the credit card helper functions from the Payment module.
  // module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  // Pass along information to the validate and submit handlers.
  $form_state['bank_account'] = $bank_account;
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
      'type' => $bank_account->bank_account_type,
      'owner' => $bank_account->bank_account_name,
      'exp_month' => $card->card_exp_month,
      'exp_year' => $card->card_exp_year,
    );
  }
  $form += commerce_payment_credit_card_form($fields, $defaults);
  if ($op == 'update') {
    $form['credit_card']['type']['#access'] = FALSE;
    $form['credit_card']['number']['#access'] = FALSE;
  }

  $payment_method = commerce_payment_method_load($bank_account->payment_method);
  $form['credit_card']['cardonfile_instance_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use as default card for payments with %method', array('%method' => $payment_method['display_title'])),
    '#default_value' => FALSE,
  );
  // Disable the checkbox if we are adding a new card and the user doesn't have
  // any other active cards with the same instance ID. Also disable it, if we
  // are updating the current default card, so the user can't uncheck the
  // checkbox.
  $existing_bank_accounts = commerce_cardonfile_load_multiple_by_uid($bank_account->uid, $bank_account->instance_id, TRUE);
  if (($op == 'create' && !$existing_bank_accounts) || ($op == 'update' && $bank_account->instance_default)) {
    $form['credit_card']['cardonfile_instance_default']['#default_value'] = TRUE;
    $form['credit_card']['cardonfile_instance_default']['#access'] = FALSE;
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ($op == 'create') ? t('Add card') : t('Update card'),
    '#suffix' => l(t('Cancel'), 'user/' . $bank_account->uid . '/cards'),
  );

  return $form;
}*/
