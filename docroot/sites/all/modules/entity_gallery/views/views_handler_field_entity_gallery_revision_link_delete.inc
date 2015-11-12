<?php

/**
 * @file
 * Definition of views_handler_field_entity_gallery_revision_link_delete.
 */

/**
 * Field handler to present link to delete an entity gallery revision.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_entity_gallery_revision_link_delete extends views_handler_field_entity_gallery_revision_link {

  function access() {
    return user_access('delete entity gallery revisions') || user_access('administer entity galleries');
  }

  function render_link($data, $values) {
    list($entity_gallery, $vid) = $this->get_revision_entity($values, 'delete');
    if (!isset($vid)) {
      return;
    }

    // Current revision cannot be deleted.
    if ($entity_gallery->vid == $vid) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = 'gallery/' . $entity_gallery->egid . "/revisions/$vid/delete";
    $this->options['alter']['query'] = drupal_get_destination();

    return !empty($this->options['text']) ? $this->options['text'] : t('delete');
  }
}
