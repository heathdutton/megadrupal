<?php

/**
 * @file
 * Handler for the property price field
 * Adds check for price_visible value
 */
class views_handler_field_price extends views_handler_field_numeric {

  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields = array(
      'price_visible' => array('table' => $view->base_table, 'field' => 'price_visible'),
    );
  }

  function render($values) {
    $options = $this->options;
    $visible_value = 0;

    if (isset($this->aliases['price_visible'])) {
      $visible_value = $values->{$this->aliases['price_visible']};
    }
    if ($visible_value) {
      $price = $values->{$this->field_alias};
      if (!empty($price)) {
        return parent::render($values);
      }
      else {
        return $options['empty_text'];
      }
    }
    else {
      return $options['empty_text'];
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['empty_text'] = array('default' => NULL);

    return $options;
  }

  /**
   * Provide formatter option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $options = $this->options;

    $form['empty_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Value when price is 0 or empty'),
      '#description' => t('Specify the value to show when no price is available or 0. Do not combine with the "Hide if empty" option.'),
      '#default_value' => isset($options['empty_text']) ? $options['empty_text'] : NULL,
    );
  }
}