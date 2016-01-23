<?php
/**
 * Creates radioactivity fields that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('RadioactivityField')->set('field_name', 'field_my_field');
 * @endcode
 */
class CINC_RadioactivityField extends CINC_Field {

  public static $dependencies = array('field', 'radioactivity');
  public static $field_types = array('radioactivity');

  /**
   * Constructor
   */
  function __construct() {
    parent::__construct();

    $this
      ->set('type', 'radioactivity')
      ->set('module', 'radioactivity')
      ->set('settings', array(
      ))
      ->set_instance('widget', array(
        'profile' => 'default',
        'history' => 0,
        'history_limit' => 8,
      ));

  }

}
