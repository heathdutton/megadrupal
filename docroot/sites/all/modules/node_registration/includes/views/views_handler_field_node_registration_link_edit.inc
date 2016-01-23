<?php

/**
 * @file
 * 
 */

class views_handler_field_node_registration_link_edit extends views_handler_field_node_registration_link {

  /**
   * Renders the link.
   */
  function render_link($registration, $values) {
    // Ensure user has access to edit this registration.
    if (!node_registration_access($registration, 'edit')) {
      return;
    }

    $uri = node_registration_uri($registration);
    $path = $uri['path'] . '/edit';

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = $path;
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    return $text;
  }
}
