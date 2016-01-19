<?php

/**
 * Creates integer fields that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('IntegerField')->set('field_name', 'my_field');
 * @endcode
 */
class CINC_IntegerField extends CINC_Field {

  public static $dependencies = array('field', 'number');
  public static $field_types = array('integer');

  function __construct() {

    parent::__construct();

    $this
      ->set('type', 'number_integer')
      ->set('module', 'number')
      ->set('columns', array(
        'value' => array(
          'type' => 'int',
          'not null' => FALSE,
        ),
      ))
      ->set_instance('widget', array(
        'type' => 'number',
        'module' => 'number',
      ));

  }

}
