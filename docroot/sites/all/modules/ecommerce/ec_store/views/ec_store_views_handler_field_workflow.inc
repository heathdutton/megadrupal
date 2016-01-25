<?php

/**
 * @file
 * Provide output of workflow status descriptions.
 */

class ec_store_views_handler_field_workflow extends views_handler_field {
  function sort_clickable($order) {
    $this->query->add_orderby('ec_workflow_statuses', 'weight', $order);
  }

  function render($values) {
    return drupal_ucfirst($values->{$this->field_alias});
  }
}

