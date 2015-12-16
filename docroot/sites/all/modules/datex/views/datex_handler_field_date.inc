<?php

/**
 * Date field handler for views with datex support.
 */
class datex_handler_field_date extends views_handler_field_date {
  public function render($values) {
    static $parent_handled = array(
      'raw time ago',
      'time ago',
      'raw time hence',
      'time hence',
      'raw time span',
      'inverse time span',
      'time span',
    );
    
    $format = $this->options['date_format'];  
    $calendar = _datex_language_calendar(NULL, DATEX_NONPATCHING_MODE);

    // In this cases, Everything will be handled properly by parent and there
    // is no need for datex to interfere with views original handler.    
    if (!$calendar || in_array($format, $parent_handled)) {
      return parent::render($values);
    }

    if ($value = $this->get_value($values)) {
      if ($format === 'custom') {
        $format = $this->options['custom_date_format'];
      }
      else {
        $format = variable_get('date_format_' . $format);
      }

      if (empty($this->options['timezone'])) {
        $tz = drupal_get_user_timezone();
      }
      else {
        $tz = $this->options['timezone'];
      }

      return datex_format($calendar, $value, $format, $tz);
    }
  }
}

