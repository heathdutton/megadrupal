<?php

/**
 * @file
 * Definition of views_handler_field_membership_cycles.
 */

/**
 * Render a field as a numeric value.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_membership_cycles extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['button_type'] = 'button_type';
    $this->additional_fields['length'] = 'length';
    $this->additional_fields['cycles'] = 'cycles';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['separator'] = array('default' => ',', 'translatable' => TRUE);
    $options['prefix'] = array('default' => '', 'translatable' => TRUE);
    $options['suffix'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['separator'] = array(
      '#type' => 'select',
      '#title' => t('Thousands marker'),
      '#options' => array(
        '' => t('- None -'),
        ',' => t('Comma'),
        ' ' => t('Space'),
        '.' => t('Decimal'),
        '\'' => t('Apostrophe'),
      ),
      '#default_value' => $this->options['separator'],
      '#description' => t('What single character to use as the thousands separator.'),
      '#size' => 2,
    );
    $form['prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Prefix'),
      '#default_value' => $this->options['prefix'],
      '#description' => t('Text to put before the number, such as currency symbol.'),
    );
    $form['suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Suffix'),
      '#default_value' => $this->options['suffix'],
      '#description' => t('Text to put after the number, such as currency symbol.'),
    );

    parent::options_form($form, $form_state);
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    if ((int) $values->{$this->aliases['button_type']} === 0 || (int) $values->{$this->aliases['length']} === 0) {
      return t('N/A');
    }

    $value = $values->{$this->aliases['cycles']};

    if ((int) $value > 0) {
      $value = number_format($value, 0, ".", $this->options['separator']);
    }
    else {
      $value = t('Unlimited');
    }

    // Check to see if hiding should happen before adding prefix and suffix.
    if ($this->options['hide_empty'] && empty($value) && ($value !== 0 || $this->options['empty_zero'])) {
      return '';
    }

    return $this->sanitize_value($this->options['prefix'], 'xss')
        . $this->sanitize_value($value)
        . $this->sanitize_value($this->options['suffix'], 'xss');
  }

}
