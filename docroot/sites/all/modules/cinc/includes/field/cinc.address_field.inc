<?php

/**
 * Creates address fields that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('AddressField')->set('field_name', 'my_field');
 * @endcode
 */
class CINC_AddressField extends CINC_Field {

  public static $dependencies = array('field', 'addressfield');
  public static $field_types = array('addressfield');

  function __construct() {

    parent::__construct();

    $this
      ->set('type', 'addressfield')
      ->set('module', 'addressfield')
      ->set_instance('widget', array(
        'type' => 'addressfield_standard',
        'module' => 'addressfield',
        'settings' => array(
          'available_countries' => array(),
          'format_handlers' => array(
            'address' => 'address',
            'address-hide-country' => 0,
            'organisation' => 0,
            'name-full' => 0,
            'name-oneline' => 0,
          ),
        ),
      ));

  }

}