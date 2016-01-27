<?php

class cloudwords_handler_relationship extends views_handler_relationship {

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    $key = $this->definition['filter key'];

    if ($this->view->exposed_input &&
      !empty($this->view->exposed_input[$key]) &&
      $this->view->exposed_input[$key] == $this->definition['filter value']) {
      return parent::query();
    }
    unset($this->view->filter[$this->definition['filter remove']]);
  }

}
