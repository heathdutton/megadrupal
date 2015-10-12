<?php

/**
 * Creates video embed fields that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('VideoEmbedField')->set('field_name', 'my_field');
 * @endcode
 */
class CINC_VideoEmbedField extends CINC_Field {

  public static $dependencies = array('field', 'video_embed_field');
  public static $field_types = array('video_embed_field');

  /**
   * Constructor
   */
  public function __construct() {

    parent::__construct();

    $this
      ->set('type', 'video_embed_field')
      ->set('module', 'video_embed_field')
      ->set('settings', array())
      ->set_instance('settings', array(
        'user_register_form' => FALSE,
        'description_field' => 0,
        'description_length' => 128,
        'allowed_providers' => array(
          'youtube',
          'vimeo',
        ),
      ))
      ->set_instance('widget', array(
        'type' => 'video_embed_field_video',
        'settings' => array(),
        'module' => 'video_embed_field',
      ));
  }

}
