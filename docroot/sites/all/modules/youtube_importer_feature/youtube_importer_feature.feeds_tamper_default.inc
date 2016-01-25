<?php
/**
 * @file
 * youtube_importer_feature.feeds_tamper_default.inc
 */

/**
 * Implements hook_feeds_tamper_default().
 */
function youtube_importer_feature_feeds_tamper_default() {
  $export = array();

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE;
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'yt_importer-jsonpath_parser_35-rewrite';
  $feeds_tamper->importer = 'yt_importer';
  $feeds_tamper->source = 'jsonpath_parser:35';
  $feeds_tamper->plugin_id = 'rewrite';
  $feeds_tamper->settings = array('text' => '<object width="640" height="480"><param name="movie" value="http://www.youtube.com/v/[jsonpath_parser:5]?version=3&autohide=1&showinfo=0&rel=0&iv_load_policy=3"></param><param name="allowScriptAccess" value="always"></param><embed src="http://www.youtube.com/v/[jsonpath_parser:5]?version=3&autohide=1&showinfo=0&rel=0&iv_load_policy=3" type="application/x-shockwave-flash" allowscriptaccess="always" width="640" height="480"></embed></object>');
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Rewrite';
  $export['yt_importer-jsonpath_parser_35-rewrite'] = $feeds_tamper;

  return $export;
}
