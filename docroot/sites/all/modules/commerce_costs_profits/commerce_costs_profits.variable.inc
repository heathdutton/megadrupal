<?php

function commerce_costs_profits_group_info() {
  $groups = array();

  $groups['commerce_costs_profits_main_settings'] = array(
    'title' => t('Main costs and profits setttings'),
    'access' => 'commerce configure costs products settings',
  );

  return $groups;
}

function commerce_costs_profits_variable_info() {
  $variables = array();

  $variables['commerce_costs_profits_default_margin'] = array(
    'title' => t('Default product margin'),
    'description' => t('If no module provides margin calculation for product and product price recalculation called, this value is used.'),
    'group' => 'commerce_costs_profits_main_settings',
    'type' => 'number',
    'element' => array(
      '#type' => 'textfield',
      '#size' => 2,
      '#maxlength' => 2,
      '#field_suffix' => '%',
    ),
    'default' => 0,
  );
  $variables['commerce_costs_profits_auto_price_recalculation'] = array(
    'title' => t('Automatically recalculate product price on cost change'),
    'group' => 'commerce_costs_profits_main_settings',
    'description' => t('Should product price be recalculated by this module using margins each time product cost changes.'),
    'type' => 'boolean',
    'default' => 0,
  );

  return $variables;
}
