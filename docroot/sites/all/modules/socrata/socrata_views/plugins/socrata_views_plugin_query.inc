<?php
/**
 * @file
 * A Views query plugin for executing queries against a Socrata dataset
 */

class socrata_views_plugin_query extends views_plugin_query {
  // Properties
  var $where = array();
  var $orderby = array();
  var $group_operator = 'AND';
  var $groupby = array();
  var $has_aggregate = FALSE;

  /**
   * Constructor; Create the basic query object and fill with default values.
   */
  function init($base_table, $base_field, $options) {
    parent::init($base_table, $base_field, $options);
  }

  /**
   * Get aggregation info for group by queries.
   */
  function get_aggregation_info() {
    return array(
      'group' => array(
        'title' => t('Group results together'),
        'is aggregate' => FALSE,
      ),
      'count' => array(
        'title' => t('Count'),
        'method' => 'socrata_query_default_aggregation_method_simple',
        'handler' => array(
          'argument' => 'views_handler_argument_group_by_numeric',
          'field' => 'views_handler_field_numeric',
          'filter' => 'views_handler_filter_group_by_numeric',
          'sort' => 'views_handler_sort_group_by_numeric',
        ),
      ),
      'sum' => array(
        'title' => t('Sum'),
        'method' => 'socrata_query_default_aggregation_method_simple',
        'handler' => array(
          'argument' => 'views_handler_argument_group_by_numeric',
          'filter' => 'views_handler_filter_group_by_numeric',
          'sort' => 'views_handler_sort_group_by_numeric',
        ),
      ),
      'avg' => array(
        'title' => t('Average'),
        'method' => 'socrata_query_default_aggregation_method_simple',
        'handler' => array(
          'argument' => 'views_handler_argument_group_by_numeric',
          'filter' => 'views_handler_filter_group_by_numeric',
          'sort' => 'views_handler_sort_group_by_numeric',
        ),
      ),
      'min' => array(
        'title' => t('Minimum'),
        'method' => 'socrata_query_default_aggregation_method_simple',
        'handler' => array(
          'argument' => 'views_handler_argument_group_by_numeric',
          'filter' => 'views_handler_filter_group_by_numeric',
          'sort' => 'views_handler_sort_group_by_numeric',
        ),
      ),
      'max' => array(
        'title' => t('Maximum'),
        'method' => 'socrata_query_default_aggregation_method_simple',
        'handler' => array(
          'argument' => 'views_handler_argument_group_by_numeric',
          'filter' => 'views_handler_filter_group_by_numeric',
          'sort' => 'views_handler_sort_group_by_numeric',
        ),
      ),
    );
  }

  /**
   * Generate a query and a countquery from all of the information supplied
   * to the object.
   *
   * @param bool $get_count
   *   Provide a countquery if this is true, otherwise provide a normal query.
   */
  function query($get_count = FALSE) {
    // @todo: for SoQL clauses ( http://dev.socrata.com/docs/queries.html ),
    // provide some sort of translation function on SocrataSelectQuery to
    // negotiate SQL vs SoQL output, and where appropriate below call that
    // method instead of both calling the SelectQuery method and appending
    // values to $query->params.

    // Build the query.
    $query = db_select($this->base_table)->extend('SocrataSelectQuery');
    $query->addTag('socrata');
    $query->addTag('socrata_' . $this->view->name);

    // Construct where clause from Views filter grouping.
    $groups = array();
    foreach ($this->where as $where) {
      $queries = array();
      foreach ($where['conditions'] as $cond) {
        // Multiple values for condition, suss out.
        if (is_array($cond['value']) && !is_string($cond['value']) && !empty($cond['value'])) {
          $in_queries = array();
          foreach ($cond['value'] as $in_val) {
            $in_queries[] = $this->_construct_query_comp($cond['field'], $in_val, $cond['operator']);
          }
          if (!empty($in_queries)) {
            $queries[] = '(' . implode(' AND ', $in_queries) . ')';
          }
        }
        // Otherwise simple field-value comparison.
        else {
          $queries[] = $this->_construct_query_comp($cond['field'], $cond['value'], $cond['operator']);
        }
      }
      if (!empty($queries)) {
        $groups[] = '(' . implode(" {$where['type']} ", $queries) . ')';
        $query->where(implode(" {$this->group_operator} ", $groups));
      }
    }
    $query->params['$where'] = implode(" {$this->group_operator} ", $groups);

    // Store off requested fields.
    $this->has_aggregate = $this->view->display_handler->get_option('group_by');

    if (!empty($this->fields)) {
      $fields_list = $non_aggregates = array();
      foreach ($this->fields as $field => $field_info) {
        // If an aggregate function is specified, wrap it around the field.
        if (isset($field_info['function'])) {
          $fields_list[] = $field_info['function'] . '(' . $field . ')';
        }
        else {
          $fields_list[] = $field;
          $non_aggregates[] = $field;
        }
      }
    }
    if ($this->has_aggregate && (!empty($this->groupby) || !empty($non_aggregates))) {
      $groupby = array_unique(array_merge($this->groupby, $non_aggregates));
      $query->params['$group'] = implode(',', $groupby);
    }
    else {
      $fields_list = $non_aggregates;
      $query->params['$group'] = NULL;
    }
    $query->params['$select'] = implode(',', $fields_list);
    $query->fields($this->base_table, $fields_list);

    // If this is a full query build vs a counter query, add on options.
    if (!$get_count) {
      // Suss out offset-limit options.
      if (!empty($this->limit)) {
        $query->params['$limit'] = $this->limit;
        $query->range(0, $this->limit);
      }
      if (!empty($this->offset)) {
        $query->params['$offset'] = $this->offset;
        $query->range($this->offset, $this->limit);
      }

      // Suss out sort fields.
      if (!empty($this->orderby)) {
        $sort_fields = array();
        foreach ($this->orderby as $orderby) {
          $sort_fields[] = "{$orderby['field']} {$orderby['direction']}";
          $query->orderBy($orderby['field'], $orderby['direction']);
        }
        $query->params['$order'] = implode(',', $sort_fields);
      }
    }
    else {
      if (!$this->has_aggregate) {
        $query->params['$select'] = 'count(*)';
        $query->addExpression('count(*)');
      }
    }

    return $query;
  }

  /**
   * Util method for constructing components for predicate.
   */
  function _construct_query_comp($field, $value, $op) {
    $comp = '';

    // Check to see if the predicate component is a prebuilt formula.
    if ('formula' == $op) {
      $comp = $field;
    }

    // Otherwise, build a "normal" comparison predicate component.
    else {
      $comp = "{$field} {$op} {$value}";
    }

    return $comp;
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
    _socrata_dbg($view->build_info);
    $query = $view->build_info['query'];
    $count_query = $view->build_info['count_query'];

    $start = microtime(TRUE);
    $result = array();
    $table = $this->base_table;

    // Load source info.
    $source = $query->getSource();
    if ($source) {
      // Get total count of items and force initial limit if not set.
      $num_dataset_rows = 0;
      $resp = $count_query->execute();
      if ($resp !== FALSE && !empty($resp['data']) && isset($resp['data'][0]['count'])) {
        $num_dataset_rows = $resp['data'][0]['count'];
      }

      // Let the pager modify the query to add limits.
      $this->pager->pre_execute($query);

      // Execute main query, looping if we need to get more than 1000 rows.
      do {
        $resp = $query->execute();
        if ($resp !== FALSE) {
          // Have to map Scorata result field labels back onto what Views knows them as.
          $original_field_names = $view->query->fields;
          $field_name_map = array();
          foreach ($original_field_names as $field => $attributes) {
            if ($attributes['alias']) {
              $field_alias = $attributes['alias'];
            }
            else {
              $field_alias = $field;
            }
            $field_name_map[$field_alias] = $field;
          }

          foreach ($resp['data'] as $row) {
            $new_row = new stdClass();
            // Map the results with the correct field alias in field order.
            foreach ($field_name_map as $alias => $field) {
              // If Socrata returns a row with empty data, put in empty string.
              if (isset($row[$alias])) {
                $value = $row[$alias];
              }
              else {
                $value = '';
              }
              $new_row->{$field} = $value;
            }
            $result[] = $new_row;
          }

          // If an "all items" query, bump offset, go again.
          if (empty($query->params['$limit'])) {
            $query->params['$offset'] = count($result);
          }
        }
      } while (empty($query->params['$limit']) && !empty($resp['data']));
    }

    // Store off values from query in View.
    $view->result = $result;
    $view->total_rows = count($result);
    $this->pager->post_execute($view->result);

    // Execute count query for pager if necessary.
    if ($this->pager->use_count_query()) {
      $this->pager->total_items = $num_dataset_rows;
      $view->total_rows = $this->pager->get_total_items();
      $this->pager->update_page_info();
    }

    // Wrap up query.
    $view->execute_time = microtime(TRUE) - $start;
  }

  /***************************************************************************
   * REQUIRED methods for a Views query plugin leveraging base handlers
   **************************************************************************/

  /**
   * Add field to the query
   */
  function add_field($table, $field, $alias = '', $params = array()) {
    if (isset($params['function'])) {
      $alias = $params['function'] . '_' . $field;
    }
    // Create a field info array.
    $field_info = array(
      'field' => $field,
      'table' => $table,
      'alias' => $alias,
    ) + $params;
    $this->fields[$field] = $field_info;
    return $field;
  }

  /**
   * Copied from views_plugin_query_default, called by filter handlers.
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

    $exploded_field = explode('.', $field);
    $this->where[$group]['conditions'][] = array(
      'field' => array_pop($exploded_field),
      'value' => $value,
      'operator' => $operator,
    );
  }

  /**
   * Ripped straight out of views_plugin_query_default.
   */
  function add_where_expression($group, $snippet, $args = array()) {
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
      'field' => $snippet,
      'value' => $args,
      'operator' => 'formula',
    );
  }

  /**
   * Copied from views_plugin_query_default, 
   * modified for our simplified case, called by sort handlers.
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

    // If using an aggregate function, wrap the field in the function.
    if ($params && isset($params['function'])) {
      $as = $params['function'] . '(' . $as . ')';
    }

    $this->orderby[] = array(
      'field' => $as,
      'direction' => strtoupper($order),
    );
  }

  /**
   * Add a simple GROUP BY clause to the query. The caller is responsible
   * for ensuring that the fields are fully qualified and the table is properly
   * added.
   */
  function add_groupby($clause) {
    // Only add it if it's not already in there.
    if (!in_array($clause, $this->groupby)) {
      $this->groupby[] = $clause;
    }
  }

  /**
   * Remove all fields that may have been added; primarily used for summary
   * mode where we're changing the query because we didn't get data we needed.
   */
  function clear_fields() {
    $this->fields = array();
  }

  /**
   * Dummy placeholder method to satisfy Views core handlers.
   */
  function ensure_table($table) {
    return $table;
  }

}

/**
 * Based on views_query_default_aggregation_method_simple().
 *
 * @param string $group_type
 * @param string $field
 *
 * @return string
 */
function socrata_query_default_aggregation_method_simple($group_type, $field) {
  return strtoupper($group_type) . '(' . $field . ')';
}
