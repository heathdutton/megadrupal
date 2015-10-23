<?php

/**
 * @file
 * Feeds Tamper is used to import multi-value fields. We split with a "/" sign.
 */

/**
 * Creates a Feed object (the importer).
 */
function mirrors_feeds_tamper_create($name, $field_id) {
  module_load_include('inc', 'mirrors', 'includes/feeds_tamper/mirrors.feeds_tamper.template');

  $feeds_tamper = mirrors_feeds_tamper_default_template($name, $field_id);
  return $feeds_tamper;
}
