<?php
/**
 * @file
 * Here you can find all the sort handlers 
 */

/**
 * Base sort handler that performs a simple sort.
 * Extends views_handler_sort
 */
class odata_handler_sort extends views_handler_sort {
  /**
   * Called to add the sort to a query.
   */
  function query() {
    $thisrealfield = _odata_toreal($this->real_field);

    if ( !empty( $this->view->exposed_data['sort_by'] ) ) {
      $field = $this->view->exposed_data['sort_by'];
      $order = $this->view->exposed_data['sort_order'] == 'DESC' ? 'desc' : 'asc';
    }
    else {
      $field = $thisrealfield;
      $order = $this->options['order'] == 'DESC' ? 'desc' : 'asc';
    }

    $this->query->add_orderby($field, $order);
  }
}

/**
 *  Performs sotring by date
 *  Extends views_handler_sort_date
 */
class odata_handler_sort_date extends views_handler_sort_date {
  /**
   * Called to add the sort to a query.
   */
  function query() {
    $thisrealfield = _odata_toreal($this->real_field);

    if ( !empty( $this->view->exposed_data['sort_by'] ) ) {
      $ordering_by = $this->view->exposed_data['sort_by'];
      $order = $this->view->exposed_data['sort_order'] == 'DESC' ? 'desc' : 'asc';
    }
    else {
      $ordering_by = $thisrealfield;
      $order = $this->options['order'] == 'DESC' ? 'desc' : 'asc';
    }

    if ( !empty($this->options['granularity']) ) {
      $ordering_by = $this->options['granularity'] . '(' . $ordering_by . ')';
    }

    $this->query->add_orderby( $ordering_by, $order);
  }
}
