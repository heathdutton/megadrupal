<?php
/**
 * Developers documentation for Cloudwords Tranlation module.
 */

/**
 * Alter the node object just prior to being saved
 *
 *
 * @param $node
 *   The node entity object passed by reference.
 * @param $data
 *   The data structure fetched from the incoming xliff import.
 * @param $source_language
 *   The host entity language.
 * @param $translation_type
 *   The translation type.  "entity_translation" or "node_translation"
 */

function hook_cloudwords_translation_before_node_save(&$node, $data, $source_language, $translation_type){
  $node->body['en'][0]['format'] = 'filtered_html';
}

/**
 * Perform additional actions on an updated translated node
 *
 *
 * @param $node
 *   The node entity object passed by reference.
 * @param $data
 *   The data structure fetched from the incoming xliff import.
 * @param $source_language
 *   The host entity language.
 * @param $translation_type
 *   The translation type.  "entity_translation" or "node_translation"
 */

function hook_cloudwords_translation_after_node_save(&$node, $data, $source_language, $translation_type){
  watchdog('cloudwords', 'Node %nid updated translation', array('%nid' => $node->nid), WATCHDOG_NOTICE, $link = NULL);
}
