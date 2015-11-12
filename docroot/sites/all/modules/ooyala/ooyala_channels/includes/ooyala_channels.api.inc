<?php

/**
 * @file
 * Ooyala Channel APIs.
 */

/**
 * Assign a set of embed codes to the lineup of a channel.
 *
 * @param $channel_embed_code
 *   The embed code of the channel to edit.
 * @param $embed_codes
 *   An array of embed codes to assign to the channel.
 *
 * @return
 *   TRUE if the operation completed, FALSE otherwise.
 */
function ooyala_channels_assign($channel_embed_code, $embed_codes) {
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');
  $ooyala_api = new OoyalaDrupalWrapper();
  $response = json_decode($ooyala_api->api->request(PUT, "/v2/assets/$channel_embed_code/lineup", json_encode($embed_codes)));
  // TODO: Return something.
}

/**
 * Create a channel in the Ooyala backlot.
 *
 * @param $title
 *   The title of the channel to create.
 *
 * @return
 *   The embedCode of the created channel.
 */
function ooyala_channels_create($title) {
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');
  $body = json_encode(array("name" => $title, "asset_type"=> 'channel'));
  $ooyala_api = new OoyalaDrupalWrapper();
  $response = json_decode($ooyala_api->api->request(POST, "/v2/assets", $body));
  if (isset($response->embed_code)) {
    return $response->embed_code;
  }
  else {
    FALSE;
  }
}

/**
 * List the elements of a channel. This includes the details of the channel as
 * well as the items assigned to the channel.
 *
 * @param $channel_embed_code
 *   The embed code of the channel.
 *
 * @return
 *   An array containing the channel details, or FALSE if an error occured.
 */
function ooyala_channels_list($channel_embed_code) {
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');
  $ooyala_api = new OoyalaDrupalWrapper();
  $response = json_decode($ooyala_api->api->request(GET, "/v2/assets/$channel_embed_code/lineup"));
  // Parse the result of listing the channel.
  if (!empty($response)) {
    $channel = array();
    // Place all items into a lineup property.
    $channel['embed_code'] = $channel_embed_code;
    $channel['lineup'] = array();
    foreach ($response as $child) {
      $item = ooyala_api_video_load($child);
      $channel['lineup'][$child] = $item;
    }
    return $channel;
  }
  return FALSE;
}

/**
 * Recursively convert an object into an array.
 */
function _ooyala_channels_listing_to_array($object) {
  $object = (array)$object;
  // You can't remove child nodes from a SimpleXML object, so instead of
  // dealing with DOMNodes we unset any 'item' keys we find. Their values
  // should all ready be in $object->lineup.
  if (isset($object['item'])) {
    unset($object['item']);
  }
  foreach ($object as $key => $value) {
    if (is_array($value) || is_object($value)) {
      $object[$key] = _ooyala_channels_listing_to_array($object[$key]);
    }
  }

  return $object;
}

