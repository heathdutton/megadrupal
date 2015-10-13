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
class representative_image_views_handler_field_revision_representative_image extends representative_image_views_handler_field {

  function render($values, $entity = NULL, $type = '') {
    $node = node_load(NULL, $values->vid);
    $output = parent::render($values, $node, 'node');
    return $output;
  }

}
