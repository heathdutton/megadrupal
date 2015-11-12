<?php

/**
 * Creates list_text fields that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('ListTextField')->set('field_name', 'my_field');
 * @endcode
 */
class CINC_ListTextField extends CINC_Field {

  /**
   * Constructor
   */
  public function __construct() {

    parent::__construct();

    $this
      ->set('type', 'list_text')
      ->set('module', 'list')
      ->set('settings', array(
        'max_length' => 255,
        'allowed_values' => array(),
        'allowed_values_function' => '',
      ))
      ->set_instance('settings', array(
        'text_processing' => 0,
      ))
      ->set_instance('widget', array(
        'type' => 'options_select',
        'module' => 'options',
      ));
  }

}
