<?php

/**
 * Creates long text fields that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('LongTextField')->set('field_name', 'my_field');
 * @endcode
 */
class CINC_LongTextField extends CINC_Field {

  public static $dependencies = array('field', 'text');
  public static $field_types = array('text_long', 'text_with_summary');

  function __construct() {

    parent::__construct();

    $this
      ->set('type', 'text_long')
      ->set('module', 'text')
      ->set_instance('settings', array(
        'text_processing' => 0,
      ))
      ->set_instance('widget', array(
        'type' => 'text_textarea',
        'module' => 'text',
        'settings' => array(
          'rows' => 15,
        ),
      ));

  }

}