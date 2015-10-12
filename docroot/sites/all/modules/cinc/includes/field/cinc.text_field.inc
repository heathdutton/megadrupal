<?php

/**
 * Creates text fields that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('TextField')->set('field_name', 'my_field');
 * @endcode
 */
class CINC_TextField extends CINC_Field {

  public static $dependencies = array('field', 'text');
  public static $field_types = array('text');

  function __construct() {

    parent::__construct();

    $this
      ->set('type', 'text')
      ->set('module', 'text')
      ->set('settings', array(
        'max_length' => 255,
      ))
      ->set_instance('settings', array(
        'text_processing' => 0,
      ))
      ->set_instance('widget', array(
        'type' => 'text_textfield',
        'module' => 'text',
        'settings' => array(
          'size' => 60,
        ),
      ));

  }

}
