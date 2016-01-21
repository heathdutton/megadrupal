<?php

/**
 * Creates link fields that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('BooleanField')->set('field_name', 'field_my_field');
 * @endcode
 */
class CINC_BooleanField extends CINC_Field {

  public static $dependencies = array('field', 'list');
  public static $field_types = array('list_boolean');

  function __construct() {

    parent::__construct();

    $this
      ->set('type', 'list_boolean')
      ->set('module', 'list')
      ->set('settings', array(
        'allowed_values' => array(
          0 => '',
          1 => ''
        ),
        'allowed_values_function' => array(),
      ))
      ->set_instance('settings', array(
        'allowed_values' => array(),
        'allowed_values_function' => array(),
      ))
      ->set_instance('widget', array(
        'type' => 'options_onoff',
        'module' => 'options',
        'settings' => array(
          'display_label' => 1,
        ),
      ));

  }

}
