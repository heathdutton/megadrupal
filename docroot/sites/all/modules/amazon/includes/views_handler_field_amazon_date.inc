<?php

/**
 * A handler to provide proper displays for dates.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_amazon_date extends views_handler_field_date {

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $time = REQUEST_TIME;
    $form['date_format']['#options'] = array(
      'default' => format_date($time, 'custom', 'Y-m-d'),
      'custom' => t('Custom'),
    );
  }

  function render($values) {
    $value = strtotime($this->get_value($values));
    $format = $this->options['date_format'];
    $default_format = 'Y-m-d';
    if (in_array($format, array('custom', 'raw time ago', 'time ago', 'raw time span', 'time span'))) {
      $custom_format = $this->options['custom_date_format'];
    }

    if (!$value) {
      return theme('views_nodate');
    }
    else {
      $time_diff = REQUEST_TIME - $value; // will be positive for a datetime in the past (ago), and negative for a datetime in the future (hence)
      switch ($format) {
        case 'custom':
          return format_date($value, $format, $custom_format);
        default:
          return format_date($value, 'custom', $default_format);
      }
    }
  }
}
