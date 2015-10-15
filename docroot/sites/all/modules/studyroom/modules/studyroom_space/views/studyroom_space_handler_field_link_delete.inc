<?php

/**
 * @file
 * Definition of studyroom_space_handler_field_link_edit.
 */

/**
 * Contains a field handler to display deletes links as fields.
 */
class studyroom_space_handler_field_link_delete extends studyroom_space_handler_field_link {
  /**
   * Render the delete link to the space.
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
