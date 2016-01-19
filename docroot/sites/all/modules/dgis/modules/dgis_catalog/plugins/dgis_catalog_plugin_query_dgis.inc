<?php

/**
 * @file
 * Defines the default query object which builds and execute a 2gis query.
 */

class dgis_catalog_views_plugin_query_dgis extends views_plugin_query {

  function build(&$view) {
    $view->init_pager($view);
    // Let the pager modify the query to add limits.
    $this->pager->query();
  }

  function add_field($table, $field, $alias = '', $params = array()) {
    // We check for this specifically because it gets a special alias.
    if ($table == $this->base_table && $field == $this->base_field && empty($alias)) {
      $alias = $this->base_field;
    }

    if (!$alias && $table) {
      $alias = $table . '_' . $field;
    }

    // Make sure an alias is assigned
    $alias = $alias ? $alias : $field;

    // PostgreSQL truncates aliases to 63 characters: http://drupal.org/node/571548

    // We limit the length of the original alias up to 60 characters
    // to get a unique alias later if its have duplicates
    $alias = drupal_substr($alias, 0, 60);

    // Create a field info array.
    $field_info = array(
      'field' => $field,
      'table' => $table,
      'alias' => $alias,
    ) + $params;

    // Test to see if the field is actually the same or not. Due to
    // differing parameters changing the aggregation function, we need
    // to do some automatic alias collision detection:
    $base = $alias;
    $counter = 0;
    while (!empty($this->fields[$alias]) && $this->fields[$alias] != $field_info) {
      $field_info['alias'] = $alias = $base . '_' . ++$counter;
    }

    if (empty($this->fields[$alias])) {
      $this->fields[$alias] = $field_info;
    }

    return $alias;
  }

  function add_orderby($table, $field = NULL, $order = 'ASC', $alias = '', $params = array()) {
    $as = $alias;
    if ($field) {
      $as = $this->add_field($table, $field, $as, $params);
    }
    $this->orderby[] = array(
      'field' => $as,
      'direction' => strtoupper($order)
    );
  }

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
      'field' => $field,
      'value' => $value,
      'operator' => ltrim($operator, '!'),
      'negate' => drupal_substr($operator, 0, 1) == '!',
    );
  }

  function build_condition() {
    $operator = array('AND' => '&', 'OR' => '|');
    $main_group = '';
    if (!isset($this->where)) {
      $this->where = array(); // Initialize where clause if not set
    }
    foreach ($this->where as $group => $info) {
      if (!empty($info['conditions'])) {
        $sub_group = '';
        foreach ($info['conditions'] as $key => $clause) {
          $item       = '(' . $clause['field'] . $clause['operator'] . $clause['value'] . ')';
          $sub_group .= $clause['negate'] ? "(!$item)" : $item;
        }
        $main_group .= count($info['conditions']) <= 1 ? $sub_group : '(' . $operator[$info['type']] . $sub_group . ')';
      }
    }
    return count($this->where) <= 1 ? $main_group : '(' . $operator[$this->group_operator] . $main_group . ')';
  }

  function ensure_table($table, $relationship = NULL, $join = NULL) {
    return $table;
  }

  function execute(&$view) {
    $start = microtime();

    $what = '';
    $where = FALSE;
    foreach ($this->where as $group) {
      foreach ($group['conditions'] as $condition) {
        if ($condition['field'] == 'dgis_catalog.dgis_catalog_what') {
          $what = $condition['value'];
        }
        if ($condition['field'] == 'dgis_catalog.dgis_catalog_where') {
          $where = $condition['value'];
        }
      }
    }

    $limit = DGIS_PAGE_SIZE_MAX;
    if (property_exists($view->query, 'limit')) {
      $limit = $view->query->limit;
    }
    $offset = 0;
    if (property_exists($view->query, 'offset')) {
      $offset = $view->query->offset;
    }

    $dgis_total = 0;
    if (($offset + $limit) <= DGIS_PAGE_SIZE_MAX) {
      $result = dgis_get_firms($what, 'relevance', 1, DGIS_PAGE_SIZE_MAX, FALSE, FALSE, $where, $dgis_total);
    }
    else {
      // According to the fact that we can't set offset using 2gis API
      // and have limitation of maximum number of data per request we need to do sometimes few requests.
      $result = array();
      $page = floor($offset / DGIS_PAGE_SIZE_MAX) + 1;
      $received = ($page - 1) * DGIS_PAGE_SIZE_MAX;
      while(($offset + $limit) > $received) {
        $receive_iteration = dgis_get_firms($what, 'relevance', $page, DGIS_PAGE_SIZE_MAX, FALSE, FALSE, $where, $dgis_total);
        if (empty($receive_iteration)) {
          break;
        }
        $result = array_merge($result, $receive_iteration);
        $received += DGIS_PAGE_SIZE_MAX;
        $page++;
      }
      if ($page > 1) {
        $offset = $offset % DGIS_PAGE_SIZE_MAX;
      }
    }

    $sort_fields = array();
    if (!empty($this->orderby)) {
      foreach ($this->orderby as $orderby) {
        $sort_fields[drupal_strtolower($orderby['field'])] = $orderby['direction'];
      }
    }

    $params = array();
    foreach ($result as $key => $row) {
      $temp = array();
      foreach($this->fields as $field) {
        if (isset($row[$field['field']])) {
          $params[$field['alias']][$key] = $row[$field['field']];
          $temp[$field['alias']] = $row[$field['field']];
          if (is_array($temp[$field['alias']])) {
            $temp[$field['alias']] = implode(', ', $temp[$field['alias']]);
          }
        }
        else {
          $params[$field['alias']][$key] = NULL;
        }
      }
      $result[$key] = $temp;
    }

    $array_multisort_params = array();
    foreach ($sort_fields as $field => $direction) {
      $direction = ($direction == 'ASC') ? SORT_ASC : SORT_DESC;
      $array_multisort_params[] = $params[$field];
      $array_multisort_params[] = $direction;
    }
    $array_multisort_params[] = &$result;
    call_user_func_array('array_multisort', $array_multisort_params);

    $view_result = array();
    for ($i = $offset; $i < ($offset + $limit); $i++) {
      if (!isset($result[$i])) {
        break;
      }
      $row = $result[$i];
      $val = new stdClass();
      foreach($row as $field => $value) {
        $val->{$field} = $value;
      }
      $view_result[] = $val;
    }

    $total = count($result);

    $view->result = $view_result;
    $view->total_rows = $total;
    $view->execute_time = microtime() - $start;
    $view->query->pager->total_items  = $dgis_total;
    $view->query->pager->update_page_info();
  }

  function option_definition() {
    $options = parent::option_definition();

    return $options;
  }

  function alter(&$view) {
    foreach (module_implements('views_query_alter') as $module) {
      $function = $module . '_views_query_alter';
      $function($view, $this);
    }
  }
}
