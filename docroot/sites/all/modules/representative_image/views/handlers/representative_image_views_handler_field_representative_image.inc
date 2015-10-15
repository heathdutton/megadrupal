<?php

/**
 * @file
 * Definition of representative_image_views_handler_field_representative_image.
 */

require_once 'representative_image_views_handler_field.inc';

/**
 * A handler to provide proper displays for the representative image.
 *
 * @ingroup views_field_handlers
 */
class representative_image_views_handler_field_representative_image extends representative_image_views_handler_field {

  function render($values, $entity = NULL, $type = '') {
    $output = '';

    if (empty($this->relationship)) {
      // Get the entity type from the views data.
      $data = views_fetch_data($this->table);
      $type = $data['representative_image']['entity_type'];
      $key = $data['representative_image']['entity_key'];

      // Load the entity and render the field.
      $entities = entity_load($type, array($this->get_value($values, $key)));
      $entity = array_pop($entities);
      $output = is_object($entity) ? parent::render($values, $entity, $type) : '';
    }
    else {
      drupal_set_message(t('Representative image fields do not work when using a relationship. You should remove the %field field from your view.', array('%field' => $this->definition['group'] . ': ' . $this->definition['title'])), 'warning', FALSE);
    }

    return $output;
  }

}
