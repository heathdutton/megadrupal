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
class views_doubledate_handler_field_date extends views_handler_field_date {
  function option_definition() {
    $options = parent::option_definition();

    $options['views_doubledate_date_type_1'] = array('default' => 'short');
    $options['views_doubledate_custom_date_format_1'] = array('default' => '');
    $options['views_doubledate_offset'] = array('default' => '');
    $options['views_doubledate_date_type_2'] = array('default' => 'short');
    $options['views_doubledate_custom_date_format_2'] = array('default' => '');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Copy existing date field format settings and configure for
    // reuse under the new format.
    $form['views_doubledate_date_type_1'] = $form['date_format'];
    $form['views_doubledate_date_type_1']['#title'] = t('Primary date format');
    $form['views_doubledate_date_type_1']['#process'] = array('ctools_dependent_process');
    $form['views_doubledate_date_type_1']['#dependency'] = array('edit-options-date-format' => array('views_doubledate'));
    $form['views_doubledate_date_type_1']['#default_value'] = isset($this->options['views_doubledate_date_type_1']) ? $this->options['views_doubledate_date_type_1'] : '';

    $form['views_doubledate_custom_date_format_1'] = $form['custom_date_format'];
    $form['views_doubledate_custom_date_format_1']['#title'] = t('Primary custom date format, if needed');
    $form['views_doubledate_custom_date_format_1']['#process'] = array('ctools_dependent_process');
    $form['views_doubledate_custom_date_format_1']['#dependency'] = array('edit-options-date-format' => array('views_doubledate'));
    $form['views_doubledate_custom_date_format_1']['#default_value'] = isset($this->options['views_doubledate_custom_date_format_1']) ? $this->options['views_doubledate_custom_date_format_1'] : '';

    // Add the new format.
    $form['date_format']['#options']['views_doubledate'] = t('Apply two date formats to a single field.');
    $form['views_doubledate_offset'] = array(
      '#type' => 'textfield',
      '#title' => t('Offset'),
      '#default_value' => isset($this->options['views_doubledate_offset']) ? $this->options['views_doubledate_offset'] : '',
      '#description' => t('An offset from the current time such as "+1 day" or "-2 hours -30 minutes"'),
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-options-date-format' => array('views_doubledate')),
    );

    // Copy primary date format and customize as needed.
    $form['views_doubledate_date_type_2'] = $form['views_doubledate_date_type_1'];
    $form['views_doubledate_date_type_2']['#title'] = t('Secondary date format');
    $form['views_doubledate_date_type_2']['#default_value'] = isset($this->options['views_doubledate_date_type_2']) ? $this->options['views_doubledate_date_type_2'] : '';

    // Copy custom date formatter and customize as needed.
    $form['views_doubledate_custom_date_format_2'] = $form['views_doubledate_custom_date_format_1'];
    $form['views_doubledate_custom_date_format_2']['#title'] = t('Secondary custom date format, if needed');
    $form['views_doubledate_custom_date_format_2']['#default_value'] = isset($this->options['views_doubledate_custom_date_format_2']) ? $this->options['views_doubledate_custom_date_format_2'] : '';

  }

  function render($values) {
    $value = $values->{$this->field_alias};

    if ($this->options['date_format'] == 'views_doubledate') {
      $offset = intval(strtotime($this->options['views_doubledate_offset'], 0));
      if (REQUEST_TIME + $offset < $value) {
        $format_number = 1;
      }
      else {
        $format_number = 2;
      }
      $sub_format = $this->options['views_doubledate_date_type_' . $format_number];

      if (in_array($sub_format, array('custom', 'raw time ago', 'time ago', 'raw time span', 'time span'))) {
        $custom_sub_format = $this->options['views_doubledate_custom_date_format_' . $format_number];
      }

      if (!$value) {
        // @todo Please change this theme call to use an associative
        // array for the $variables parameter.
        return theme('views_nodate');
      }
      else {
		// Will be positive for a datetime in the past (ago), and
		// negative for a datetime in the future (hence).
        $time_diff = REQUEST_TIME - $value;
        switch ($sub_format) {
          case 'raw time ago':
            return format_interval($time_diff, is_numeric($custom_sub_format) ? $custom_sub_format : 2);
          case 'time ago':
            return t('%time ago', array('%time' => format_interval($time_diff, is_numeric($custom_sub_format) ? $custom_sub_format : 2)));
          case 'raw time span':
            return ($time_diff < 0 ? '-' : '') . format_interval(abs($time_diff), is_numeric($custom_sub_format) ? $custom_sub_format : 2);
          case 'time span':
            return t(($time_diff < 0 ? '%time hence' : '%time ago'), array('%time' => format_interval(abs($time_diff), is_numeric($custom_sub_format) ? $custom_sub_format : 2)));
          case 'custom':
            return format_date($value, $sub_format, $custom_sub_format);
          default:
            return format_date($value, $sub_format);
        }
      }

    }
    // Alternatively, render the date using the normal date handler.
    else {
      return parent::render($values);
    }
  }
}
