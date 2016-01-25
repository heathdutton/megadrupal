<?php
/**
 * @file
 * invites_og.features.inc
 */

/**
 * Implements hook_default_invites_type().
 */
function invites_og_default_invites_type() {
  $items = array();
  $items['og'] = entity_import('invites_type', '{
    "type" : "og",
    "label" : "OG",
    "weight" : "0",
    "data" : "",
    "rdf_mapping" : []
  }');
  return $items;
}
