<?php

/**
 * @file
 * All ec_receipt admin functions.
 */

/**
 * Confirm refund receipt to customer form.
 */
function ec_receipt_refund(&$form, &$form_state, $receipt) {
  $form = ec_receipt_view_form($form_state, $receipt);

  return confirm_form($form, t('Are you sure you want to refund this receipt?'), isset($_GET['destination']) ? $_GET['destination'] : 'admin/store/receipts', t('This will refund the money to the customer'), t('Refund'));
}

/**
 * Confirm refund receipt to customer form submit.
 */
function ec_receipt_refund_submit(&$form, &$form_state) {
  $receipt = & $form_state['values']['receipt'];
  $alloc[] = array(
    'type' => 'refund',
    'id' => $receipt->erid,
  );
  ec_receipt_allocate($receipt, $alloc);
  $type = ec_receipt_get_types('type', $receipt->type);

  if (isset($type->allow_refunds) && $type->allow_refunds) {
    ec_receipt_invoke($receipt->type, 'refund', $receipt);
  }
  else {
    $receipt->status = RECEIPT_STATUS_REFUND_PENDING;
    ec_receipt_save($receipt);
    drupal_set_message(t('Receipt %erid has been marked as Refund Pending. Once this receipt has been refunded, please mark this receipt as Refunded.', array('%erid' => $receipt->erid)));
  }

  $form_state['redirect'] = array('admin/store/receipts', 'erid=' . $receipt->erid);
}

/**
 * Administration for receipt types.
 */
function ec_receipt_admin_rtypes_form($form, &$form_state) {
  $rtypes = ec_receipt_get_types();
  $form = array();
  $form['types'] = array(
    '#tree' => TRUE,
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'ec_receipt') . '/css/admin.css',
      ),
    ),
  );
  if (!empty($rtypes)) {
    foreach ($rtypes as $type => $info) {
      if (empty($info->internal) || !$info->internal) {
        $form['types'][$type]['type'] = array(
          '#type' => 'value',
          '#value' => $info->type,
        );
        $form['types'][$type]['name'] = array(
          '#markup' => l($info->name, 'admin/config/store/rtypes/' . $type),
        );
        $form['types'][$type]['icon'] = array(
          '#markup' => isset($info->icon) ? theme('ec_receipt_icon', array('icon' => $info->icon)) : '',
        );
        $form['types'][$type]['description'] = array(
          '#markup' => $info->description,
        );
        $form['types'][$type]['allow_payments'] = array(
          '#markup' => isset($info->allow_payments) ? ($info->allow_payments ? t('Yes') : t('No')) : '<em>' . t('N/A') . '</em>',
        );
        $form['types'][$type]['allow_admin_payments'] = array(
          '#markup' => isset($info->allow_admin_payments) ? ($info->allow_admin_payments ? t('Yes') : t('No')) : '<em>' . t('N/A') . '</em>',
        );
        $form['types'][$type]['allow_refunds'] = array(
          '#markup' => isset($info->allow_refunds) ? ($info->allow_refunds ? t('Yes') : t('No')) : '<em>' . t('N/A') . '</em>',
        );
        $form['types'][$type]['allow_payto'] = array(
          '#markup' => isset($info->allow_payto) ? ($info->allow_payto ? t('Yes') : t('No')) : '<em>' . t('N/A') . '</em>',
        );
        $form['types'][$type]['allow_recurring'] = array(
          '#markup' => isset($info->allow_recurring) ? ($info->allow_recurring ? t('Yes') : t('No')) : '<em>' . t('N/A') . '</em>',
        );
        $form['types'][$type]['weight'] = array(
          '#type' => 'weight',
          '#default_value' => !empty($info->weight) ? $info->weight : 0,
        );
      }
    }
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
    );
  }
  else {
    drupal_set_message(t('No payment gateways or receipt methods have been enabled'), 'error');
  }
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_receipt_admin_rtypes_form_submit(&$form, &$form_state) {
  foreach (element_children($form_state['values']['types']) as $type) {
    if ($info = ec_receipt_get_types('type', $type)) {
      ec_receipt_type_save((object) $form_state['values']['types'][$type]);
    }
    else {
      ec_receipt_type_delete($type);
    }
  }
}

/**
 * Settings form for ec_receipt.
 */
function ec_receipt_settings($form, &$form_state) {
  $form = array();

  $options = array('' => '--');
  $options += ec_receipt_currency_list();

  $form['ec_default_currency'] = array(
    '#type' => 'select',
    '#title' => t('Default currency'),
    '#default_value' => variable_get('ec_default_currency', ''),
    '#options' => $options,
    '#required' => TRUE,
    '#description' => t('Currencies are dependent on payment gateways. If there appear to be no currencies available, ensure at least one "Payment Methods" module is enabled.'),
  );

  $form['ec_receipt_valid_cards'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Valid Card Types'),
    '#default_value' => variable_get('ec_receipt_valid_cards', array()),
    '#options' => ec_receipt_card_types(TRUE),
    '#description' => t('Select card types which will be accepted. Note that not all payment gateways allow this functionality.'),
  );

  $form['ec_receipt_return_url'] =  array(
    '#type' => 'textfield',
    '#title' => t('Successful payment URL'),
    '#default_value' => variable_get('ec_receipt_return_url', 'node'),
    '#size' => 70,
    '#maxlength' => 180,
    '#description' => t('This is the destination to which you would like to send your customers when their payment has been successfully completed. The URL must be a Drupal system path. If you are not using clean URLs, specify the part after \'?q=\'. If unsure, specify \'node\'. You may also enter \'%order-history\' to link to the user\'s order history.'),
  );

  $form['ec_receipt_cancel_url'] =  array(
    '#type' => 'textfield',
    '#title' => t('Cancel payment URL'),
    '#default_value' => variable_get('ec_receipt_cancel_url', 'node'),
    '#size' => 70,
    '#maxlength' => 180,
    '#description' => t('This is the destination to which you would like to send your customers if they cancel their payment. If you specify \'%cancel-order\' the customer will be prompted to cancel the transaction. The URL must be a Drupal system path. If you are not using clean URLs, specify the part after "?q=". If unsure, specify "node".'),
  );

  return system_settings_form($form);
}

/**
 * Edit Receipt types form.
 */
function ec_receipt_admin_type_form($form, &$form_state, $type) {
  $info = ec_receipt_get_types('type', $type);
  $form = array();
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $info->type,
  );

  if (isset($info->gateway_requirements)) {
    $form['requirements'] = array(
      '#type' => 'item',
      '#title' => t('Requirements'),
      '#value' => is_array($info->gateway_requirements) ? theme('item_list', array('items' => $info->gateway_requirements)) : $info->gateway_requirements,
    );
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $info->name,
    '#description' => t('This is the name of the payment method which will be shown to your customers'),
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $info->description,
    '#description' => t('Description of the payment gateway as the customer will see it.'),
  );

  $yn_options = array(
    0 => t('No'),
    1 => t('Yes'),
  );

  $form['allow_payments'] = array(
    '#type' => 'select',
    '#title' => t('Allow payments'),
    '#default_value' => isset($info->allow_payments) ? $info->allow_payments : "0",
    '#options' => isset($info->allow_payments) ?  $yn_options : array(t('N/A')),
    '#disabled' => isset($info->allow_payments) ? FALSE : TRUE,
    '#description' => t('Disable/Enable Payments by customers'),
  );
  $form['allow_admin_payments'] = array(
    '#type' => 'select',
    '#title' => t('Allow 3rd party payments'),
    '#default_value' => isset($info->allow_admin_payments) ? $info->allow_admin_payments : 0,
    '#options' => isset($info->allow_admin_payments) ? $yn_options  : array(t('N/A')),
    '#disabled' => isset($info->allow_admin_payments) ? FALSE : TRUE,
    '#description' => t('Disable/Enable Payments for customers by eC Store Owners'),
  );
  $form['allow_refunds'] = array(
    '#type' => 'select',
    '#title' => t('Allow refunds'),
    '#default_value' => isset($info->allow_refunds) ? $info->allow_refunds : 0,
    '#options' => isset($info->allow_refunds) ? $yn_options : array(t('N/A')),
    '#disabled' => isset($info->allow_refunds) ? FALSE : TRUE,
    '#description' => t('Disable/Enable Refunds to customers'),
  );
  $form['allow_payto'] = array(
    '#type' => 'select',
    '#title' => t('Allow payments to customers'),
    '#default_value' => isset($info->allow_payto) ? $info->allow_payto : 0,
    '#options' => isset($info->allow_payto) ? $yn_options : array(t('N/A')),
    '#disabled' => isset($info->allow_payto) ? FALSE : TRUE,
    '#description' => t('Disable/Enable Payments to customers'),
  );
  $form['allow_recurring'] = array(
    '#type' => 'select',
    '#title' => t('Allow recurring payments'),
    '#default_value' => isset($info->allow_recurring) ? $info->allow_recurring : 0,
    '#options' => isset($info->allow_recurring) ? $yn_options : array(t('N/A')),
    '#disabled' => isset($info->allow_recurring) ? FALSE : TRUE,
    '#description' => t('Disable/Enable Payments by customers to be recurring'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update receipt type'),
  );
  return $form;
}

/**
 * Edit Receipt types form submit.
 */
function ec_receipt_admin_type_form_submit(&$form, &$form_state) {
  ec_receipt_type_save((object) $form_state['values']);
}

/**
 * Administer allocation types form.
 */
function ec_receipt_admin_atypes_form($form, &$form_state) {
  $form = array();

  $form['types'] = array(
    '#tree' => TRUE,
  );
  $types = ec_receipt_get_atypes();

  foreach ($types as $type => $info) {
    $form['types'][$type]['type'] = array(
      '#markup' => $info->type,
    );
    $form['types'][$type]['name'] = array(
      '#markup' => $info->name,
    );
    $form['types'][$type]['description'] = array(
      '#markup' => $info->description,
    );
  }

  return $form;
}

/**
 * Receipt view form.
 */
function ec_receipt_view_form($form, &$form_state, $receipt, $links = FALSE) {
  $form['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'ec_receipt') . '/css/receipt.css',
    ),
  );

  $form['receipt'] = array(
    '#type' => 'value',
    '#markup' => $receipt,
  );
  $form['erid'] = array(
    '#type' => 'item',
    '#title' => t('Receipt No'),
    '#markup' => $receipt->erid,
  );
  $form['type'] = array(
    '#type' => 'item',
    '#title' => t('Type'),
    '#markup' => !($type = ec_receipt_get_types('name', $receipt->type)) ? $type : $receipt->type,
  );
  $form['amounts'] = array(
    '#prefix' => '<div id="receipt-amounts">',
    '#suffix' => '</div>',
  );
  $form['amounts']['amount'] = array(
    '#type' => 'item',
    '#title' => t('Amount'),
    '#markup' => format_currency($receipt->amount),
  );
  $form['amounts']['allocated'] = array(
    '#type' => 'item',
    '#title' => t('Allocated'),
    '#markup' => format_currency($receipt->allocated),
  );
  $form['amounts']['balance'] = array(
    '#type' => 'item',
    '#title' => t('Balance'),
    '#markup' => format_currency($receipt->balance),
  );

  $name = ec_customer_get_name($receipt->ecid);
  $view_link = ec_customer_links($receipt->ecid, 'view');

  $form['customer'] = array(
    '#type' => 'item',
    '#title' => t('Customer'),
    '#markup' => !$links && empty($view_link) ? $name : l($name, $view_link[0]),
  );
  $form['alloc'] = array(
    '#type' => 'item',
    '#title' => t('Allocation'),
    '#markup' => views_embed_view('ec_receipt_allocation_list'),
  );

  return $form;
}

/**
 * Reverse allocation form.
 */
function ec_receipt_reverse_allocation_form($form, $form_state, $receipt, $eaid = NULL) {
  if (empty($eaid) || !($alloc = db_query('SELECT * FROM {ec_receipt_allocation} WHERE eaid = :eaid', array(':eaid' => $eaid))->execute()->fetchObject())) {
    drupal_not_found();
    exit();
  }

  $form['receipt'] = array(
    '#type' => 'value',
    '#value' => $receipt,
  );
  $form['alloc'] = array(
    '#type' => 'value',
    '#value' => $alloc,
  );

  return confirm_form($form, t('Reverse allocation for receipt %erid', array('%erid' => $receipt->erid)), 'store/receipt/' . $receipt->erid . '/view', NULL, t('Reverse'));
}

/**
 * Reverse allocation form validate.
 */
function ec_receipt_reverse_allocation_form_validate(&$form, &$form_state) {
  if ($object = ec_receipt_alloc_invoke($form_state['values']['alloc']->type, 'load', $form_state['values']['alloc']->etid)) {
    ec_receipt_alloc_invoke($form_state['values']['alloc']->type, 'can_reverse', $object, TRUE);
  }
}

/**
 * Reverse allocation form submit.
 */
function ec_receipt_reverse_allocation_form_submit(&$form, &$form_state) {
  ec_receipt_reverse_allocation($form_state['values']['receipt'], $form_state['values']['alloc']);
  $form_state['redirect'] = array('store/receipt/' . $form_state['values']['receipt']->erid . '/view');
}

/**
 *
 */
function ec_receipt_complete_form($form, &$form_state, $receipt) {
  $form = array();

  $form_state['receipt'] = $receipt;

  $form['created'] = array(
    '#type' => 'date_text',
    '#title' => 'Date received',
    '#default_value' => format_date($receipt->created, 'custom', DATE_FORMAT_DATETIME),
    '#description' => t('Enter the date that this receipt was received.'),
  );

  $form['amount'] = array(
    '#type' => 'price',
    '#title' => t('Amount'),
    '#default_value' => $receipt->amount,
    '#description' => t('Enter the amount of the receipt.'),
  );

  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $receipt->status,
    '#options' => ec_receipt_get_statuses(),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_receipt_complete_form_submit(&$form, &$form_state) {
  $receipt = & $form_state['receipt'];

  $form_state['values']['created'] = date_convert($form_state['values']['created'], DATE_DATETIME, DATE_UNIX);

  foreach ($form_state['values'] as $key => $value) {
    $receipt->{$key} = $value;
  }

  ec_receipt_save($receipt);
}
