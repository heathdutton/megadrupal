<?php

/**
 * @file
 * Definition of views_handler_field_entity_gallery_revision_link_revert.
 */

/**
 * Field handler to present a link to revert an entity gallery to a revision.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_entity_gallery_revision_link_revert extends views_handler_field_entity_gallery_revision_link {

  function access() {
    return user_access('revert entity gallery revisions') || user_access('administer entity galleries');
  }

  function render_link($data, $values) {
    list($entity_gallery, $vid) = $this->get_revision_entity($values, 'update');
    if (!isset($vid)) {
      return;
    }

    // Current revision cannot be reverted.
    if ($entity_gallery->vid == $vid) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = 'gallery/' . $entity_gallery->egid . "/revisions/$vid/revert";
    $this->options['alter']['query'] = drupal_get_destination();

    return !empty($this->options['text']) ? $this->options['text'] : t('revert');
  }
}
