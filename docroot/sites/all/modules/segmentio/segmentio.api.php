<?php

/**
 * @file
 * Segment.io Drupal module API
 */

/**
 * Retrieves hook information available to Segment.io.
 *
 * @return An associative array where the key represents a
 *   "module:hook" function, and the value is the name.
 */
function hook_segmentio_info() {
  return array(
  	'example:segmentio_mytrackinginfo' => t("MyModule's MyTrackingInfo"),
  );
}

/**
 * Segment.io callback; Retrieve the tracking code for mytrackinginfo.
 */
function example_segmentio_mytrackinginfo($page = array()) {
  $code = array();
  $code[] = 'window.analytics.track("Signed Up", {
  	plan: "Startup",
  	source: "Analytics Academy"
  });';
  return $code;
}
