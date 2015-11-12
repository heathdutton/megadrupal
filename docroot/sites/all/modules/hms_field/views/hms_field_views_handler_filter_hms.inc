<?php

/**
 * @file
 * Definition of hms_field_views_handler_filter_hms.
 */

/**
 * Filter to handle hms fields stored as a integer.
 *
 */
class hms_field_views_handler_filter_hms extends views_handler_filter_numeric {

  function option_definition() {
    $options = parent::option_definition();
    $all_options = array_keys(_hms_format_options());
    $options['value']['contains']['input_format'] = array('default' => reset($all_options));
    return $options;
  }

  function value_form(&$form, &$form_state) {
    $input = $form_state['input'];
    parent::value_form($form, $form_state);
    // We need to set our own input values, they are special.
    $form_state['input'] = $input;
    if (!empty($form_state['exposed'])) {
      $identifier = $this->options['expose']['identifier'];
    }
    $children = element_children($form['value']);

    if (!count($children)) {
      if ($form['value']['#type'] == 'textfield') {
        $form['value']['#type'] = 'hms';
        $form['value']['#format'] = $this->options['value']['input_format'];
        if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
          $form_state['input'][$identifier] = _hms_seconds_to_formatted (
            $this->value['value'],
            $this->options['value']['input_format']
          );
          if (empty($form_state['input'][$identifier])) {
            $form_state['input'][$identifier] = '';
          }
        }
      }
    }
    else {
      foreach($children as $key) {
        if ($form['value'][$key]['#type'] == 'textfield') {
          $form['value'][$key]['#type'] = 'hms';
          $form['value'][$key]['#format'] = $this->options['value']['input_format'];
          if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier][$key])) {
            $form_state['input'][$identifier][$key] = _hms_seconds_to_formatted (
              $this->value[$key],
              $this->options['value']['input_format']
            );
            if (empty($form_state['input'][$identifier][$key])) {
              $form_state['input'][$identifier][$key] = '';
            }
          }
        }
      }
    }
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['value']['input_format'] = array(
      '#type' => 'select',
      '#title' => t('Input format'),
      '#weight' => -5,
      '#options' => _hms_format_options(),
      '#default_value' => $this->options['value']['input_format'],
    );
  }

  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);
    foreach($form_state['input']['options']['value'] as $key => $value) {
      if ($form_state['complete form']['options']['value'][$key]['#type'] == 'hms') {
        $form_state['complete form']['options']['value'][$key]['#format'] =
        $form['options']['value'][$key]['#format'] = $form_state['values']['options']['value']['input_format'];
        if(strlen($value)) {
          // We recalculate and set the value to the possibly new format.
          $sec = _hms_formatted_to_seconds($value, $form_state['values']['options']['value']['input_format']);
          form_set_value($form_state['complete form']['options']['value'][$key], $sec, $form_state);
        }
      }
    }
  }
}

