<?php

/**
 * @file
 * Canpar shipping settings.
 *
 * @author Tim Rohaly.    <http://drupal.org/user/202830>
 */


/**
 * Default Canpar rate API settings.
 *
 * Configures Canpar URL, available services, and other settings related
 * to shipping quotes.
 *
 * @return
 *   Forms for store administrator to set configuration options.
 *
 * @see uc_canpar_admin_settings_validate()
 * @ingroup forms
 */
function uc_canpar_admin_settings() {

  // Put fieldsets into vertical tabs.
  $form['canpar-settings'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('module', 'uc_canpar') . '/uc_canpar.admin.js',
      ),
    ),
  );

  // Container for credentials forms.
  $form['uc_canpar_credentials'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Credentials'),
    '#description'   => t('Account number and authorization information.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#group'         => 'canpar-settings',
  );

  // Form to set choose between BASE rates and CUSTOM rates.
  // ***Defaults to BASE!***
  $form['uc_canpar_credentials']['uc_canpar_quote_type'] = array(
    '#type'          => 'select',
    '#title'         => t('Canpar quote type'),
    '#description'   => t('Choose to present the customer with Canpar base prices or your custom discounted Canpar account prices. The shipper number and user token fields below are mandatory when custom prices is selected.'),
    '#options'       => array(
      'base'    => t('Base prices'),
      'custom' => t('Discount custom prices'),
    ),
    '#default_value' => variable_get('uc_canpar_quote_type', 'base'),
  );

  // Form to set the shipper number.
  $form['uc_canpar_credentials']['uc_canpar_shipper_number'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Canpar shipper number'),
    '#default_value' => variable_get('uc_canpar_shipper_number', 99999999),
    '#required'      => FALSE,
    '#description'   => t('Your Canpar shipper number. Required if you have selected custom prices above.'),
  );

  // Form to set the user token.
  $form['uc_canpar_credentials']['uc_canpar_user_token'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Canpar user token'),
    '#default_value' => variable_get('uc_canpar_user_token', 'CANPAR'),
    '#required'      => FALSE,
    '#description'   => t('Your Canpar user token. Required if you have selected custom prices above.'),
  );

  // Container for service selection forms.
  $form['uc_canpar_service_selection'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Service options'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#group'         => 'canpar-settings',
  );

  // Form to restrict Canpar services available to customer.
  $form['uc_canpar_service_selection']['uc_canpar_services'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Canpar services'),
    '#default_value' => variable_get('uc_canpar_services', _uc_canpar_service_list()),
    '#options'       => _uc_canpar_service_list(),
    '#description'   => t('Select the shipping services that are available to customers.'),
  );

  // Container for quote options forms.
  $form['uc_canpar_quote_options'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Quote options'),
    '#description'   => t('Preferences that affect computation of quote.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#group'         => 'canpar-settings',
  );

  // Form to specify turnaround time.
  $form['uc_canpar_quote_options']['uc_canpar_turnaround'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Turn-around time'),
    '#default_value' => variable_get('uc_canpar_turnaround', '24'),
    '#description'   => t('Number of hours for turn-around time before shipping. This simply adds this number of hours to the delivery date as returned by Canpar.'),
  );

  // Form to specify date format.
  $form['uc_canpar_quote_options']['uc_canpar_datefmt'] = array(
    '#type'          => 'select',
    '#title'         => t('Delivery date format'),
    '#default_value' => variable_get('uc_canpar_datefmt', ''),
    '#description'   => t('Format to display estimated delivery date.'),
    '#options'       => _uc_canpar_get_date_options(),
  );

  // Form to specify ship-from postal code.
  $orig = variable_get('uc_quote_store_default_address', new stdClass());
  $form['uc_canpar_quote_options']['uc_canpar_postalcode'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Ship from postal code'),
    '#default_value' => variable_get('uc_canpar_postalcode', isset($orig->postal_code) ? $orig->postal_code : ''),
    '#description'   => t('Postal code to ship from.'),
    '#required'      => TRUE,
  );

  // Form to add optional Insurance coverage in the amount of the order.
  $form['uc_canpar_quote_options']['uc_canpar_insurance'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Add Insurance to shipping quote'),
    '#default_value' => variable_get('uc_canpar_insurance', FALSE),
    '#description'   => t('When enabled, the quote presented to the customer will include the cost of insurance for the full sales price of all products in the order.'),
  );

  // Container for markup forms.
  $form['uc_canpar_markups'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Markups'),
    '#description'   => t('Modifiers to the shipping weight and quoted rate.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#group'         => 'canpar-settings',
  );

  // Form to select type of rate markup.
  $form['uc_canpar_markups']['uc_canpar_rate_markup_type'] = array(
    '#type'          => 'select',
    '#title'         => t('Rate markup type'),
    '#default_value' => variable_get('uc_canpar_rate_markup_type', 'percentage'),
    '#options'       => array(
      'percentage' => t('Percentage (%)'),
      'multiplier' => t('Multiplier (×)'),
      'currency'   => t('Addition (!currency)', array('!currency' => variable_get('uc_currency_sign', '$'))),
    ),
  );

  // Form to select rate markup amount.
  $form['uc_canpar_markups']['uc_canpar_rate_markup'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Canpar shipping rate markup'),
    '#default_value' => variable_get('uc_canpar_rate_markup', '0'),
    '#description'   => t('Markup shipping rate quote by dollar amount, percentage, or multiplier. Please note if this field is not blank, it overrides the "Handling fee" set up in your SellOnline account. If blank, the handling fee from your SellOnline account will be used.'),
  );

  // Form to select type of weight markup.
  $form['uc_canpar_markups']['uc_canpar_weight_markup_type'] = array(
    '#type'          => 'select',
    '#title'         => t('Weight markup type'),
    '#default_value' => variable_get('uc_canpar_weight_markup_type', 'percentage'),
    '#options'       => array(
      'percentage' => t('Percentage (%)'),
      'multiplier' => t('Multiplier (×)'),
      'mass'       => t('Addition (!mass)', array('!mass' => '#')),
    ),
  );

  // Form to select weight markup amount.
  $form['uc_canpar_markups']['uc_canpar_weight_markup'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Canpar shipping weight markup'),
    '#default_value' => variable_get('uc_canpar_weight_markup', '0'),
    '#description'   => t('Markup Canpar shipping weight before quote, on a per-package basis, by weight amount, percentage, or multiplier.'),
  );

  // Register additional validation handler
  $form['#validate'][] = 'uc_canpar_admin_settings_validate';

  $form = system_settings_form($form);

  // Add Cancel link.
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/store/settings/quotes'),
  );

  return $form;
}

/**
 * Validation handler for uc_canpar_admin_settings form.
 *
 * Requires shipper number and user token for custom quotes only.
 * Ensures markups are numeric.
 *
 * @see uc_canpar_admin_settings()
 */
function uc_canpar_admin_settings_validate($form, &$form_state) {

  // Canpar shipper number and user token are required for custom quotes,
  // optional otherwise.
  if ($form_state['values']['uc_canpar_quote_type'] == 'custom') {
    if (trim($form_state['values']['uc_canpar_shipper_number']) == '') {
      form_set_error(
        'uc_canpar_shipper_number',
        t('Shipper number field is required when custom quotes are selected.')
      );
    }
    if (trim($form_state['values']['uc_canpar_user_token']) == '') {
      form_set_error(
        'uc_canpar_user_token',
        t('User token field is required when custom quotes are selected.')
      );
    }
  }

  // Ensure rate markup is numeric - no % or $ or any other units specified.
  if (!is_numeric($form_state['values']['uc_canpar_rate_markup'])) {
    form_set_error(
      'uc_canpar_rate_markup',
      t('Rate markup must be a numeric value.')
    );
  }

  // Ensure weight markup is numeric - no % or lb or any other units specified.
  if (!is_numeric($form_state['values']['uc_canpar_weight_markup'])) {
    form_set_error(
      'uc_canpar_weight_markup',
      t('Weight markup must be a numeric value.')
    );
  }
}
