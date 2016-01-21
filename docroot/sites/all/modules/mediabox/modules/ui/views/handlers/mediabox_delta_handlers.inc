<?php
/**
 * @file
 * Mediabox view delta handlers which will not produce duplicate results like
 * base views delta handler.
 *
 * @todo - this is not tested for straight relations defined in
 * mediabox_ui_field_views_data().
 */

class mediabox_delta_field_handler extends views_handler_field_numeric {

  function query() {
    // If we already have relationship in the query then reuse it - this will
    // prevent double joining when we have mediabox reverse relationships from
    // mediabox field to base entity.
    if (isset($this->query->relationships[$this->relationship])) {
      $this->table_alias = $this->table;
      $this->field_alias = $this->table;
    }
    else {
      $this->ensure_my_table();
    }

    parent::query();
  }

}

class mediabox_delta_sort_handler extends views_handler_sort {

  function query() {
    if (isset($this->query->relationships[$this->relationship])) {
      $this->table_alias = $this->table;
      $this->field_alias = $this->table;
    }
    else {
      $this->ensure_my_table();
    }
    
    parent::query();
  }

}

class mediabox_delta_filter_handler extends views_handler_filter_numeric {

  function query() {
    if (isset($this->query->relationships[$this->relationship])) {
      $this->table_alias = $this->table;
      $this->field_alias = $this->table;
    }
    else {
      $this->ensure_my_table();
    }

    parent::query();
  }

}

class mediabox_delta_argument_handler extends views_handler_argument_numeric {

  function query($group_by = FALSE) {
    if (isset($this->query->relationships[$this->relationship])) {
      $this->table_alias = $this->table;
      $this->field_alias = $this->table;
    }
    else {
      $this->ensure_my_table();
    }

    parent::query($group_by);
  }

}
