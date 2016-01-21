<?php

/**
 * @file
 *   Hook implementations dealing with fields provided by list.module
 */

/**
 * Implements hook_commerce_bpc_is_combination_field().
 */
function commerce_bpc_commerce_bpc_is_combination_field($instance) {
  $field = field_info_field($instance['field_name']);
  if ($field['module'] == 'list') {
    return !_commerce_bpc_get_value($instance, array('commerce_bpc', 'is_static'), FALSE);
  }
}
/**
 * Implements hook_commerce_bpc_MODULE_NAME_form_element_alter().
 */
function commerce_bpc_commerce_bpc_list_form_element_alter(&$form, &$form_state, &$path) {

  $element = drupal_array_get_nested_value($form, $path);
  $lang = $element['#language'];
  $field_name = $element[$lang]['#field_name'];
  $instance = field_info_instance('commerce_product', $field_name, $form['product_type']['#value']);
  if (commerce_bpc_commerce_bpc_is_combination_field($instance)) {
    switch ($instance['widget']['type']) {
      case 'options_onoff':
        $field = field_info_field($field_name);
        $element[$lang]['#options'] = $field['settings']['allowed_values'];
        $element[$lang]['#type'] = 'checkboxes';
        break;
      case 'options_buttons':
        // Make sure checkboxes are being used.
        $element[$lang]['#type'] = 'checkboxes';
    }
    $element[$lang]['#multiple'] = TRUE;
    // Unset -none- option if present.
    unset($element[$lang]['#options']['_none']);
    // No pre-selection.
    $element[$lang]['#default_value'] = array();

    // Move to comibinations-fieldset.
    $form['combinations'][$field_name] = $element;
    drupal_array_set_nested_value($form, $path, NULL);

    // Change path to allow subsequent hooks operate on the form element.
    $path = array('combinations', $field_name);

    if (empty($form_state['commerce_bpc']['list']['combination_fields'])
    || !in_array($field_name, $form_state['commerce_bpc']['list']['combination_fields'])) {
      // Record what we have done. As this hook may be run multiple times
      // due to form rebuilds, we need to make sure that we record each field
      // only once.
      $form_state['commerce_bpc']['list']['combination_fields'][] = $field_name;
    }
  }
}
/**
 * Implements hook_commmerce_bpc_get_combinations().
 */
function commerce_bpc_commerce_bpc_get_combinations($form, &$form_state, &$combinations) {
  $new_combinations = array();
  if (!empty($form_state['commerce_bpc']['list'])) {
    $fields = $form_state['commerce_bpc']['list']['combination_fields'];
    foreach ($fields as $field_name) {
      $new_combinations = array();
      $langs = array_keys($form_state['values']['combinations'][$field_name]);
      $lang = reset($langs);
      $values = $form_state['values']['combinations'][$field_name][$lang];

      foreach ($combinations as $combination) {
        foreach ($values as $columns) {
          if (!list_field_is_empty($columns, $field_name)) {
            $new_combinations[] = $combination + array(
              $field_name => array($lang => array($columns)),
            );
          }
          else {
            $new_combinations[] = $combination;
            // If the values for a field are empty, we make sure that
            // we (re)-add the original combination only once.
            break;
          }
        }
      }
      $combinations = $new_combinations;
    }
  }
}

/**
 * Implements hook_commerce_bpc_tokens().
 *
 */
function commerce_bpc_commerce_bpc_tokens($product_type, $combination, $options) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  foreach ($combination as $field_name => $values) {
    $field = field_info_field($field_name);
    $instance = field_info_instance('commerce_product', $field_name, $product_type);
    if ($field['module'] == 'list' &&  !_commerce_bpc_get_value($instance, array('commerce_bpc', 'is_static'), FALSE)) {
      // TODO: Treat languages properly.
      $items = reset($values);
      // We only allow a single value per combination, so we can treat
      // this like a single value field.
      if ($value = $items[0]['value']) {
        $replacements['values'][$field_name] = $value;
        $labels = list_allowed_values($field);
        $replacements['labels'][$field_name] = $sanitize ? check_plain($labels[$value]) : $labels[$value] ;
      }
    }
  }
  return $replacements;
}
/**
 * Implements hook_commerce_bpc_token_sample_values().
 */
function commerce_bpc_commerce_bpc_token_sample_values($instance) {
  $samples = array();
  $field_name = $instance['field_name'];
  $field = field_info_field($field_name);
  if ($field['module'] == 'list') {
    $options = list_allowed_values($field);
    $values = array_keys($options);
    $samples['value'] = array(LANGUAGE_NONE => array(array('value' => reset($values))));
    $samples['label'] = reset($options);
  }
  return $samples;
}
