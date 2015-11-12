<?php

/**
 * @file
 * Configure settings for Gharpay.
 */

/**
 * Configuration form for Gharpay.
 */
function gharpay_config_form($form, &$form_state) {

  $form = array();

  // Allow only India based transactions.
  $form['gharpay_allow_only_in_india'] = array(
    '#type' => 'checkbox',
    '#title' => t("Pay using Gharpay only in India"),
    '#default_value' => variable_get('gharpay_allow_only_in_india', 0),
    '#description' => t("If selected, users only from India would be allowed to pay using Gharpay."),
    );

  $default_gharpay_description = t("Gharpay has an Indian network of executives to reach customer's doorsteps and accept payments.");
  // Add a description about Gharpay.
  $form['gharpay_description'] = array(
    '#type' => 'textarea',
    '#title' => t("Help text"),
    '#description' => t("Describe Gharpay's payment method. This would be shown to the customers. <i>Leave blank for no description</i>"),
    '#default_value' => variable_get('gharpay_description', $default_gharpay_description),
    );
  // @TODO: Add a function to process surcharge.
  $form['gharpay_surcharge_unit'] = array(
    '#type' => 'radios',
    '#title' => t("Surcharge"),
    '#description' => t("Optional."),
    '#default_value' => variable_get('gharpay_surcharge_unit', 'none'),
    '#options' => array(
        'none' => t("None"),
        'rs' => t("In Rs."),
        'percent' => t("Percent of total payment"),
      ),
    );
  $form['gharpay_surcharge_in_rs'] = array(
    '#type' => 'textfield',
    '#title' => t('Value'),
    '#default_value' => variable_get('gharpay_surcharge_in_rs', ''),
    '#states' => array(
      'visible' => array(
        'input[name="gharpay_surcharge_unit"]' => array('value' => 'rs'),
        ),
      ),
    );
  $form['gharpay_surcharge_in_percent'] = array(
    '#type' => 'textfield',
    '#title' => t('Value'),
    '#default_value' => variable_get('gharpay_surcharge_in_percent', ''),
    '#states' => array(
      'visible' => array(
        'input[name="gharpay_surcharge_unit"]' => array('value' => 'percent'),
        ),
      ),
    );

  return system_settings_form($form);
}