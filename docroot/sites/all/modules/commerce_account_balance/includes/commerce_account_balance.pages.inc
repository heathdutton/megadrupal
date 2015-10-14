<?php

/**
 * @file
 * Page callbacks and  title callbacks.
 */

/**
 * Page callback for the  'deposit' page.
 */
function commerce_account_balance_deposit_form_wrapper($account = NULL) {

  if (!$account) {
    global $user;
    $account = $user;
  }

  module_load_include('inc', 'commerce_account_balance', 'includes/commerce_account_balance.forms');
  $form = drupal_get_form('commerce_account_balance_deposit_form', $account);
  return $form;
}

/**
 * Modal callback for confirming purchase and executing rule.
 */
function commerce_account_balance_purchase_confirmation($js, $field_name, $delta, $entity_type, $bundle_name, $entity_id) {

  if (!$js) {
    return t("This module doesn't work without javascript.");
  }
  else {
    // Check if this is an exisiting instance.
    $field_info = field_info_field($field_name);
    $cardinality = $field_info['cardinality'];
    $instance = field_info_instance($entity_type, $field_name, $bundle_name);
    $entity = entity_load_single($entity_type, $entity_id);
    $entity_wrapper = entity_metadata_wrapper($entity_type, $entity);
    if ($cardinality == 1) {
      $field_data = $entity_wrapper->{$field_name}->value();
    }
    else {
      $field_data = $entity_wrapper->{$field_name}[$delta]->value();
    }
    $rules_config = entity_load_single('rules_config', $field_data['rules_config']);
    $amount = $field_data['amount'];
    $label = entity_label($entity_type, $entity);
    global $user;

    if (!$instance || $entity == '' || $rules_config == '') {
      return drupal_not_found();
    }

    ctools_include('modal');
    ctools_include('ajax');
    ctools_add_js('ajax-responder');

    if ($user->uid == 0) {

      $form_state = array(
        'title' => t('Login'),
        'ajax' => TRUE,
      );
      drupal_set_message(t('You need to login before you can purchase with
        your account.'), 'status', FALSE);
      $commands = ctools_modal_form_wrapper('user_login', $form_state);
      if (!empty($form_state['executed'])) {
        // We need to unset the messages because other wise the previous
        // messages will be repeated on the main screen.
        unset($_SESSION['messages']);
        $commands[] = ctools_ajax_command_reload();
      }
    }
    else {
      // Check if the user has enough balance.
      // If not, then give the option to deposit.
      $balance = _commerce_account_balance_get_balance_raw($user);
      $form_state = array(
        'title' => t('Confirm purchase'),
        'ajax' => TRUE,
        'build_info' => array(
          'args' => array($amount, $balance, $label, $entity, $rules_config),
        ),
      );

      form_load_include($form_state, 'inc', 'commerce_account_balance', 'includes/commerce_account_balance.forms');

      if ($balance >= $amount) {
        $commands = ctools_modal_form_wrapper('commerce_account_balance_purchase_item_form', $form_state);
        if ($form_state['executed'] == TRUE) {
          $commands[] = ctools_ajax_command_reload();
        }
        elseif ($form_state['executed'] == FALSE && isset($form_state['triggering_element'])) {
          if ($form_state['triggering_element']['#name'] == "cancel") {
            $commands[] = ctools_modal_command_dismiss();
          }
        }
      }
      else {
        $commands = ctools_modal_form_wrapper('commerce_account_balance_insufficient_balance_form', $form_state);
        if ($form_state['executed'] == TRUE) {
          $commands[] = ctools_ajax_command_redirect($form_state['redirect']);
        }
        elseif ($form_state['executed'] == FALSE && isset($form_state['triggering_element'])) {
          if ($form_state['triggering_element']['#name'] == "cancel") {
            $commands[] = ctools_modal_command_dismiss();
          }
        }
      }
    }
    print ajax_render($commands);
  }
}
