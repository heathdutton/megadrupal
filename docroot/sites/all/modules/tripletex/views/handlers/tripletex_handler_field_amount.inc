<?php

/**
 * @file
 * Definition of tripletex_handler_field_amount.
 */

class tripletex_handler_field_amount extends views_handler_field_numeric {

  function option_definition() {
    $options = parent::option_definition();

    $options['set_precision'] = array('default' => TRUE);
    $options['precision'] = array('default' => 2);
    $options['decimal'] = array(
      'default' => ',',
      'translatable' => TRUE,
    );
    $options['separator'] = array(
      'default' => ' ',
      'translatable' => TRUE,
    );
    $options['prefix'] = array(
      'default' => 'kr. ',
      'translatable' => TRUE,
    );
    $options['suffix'] = array(
      'default' => '',
      'translatable' => TRUE,
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    if (!empty($this->definition['float'])) {

      $form['use_default_formatting'] = array(
        '#type' => 'checkbox',
        '#title' => t('Default formatting'),
        '#description' => t('If checked, the default formatting from Tripletex will be used'),
        '#default_value' => isset($this->options['use_default_formatting'])?$this->options['use_default_formatting']:FALSE,
      );
    }

  }

  function render($values) {

    $value = $values->{$this->field_alias};
    if (isset($this->options['use_default_formatting']) && $this->options['use_default_formatting'] ) {
      $value = $value;  // TODO: Add formatting based on default Tripletex settings
    }
    elseif (!empty($this->options['set_precision'])) {
      $value = number_format($value, $this->options['precision'], $this->options['decimal'], $this->options['separator']);
    }
    else {
      $remainder = abs($value) - intval(abs($value));
      $value = $value > 0 ? floor($value) : ceil($value);
      $value = number_format($value, 0, '', $this->options['separator']);
      if ($remainder) {
        // The substr may not be locale safe.
        $value .= $this->options['decimal'] . substr($remainder, 2);
      }
    }

    // Check to see if hiding should happen before adding prefix and suffix.
    if ($this->options['hide_empty'] && empty($value) && ($value !== 0 || $this->options['empty_zero'])) {
      return '';
    }

  return check_plain($this->options['prefix'] . $value . $this->options['suffix']);
  }
}
