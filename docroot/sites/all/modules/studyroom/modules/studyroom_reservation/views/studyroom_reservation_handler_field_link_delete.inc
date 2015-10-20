<?php

/**
 * @file
 * Definition of studyroom_reservation_handler_field_link_delete.
 */

/**
 * Contains a field handler to display deletes links as fields.
 */
class studyroom_reservation_handler_field_link_delete extends studyroom_reservation_handler_field_link {
  /**
   * Render a delete link to the reservation.
   */
  public function render_link($entity, $values) {
    if (entity_access('delete', $this->entity_type, $entity)) {
      $this->options['alter']['make_link'] = TRUE;
      $uri = entity_uri($this->entity_type, $entity);
      $this->options['alter']['path'] = $uri['path'] . '/delete';
      $this->options['alter']['options'] = $uri['options'];
      $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
      return $text;
    }
  }
}
