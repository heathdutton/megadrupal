<?php
/**
 * @file
 * Giant Bomb data property handler.
 */

class GiantBombDateProperty extends GiantBombBaseProperty {
  /**
   * 
   * @param $value
   *   Expecting: "2008-07-07 16:41:34.797705"
   * @param $settings
   *   Saved form values from $this::form()
   */
  function __construct($value, $settings = array()) {
    $this->settings = $settings;

    $matches = array();
    preg_match(DATE_REGEX_DATETIME, $value, $matches);
    if (count($matches)) {
      $datetime = $matches[0];
      $this->value = new DateObject($datetime, 'America/Los_Angeles', DATE_FORMAT_DATETIME);
    }
    else {
      $this->value = NULL;
    }
  }

  /**
   * @see GiantBombBaseProperty::validate()
   */
  function validate() {
    return isset($this->value);
  }

  /**
   * @see GiantBombBaseProperty::value()
   */
  function value() {
    if (isset($this->value)) {
      $format = isset($this->settings['output_format']) ? $this->settings['output_format'] : '';
      return date_format_date($this->value, 'custom', $format);
    }

    return FALSE;
  }

  static function form(&$form, &$form_state, $settings = array()) {
    $property_form['output_format'] = array(
      '#title' => t('Output'),
      '#type' => 'select',
      '#description' => t('If using a Date field, ensure this matches the field type.'),
      '#empty_option' => t('- None -'),
      '#default_value' => !empty($settings['output_format']) ? $settings['output_format'] : '',
      '#options' => array(
        DATE_FORMAT_DATETIME => t('Date'), // field: datetime (DATE_DATETIME)
        DATE_FORMAT_ISO => t('Date (ISO format)'), // field: date (DATE_ISO) 
        DATE_FORMAT_UNIX => t('Date (Unix Timestamp)'), // field: datestamp (DATE_UNIX)
        DATE_FORMAT_ICAL => t('iCal'), // field: ical (DATE_ICAL)
      ),
    );
    return $property_form;
  }
}