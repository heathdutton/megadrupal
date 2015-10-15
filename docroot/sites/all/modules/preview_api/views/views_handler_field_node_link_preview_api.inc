<?php
/**
 * @file
 * Definition of views_handler_field_node_link_preview_api.
 */

/**
 * Field handler to present a link to preview a node.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_node_link_preview_api extends views_handler_field_node_link {
  /**
   * Renders the link.
   */
  function render_link($node, $values) {
    $this->options['alter']['make_link'] = TRUE;
    $url = preview_api_url(array('nid' => $node->nid, 'vid' => $node->vid));
    $this->options['alter']['path'] = $url['path'];
    $this->options['alter']['query'] = $url['options']['query'];
    $text = !empty($this->options['text']) ? $this->options['text'] : t('preview');
    return $text;
  }
}
