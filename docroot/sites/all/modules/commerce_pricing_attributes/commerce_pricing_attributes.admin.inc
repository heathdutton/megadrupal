<?php

/**
 * Administatiorn settings form.
 */
function commerce_pricing_attributes_default_setting_form($form, &$form_state){
  $entity = $form_state['build_info']['args'][0];
  $field_instance = $form_state['build_info']['args'][1];
  $field_info = field_info_field($field_instance['field_name']);

  $defaults = isset($field_instance['settings']['commerce_pricing_attributes'])?$field_instance['settings']['commerce_pricing_attributes']:array();

  $options = list_allowed_values($field_info, $field_instance);
  $options = array('_none' => t('- Select -')) + $options;

  $form['#tree'] = TRUE;

  $form['options'] = array(
    '#theme' => 'commerce_pricing_attributes_details',
    '#display_default' => FALSE,
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('commerce-pricing-attributes-set-details-options')
    ),
  );

  foreach($options as $option_value => $option_key){
    $option_value = (String)$option_value;

    // If the field type is boolean list and doesn't have labels defined.
    if(empty($option_key)){
      $option_key = $option_value;
    }

    if($option_value === '_none'){
      $form['options'][$option_value]['enabled'] = array('#markup' => $option_key);
    }
    else{
      $form['options'][$option_value]['enabled'] = array(
        '#type' => 'checkbox',
        '#title' => $option_key,
        '#title_display' => 'after',
        '#return_value' => 1,
        '#attributes' => array(
          'id' => 'commerce-pricing-attributes-set-details-options-' . drupal_html_class($option_value) . '-enabled',
        ),
        '#default_value' => isset($defaults[$option_value]['enabled'])?$defaults[$option_value]['enabled']:TRUE,
      );
    }

    if($option_value === '_none'){
      $form['options'][$option_value]['price_op'] = array();
      $form['options'][$option_value]['price'] = array();
    }
    else{
      $form['options'][$option_value]['price_op'] = array(
        '#type' => 'select',
        '#options' => array('plus' => '+', 'minus' => '-'),
        '#default_value' => isset($defaults[$option_value]['price_op'])?$defaults[$option_value]['price_op']:'plus',
      );

      $default_currency_code = commerce_default_currency();
      $form['options'][$option_value]['price'] = array(
        '#type' => 'textfield',
        '#default_value' => isset($defaults[$option_value]['price'])?commerce_currency_amount_to_decimal($defaults[$option_value]['price'], $default_currency_code):0,
        '#required' => TRUE,
        '#size' => 10,
      );

      // Build a currency options list from all enabled currencies.
      $options = array();

      foreach (commerce_currencies(TRUE) as $currency_code => $currency) {
        $options[$currency_code] = check_plain($currency['code']);
      }

      // If only one currency option is available, don't use a select list.
      if (count($options) == 1) {
        $currency_code = key($options);

        $form['options'][$option_value]['price']['#field_suffix'] = $currency_code;

        $form['options'][$option_value]['currency_code'] = array(
          '#type' => 'value',
          '#default_value' => $currency_code,
        );
      }
      else {
        $form['options'][$option_value]['currency_code'] = array(
          '#type' => 'select',
          '#options' => $options,
          '#default_value' => isset($defaults[$option_value]['currency_code']) ? $defaults[$option_value]['currency_code'] : $default_currency_code,
        );
      }

      $form['options'][$option_value]['calculate'] = array(
        '#type' => 'select',
        '#options' => array('per_order' => t('per order'), 'per_item' => t('per item')),
        '#default_value' => isset($defaults[$option_value]['calculate'])?$defaults[$option_value]['calculate']:'per_item',
      );
    }

    $form['options'][$option_value]['weight'] = array(
      '#type' => 'item',
      '#input' => TRUE,
      '#delta' => 100,
      '#default_value' => isset($defaults[$option_value]['weight'])?$defaults[$option_value]['weight']:0,
      '#process' => array('form_process_weight'),
      '#attributes' => array(
        'class' => array('commerce-pricing-attributes-set-details-options-weight'),
      ),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['#submit'] = array('commerce_pricing_attributes_default_setting_form_submit');

  $form['#attached']['css'][] = drupal_get_path('module', 'commerce_pricing_attributes') . '/commerce_pricing_attributes.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'commerce_pricing_attributes') . '/commerce_pricing_attributes.js';

  return $form;
}

/**
 * Submit handler of commerce_pricing_attributes_default_setting_form().
 */
function commerce_pricing_attributes_default_setting_form_submit($form, &$form_state){
  $entity = $form_state['build_info']['args'][0];
  $instance = $form_state['build_info']['args'][1];

  foreach ($form_state['values']['options'] as &$option) {
    if(isset($option['price'])){
      $option['price'] = commerce_currency_decimal_to_amount($option['price'], $option['currency_code']);
    }
  }

  $instance['settings']['commerce_pricing_attributes'] = $form_state['values']['options'];
  field_update_instance($instance);

  $form_state['redirect'] = 'admin/commerce/products/option-sets/manage/'.$instance['bundle'].'/fields/';
  drupal_set_message(t('Field\'s Pricing Attributes have been saved!'), 'status');
}
