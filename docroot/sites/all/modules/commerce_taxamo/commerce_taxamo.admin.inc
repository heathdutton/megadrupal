<?php

/**
 * @file
 * The administration pages functions of Taxamo for Commerce module.
 */


/**
 * Callback function to render Taxamo settings page.
 */
function commerce_taxamo_content_admin_taxamo() {
  $build = array();

  if (!commerce_taxamo_load_taxamo_php()) {
    drupal_set_message(t('Taxamo PHP library not found. Please consult the README.txt for installation instructions.'), 'warning');
    $build['warning'] = array('#markup' => t("Before to set up the module, please consult the README.txt for installation instructions."));
  }
  else {
    $build['commerce_taxamo_settings_form'] = drupal_get_form('commerce_taxamo_settings_form');
  }

  return $build;
}

/**
 * Function to generate the administration settings form for Taxamo.
 */
function commerce_taxamo_settings_form($form, &$form_state) {
  commerce_taxamo_ensure_fields();
  commerce_taxamo_update_taxamo_product_types();

  $form['tokens'] = array(
    '#title' => t("Tokens"),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['tokens']['commerce_taxamo_mode'] = array(
    '#title' => t("API to use"),
    '#type' => 'radios',
    '#options' => array(
      'test' => t("Test"),
      'live' => t("Live"),
    ),
    '#default_value' => commerce_taxamo_get_var('commerce_taxamo_mode'),
    '#required' => TRUE,
  );

  $form['tokens']['test'] = array(
    '#title' => t("Test Tokens"),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => commerce_taxamo_get_var('commerce_taxamo_public_token_test'),
  );

  $form['tokens']['test']['commerce_taxamo_public_token_test'] = array(
    '#title' => t('Public test token'),
    '#type' => 'textfield',
    '#default_value' => commerce_taxamo_get_var('commerce_taxamo_public_token_test'),
    '#description' => t("Public token is available from a merchant dashboard, in <strong>Account settings</strong> panel."),
  );

  $form['tokens']['test']['commerce_taxamo_private_token_test'] = array(
    '#title' => t('Private test token'),
    '#type' => 'textfield',
    '#default_value' => commerce_taxamo_get_var('commerce_taxamo_private_token_test'),
    '#description' => t("Private token is available from a merchant dashboard, in <strong>Account settings</strong> panel."),
  );

  $form['tokens']['live'] = array(
    '#title' => t("Live Tokens"),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => commerce_taxamo_get_var('commerce_taxamo_public_token_live'),
  );

  $form['tokens']['live']['commerce_taxamo_public_token_live'] = array(
    '#title' => t('Public live token'),
    '#type' => 'textfield',
    '#default_value' => commerce_taxamo_get_var('commerce_taxamo_public_token_live'),
    '#description' => t("Public token is available from a merchant dashboard, in <strong>Account settings</strong> panel."),
  );

  $form['tokens']['live']['commerce_taxamo_private_token_live'] = array(
    '#title' => t('Private live token'),
    '#type' => 'textfield',
    '#default_value' => commerce_taxamo_get_var('commerce_taxamo_private_token_live'),
    '#description' => t("Private token is available from a merchant dashboard, in <strong>Account settings</strong> panel."),
  );

  $form['general'] = array(
    '#title' => t("General settings"),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $commerce_product_types = commerce_product_types();
  $options = array();
  foreach ($commerce_product_types as $key => $commerce_product_type) {
    $options[$key] = $commerce_product_type['name'];
  }
  $form['general']['commerce_taxamo_product_types_to_vat'] = array(
    '#title' => t("Product types to calculate VAT"),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => commerce_taxamo_get_var('commerce_taxamo_product_types_to_vat'),
    '#description' => t("Only the products of these product types will be sent to Taxamo to calculate its VAT. The rest of the products will be sent marked with Informative flag. Remember that Taxamo should be used to calculate VAT only for Digital Products, so it would be a great idea to have categories containing only digital products and select them here."),
  );

  $form['general']['commerce_taxamo_component_display_title'] = array(
    '#title' => t('Display title'),
    '#type' => 'textfield',
    '#default_value' => commerce_taxamo_get_var('commerce_taxamo_component_display_title'),
    '#description' => t("Text used on checkout for Taxamo calculated tax amount."),
    '#required' => TRUE,
  );

  t(commerce_taxamo_get_var('commerce_taxamo_component_display_title'));

  $vat_number_options = commerce_taxamo_vat_number_field_options();
  $form['general']['commerce_taxamo_vat_number_field'] = array(
    '#title' => t('VAT number field'),
    '#type' => 'select',
    '#options' => $vat_number_options,
    '#default_value' => commerce_taxamo_get_var('commerce_taxamo_vat_number_field'),
    '#description' => t("The selected field would contain the buyer's VAT Number. If the selected field does not have a value for a customer, the VAT number information will not be sent to Taxamo and the order will be processed as a B2C order. If you don't plan to do B2B transactions, simply choose %no_vat. If you have not created a field yet, please create it first and then come back to select it here.", array('%no_vat' => $vat_number_options[0])),
  );

  $form['general']['commerce_taxamo_product_type_default_value'] = array(
    '#title' => t('Default Product Type'),
    '#type' => 'radios',
    '#options' => commerce_taxamo_product_types(),
    '#default_value' => commerce_taxamo_get_var('commerce_taxamo_product_type_default_value'),
    '#description' => t("This value is used on newly created products and products that do not have a Taxamo Product Type selected."),
    '#required' => TRUE,
  );

  $form['advanced'] = array(
    '#title' => t("Advanced options"),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advanced']['statuses'] = array(
    '#title' => t("Order statuses"),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advanced']['statuses']['commerce_taxamo_order_statuses_delete'] = array(
    '#title' => t("Order statuses to delete Taxamo line item"),
    '#type' => 'select',
    '#options' => commerce_order_status_options_list(),
    '#default_value' => commerce_taxamo_get_var('commerce_taxamo_order_statuses_delete'),
    '#multiple' => TRUE,
    '#description' => t("When the order is in one of the selected statuses, the Taxamo line item will be removed from the order. This is used to do not show outdated Taxes information.<br />It is only removed if the order is not completed."),
  );

  $form['advanced']['statuses']['commerce_taxamo_order_statuses_precalculate'] = array(
    '#title' => t("Order statuses to precalculate taxes using Taxamo"),
    '#type' => 'select',
    '#options' => commerce_order_status_options_list(),
    '#default_value' => commerce_taxamo_get_var('commerce_taxamo_order_statuses_precalculate'),
    '#multiple' => TRUE,
    '#description' => t("Taxes are retrieved from Taxamo only when the order is in one of the selected statuses."),
  );

  $form['advanced']['statuses']['commerce_taxamo_order_statuses_confirm'] = array(
    '#title' => t("Order statuses to confirm taxes calculated by Taxamo"),
    '#type' => 'select',
    '#options' => commerce_order_status_options_list(),
    '#default_value' => commerce_taxamo_get_var('commerce_taxamo_order_statuses_confirm'),
    '#multiple' => TRUE,
    '#description' => t("When the order is in one of the selected statuses, the system will inform Taxamo that the precalculated taxes are confirmed."),
  );

  $form['advanced']['statuses']['commerce_taxamo_order_statuses_full_refund'] = array(
    '#title' => t("Order statuses to send a full refund to Taxamo"),
    '#type' => 'select',
    '#options' => commerce_order_status_options_list(),
    '#default_value' => commerce_taxamo_get_var('commerce_taxamo_order_statuses_full_refund'),
    '#multiple' => TRUE,
    '#description' => t("When the order is in one of the selected statuses, and the Taxamo transaction had already been confirmed, the system will inform Taxamo do a full refund of the transaction."),
  );

  $form['#validate'][] = 'commerce_taxamo_settings_form_validate';

  return system_settings_form($form);
}

/**
 * Taxamo settings form validation function
 */
function commerce_taxamo_settings_form_validate(&$form, &$form_state) {
  // Check that the tokens for the selected mode are available.
  $mode = $form_state['values']['commerce_taxamo_mode'];
  if (!empty($mode)) {
    $token_keys = array(
      "commerce_taxamo_public_token_$mode",
      "commerce_taxamo_private_token_$mode",
    );
    foreach ($token_keys as $key) {
      if (empty($form_state['values'][$key])) {
        form_set_error($key, t("Tokens required for the selected mode."));
      }
    }
  }
}

/**
 * Helper function to generate the options array to be passed to the VAT number
 * select field.
 * @return array Array containing all the possible options available.
 */
function commerce_taxamo_vat_number_field_options() {
  $options = array(
    0 => t("No VAT number field"),
  );

  $entities = array(
    'user' => array(
      'user',
    ),
    'commerce_customer_profile' => array(
      'billing',
    ),
  );

  foreach ($entities as $entity_type => $bundles) {
    foreach ($bundles as $bundle_name) {
      $key = t("!entity_type (!bundle_name)", array('!entity_type' => $entity_type, '!bundle_name' => $bundle_name));
      $options[$key] = array();
      $field_instances = field_info_instances($entity_type, $bundle_name);
      foreach ($field_instances as $field_name => $field_instance) {
        $field_key = "$entity_type||$bundle_name||$field_name";
        $options[$key][$field_key] = $field_instance['label'];
      }
    }
  }

  return $options;
}
