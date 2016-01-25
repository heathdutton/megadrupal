<?php

/**
 * @file
 * 
 */

class views_handler_field_node_registration_link_delete extends views_handler_field_node_registration_link {

  /**
   * Renders the link.
   */
  function render_link($registration, $values) {
    // Ensure user has access to delete this registration.
    if (!node_registration_access($registration, 'delete')) {
      return;
    }

    $uri = node_registration_uri($registration);
    $path = $uri['path'] . '/delete';

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = $path;
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    return $text;
  }
}
