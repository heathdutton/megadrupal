<?php
/**
 * @file
 * message_types.features.inc
 */

/**
 * Implements hook_default_wechat_response_message_type().
 */
function wechat_default_wechat_response_message_type() {

  $items = array();
  $items['image'] = entity_import('wechat_response_message_type', '{
    "type" : "image",
    "label" : "Image",
    "weight" : "0",
    "data" : null,
    "rdf_mapping" : []
  }');
  $items['music'] = entity_import('wechat_response_message_type', '{
    "type" : "music",
    "label" : "Music",
    "weight" : "0",
    "data" : null,
    "rdf_mapping" : []
  }');
  $items['news'] = entity_import('wechat_response_message_type', '{
    "type" : "news",
    "label" : "News",
    "weight" : "0",
    "data" : null,
    "rdf_mapping" : []
  }');
  $items['text'] = entity_import('wechat_response_message_type', '{
    "type" : "text",
    "label" : "Text",
    "weight" : "0",
    "data" : null,
    "rdf_mapping" : []
  }');
  $items['video'] = entity_import('wechat_response_message_type', '{
    "type" : "video",
    "label" : "Video",
    "weight" : "0",
    "data" : null,
    "rdf_mapping" : []
  }');
  $items['voice'] = entity_import('wechat_response_message_type', '{
    "type" : "voice",
    "label" : "Voice",
    "weight" : "0",
    "data" : null,
    "rdf_mapping" : []
  }');
  return $items;
}
