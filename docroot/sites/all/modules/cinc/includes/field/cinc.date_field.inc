<?php

/**
 * Creates date fields that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('DateField')->set('field_name', 'my_field');
 * @endcode
 */
class CINC_DateField extends CINC_Field {

  public static $dependencies = array('field', 'date');
  public static $field_types = array('date', 'datestamp');

  function __construct() {

    parent::__construct();

    $this
      ->set('type', 'datestamp')
      ->set('module', 'date')
      ->set('settings', array(
        'granularity' => array(
          'year' => 'year',
          'month' => 'month',
          'day' => 'day',
          'hour' => 'hour',
          'minute' => 'minute',
          'second' => 0,
        ),
        'tz_handling' => 'none',
        'timezone_db' => '',
        'cache_enabled' => 0,
        'cache_count' => 4,
        'todate' => 'optional',
      ))
      ->set_instance('settings', array(
        'default_value' => 'now',
        'default_value_code' => '',
        'default_value2' => 'same',
        'default_value_code2' => '',

      ))
      ->set_instance('widget', array(
        'type' => 'date_text',
        'module' => 'date',
        'settings' => array(
          'input_format' => 'Y-m-d H:i:s',
          'input_format_custom' => '',
          'year_range' => '-3:+3',
          'increment' => 15,
          'label_position' => 'above',
          'text_parts' => '',
        ),
      ));

  }

}