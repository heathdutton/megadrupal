<?php

namespace Drupal\aws_glacier_ui\Views;

/**
 * Class VaultCreationDate
 * @package Drupal\aws_glacier_ui\Views
 */
class VaultDate extends \views_handler_field_entity{
  /**
   * {@inheritDoc}
   */
  function render($values) {
    if (($entity = $this->get_value($values))) {
      return aws_glacier_date_getter($entity, array(), $this->real_field);
    }
    return '';
  }
  /**
   * {@inheritDoc}
   *
   * Add an addtional check for isset($this->aliases[$field]).
   */
  function get_value($values, $field = NULL) {
    if (isset($this->entities[$this->view->row_index])) {
      $entity = $this->entities[$this->view->row_index];
      // Support to get a certain part of the entity.
      if (isset($field) && isset($entity->{$field})) {
        return $entity->{$field};
      }
      // Support to get a part of the values as the normal get_value.
      elseif (isset($field) && isset($this->aliases[$field]) && isset($values->{$this->aliases[$field]})) {
        return $values->{$this->aliases[$field]};
      }
      else {
        return $entity;
      }
    }
    return FALSE;
  }
}
