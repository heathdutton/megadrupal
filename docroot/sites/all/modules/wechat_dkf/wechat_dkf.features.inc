<?php
/**
 * @file
 * wechat_dkf.features.inc
 */

/**
 * Implements hook_default_wechat_response_message_type().
 */
function wechat_dkf_default_wechat_response_message_type() {
  $items = array();
  $items['transfer_customer_service'] = entity_import('wechat_response_message_type', '{
    "type" : "transfer_customer_service",
    "label" : "Transfer customer service",
    "weight" : "0",
    "data" : null,
    "rdf_mapping" : []
  }');
  return $items;
}
