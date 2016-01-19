<?php

/**
 * @file
 * PinIt Hooks.
 */

/**
 * Implements hook_permission().
 */
function pinit_permission() {
  return array(
    PINIT_PERM_ADMIN => array(
      'title' => t('Administer Pin It'),
      'description' => t('Administer default Pin It options'),
    ),
    PINIT_PERM_EDIT => array(
      'title' => t('Edit Pin It'),
      'description' => t('Edit Pin it values on a node'),
    ),
  );
}

/**
 * Implements hook_node_load().
 */
function pinit_node_load($nodes, $types) {
  foreach ($nodes as &$node) {
    if (PinIt::instance()->pinsAreEnabledForContentType($node->type)) {
      $node->pinit = PinIt::instance()->loadNodeData($node);
    }
  }
}

/**
 * Implements hook_node_view().
 */
function pinit_node_view($node, $view_mode, $langcode) {
  // Only show pin button if viewing full node as a page.
  if ('full' == $view_mode && !empty($node->pinit)) {
    PinIt::instance()->renderData($node, $node->pinit);
  }
}

/**
 * Implements hook_node_insert().
 */
function pinit_node_insert($node) {
  pinit_node_update($node);
}

/**
 * Implements hook_node_update().
 */
function pinit_node_update($node) {
  if (!empty($node->pinit)) {
    PinIt::instance()->saveNodeData($node->nid, $node->pinit);
  }
}

/**
 * Implements hook_node_delete().
 */
function pinit_node_delete($node) {
  PinIt::instance()->deleteNodedata($node->nid);
}
