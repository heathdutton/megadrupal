<?php

/**
 * @file
 * A custom two-part date field handler for Views.
 */

/**
 * A handler to display dates in two different formats, based on an offset from
 * the current time.
 *
 * @ingroup views_field_handlers
 */
class jalalidate_handler_field_date extends views_handler_field_date {
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['date_format']['#options']['jalalidate_custom'] = t('Jalali format');
  } 
     
  function render($values) {
    $type = $this->options['date_format'];
    $value = $values->{$this->field_alias};
    if (in_array($type, array('raw time ago', 'time ago', 'raw time span', 'time span'))) {
      return parent::render($values);
    }
    if ($type == 'custom') {
      $format = $this->options['custom_date_format'];
    }
    else {
      $format = variable_get('date_format_' . $type);
    }
    
    $time = jalalidate_convert_format($value, $format);
    return $time;
  }
}

