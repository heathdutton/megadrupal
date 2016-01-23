<?php

/**
 * Creates link fields that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('LinkField')->set('field_name', 'my_field');
 * @endcode
 */
class CINC_LinkField extends CINC_Field {

  public static $dependencies = array('field', 'link');
  public static $field_types = array('link_field');

  function __construct() {

    parent::__construct();

    $this
      ->set('type', 'link_field')
      ->set('module', 'link')
      ->set_instance('settings', array(
        'validate_url' => 1,
        'url' => 0,
        'title' => 'optional',
        'title_value' => '',
        'title_maxlength' => 128,
        'enable_tokens' => 0,
        'display' => array(
          'url_cutoff' => 80,
        ),
        'attributes' => array(
          'target' => 'default',
          'class' => '',
          'rel' => '',
          'configurable_title' => 0,
          'title' => '',
        ),
        'rel_remove' => 'default',
      ))
      ->set_instance('widget', array(
        'type' => 'link_field',
        'module' => 'link',
      ));

  }

}