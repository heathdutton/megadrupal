<?php
/**
 * @file
 *
 * A Views query plugin for executing queries against a QuickBase datastore
 */

class quickbase_query_views_plugin_query extends views_plugin_query {
  /**
   * Properties
   */
  var $qb = NULL;
  var $where = array();
  var $orderby = array();
  var $group_operator = NULL;

  /**
   * Constructor; Create the basic query object and fill with default values.
   */
  function init($base_table, $base_field, $options) {
    parent::init($base_table, $base_field, $options);

    // Initialize the QuickBase object we'll use for queries
    $this->qb = quickbase();
  }

  /**
   * Get aggregation info for group by queries.
   *
   * If NULL, aggregation is not allowed.
   */
  function get_aggregation_info() {
    return NULL;
  }

  /**
   * Generate a query and a countquery from all of the information supplied
   * to the object.
   *
   * @param $get_count
   *   Provide a countquery if this is true, otherwise provide a normal query.
   */
  function query($get_count = FALSE) {
    // Construct QuickBase query from Views filter grouping
    $groups = array();
    foreach ($this->where as $where) {
      $queries = array();
      foreach ($where['conditions'] as $cond) {
        // Multiple values for condition, suss out
        if (is_array($cond['value']) && !is_string($cond['value'])) {
          $in_queries = array();
          foreach ($cond['value'] as $in_val) {
            $in_queries[] = '{' . $cond['field'] . ".{$cond['operator']}." . '"' . $in_val . '"}';
          }
          if (!empty($in_queries)) {
            $op = ('X' == $cond['operator'][0]) ? 'AND' : 'OR';
            $queries[] = '(' . implode($op, $in_queries) . ')';
          }
        }
        // Otherwise simple field-value comparison
        else {
          $queries[] = '{' . $cond['field'] . ".{$cond['operator']}." . '"' . $cond['value'] . '"}';
        }
      }
      if (!empty($queries)) {
        $groups[] = '(' . implode($where['type'], $queries) . ')';
      }
    }
    $query['query'] = implode($this->group_operator, $groups);

    // Store off requested fields
    $query['clist'] = $this->fields;

    $query['options'] = array();
    $options = array();

    // If this is a full query build vs a counter query, add on options
    if (!$get_count) {
      // Suss out offset-limit options
      $options[] = 'num-' . $this->limit;
      $options[] = 'skp-' . $this->offset;

      // Suss out sort fields
      $query['slist'] = array();
      if (!empty($this->orderby)) {
        foreach ($this->orderby as $orderby) {
          $query['slist'][] = $orderby['field'];
          $options[] = 'sortorder-' . (('ASC' == $orderby['direction']) ? 'A' : 'D');
        }
      }
      $query['options']['options'] = implode('.', $options);
    }

    return $query;
  }

  /**
   * Let modules modify the query just prior to finalizing it.
   */
  function alter(&$view) {
    foreach (module_implements('views_query_alter') as $module) {
      $function = $module . '_views_query_alter';
      $function($view, $this);
    }
  }

  /**
   * Builds the necessary info to execute the query.
   *
   * @param view $view
   *   The view which is executed.
   */
  function build(&$view) {
    // Store the view in the object to be able to use it later.
    $this->view = $view;

    $view->init_pager();

    // Let the pager modify the query to add limits.
    $this->pager->query();

    $view->build_info['query'] = $this->query();
    $view->build_info['count_query'] = $this->query(TRUE);
  }

  /**
   * Executes the query and fills the associated view object with according
   * values.
   *
   * Values to set: $view->result, $view->total_rows, $view->execute_time,
   * $view->pager['current_page'].
   *
   * $view->result should contain an array of objects. The array must use a
   * numeric index starting at 0.
   *
   * @param view $view
   *   The view which is executed.
   */
  function execute(&$view) {
    $query = $view->build_info['query'];
    $count_query = $view->build_info['count_query'];
    //dpm($this);

    $start = microtime(TRUE);
    $result = array();
    $table = $this->base_table;

    // Let the pager modify the query to add limits.
    $this->pager->pre_execute($query);

    // Execute main result set query
    if ($this->qb) {
      $resp = $this->qb->DoQuery($table, array($query['query']), $query['clist'], $query['slist'], $query['options']);
      if ($resp && !empty($resp->table->records)) {
        $fields = $resp->table->fields->field;
        foreach ($resp->table->records->record as $record) {
          $row = new stdClass();
          foreach ($record->f as $f) {
            if (isset($query['clist'][(string) $f->attributes()->id])) {
              $row->{$query['clist'][(string) $f->attributes()->id]} = (string) $f[0];
            }
          }
          $result[] = $row;
        }
      }
    }

    // Store off values from query in View
    $view->result = $result;
    $view->total_rows = count($result);
    $this->pager->post_execute($view->result);

    // Execute count query for pager if necessary
    if ($this->pager->use_count_query()) {
      $resp = $this->qb->DoQueryCount($table, array($query['query']));
      if ($resp) {
        $this->pager->total_items = (int) $resp->numMatches;
        $view->total_rows = $this->pager->get_total_items();
        $this->pager->update_page_info();
      }
    }

    // Wrap up query
    $view->execute_time = microtime(TRUE) - $start;
    _quickbase_query_dbg($view);
  }

  /***************************************************************************
   * REQUIRED methods for a Views query plugin leveraging base handlers
   **************************************************************************/

  /**
   * Add field to the query
   */
  function add_field($table, $field, $required = FALSE) {
    $this->fields[$field] = $field;
    return $field;
  }

  /**
   * Copied from views_plugin_query_default, called by filter handlers
   */
  function add_where($group, $field, $value = NULL, $operator = NULL) {
    // Ensure all variants of 0 are actually 0. Thus '', 0 and NULL are all
    // the default group.
    if (empty($group)) {
      $group = 0;
    }

    // Check for a group.
    if (!isset($this->where[$group])) {
      $this->set_where_group('AND', $group);
    }

    $this->where[$group]['conditions'][] = array(
      'field' => array_pop(explode('.', $field)),
      'value' => $value,
      'operator' => $operator,
    );
  }

  /**
   * Copied from views_plugin_query_default, modified for our simplified case, called by sort handlers
   */
  function add_orderby($table, $field, $order = 'ASC', $alias = '', $params = array()) {
    // Only fill out this aliasing if there is a table;
    // otherwise we assume it is a formula.
    if (!$alias && $table) {
      $as = $field;
    }
    else {
      $as = $alias;
    }

    if ($field) {
      $as = $this->add_field($table, $field, $as, $params);
    }

    $this->orderby[] = array(
      'field' => $as,
      'direction' => strtoupper($order)
    );
  }

  /**
   * Remove all fields that may have been added; primarily used for summary
   * mode where we're changing the query because we didn't get data we needed.
   */
  function clear_fields() {
    $this->fields = array();
  }

  /**
   * Dummy placeholder method to satisfy Views core handlers
   */
  function ensure_table($table) {
    return $table;
  }

}
