<?php

/**
 * @file
 * Views handler override for field that exposes views query data.
 */


/**
 * @inheritdoc
 */
class architecture_query_views_handler extends views_handler_field {

  /**
  * Look up the query from the view object and display ID.
  */
  function render($values) {
    $value = ($this->get_value($values));
    if (is_object($value['view'])) {
      $view = $value['view'];
      $display_id = $value['display_id'] ? $value['display_id']: NULL;
      $view->set_display($display_id);
      $view->pre_execute();
      $view->execute();
      if (get_class($view->query->query()) == 'SelectQuery') {
        return($view->query->query()->__toString());
      }
    }
    return 'No View object given in field handler.';
  }
}
