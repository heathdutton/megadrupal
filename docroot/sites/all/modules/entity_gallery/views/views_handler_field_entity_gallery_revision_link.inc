<?php

/**
 * @file
 * Definition of views_handler_field_entity_gallery_revision_link.
 */

/**
 * Field handler to present a link to an entity gallery revision.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_entity_gallery_revision_link extends views_handler_field_entity_gallery_link {

  function construct() {
    parent::construct();
    $this->additional_fields['entity_gallery_vid'] = array('table' => 'entity_gallery_revision', 'field' => 'vid');
  }

  function access() {
    return user_access('view entity gallery revisions') || user_access('administer entity galleries');
  }

  function render_link($data, $values) {
    list($entity_gallery, $vid) = $this->get_revision_entity($values, 'view');
    if (!isset($vid)) {
      return;
    }

    // Current revision uses the entity gallery view path.
    $path = 'gallery/' . $entity_gallery->egid;
    if ($entity_gallery->vid != $vid) {
      $path .= "/revisions/$vid/view";
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = $path;
    $this->options['alter']['query'] = drupal_get_destination();

    return !empty($this->options['text']) ? $this->options['text'] : t('view');
  }

  /**
   * Returns the revision values of an entity gallery.
   *
   * @param object $values
   *   An object containing all retrieved values.
   * @param string $op
   *   The operation being performed.
   *
   * @return array
   *   A numerically indexed array containing the current entity gallery object
   *   and the revision ID for this row.
   */
  function get_revision_entity($values, $op) {
    $vid = $this->get_value($values, 'entity_gallery_vid');
    $entity_gallery = $this->get_value($values);
    // Unpublished entity galleries ignore access control.
    $entity_gallery->status = 1;
    // Ensure user has access to perform the operation on this entity gallery.
    if (!entity_gallery_access($op, $entity_gallery)) {
      return array($entity_gallery, NULL);
    }
    return array($entity_gallery, $vid);
  }
}
