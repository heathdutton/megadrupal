<?php
/**
 * @file
 * Admin functions for node to block module.
 */

/**
 * Retrieves a JSON object containing suggestions for existing nodes.
 */
function nodetoblock_node_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    $result = db_select('node')->fields('node', array('nid', 'title'))->condition('title', db_like($string) . '%', 'LIKE')->range(0, 10)->execute();
    foreach ($result as $node) {
      $matches["{$node->title} ({$node->nid})"] = check_plain($node->title);
    }
  }

  drupal_json_output($matches);
}
