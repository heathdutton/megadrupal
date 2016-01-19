<?php

/**
 * Menu callback: display an overview of available types.
 */
function commerce_payment_fields_transaction_types_overview() {

  $header = array(
    t('Name'),
    t('Operations'),
  );

  $rows = array();

  // Loop through all defined customer profile types.
  foreach (commerce_payment_fields_fieldable_payment_methods() as $method_id => $payment_method) {
    // Convert underscores to hyphens for the menu item argument.
    $method_arg = strtr($payment_method['method_id'], '_', '-');
    // Build the operation links for the current profile type.
    $links = menu_contextual_links('commerce-payment-transaction-type', 'admin/commerce/config/payments', array($method_arg));
    $method_info = check_plain($payment_method['title']);
    $method_info .= ' <small>' . t('(Machine name: @type)', array('@type' => $payment_method['method_id'])) . '</small>';
    $method_info .= '<div class="description">' . filter_xss_admin($payment_method['description']) . '</div>';
    // Add the profile type's row to the table's rows array.
    $rows[] = array(
      $method_info,
      theme('links', array('links' => $links, 'attributes' => array('class' => 'links inline operations'))),
    );
  }

  // If no profile types are defined...
  if (empty($rows)) {
    // Add a standard empty row.
    $data = t('There are no fieldable payment transaction types defined on this site.');
    $data .= '<br />';
    $data .= t('New <strong><em>fieldable</em></strong> property should be defined in <em>hook_commerce_payment_method_info()</em> for the payment method definition.');
    $rows[] = array(
      array(
        'data' => $data,
        'colspan' => 2,
      )
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Form callback: edit the global order settings.
 */
function commerce_payment_fields_transaction_settings_form($form, &$form_state, $method_id) {
  $form['commerce_payment_transaction_' . $method_id . '_help_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Payment transaction creation help text'),
    '#description' => t('Supply an optional help message to be displayed above the payment transaction forms.'),
    '#default_value' => variable_get('commerce_payment_transaction_' . $method_id . '_help_text', ''),
    '#weight' => -10,
  );
  $form['commerce_payment_transaction_' . $method_id . '_auto_revision'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create new revisions when payment transaction are updated by default.'),
    '#description' => t('This default may be overridden on the payment transaction edit form but will always be respected for other payment transaction status updates.'),
    '#default_value' => variable_get('commerce_payment_transaction_' . $method_id . '_auto_revision', TRUE),
    '#weight' => 0,
  );

  return system_settings_form($form);
}
