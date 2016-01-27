<?php
/**
 * @file
 * User page callbacks and forms for payment processing with Card on File.
 * Callback methods wrap payment method-specific callback methods to provide
 * the correct logic for the current payment method.
 */

/**
 * Gets a title describing a card on file.
 *
 * @see commerce_cardonfile_card_title()
 */
function commerce_iats_cardonfile_card_title($card_data) {
  if ($card_data->payment_method == 'iats_ach_eft') {
    module_load_include('inc', 'commerce_iats_cardonfile', 'includes/commerce_iats_cardonfile.ach_eft.pages');
    return commerce_iats_cardonfile_ach_eft_card_title($card_data);
  }
  else {
    module_load_include('inc', 'commerce_cardonfile', 'includes/commerce_cardonfile.pages');
    return commerce_cardonfile_card_title($card_data);
  }
}

/**
 * Displays form elements for a card on file.
 *
 * @see commerce_cardonfile_card_form_page()
 */
function commerce_iats_cardonfile_card_form_page($op, $card_data, $account) {
  if ($card_data->payment_method == 'iats_ach_eft') {
    module_load_include('inc', 'commerce_iats_cardonfile', 'includes/commerce_iats_cardonfile.ach_eft.pages');
    return commerce_iats_cardonfile_ach_eft_card_form_page($op, $card_data, $account);
  }
  else {
    module_load_include('inc', 'commerce_cardonfile', 'includes/commerce_cardonfile.pages');
    return commerce_cardonfile_card_form_page($op, $card_data, $account);
  }
}

/**
 * Creates form elements for updating a card on file.
 *
 * @see commerce_cardonfile_card_form()
 */
function commerce_iats_cardonfile_card_form($form, &$form_state, $op, $card_data) {
  if ($card_data->payment_method == 'iats_ach_eft') {
    module_load_include('inc', 'commerce_iats_cardonfile', 'includes/commerce_iats_cardonfile.ach_eft.pages');
    return commerce_iats_cardonfile_ach_eft_card_form($form, $form_state, $op, $card_data);
  }
  else {
    module_load_include('inc', 'commerce_cardonfile', 'includes/commerce_cardonfile.pages');
    return commerce_cardonfile_card_form($form, $form_state, $op, $card_data);
  }
}

/**
 * Validates a submitted card on file.
 *
 * @see commerce_cardonfile_card_form_validate()
 */
function commerce_iats_cardonfile_card_form_validate($form, &$form_state) {
  if ($form_state['card']->payment_method == 'iats_ach_eft') {
    module_load_include('inc', 'commerce_iats_cardonfile', 'includes/commerce_iats_cardonfile.ach_eft.pages');
    commerce_iats_cardonfile_ach_eft_card_form_validate($form, $form_state);
  }
  else {
    module_load_include('inc', 'commerce_cardonfile', 'includes/commerce_cardonfile.pages');
    commerce_cardonfile_card_form_validate($form, $form_state);
  }
}

/**
 * Creates or updates a card on file.
 *
 * @see commerce_cardonfile_card_form_submit()
 */
function commerce_iats_cardonfile_card_form_submit($form, &$form_state) {
  if ($form_state['card']->payment_method == 'iats_ach_eft') {
    module_load_include('inc', 'commerce_iats_cardonfile', 'includes/commerce_iats_cardonfile.ach_eft.pages');
    commerce_iats_cardonfile_ach_eft_card_form_submit($form, $form_state);
  }
  else {
    module_load_include('inc', 'commerce_cardonfile', 'includes/commerce_cardonfile.pages');
    commerce_cardonfile_card_form_submit($form, $form_state);
  }
}

/**
 * Displays a delete form for a card on file.
 *
 * @see commerce_cardonfile_delete_form()
 */
function commerce_iats_cardonfile_delete_form($form, &$form_state, $card_data) {
  if ($card_data->payment_method == 'iats_ach_eft') {
    module_load_include('inc', 'commerce_iats_cardonfile', 'includes/commerce_iats_cardonfile.ach_eft.pages');
    return commerce_iats_cardonfile_ach_eft_delete_form($form, $form_state, $card_data);
  }
  else {
    module_load_include('inc', 'commerce_cardonfile', 'includes/commerce_cardonfile.pages');
    return commerce_cardonfile_delete_form($form, $form_state, $card_data);
  }
}

/**
 * Deletes a card on file.
 *
 * @see commerce_cardonfile_delete_form_submit()
 */
function commerce_iats_cardonfile_delete_form_submit($form, &$form_state) {
  if (isset($form_state['card']) && ($form_state['card']->payment_method == 'iats_ach_eft')) {
    module_load_include('inc', 'commerce_iats_cardonfile', 'includes/commerce_iats_cardonfile.ach_eft.pages');
    commerce_iats_cardonfile_ach_eft_delete_form_submit($form, $form_state);
  }
  else {
    module_load_include('inc', 'commerce_cardonfile', 'includes/commerce_cardonfile.pages');
    commerce_cardonfile_delete_form_submit($form, $form_state);
  }
}
