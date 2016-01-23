<?php
/**
 * @file
 * Plug in for the View
 */
class odata_plugin_query_odata extends views_plugin_query {

  /**
   * A pager plugin that should be provided by the display.
   */
  var $pager = NULL;

  /**
   * The object used for the query.
   */
  var $query;

  /**
   * Generate a query and a countquery from all of the information supplied
   * to the object.
   *
   * @param $get_count
   *   Provide a countquery if this is true, otherwise provide a normal query.
   */
  function query($view, $get_count = FALSE) {

    //Group Preprocessing
    $view_display = $view->display_handler->display->id;
    $operations_group = array();
    if (isset( $view->display[$view_display]->display_options['filter_groups'] )) {
      $operations_group = $view->display[$view_display]->display_options['filter_groups'];
    }
    elseif (isset( $view->display['default']->display_options['filter_groups'] )) {
      $operations_group = $view->display['default']->display_options['filter_groups'];
    }

    $query = array();

    // Add fields that the user has added to the Fields section in Views UI.
    if (isset( $this->select_fields ))
    foreach ($this->select_fields as $key => $field) {
      // Do not add variable to SELECT if it will be replaced with a value.
      if (!isset($this->replacements[$field])) {
        // Add a question mark before each variable.
        $query[] = "$field";
      }
    }

    $fields = implode(',', $query);

    $query ='';
    if (count($fields) > 0) {
      $query .= '$select=';
      $query .= $fields;
    }

    if (!empty($this->orderby)) {
      $orderby = '&$orderby=';
      $comma=FALSE;
      foreach ($this->orderby as $field => $order) {
      if ( $comma ) $orderby .= ',';
        $orderby .= $field . '+' . $order;
        $comma=TRUE;
      }
    }
    if (!empty($this->orderby)) {
      $query .= $orderby;
    }

    //Adds filters in Open Data query
    if (!empty($this->filters) || !empty($this->context_filters) ) {//To DO
    $query .='&$filter=';
    if (!empty($this->filters) ) {
        $query .= '(';
        $generaloperator;
        if ( empty($operations_group['operator']) || $operations_group['operator']=='AND') {
          $generaloperator='+and+';
        }
        else { //OR
          $generaloperator='+or+';
        }
      $operator_needed=FALSE;
        foreach ($this->filters as $group => $value) {
          if (!empty($value)) {
            if ( empty($operations_group['groups']) || empty($operations_group['groups'][$group]) || $operations_group['groups'][$group] == 'AND' ) {
              $operator = '+and+';
              if ($operator_needed) $query .= $generaloperator;
              $query .= '(' ;
            }
            else {
              $operator = '+or+';
        if ($operator_needed) $query .= $generaloperator;
              $query .= '(' ;
            }//if
            $query .= implode($operator, $value);
            $query .= ')';
          }
      $operator_needed=TRUE;
        }
        $query .= ')';
    if (!empty($this->context_filters))$query .= '+and+';
      }
    //*
    if (!empty($this->context_filters)) {
        $query .= '(';
    $query .= implode('+and+', $this->context_filters);
    $query .= ')';
      }
    //*/
    }

    $page = $this->pager->current_page;
    $skip = (($this->pager->use_pager()) ? $page * $this->pager->options['items_per_page'] : 0) + $this->pager->options['offset'];

    if ( !$get_count && isset($this->limit) ) {
      $query .= '&$top=' . $this->limit;
    }

    if (!$get_count && $this->pager->use_pager() ) {
      $query .= '&$skip=' . $skip;
    }
    elseif ( $get_count ) {
      $query .= '&$skip=' . $this->pager->options['offset']; ;
    }

    return $query;
  }

  var $odata_query_without_top;

  /**
   * Let modules modify the query just prior to finalizing it.
   */
  function alter(&$view) {
  }

  /**
   * Set a LIMIT on the query, specifying a maximum number of results.
   */
  function set_limit($limit) {
    $this->limit = ($limit) ? $limit : 10000000;
  }

  function use_pager() {
    return FALSE;
  }

  /**
   * Set an OFFSET on the query, specifying a number of results to skip
   */
  function set_offset($offset) {
    $this->offset = $offset;
  }

  function add_orderby($table, $field, $order = '', $alias = '', $params = array()) {
    if ($table) {
      $this->orderby[$table] = $field;
    }
    else {
      $this->orderby[$alias] = $order;
    }
  }

  /**
   *  The array that contains all of the filters
   */
  var $filters=array();
  /**
   *  Adds a filter
   */
  function add_filter($operand1, $operation, $operand2, $group=0) {
    if (!isset($this->filters[$group])) $this->filters[$group] = array();
    $this->filters[$group][] =   $operand1 . '+' . $operation . '+' . $operand2 ;
  }
  
  var $context_filters = array();

  function add_ready_contextual_filter($open_string) {
    $this->context_filters[] = $open_string;
  }

  /**
   *  Adds a filter by giving a string as a parameter
   */
  function add_ready_filter($open_string, $group=0) {
    if (!isset($this->filters[$group])) $this->filters[$group]=array();
    $this->filters[$group][] =   $open_string ;
  }

  /**
   *  Adds a filter that referes to between
   */
  function add_between_filter($operand1, $valuesmin, $valuesmax , $not=FALSE, $group=0) {
    if (!isset($this->filters[$group]))$this->filters[$group]=array();
    if ($not) {
      $this->filters[$group][] = '(' . $operand1 . '+le+' . $valuesmin . '+or+' . $operand1 . '+ge+' . $valuesmax . ')';
    }
    else {
      $this->filters[$group][] = '(' . $operand1 . '+ge+' . $valuesmin . '+and+' . $operand1 . '+le+' . $valuesmax . ')';
    }
  }

  /**
   * Builds the necessary info to execute the query.
   */
  function build(&$view) {
    $view->init_pager($view);

    // Let the pager modify the query to add limits.
    $this->pager->query();

    $view->build_info['query'] = $this->query($view);
    $view->build_info['count_query'] = $this->query($view, TRUE);
  }

  /**
   * Executes the query and fills the associated view object with according
   * values.
   *
   * Values to set: $view->result, $view->total_rows, $view->execute_time,
   * $view->pager['current_page'].
   */
  function execute(&$view) {
    $query = $view->build_info['query'];

    $views_data = odata_views_data();
    $start = microtime(TRUE);

    $reply = odata_request($views_data[$view->base_table]['table']['endpoint'] . '/?' . $query);

    if (!empty($reply['error'])) {
      $view->result = array();
      vsm(t('Error ' . $reply['error'] . ': ' . $reply['details'] ));
      return;
    }

    $views_data[$view->base_table]['table']['endpoint'];
    foreach ($reply as $key => $values) {
      if (isset($values['__metadata']))unset($values['__metadata']);
      $rows[$key] = $values;
    }

    if (empty($rows)) {
      $view->result = array();
      vsm(t('No result returned. Please check your query and the endpoint status.'));
      return;
    }

    foreach ($rows as $index => $row) {
      $views_row = array();
      foreach ($row as $name => $value) {
        if (isset($value)) {
          $views_row[$name] = $value;
        }
      }
      $views_result[] = (object) $views_row;
    }
    // Execute count query for pager.
    $count = drupal_http_request($views_data[$view->base_table]['table']['endpoint'] . '/$count?' . $view->build_info['count_query'] );
    if ($this->pager->use_count_query() || !empty($view->get_total_rows)) {
      $this->pager->total_items = $count->data;

      if (!empty($this->pager->options['offset'])) {
        $this->pager->total_items -= $this->pager->options['offset'];
      }
      $this->pager->update_page_info();
    }

    if ($this->pager->use_pager()) {
      $view->total_rows = $this->pager->get_total_items();
    }
    else {
      $view->total_rows = $count->data;
    }

    $view->result = $views_result;

    $view->execute_time = microtime(TRUE) - $start;
  }
}

