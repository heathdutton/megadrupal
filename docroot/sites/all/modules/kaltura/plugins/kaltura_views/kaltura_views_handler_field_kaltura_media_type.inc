<?php

/**
 * @file
 * Class kaltura_views_handler_field_kaltura_media_type.
 */

/**
 * Media Type field handler class.
 */
class kaltura_views_handler_field_kaltura_media_type extends views_handler_field_numeric {
  function render($values) {
    $kaltura_settings = new KalturaSettings();
    return $kaltura_settings->media_types_map[$values->node_kaltura_kaltura_media_type];
  }
}
