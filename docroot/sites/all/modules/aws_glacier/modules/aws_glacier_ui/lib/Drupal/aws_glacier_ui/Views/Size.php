<?php

namespace Drupal\aws_glacier_ui\Views;

/**
 * Class Size
 * @package Drupal\aws_glacier_ui\Views
 */
class Size extends \views_handler_field_entity {

  /**
   * {@inheritDoc}
   */
  function render($values) {
    if (($entity = $this->get_value($values))) {
      return aws_glacier_sizes_getter($entity, array(), $this->real_field);
    }
    return '';
  }
}
