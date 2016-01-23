<?php

/**
 * Creates text fields that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('FileField')->set('field_name', 'my_field');
 * @endcode
 */
class CINC_FileField extends CINC_Field {

  public static $field_types = array('file');

  function __construct() {

    parent::__construct();

    $this
      ->set('type', 'file')
      ->set('module', 'file')
      ->set('settings', array(
        'display_field' => 0,
        'display_default' => 0,
        'uri_scheme' => 'public',
      ));

  }

}
