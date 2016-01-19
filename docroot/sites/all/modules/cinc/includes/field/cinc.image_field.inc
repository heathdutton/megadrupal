<?php

/**
 * Creates image fields that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('ImageField')->set('field_name', 'my_field');
 * @endcode
 */
class CINC_ImageField extends CINC_Field {

  public static $dependencies = array('field', 'image');
  public static $field_types = array('image');

  function __construct() {

    parent::__construct();

    $this
      ->set('type', 'image')
      ->set('module', 'image')
      ->set('settings', array(
        'uri_scheme' => 'public',
        'default_image' => 0,
      ))
      ->set_instance('settings', array(
        'file_directory' => '',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'alt_field' => '',
        'title_field' => '',
        'default_image' => 0,

      ))
      ->set_instance('widget', array(
        'type' => 'image_image',
        'module' => 'image',
        'settings' => array(
          'progress_indicator' => 'throbber',
          'preview_image_style' => 'medium',
        ),
      ));

  }

}