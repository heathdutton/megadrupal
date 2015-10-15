<?php

/**
 * @file
 * Definition of views_handler_field_membership_length_type.
 */

/**
 * Render a field as a numeric value.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_membership_length_type extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['length'] = 'length';
    $this->additional_fields['length_type'] = 'length_type';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['prefix'] = array('default' => '', 'translatable' => TRUE);
    $options['suffix'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Prefix'),
      '#default_value' => $this->options['prefix'],
      '#description' => t('Text to put before the field, such as currency symbol.'),
    );
    $form['suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Suffix'),
      '#default_value' => $this->options['suffix'],
      '#description' => t('Text to put after the field, such as currency symbol.'),
    );

    parent::options_form($form, $form_state);
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $length = $values->{$this->aliases['length']};

    if ((int) $length === 0) {
      return t('N/A');
    }

    switch ($values->{$this->aliases['length_type']}) {
      case "D":
        $value = t('day(s)');
        break;
      case "W":
        $value = t('week(s)');
        break;
      case "M":
        $value = t('month(s)');
        break;
      case "Y":
        $value = t('year(s)');
        break;
      default:
        $value = '';
        break;
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
