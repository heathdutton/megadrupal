<?php

/**
 * @file
 * Commerce AvaTax Exemptions checkout panes.
 */

/**
 * Implements base_settings_form().
 */
function commerce_avatax_exemptions_pane_settings_form($checkout_pane) {
  $form = array();
  $states = $states = array(
    'AL' => t('Alabama'),
    'AK' => t('Alaska'),
    'AZ' => t('Arizona'),
    'AR' => t('Arkansas'),
    'CA' => t('California'),
    'CO' => t('Colorado'),
    'CT' => t('Connecticut'),
    'DE' => t('Delaware'),
    'DC' => t('District Of Columbia'),
    'FL' => t('Florida'),
    'GA' => t('Georgia'),
    'HI' => t('Hawaii'),
    'ID' => t('Idaho'),
    'IL' => t('Illinois'),
    'IN' => t('Indiana'),
    'IA' => t('Iowa'),
    'KS' => t('Kansas'),
    'KY' => t('Kentucky'),
    'LA' => t('Louisiana'),
    'ME' => t('Maine'),
    'MD' => t('Maryland'),
    'MA' => t('Massachusetts'),
    'MI' => t('Michigan'),
    'MN' => t('Minnesota'),
    'MS' => t('Mississippi'),
    'MO' => t('Missouri'),
    'MT' => t('Montana'),
    'NE' => t('Nebraska'),
    'NV' => t('Nevada'),
    'NH' => t('New Hampshire'),
    'NJ' => t('New Jersey'),
    'NM' => t('New Mexico'),
    'NY' => t('New York'),
    'NC' => t('North Carolina'),
    'ND' => t('North Dakota'),
    'OH' => t('Ohio'),
    'OK' => t('Oklahoma'),
    'OR' => t('Oregon'),
    'PA' => t('Pennsylvania'),
    'RI' => t('Rhode Island'),
    'SC' => t('South Carolina'),
    'SD' => t('South Dakota'),
    'TN' => t('Tennessee'),
    'TX' => t('Texas'),
    'UT' => t('Utah'),
    'VT' => t('Vermont'),
    'VA' => t('Virginia'),
    'WA' => t('Washington'),
    'WV' => t('West Virginia'),
    'WI' => t('Wisconsin'),
    'WY' => t('Wyoming'),
    'AA' => t('Armed Forces (Americas)'),
    'AE' => t('Armed Forces (Europe, Canada, Middle East, Africa)'),
    'AP' => t('Armed Forces (Pacific)'),
    'AS' => t('American Samoa'),
    'FM' => t('Federated States of Micronesia'),
    'GU' => t('Guam'),
    'MH' => t('Marshall Islands'),
    'MP' => t('Northern Mariana Islands'),
    'PW' => t('Palau'),
    'PR' => t('Puerto Rico'),
    'VI' => t('Virgin Islands'),
  );
  $form['commerce_avatax_exemptions_states'] = array(
    '#title' => t('States to show exemption pane'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => $states,
    '#default_value' => variable_get('commerce_avatax_exemptions_states', array_keys($states)),
  );

  $form['commerce_avatax_exemptions_roles'] = array(
    '#title' => t('Select Roles that should be allowed to use exemption'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => user_roles(),
    '#default_value' => variable_get('commerce_avatax_exemptions_roles', array(1)),
  );
  $form['commerce_avatax_exemptions_description'] = array(
    '#title' => t('Description text to show on the exemption pane'),
    '#type' => 'textarea',
    '#default_value' => variable_get('commerce_avatax_exemptions_description'),
  );
  return $form;
}


/**
 * Validation callback for the admin form.
 */
function commerce_avatax_exemptions_pane_settings_form_validate($form, &$form_state, $checkout_pane) {
  $states = &$form_state['values']['commerce_avatax_exemptions_states'];
  if (!is_array($states)) {
    if ($states) {
      $states = array($states);
    }
    else {
      $states = array();
    }
  }

  $roles = &$form_state['values']['c'];
  if (!is_array($roles)) {
    if ($roles) {
      $roles = array($roles);
    }
    else {
      $roles = array();
    }
  }
}

/**
 * Implements base_checkout_form().
 */
function commerce_avatax_exemptions_pane_checkout_form($form, $form_state, $checkout_pane, $order) {
  // Reset exemptions every time the form is loaded.
  if (isset($order->data['commerce_avatax_exemptions_category'])) {
    unset($order->data['commerce_avatax_exemptions_category']);
    commerce_order_save($order);
  }
  $checkout_form['commerce_avatax_exemptions_category'] = array(
    '#title' => t('Sales tax exemption category'),
    '#type' => 'select',
    '#options' => array(
      '0' => t('None'),
      'E' => t('Charitable or benevolent org'),
      'H' => t('Commercial agricultural production'),
      'J' => t('Direct pay permit'),
      'K' => t('Direct mail'),
      'A' => t('Federal government'),
      'D' => t('Foreign diplomat'),
      'I' => t('Industrial production / manufacturer'),
      'N' => t('Local government'),
      'B' => t('State government'),
      'C' => t('Tribe / Status Indian / Indian Band'),
      'F' => t('Religious or educational org'),
      'G' => t('Resale'),
      'L' => t('Other'),
    ),
    '#description' => check_plain(variable_get('commerce_avatax_exemptions_description', '')),
    '#default_value' => isset($order->data['commerce_avatax_exemptions_category']) ? $order->data['commerce_avatax_exemptions_category'] : '0',
  );
  $checkout_form['#access'] = FALSE;
  drupal_add_css(drupal_get_path('module', 'commerce_avatax_exemptions') . '/theme/commerce_avatax_exemptions.css');
  return $checkout_form;
}

/**
 * Implements base_checkout_form_submit().
 */
function commerce_avatax_exemptions_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  if (isset($form_state['values']['commerce_avatax_exemptions']['commerce_avatax_exemptions_category'])) {
    $order->data['commerce_avatax_exemptions_category'] = $form_state['values']['commerce_avatax_exemptions']['commerce_avatax_exemptions_category'];
    commerce_order_save($order);
  }
}
