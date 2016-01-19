<?php
/**
 * @file
 * message_types.features.inc
 */

/**
 * Implements hook_default_wechat_request_message_type().
 */
function wechat_default_wechat_request_message_type() {
  $items = array();
  
  $items['text'] = entity_import('wechat_request_message_type', '{
    "type" : "text",
    "label" : "Text",
    "weight" : "0",
    "data" : null,
    "rdf_mapping" : []	
  }');
  
  $items['image'] = entity_import('wechat_request_message_type', '{
    "type" : "image",
    "label" : "Image",
    "weight" : "1",
    "data" : null,
    "rdf_mapping" : []	
  }');
  
  $items['voice'] = entity_import('wechat_request_message_type', '{
    "type" : "voice",
    "label" : "Voice",
    "weight" : "2",
    "data" : null,
    "rdf_mapping" : []	
  }');
  
  $items['video'] = entity_import('wechat_request_message_type', '{
    "type" : "video",
    "label" : "Video",
    "weight" : "3",
    "data" : null,
    "rdf_mapping" : []	
  }');
  
  $items['location'] = entity_import('wechat_request_message_type', '{
    "type" : "location",
    "label" : "Location",
    "weight" : "4",
    "data" : null,
    "rdf_mapping" : []	
  }');
  $items['link'] = entity_import('wechat_request_message_type', '{
    "type" : "link",
    "label" : "Link",
    "weight" : "5",
    "data" : null,
    "rdf_mapping" : []	
  }');
  $items['event'] = entity_import('wechat_request_message_type', '{
    "type" : "event",
    "label" : "Event",
    "weight" : "6",
    "data" : null,
    "rdf_mapping" : []	

  }');  
  return $items;
}
