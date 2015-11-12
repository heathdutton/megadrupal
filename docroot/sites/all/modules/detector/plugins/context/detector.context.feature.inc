<?php

/**
 * Expose Detector features as a context condition.
 */
class detector_feature_context_condition extends context_condition {
  function condition_values() {
    global $ua;
    $arr = _detector_flatten_list($ua);
    
    $values = array();
    foreach (array_keys($arr) as $key) {
      $values[$key] = $key;
    }
    return $values;
  }
  
  /**
   * Override of condition_form().
   * Use select widget.
   */
  function condition_form($context) {
    $form = parent::condition_form($context);
    $form['#type'] = 'select';
    return $form;
  }
  
  /**
   * Override of condition_form_submit().
   * Trim any identifier padding for non-unique path menu items.
   */
  function condition_form_submit($values) {
    // Trim any identifier padding for non-unique path menu items.
    $values = array($values);
    return $values;
  }
  
  function options_form($context) {
    $defaults = $this->fetch_from_context($context, 'options');
    return array(
      'value_match' => array(
        '#title' => t('Feature matches value'),
        '#type' => 'textfield',
        '#description' => t('Set this context where the feature matches this value.'),
        '#default_value' => isset($defaults['value_match']) ? $defaults['value_match'] : '',
      ),
      'value_preset' => array(
        '#title' => t('Feature matches preset value'),
        '#type' => 'checkboxes',
        '#description' => t('Use this for more common values.'),
        '#options' => array(
          'TRUE' => 'TRUE',
          'FALSE' => 'FALSE',
        ),
        '#default_value' => isset($defaults['value_preset']) ? $defaults['value_preset'] : null,
      ),
    );
  }
  
  function execute($key, $val = null) {
    foreach ($this->get_contexts($key) as $context) {
      $options = $this->fetch_from_context($context, 'options');
      $value = $options['value_match'];
      foreach ($options['value_preset'] as $pre_key => $pre_val) {
        if ($pre_key == $pre_val) {
          $value = $pre_key;
          break;
        }
      }
        
      if (!empty($value) && $value === $val) {
        $this->condition_met($context, $key);
      }
      elseif (empty($value) && empty($val)) {
        $this->condition_met($context, $key);
      }
    }
  }
}
