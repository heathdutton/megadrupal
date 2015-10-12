<?php

/**
 * @file
 * Views interface for Monster Menus
 */

/**
 * Field handler to present a link node edit which is MM-aware.
 */
class mm_views_handler_field_node_link_edit extends views_handler_field_node_link {
  function render_link($node, $values) {
    // Ensure user has access to edit this node.
    if (!node_access('update', $node)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $path = "node/$node->nid/edit";
    $mmtids = mm_content_get_by_nid($node->nid);
    if (count($mmtids) == 1) {
      $path = "mm/$mmtids[0]/$path";
    }
    $this->options['alter']['path'] = $path;
    $this->options['alter']['query'] = drupal_get_destination();

    return !empty($this->options['text']) ? $this->options['text'] : t('edit');
  }
}

