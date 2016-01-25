<?php
/**
 * @file
 * Custom offline payment methods for Drupal Commerce.
 *
 * Admin functions for Commerce Custom offline payments.
 */

/**
 * Returns an administrative overview of all custom offline payments.
 *
 * @return string
 * A HTML-formatted string with the administrative payment content.
 *
 * @see commerce_cop_menu()
 */
function commerce_cop_admin_overview() {
  $rows = array();
  foreach (commerce_cop_get_payments() as $payment) {
    // Build the operation links for the current item.
    $links = menu_contextual_links('commerce-cop-' . $payment['id'], 'admin/commerce/config/custom-offline-payments', array($payment['id']));
    $rows[] = array(
      $payment['title'],
      ($payment['status']) ? t('Enabled') : t('Disabled'),
      theme('links', array('links' => $links, 'attributes' => array('class' => 'links inline operations'))),
      );
  }
  if (empty($rows)) {
    return t('No custom offline payments are defined.');
  }
  else {
    $headers = array(t('Payment'), t('Status'), t('Operations'));
    return theme('table', array('header' => $headers, 'rows' => $rows, 'empty' => t('No books available.')));
  }
}

/**
 * Form to edit checkout payment.
 */
function commerce_cop_edit_payment_form($form, $form_state, $payment = array()) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#size' => 30,
    '#title' => t('Title'),
    '#required' => TRUE,
    '#default_value' => isset($payment['title']) ? $payment['title'] : '',
  );

  $form['id'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#maxlength' => 32,
    '#required' => TRUE,
    '#default_value' => isset($payment['id']) ? $payment['id'] : '',
    '#disabled' => isset($payment['id']),
    '#machine_name' => array(
      'exists' => 'custom_offline_payment_load',
      'source' => array('title'),
    ),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Help text displayed to end user on checkout payment.'),
    '#default_value' => isset($payment['description']) ? $payment['description'] : '',
  );

  $information_description = t('Information you would like to be shown to users when they select this payment method, such as delivery payment details.');
  $information_description .= '<br />';
  $information_description .= t('This could be used as default information for the new payment method rules of this payment.');
  $form['information'] = array(
    '#type' => 'text_format',
    '#title' => t('Information'),
    '#description' => $information_description,
    '#default_value' => isset($payment['information']) ? $payment['information'] : '',
    '#format' => isset($payment['format']) ? $payment['format'] : 'plain_text',
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Status'),
    '#description' => t('Disable or enable the default payment method.'),
    '#default_value' => isset($payment['status']) ? $payment['status'] : FALSE,
  );

  $form['checkout'] = array(
    '#type' => 'checkbox',
    '#title' => t('Available on checkout'),
    '#description' => t('TRUE or FALSE indicating whether or not payments can be processed via this payment method through the checkout form.'),
    '#default_value' => isset($payment['checkout']) ? $payment['checkout'] : TRUE,
  );

  $form['terminal'] = array(
    '#type' => 'checkbox',
    '#title' => t('Available on terminal'),
    '#description' => t("TRUE or FALSE indicating whether or not payments can be processed via this payment method through the administrative payment terminal on an order's Payment tab"),
    '#default_value' => isset($payment['terminal']) ? $payment['terminal'] : TRUE,
  );

  if (module_exists('commerce_payment_fields')) {
    $form['fieldable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Payment transaction fieldable'),
      '#description' => t("TRUE or FALSE indicating whether or not payment transactions could be extended with fields"),
      '#default_value' => isset($payment['fieldable']) ? $payment['fieldable'] : FALSE,
    );
  }

  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 *
 * @param type $form
 * @param type $form_state
 */
function commerce_cop_edit_payment_form_submit($form, &$form_state) {
  $v = $form_state['values'];

  $record = $v;
  $record['title'] = filter_xss_admin($v['title']);
  $record['description']  = filter_xss_admin($v['description']);
  $record['information']  = filter_xss_admin($v['information']['value']);
  $record['format']  = $v['information']['format'];

  if (commerce_cop_payment_save($record)) {
    drupal_set_message(t('The payment has been saved'));
    $form_state['redirect'] = 'admin/commerce/config/custom-offline-payments';
    if (module_exists('commerce_payment_fields')) {
      //@to do deep integration with Drupal Commerce Payment Transaction Fields module.
      entity_info_cache_clear();
      menu_rebuild();
    }
  }

}

/**
 * payment delete form.
 */
function commerce_cop_delete_payment_form($form, $form_state, $payment) {
  $form['#payment'] = $payment;
  $question = t('Are you sure you want to delete the payment %title?', array('%title' => $payment['title']));
  $path = 'admin/commerce/config/custom-offline-payments';
  return confirm_form($form, $question, $path);
}

function commerce_cop_delete_payment_form_submit($form, &$form_state) {
  commerce_cop_payment_delete($form['#payment']);
  $form_state['redirect'] = 'admin/commerce/config/custom-offline-payments';
}
