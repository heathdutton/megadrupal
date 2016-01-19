<?php

/**
 * Theme function for 'os_reader_google_feedapi' link field formatter.
 */
function theme_google_feedapi_formatter_google_feedapi($info){
  _google_feedapi_load_common_feed_js();

  list($id) = entity_extract_ids($info['entity_type'], $info['entity']);

  $feed_div_id = implode("_", array(
    "gf",
    $info['entity_type'],
    $id,
    $info['delta'],
    $info['num_to_show'],
    $info['content_summary'],
    $info['show_content'],
    $info['time_display']
  ));

  $url_options = array('external' => true);
  if(isset($info['element']['query'])){
    $url_options['query'] = $info['element']['query'];
  }

  drupal_add_js(array(
    'googleFeedAPI' => array(
      $feed_div_id => array(
        'url' => url($info['element']['url'],$url_options),
        'num_feeds' => $info['num_to_show'],
        'content_summary' => $info['content_summary'],
        'show_content' => $info['show_content'],
        'time_display' => $info['time_display'],
      )
    )
  ), 'setting');

  return "<div id='{$feed_div_id}'></div>";
}

/**
 * Statically loads the common required javascript libraries.
 */
function _google_feedapi_load_common_feed_js() {
  static $common_js;
  if (!isset($common_js)) {
    $common_js = TRUE;
    // Adds this modules formatter behaviors, which depends on
    // the external Google FeedAPI Integration files.
    drupal_add_library('google_feedapi', 'google_feedapi');
  }
}
