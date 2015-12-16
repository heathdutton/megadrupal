<?php
/**
 * @file
 * Some of the admin pages for MoneySuite modules.
 *
 * Original author: Leighton Whiting - Released under GENERAL PUBLIC LICENSE 
 * Current maintenance by multiple MoneySuite users.
 */

/**
 * Page callback: Shows a list of the MoneySuite pages.
 *
 * @see ms_core_menu()
 */
function ms_core_main_page() {
  $message = t('Various MoneySuite modules can be found here. Click the links below to view and change the settings.');

  return $message . theme('admin_block_content', array('content' => system_admin_menu_block(menu_get_item())));
}

/**
 * Page callback: Shows a list of the installed Gateways.
 *
 * @see ms_core_menu()
 */
function ms_core_gateways_page() {
  $message = t('Various MoneySuite Payment Gateway modules can be found here. Click the links below to view and change the settings. To enable more MoneySuite Payment Gateways, go to the <a href="!url">module administration page</a>', array('!url' => url('admin/modules')));

  return $message . theme('admin_block_content', array('content' => system_admin_menu_block(menu_get_item())));
}

/**
 * Form constructor for the payment gateway settings form.
 *
 * @param string $name
 *   The payment gateway module name.
 *
 * @ingroup forms
 * @see ms_core_menu()
 * @see system_settings_form()
 */
function ms_core_gateway_settings_form($form, &$form_state, $name) {
  $gateway = ms_core_get_payment_gateway($name);
  $module = $gateway['module'];

  // Add in some generic settings.
  if (!empty($gateway['recurring_schedule_support'])) {
    $gateway['settings']['main'][$module . '_retry_attempts'] = array(
      '#type' => 'textfield',
      '#title' => t('Retry Attempts'),
      '#description' => t('How many times should the system retry charging a card?
        If a scheduled charge fails, the user is notified that they need to update
        their billing information, and the order is scheduled to be charged again
        in 3 days.'),
      '#default_value' => variable_get($module . '_retry_attempts', 3),
    );
    $gateway['settings']['main'][$module . '_cron_number'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of Payments Processed During Cron'),
      '#description' => t('How many payments to process during cron run? Be careful
        of setting too many, as this could lead to a timeout.'),
      '#default_value' => variable_get($module . '_cron_number', 3),
    );
  }

  if (!empty($gateway['saved_cards_support'])) {
    $gateway['settings']['email'] = array(
      '#type' => 'fieldset',
      '#title' => t('Email Settings'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $gateway['settings']['email'][$module . '_billing_error_subject'] = array(
      '#type' => 'textfield',
      '#title' => t('Card Billing Error Email Subject'),
      '#required' => FALSE,
      '#default_value' => variable_get($module . '_billing_error_subject',
        t("There was a problem when processing your credit card.")),
    );
    $gateway['settings']['email'][$module . '_billing_error_body'] = array(
      '#type' => 'textarea',
      '#title' => t('Card Billing Error Email Body'),
      '#description' => t("Leave blank to not send an email."),
      '#required' => FALSE,
      '#default_value' => variable_get($module . '_billing_error_body',
        t("Your credit card [ms_core_payment_profile:cc_num] could not
        be processed. To avoid interruption of service, please update your billing
        information here: [ms_core_payment_profile:update_url]

  Sincerely,
  Site Admin")),
    );

    $gateway['settings']['email'][$module . '_billing_error_token_help']['tokens'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('ms_core_payment_profile', 'user', 'ms_core_order'),
      '#dialog' => TRUE,
    );

    $gateway['settings']['email'][$module . '_expiring_card_reminder_mail_subject'] = array(
      '#type' => 'textfield',
      '#title' => t('Card Expiring Soon Email Subject'),
      '#required' => FALSE,
      '#default_value' => variable_get($module . '_expiring_card_reminder_mail_subject',
        t("Your card is expiring soon!")),
    );
    $gateway['settings']['email'][$module . '_expiring_card_reminder_mail_body'] = array(
      '#type' => 'textarea',
      '#title' => t('Card Expiring Soon Email Body'),
      '#description' => t("Leave blank to not send an email."),
      '#required' => FALSE,
      '#default_value' => variable_get($module . '_expiring_card_reminder_mail_body',
        t("Your credit card [ms_core_payment_profile:cc_num] is expiring soon.
        To avoid interruption of service, please update your billing
        information here: [ms_core_payment_profile:update_url]

  Sincerely,
  Site Admin")),
    );

    $gateway['settings']['email'][$module . '_expiring_card_reminder_token_help']['tokens'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('ms_core_payment_profile', 'user'),
      '#dialog' => TRUE,
    );
  }

  $gateway['settings']['core'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if (!empty($gateway['cards'])) {
    $gateway['settings']['core'][$module . '_allowed_cards'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Allowed Cards'),
      '#options' => array(
        'visa' => 'Visa',
        'mc' => 'MasterCard',
        'amex' => 'American Express',
        'discover' => 'Discover',
        'diners' => 'Diners',
        'jcb' => 'JCB',
      ),
      '#default_value' => variable_get($module . '_allowed_cards', array('visa', 'mc')),
    );
  }

  $gateway['settings']['core'][$module . '_gateway_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment method name'),
    '#ms_override' => TRUE,
    '#default_value' => variable_get($module . '_gateway_name', $gateway['display_name']),
    '#description' => t("Set the name of this payment method as it will appear on invoices and orders."),
  );
  $gateway['settings']['core'][$module . '_gateway_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Payment method description'),
    '#ms_override' => TRUE,
    '#default_value' => variable_get($module . '_gateway_description', $gateway['description']),
    '#description' => t("Set the description of this payment option as it will appear on the Checkout page."),
  );
  $gateway['settings']['core'][$module . '_show_gateway'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show on Checkout Page?'),
    '#ms_override' => TRUE,
    '#default_value' => variable_get($module . '_show_gateway', TRUE),
  );
  $gateway['settings']['core'][$module . '_checkout_weight'] = array(
    '#type' => 'select',
    '#title' => t('Weight'),
    '#ms_override' => TRUE,
    '#default_value' => variable_get($module . '_checkout_weight', 0),
    '#options' => drupal_map_assoc(range(-99, 99)),
    '#description' => t("The weight determines the order the gateways are shown on the Checkout page. Lower weights rise to the top."),
  );
  $gateway['settings']['core'][$module . '_order_title_template'] = array(
    '#type' => 'textfield',
    '#title' => t('Order Title Template'),
    '#ms_override' => TRUE,
    '#default_value' => variable_get($module . '_order_title_template', '[site:name] - Order #[ms_core_order:orderId]'),
    '#description' => t("The order title is used by the payment processor and may show on the statements."),
  );

  $gateway['settings']['core'][$module . '_order_title_template_help']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('ms_core_order'),
    '#dialog' => TRUE,
  );

  return system_settings_form($gateway['settings']);
}

/**
 * Page callback: Shows a list of the Reports available.
 *
 * @see ms_core_menu().
 */
function ms_core_reports_page() {
  $message = (module_exists('ms_reports')) ? t('Various reports generated by MoneySuite modules can be found here. Click the links below to view the reports.') : t('Various reports generated by MoneySuite modules can be found here. Click the links below to view the reports. To view core MoneySuite statistics enable the <strong>MS Reports</strong> module on the <a href="!url">module administration page</a>', array('!url' => url('admin/modules')));

  return $message . theme('admin_block_content', array('content' => system_admin_menu_block(menu_get_item())));
}
