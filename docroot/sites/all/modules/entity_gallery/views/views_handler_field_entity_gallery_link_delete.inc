<?php

/**
 * @file
 * Definition of views_handler_field_entity_gallery_link_delete.
 */

/**
 * Field handler to present a link to delete an entity gallery.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_entity_gallery_link_delete extends views_handler_field_entity_gallery_link {

  /**
   * Renders the link.
   */
  function render_link($entity_gallery, $values) {
    // Ensure user has access to delete this entity gallery.
    if (!entity_gallery_access('delete', $entity_gallery)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "gallery/$entity_gallery->egid/delete";
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    return $text;
  }
}
