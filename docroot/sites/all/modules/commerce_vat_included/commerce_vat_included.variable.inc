<?php

/**
 * Implements hook_variable_info().
 */
function commerce_vat_included_variable_info($options) {
  $variables['commerce_vat_included_price_components'] = array(
    'type' => 'options',
    'title' => t('Price components with VAT included prices'),
    'default' => array('base_price'),
    'required' => TRUE,
    'options callback' => 'commerce_price_component_titles',
  );
  return $variables;
}
