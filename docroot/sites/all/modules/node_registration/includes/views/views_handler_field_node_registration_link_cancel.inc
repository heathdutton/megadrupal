<?php

/**
 * @file
 * 
 */

class views_handler_field_node_registration_link_cancel extends views_handler_field_node_registration_link {

  /**
   * Renders the link.
   */
  function render_link($registration, $values) {
    // Ensure user has access to cancel this registration.
    if (!node_registration_access($registration, 'cancel')) {
      return;
    }

    $uri = node_registration_uri($registration);
    $path = $uri['path'] . '/cancel';

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = $path;
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('cancel');
    return $text;
  }
}
