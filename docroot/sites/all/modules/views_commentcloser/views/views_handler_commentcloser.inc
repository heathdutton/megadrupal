<?php
/**
 * Field handler to present a link to close or open commenting on a node.
 */
class views_handler_commentcloser extends views_handler_field_node_link {

  /**
   * Renders the link.
   */
  function render_link($node, $values) {
    // Ensure user has access to delete this node.
    if (!user_access('administer comments')) {
      return;
    }
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['query'] = drupal_get_destination();
    if ($node->comment == COMMENT_NODE_CLOSED) {
      // Commenting is disabled.
      $this->options['alter']['path'] = "node/$node->nid/comment/open";
      $text = !empty($this->options['text']) ? $this->options['text'] : t('open commenting');
    }
    else {
      // Commenting is open.
      $this->options['alter']['path'] = "node/$node->nid/comment/close";
      $text = !empty($this->options['text']) ? $this->options['text'] : t('close commenting');
    }
    return $text;
  }
}