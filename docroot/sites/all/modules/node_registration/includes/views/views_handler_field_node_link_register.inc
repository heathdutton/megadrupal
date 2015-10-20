<?php

/**
 * @file
 * 
 */

class views_handler_field_node_link_register extends views_handler_field_node_link {

  /**
   * Renders the link.
   */
  function render_link($node, $values) {
    // Ensure user has access to register for this event.
    $registration = entity_get_controller('node_registration')->create(array(
      'nid' => $node->nid,
    ));
    if (!node_registration_access($registration, 'add')) {
      return;
    }

    $uri = node_uri($node);
    $path = $uri['path'] . '/register';

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = $path;

    $text = !empty($this->options['text']) ? $this->options['text'] : t('register');
    return $text;
  }

}
