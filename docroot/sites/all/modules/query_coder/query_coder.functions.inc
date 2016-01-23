<?php

/**
 * @file
 * Query coder functions.
 */


/**
 * Parse query.
 */
function query_coder_parse_query($query) {
  $output = '';
  $library = libraries_get_path('PHP-SQL-Parser');
  if (file_exists($library . '/php-sql-parser.php')) {
    $full_parser_path = $library . '/php-sql-parser.php';
    require_once $full_parser_path;
  }
  else {
    $output .= t('Error, please download !library_link library and unzip it to "sites/all/libraries" folder. Full path must be "sites/all/libraries/PHP-SQL-Parser/php-sql-parser.php".', array('!library_link' => l(t('PHP-SQL-Parser'), 'https://code.google.com/p/php-sql-parser/')));
    return $output;
  }

  variable_set('query_coder_counters', array(
    'placeholders_counter' => 1,
    'placeholders' => array(),
    'or' => 0,
    'and' => 0,
  ));

  $query = query_coder_preprocess_query($query);

  $parser = new PHPSQLParser($query);
  if ($parser->parsed) {
    $output .= query_coder_code_placeholders();
    $output .= query_coder_code_query($parser->parsed);
    $output = '<br />' . $output;
  }
  else {
    $output = t('You have an error in your SQL syntax');
  }

  return $output;
}

/**
 * Preprocess query.
 */
function query_coder_preprocess_query($query) {
  $counters = variable_get('query_coder_counters', array());

  $between_expr = '/BETWEEN\s+((\S+?)\s+AND\s+(\S+?))(\s+|$)/is';
  while (preg_match($between_expr, $query, $matches)) {
    if (count($matches) == 5) {
      $placeholder_name = 'placeholder_' . $counters['placeholders_counter'];
      $counters['placeholders_counter']++;
      preg_match('/(\S+)\s+AND\s+(\S+)/is', $matches[1], $range);
      $counters['placeholders'][] = '$' . $placeholder_name . ' = array(' . $range[1] . ', ' . $range[2] . ');';
      $query = preg_replace($between_expr, 'BETWEEN_PROCESSED $' . $placeholder_name . ' ', $query, 1);
      $matches = array();
    }
  }

  $query = str_replace('BETWEEN_PROCESSED', 'BETWEEN', $query);
  variable_set('query_coder_counters', $counters);
  return $query;
}

/**
 * Coder placeholders.
 */
function query_coder_code_placeholders() {
  $output = '';
  $counters = variable_get('query_coder_counters', array());

  if (count($counters['placeholders']) > 0) {
    foreach ($counters['placeholders'] as $placeholder) {
      $output .= $placeholder . '<br />';
    }
    $output .= '<br />';
  }

  return $output;
}


/**
 * Code query.
 */
function query_coder_code_query($query) {
  $output = '';
  $var_name = 'query';
  if (array_key_exists('SELECT', $query)) {
    $output .= query_coder_code_select_query($query, $var_name);
    $output .= query_coder_code_execute_line($var_name);
  }
  if (array_key_exists('UPDATE', $query)) {
    $output .= query_coder_code_update_query($query);
  }
  if (array_key_exists('DELETE', $query)) {
    $output .= query_coder_code_delete_query($query);
  }
  if (array_key_exists('INSERT', $query)) {
    $output .= query_coder_code_insert_query($query);
  }
  return $output;
}

/**
 * Code insert query.
 */
function query_coder_code_insert_query($query) {
  $output = '';
  $output .= query_coder_code_insert_table($query['INSERT']['table']);
  $output .= query_coder_code_insert_fields($query);
  $output .= '->execute();<br />';
  return $output;
}

/**
 * Code delete query.
 */
function query_coder_code_delete_query($query) {
  $output = '';
  $var_name = 'query';
  $output .= query_coder_code_delete_table($query['FROM'][0]['table'], $var_name);
  if (isset($query['WHERE'])) {
    $conditions_code = query_coder_code_conditions($query['WHERE'], $var_name);
    $output .= $conditions_code;
  }
  $output .= query_coder_code_execute_line($var_name, 'num_deleted');
  return $output;
}

/**
 * Code update query.
 */
function query_coder_code_update_query($query) {
  $output = '';
  $var_name = 'query';
  $output .= query_coder_code_update_table($query['UPDATE'][0], $var_name);
  $output .= query_coder_code_update_fields($query['SET'], $var_name);

  if (isset($query['WHERE'])) {
    $conditions_code = query_coder_code_conditions($query['WHERE'], 'query');
    $output .= $conditions_code;
  }

  $output .= query_coder_code_execute_line($var_name, 'num_updated');

  return $output;
}

/**
 * Code select query.
 */
function query_coder_code_select_query($query, $var_name = 'query') {
  $output = '';
  $full_query = $query;
  $table_array = array_shift($query['FROM']);

  $table_code = query_coder_code_table($table_array, $var_name);
  $output .= $table_code;

  $fields_code = query_coder_code_fields($query['SELECT'], $var_name, $full_query);
  $output .= $fields_code;

  $joins_code = query_coder_code_joins($query['FROM'], $var_name);
  $output .= $joins_code;

  if (isset($query['WHERE'])) {
    $conditions_code = query_coder_code_conditions($query['WHERE'], $var_name);
    $output .= $conditions_code;
  }

  if (isset($query['GROUP'])) {
    $group_code = query_coder_code_group($query['GROUP'], $var_name);
    $output .= $group_code;
  }

  if (isset($query['ORDER'])) {
    $order_code = query_coder_code_order($query['ORDER'], $var_name);
    $output .= $order_code;
  }

  if (isset($query['HAVING'])) {
    $having_code = query_coder_code_having($query['HAVING'], $var_name);
    $output .= $having_code;
  }

  if (isset($query['LIMIT'])) {
    $limit_code = query_coder_code_limit($query['LIMIT'], $var_name);
    $output .= $limit_code;
  }

  return $output;
}

/**
 * Code delete table.
 */
function query_coder_code_delete_table($table_name, $var_name) {
  $output = '';
  $output .= '$' . $var_name . ' = db_delete(\'' . $table_name . '\');<br />';
  return $output;
}

/**
 * Code insert table.
 */
function query_coder_code_insert_table($table_name) {
  $output = '';

  $output .= 'db_insert(\'' . str_replace('\'', '', $table_name) . '\')<br />';

  return $output;
}

/**
 * Code update table.
 */
function query_coder_code_update_table($table_array, $var_name) {
  $output = '';

  $output .= '$' . $var_name . ' = db_update(\'' . $table_array['table'] . '\');<br />';

  return $output;
}

/**
 * Code table.
 */
function query_coder_code_table($table_array, $var_name) {
  $output = '';

  $output .= '$' . $var_name . ' = db_select(\'' . $table_array['table'] . '\', \'' . $table_array['alias']['name'] . '\');<br />';

  return $output;
}

/**
 * Coder insert fields.
 */
function query_coder_code_insert_fields($query) {
  $output = '';

  $output .= '->fields(array(<br />';
  $i = 0;
  foreach ($query['INSERT']['columns'] as $field) {
    $colref = $query['INSERT']['columns'][$i]['base_expr'];
    $value = $query['VALUES'][0]['data'][$i]['base_expr'];
    $value = trim($value);
    $value = str_replace('\'', '', $value);
    $colref = str_replace('\'', '', $colref);
    if (!(substr($value, 0, 1) == '$' || is_numeric($value))) {
      $value = '\'' . $value . '\'';
    }
    $output .= '&nbsp;&nbsp;\'' . $colref . '\' => ' . $value . ',<br />';
    $i++;
  }
  $output .= '))<br />';

  return $output;
}

/**
 * Code update fields.
 */
function query_coder_code_update_fields($fields, $var_name) {
  $output = '';

  $output .= '$' . $var_name . '->fields(array(<br />';
  foreach ($fields as $field) {
    if ($field['expr_type'] == 'expression') {
      $colref = $field['sub_tree'][0]['base_expr'];
      $value = '';
      for ($i = 2; $i < count($field['sub_tree']); $i++) {
        $value .= $field['sub_tree'][$i]['base_expr'] . ' ';
      }
      $value = trim($value);
      $value = str_replace('\'', '', $value);
      if (!(substr($value, 0, 1) == '$' || is_numeric($value))) {
        $value = '\'' . $value . '\'';
      }
      $output .= '&nbsp;&nbsp;\'' . $colref . '\' => ' . $value . ',<br />';
    }
  }
  $output .= '));<br />';

  return $output;
}

/**
 * Code fields.
 */
function query_coder_code_fields($fields, $var_name, $full_query) {
  $output = '';
  $colref_fields = array();

  if (count($fields) == 1 && $fields[0]['base_expr'] === '*') {
    $output .= '$' . $var_name . '->fields(\'' . $full_query['FROM'][0]['alias']['name'] . '\');<br />';
    return $output;
  }

  foreach ($fields as $field) {
    switch ($field['expr_type']) {
      case 'aggregate_function':
        $output .= '$' . $var_name . '->addExpression(\'' . $field['base_expr'] . '(' . $field['sub_tree'][0]['base_expr'] . ')\', \'' . str_replace('\'', '', $field['alias']['name']) . '\');<br />';
        break;

      case 'colref':
        $field_parts = explode('.', $field['base_expr']);
        $field['field_title'] = $field_parts[1];
        if (isset($colref_fields[$field_parts[0]])) {
          $colref_fields[$field_parts[0]][] = $field;
        }
        else {
          $colref_fields[$field_parts[0]] = array($field);

        }
        break;
    }
  }

  foreach ($colref_fields as $k => $fields_set) {
    $output .= '$' . $var_name . '->fields(\'' . $k . '\', array(';
    $fields_array_str = '';
    foreach ($fields_set as $field) {
      $fields_array_str .= '\'' . $field['field_title'] . '\', ';
    }
    $fields_array_str = trim($fields_array_str);
    $fields_array_str = substr($fields_array_str, 0, -1);
    $output .= $fields_array_str . '));<br />';
  }

  return $output;
}

/**
 * Code joins.
 */
function query_coder_code_joins($joins, $var_name) {
  $output = '';

  foreach ($joins as $join) {
    $clause = '';
    foreach ($join['ref_clause'] as $clause_part) {
      $clause .= $clause_part['base_expr'] . ' ';
    }
    switch ($join['join_type']) {
      case 'JOIN':
        $output .= '$' . $var_name . '->innerJoin(\'' . $join['table'] . '\', \'' . $join['alias']['name'] . '\', \'' . trim($clause) . '\');<br />';
        break;

      case 'LEFT':
        $output .= '$' . $var_name . '->leftJoin(\'' . $join['table'] . '\', \'' . $join['alias']['name'] . '\', \'' . trim($clause) . '\');<br />';
        break;

      case 'RIGHT':
        $output .= '$' . $var_name . '->rightJoin(\'' . $join['table'] . '\', \'' . $join['alias']['name'] . '\', \'' . trim($clause) . '\');<br />';
        break;
    }
  }

  return $output;
}

/**
 * Code conditions.
 */
function query_coder_code_conditions($conditions, $var_name) {
  if (count($conditions) == 1 && $conditions[0]['expr_type'] == 'bracket_expression') {
    $conditions_parts = query_coder_split_conditions($conditions[0]['sub_tree']);
  }
  else {
    $conditions_parts = query_coder_split_conditions($conditions);
  }
  $conditions_code = query_coder_code_conditions_parts($conditions_parts, $var_name);
  return $conditions_code;
}

/**
 * Code group.
 */
function query_coder_code_group($groups, $var_name) {
  $output = '';

  foreach ($groups as $group) {
    $output .= '$' . $var_name . '->groupBy(\'' . $group['base_expr'] . '\');<br />';
  }

  return $output;
}

/**
 * Code order.
 */
function query_coder_code_order($orders, $var_name) {
  $output = '';

  foreach ($orders as $order) {
    $output .= '$' . $var_name . '->orderBy(\'' . $order['base_expr'] . '\', \'' . $order['direction'] . '\');<br />';
  }

  return $output;
}

/**
 * Code limit.
 */
function query_coder_code_limit($limit, $var_name) {
  $output = '';

  $offset = ($limit['offset']) ? $limit['offset'] : 0;
  $rowcount = $limit['rowcount'];

  $output .= '$' . $var_name . '->range(' . $offset . ', ' . $rowcount . ');<br />';

  return $output;
}

/**
 * Code having.
 */
function query_coder_code_having($having, $var_name) {
  $output = '';

  foreach ($having as $condition) {
    switch ($condition['expr_type']) {
      case 'colref':
        $colref = $condition['base_expr'];
        break;

      case 'const':
        $const = $condition['base_expr'];
        break;

      case 'operator':
        $operators[] = $condition['base_expr'];
        break;
    }
  }

  if (count($operators > 1)) {
    foreach ($operators as $operator) {
      $operator_str = $operator . ' ';
    }
  }

  $colref = trim($colref);
  $operator_str = trim($operator_str);
  $const = trim($const);
  $const = str_replace('\'', '', $const);

  if ($operator_str === '=') {
    $output .= '$' . $var_name . '->havingCondition(\'' . $colref . '\', ' . $const . ');<br />';
  }
  else {
    $output .= '$' . $var_name . '->havingCondition(\'' . $colref . '\', ' . $const . ', \'' . $operator_str . '\');<br />';
  }

  return $output;
}

/**
 * Rebuild conditions array.
 */
function query_coder_split_conditions($conditions) {
  $conditions_parts = array();
  $i = 0;
  foreach ($conditions as $condition) {
    if ($condition['expr_type'] == 'operator' && strtoupper($condition['base_expr']) === 'AND') {
      $i++;
    }
    else {
      $conditions_parts['AND'][$i][] = $condition;
    }
  }
  foreach ($conditions_parts['AND'] as $k => $condition_set) {
    $or_exist = query_coder_operator_exist($condition_set, 'OR');
    if ($or_exist) {
      $i = 0;
      $or_set = array();
      foreach ($condition_set as $condition) {
        if ($condition['expr_type'] == 'operator' && strtoupper($condition['base_expr']) === 'OR') {
          unset($conditions_parts['AND'][$k]);
          $i++;
        }
        else {
          $or_set[$i][] = $condition;
        }
      }
      $conditions_parts['AND'][$k]['OR'] = $or_set;
    }
  }

  foreach ($conditions_parts['AND'] as $k => $condition_set) {
    foreach ($condition_set as $set_key => $subset) {
      if (is_numeric($set_key)) {
        if ($subset['expr_type'] == 'bracket_expression' && $subset['sub_tree']) {
          $conditions_parts['AND'][$k] = query_coder_split_conditions($subset['sub_tree']);
        }
      }
      else {
        foreach ($subset as $set_key_2 => $subset_2) {
          if (count($subset_2) == 1 && $subset_2[0]['expr_type'] == 'bracket_expression' && is_array($subset_2[0]['sub_tree'])) {
            $conditions_parts['AND'][$k][$set_key][$set_key_2] = query_coder_split_conditions($subset_2[0]['sub_tree']);
          }
        }
      }
    }
  }
  return $conditions_parts;
}


/**
 * Return TRUE if orepator exist in condition set.
 */
function query_coder_operator_exist($condition_set, $op) {
  foreach ($condition_set as $condition) {
    if ($condition['expr_type'] == 'operator' && strtoupper($condition['base_expr']) == $op) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Code conditions parts.
 */
function query_coder_code_conditions_parts($conditions_parts, $var_name) {
  $output = '';
  foreach ($conditions_parts[key($conditions_parts)] as $condition_set) {
    if (key($condition_set) === 'AND') {
      if (count($condition_set) == 1) {
        $output .= query_coder_code_conditions_parts($condition_set, $var_name);
      }
      else {
        $parent_var_name = $var_name;
        $counters = variable_get('query_coder_counters', array());
        if (!isset($counters['and']) || $counters['and'] == 0) {
          $var_name = 'and';
          $counters['and'] = 1;
        }
        else {
          $var_name = 'and_' . $counters['and'];
          $counters['and']++;
        }
        variable_set('query_coder_counters', $counters);
        $output .= query_coder_coder_and_condition($condition_set, $var_name, $parent_var_name);
      }
    }
    elseif (key($condition_set) === 'OR') {
      $parent_var_name = $var_name;
      $counters = variable_get('query_coder_counters', array());
      if (!isset($counters['or']) || $counters['or'] == 0) {
        $var_name = 'or';
        $counters['or'] = 1;
      }
      else {
        $var_name = 'or_' . $counters['or'];
        $counters['or']++;
      }
      variable_set('query_coder_counters', $counters);
      $output .= query_coder_coder_or_condition($condition_set, $var_name, $parent_var_name);
    }
    elseif (is_numeric(key($condition_set))) {
      $is_simple = query_coder_is_simple_condition($condition_set);
      if ($is_simple) {
        $output .= query_coder_code_simple_condition($condition_set, $var_name);
      }
      else {
        $is_have_subquery = query_coder_is_have_subquery($condition_set, $var_name);
        if ($is_have_subquery) {
          $output .= query_coder_coder_subquery_condition($condition_set, $var_name);
        }
      }
    }
  }

  return $output;
}

/**
 * Return TRUE if set have subquery.
 */
function query_coder_is_have_subquery($condition_set, $var_name) {
  foreach ($condition_set as $condition) {
    if ($condition['expr_type'] == 'subquery') {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Return true if condition simple.
 */
function query_coder_is_simple_condition($condition_set) {
  foreach ($condition_set as $condition) {
    if (!($condition['expr_type'] == 'colref' || $condition['expr_type'] == 'operator' || $condition['expr_type'] == 'const' || $condition['expr_type'] == 'in-list')) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Code simple conditions.
 */
function query_coder_code_simple_condition($condition_set, $var_name) {
  $output = '';
  $operators = array();

  $operator_flag = FALSE;
  foreach ($condition_set as $condition) {
    switch ($condition['expr_type']) {
      case 'colref':
        if ($operator_flag) {
          $const = $condition['base_expr'];
        }
        else {
          $colref = $condition['base_expr'];
        }
        break;

      case 'const':
        $const = $condition['base_expr'];
        break;

      case 'operator':
        $operators[] = $condition['base_expr'];
        $operator_flag = TRUE;
        break;

      case 'in-list':
        $const = query_coder_code_in_list_values($condition['sub_tree']);
        break;
    }
  }

  if (count($operators > 1)) {
    foreach ($operators as $operator) {
      $operator_str = $operator . ' ';
    }
  }

  $colref = trim($colref);
  $operator_str = trim($operator_str);
  $const = trim($const);
  $const = str_replace('\'', '', $const);

  if (!(substr($const, 0, 6) == 'array(' || substr($const, 0, 1) == '$' || is_numeric($const))) {
    $const = '\'' . $const . '\'';
  }

  if ($operator_str === '=') {
    $output .= '$' . $var_name . '->condition(\'' . trim($colref) . '\', ' . trim($const) . ');<br />';
  }
  else {
    if (strtoupper($operator_str) === 'IS' && strtoupper($const) === '\'NULL\'') {
      $output .= '$' . $var_name . '->isNull(\'' . $colref . '\');<br />';
    }
    else {
      $output .= '$' . $var_name . '->condition(\'' . trim($colref) . '\', ' . trim($const) . ', \'' . $operator_str . '\');<br />';
    }
  }
  return $output;
}

/**
 * Code in-list values.
 */
function query_coder_code_in_list_values($list) {
  $output = '';
  if (count($list) > 0) {
    $array_str = '';
    $output .= 'array(';

    foreach ($list as $item) {
      $array_str .= $item['base_expr'] . ', ';
    }
    $array_str = substr($array_str, 0, -2);

    $output .= $array_str;
    $output .= ')';
  }

  return $output;
}

/**
 * Code or condition.
 */
function query_coder_coder_or_condition($condition_set, $var_name, $parent_var_name) {
  $output = '';
  $output .= '<br />$' . $var_name . ' = db_or();<br />';
  $output .= query_coder_code_conditions_parts($condition_set, $var_name);
  $output .= '$' . $parent_var_name . '->condition($' . $var_name . ');<br/>';
  return $output;
}

/**
 * Code and condition.
 */
function query_coder_coder_and_condition($condition_set, $var_name, $parent_var_name) {
  $output = '';
  $output .= '<br />$' . $var_name . ' = db_and();<br />';
  $output .= query_coder_code_conditions_parts($condition_set, $var_name);
  $output .= '$' . $parent_var_name . '->condition($' . $var_name . ');<br/>';
  return $output;
}

/**
 * Code subquery condition.
 */
function query_coder_coder_subquery_condition($condition_set, $var_name) {
  $output = '';
  $counters = variable_get('query_coder_counters', array());
  $parent_var_name = $var_name;
  if (!isset($counters['subquery']) || $counters['subquery'] = 0) {
    $var_name = 'subquery';
    $counters['subquery'] = 1;
  }
  else {
    $var_name = 'subquery' . $counters['subquery'];
    $counters['subquery']++;
  }

  foreach ($condition_set as $k => $condition) {
    if ($condition['expr_type'] == 'subquery') {
      $subquery_set = $condition['sub_tree'];
      $condition_set[$k]['expr_type'] = 'const';
      $condition_set[$k]['sub_tree'] = FALSE;
      $condition_set[$k]['base_expr'] = '$' . $var_name;
    }
  }

  $output .= '<br />';
  $output .= query_coder_code_select_query($subquery_set, $var_name);
  $output .= query_coder_code_simple_condition($condition_set, $parent_var_name);
  $output .= '<br />';

  return $output;
}

/**
 * Code execute line.
 */
function query_coder_code_execute_line($var_name, $result_var_name = 'result') {
  $output = '';

  $output .= '$' . $result_var_name . ' = $' . $var_name . '->execute();<br />';

  return $output;
}
