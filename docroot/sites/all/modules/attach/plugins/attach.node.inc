<?php
/**
 * Renders attached node in teaser mode.
 *
 * This module is shipped with a minimal node template.
 */
function _attach_node_process($params) {
  if (($node = node_load((int) $params['nid'])) && ($node->status)) {
    $params['default_title'] = empty($params['title']) ? $node->title : $params['title'];

    if ($params['simple']) {
      if (variable_get('attach_utm_source', FALSE)) {
        $options = array('query' => array('utm_source' => 'attach'));
      }
      else {
        $options = array();
      }
      return '<span class="attach-node-' . $node->type . '">' . l($params['default_title'], 'node/' . $node->nid, $options) . '</span>';
    }
    else {
      $node->attach = $params;
      $content = node_view($node, 'attach');
      return render($content);
    }
  }
}

/**
 * Renders node in AJAX mode.
 *
 * Useful when there is non-cacheable content in attached node, like a form
 * (poll nodes).
 */
function attach_node_ajax_render($node) {
  print render(node_view($node, 'attach_ajax'));
}
