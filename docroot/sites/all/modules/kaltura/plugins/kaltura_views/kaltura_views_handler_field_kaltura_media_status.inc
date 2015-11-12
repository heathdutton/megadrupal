<?php

/**
 * @file
 * Class kaltura_views_handler_field_kaltura_media_status.
 */

/**
 * Media Status field handler class.
 */
class kaltura_views_handler_field_kaltura_media_status extends views_handler_field_numeric {
  function render($values) {
    $kaltura_settings = new KalturaSettings();
    return $kaltura_settings->{'media_status_map'}[$values->node_kaltura_kstatus];
  }
}
