<?php

/**
 * @file
 * KalturaSettings class.
 */

/**
 * Class KalturaSettings.
 */
class KalturaSettings {
  public $kdp_widgets = array(
      'audio' => array(
        'dark' => array( 'view_uiconf' => '605', 'remix_uiconf' => '604', 'preview_image' => 'dark-player.jpg' ),
        'gray' => array( 'view_uiconf' => '607', 'remix_uiconf' => '606', 'preview_image' => 'gray-player.jpg' ),
        'white-blue' => array( 'view_uiconf' => '609', 'remix_uiconf' => '608', 'preview_image' => 'white-blue-player.jpg' ),
      ),
      'viewplaylist' => array(
        'dark' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'dark-player.jpg' ),
        'gray' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'gray-player.jpg' ),
        'white-blue' => array( 'view_uiconf' => '609', 'remix_uiconf' => '608', 'preview_image' => 'white-blue-player.jpg' ),
      ),
      'video' => array(
        'dark' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'dark-player.jpg' ),
        'gray' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'gray-player.jpg' ),
        'white-blue' => array( 'view_uiconf' => '609', 'remix_uiconf' => '608', 'preview_image' => 'white-blue-player.jpg' ),
      ),
      'roughcut' => array(
        'dark' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'dark-player.jpg' ),
        'gray' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'gray-player.jpg' ),
        'white-blue' => array( 'view_uiconf' => '609', 'remix_uiconf' => '608', 'preview_image' => 'white-blue-player.jpg' ),
      ),
      'comment' => array(
        'dark' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'dark-player.jpg' ),
        'gray' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'gray-player.jpg' ),
        'white-blue' => array( 'view_uiconf' => '609', 'remix_uiconf' => '608', 'preview_image' => 'white-blue-player.jpg' ),
      ),
  );

  public $media_types_map = array(
    KALTURA_MEDIA_TYPE_VIDEO => 'Video',
    KALTURA_MEDIA_TYPE_IMAGE => 'Image',
    KALTURA_MEDIA_TYPE_AUDIO => 'Audio',
    KALTURA_MEDIA_TYPE_REMIX => 'Remix',
  );

  public $media_status_map = array(
    1 => 'Preconvert',
    2 => 'Ready',
    3 => 'Deleted',
    4 => 'Pending',
    5 => 'Moderate',
    6 => 'Blocked',
    7 => 'No Content',
    -1 => 'Error Converting',
  );
}
