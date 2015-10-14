<?php
/**
 * @file
 * Contains trickery that was not needed. Kept for educational purposes.
 * This file is NOT used and NOT imported!
 */

/**
 * Implements hook_node_view().
 */
function flag_hide_node_view($node, $view_mode, $langcode) {
  if (!empty($node->dont_display)) {
    // Remove "Read more" and Flag (bookmark) links.
    $node->content = array();
  }
}

/**
 * Implements hook_entity_prepare_view().
 *
 * Used to remove content from the node object.
 */
function flag_hide_entity_prepare_view($entities, $entity_type, $langcode) {
  foreach ($entities as $entity) {
    if (!empty($entity->dont_display)) {
      $entity->title = FALSE;
      $entity->comment = 0;
      $entity->link = FALSE;
      unset($entity->body);
      unset($entity->rss_elements);
    }
  }
}

/**
 * Implements hook_field_attach_view_alter().
 */
function flag_hide_field_attach_view_alter(&$output, $context) {
  if (!empty($context['entity']->dont_display)) {
    $output = array();
  }
}

/**
 * Implements hook_contextual_links_view_alter().
 *
 * A way to suppress contextual links.
 */
function flag_hide_contextual_links_view_alter(&$element, &$items) {
  if (!empty($element['#contextual_links']['node'][1])) {
    $nid = reset($element['#contextual_links']['node'][1]);
  }
  if (!empty($nid) && flag_hide_is_hidden('node', $nid)) {
    // Remove the links
    $element = array();
  }
}
