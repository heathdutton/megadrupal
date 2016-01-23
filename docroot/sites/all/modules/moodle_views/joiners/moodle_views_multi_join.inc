<?php


/**
 * @file
 * Definition of SQL join class to execute a join using multiple fields.
 */

/**
 * Like views_join but takes array of fields.
 */
class moodle_views_multi_join extends views_join {

  protected $left_fields = array();

  protected $fields = array();

  /**
   * Overrides construct().
   */
  public function construct($table = NULL, $left_table = NULL, $left_field = NULL, $field = NULL, $extra = array(), $type = 'LEFT') {
    $this->definition['left_field'] = array_shift($this->definition['left_fields']);
    $this->definition['field'] = array_shift($this->definition['fields']);
    parent::construct($table, $left_table, $left_field, $field, $extra, $type);
    $this->left_fields = $this->definition['left_fields'];
    $this->fields = $this->definition['fields'];
  }

  /**
   * Build the SQL for the join this object represents.
   *
   * When possible, try to use table alias instead of table names.
   */
  public function build_join($select_query, $table, $view_query) {
    if (empty($this->definition['table formula'])) {
      $right_table = $this->table;
    }
    else {
      $right_table = $this->definition['table formula'];
    }

    if ($this->left_table) {
      $left = $view_query->get_table_info($this->left_table);
      $left_field = "$left[alias].$this->left_field";
    }
    else {
      // This can be used if left_field is a formula or something.
      // It should be used only *very* rarely.
      $left_field = $this->left_field;
    }

    $condition = "$left_field = $table[alias].$this->field";

    foreach ($this->left_fields as $i => $left_field2) {
      $field2 = $this->fields[$i];
      $condition .= " AND $left[alias].$left_field2 = $table[alias].$field2";
    }
    $arguments = array();

    // Tack on the extra.
    if (isset($this->extra)) {
      if (is_array($this->extra)) {
        $extras = array();
        foreach ($this->extra as $info) {
          // Figure out the table name. Remember, only use aliases provided
          // if at all possible.
          $join_table = '';
          if (!array_key_exists('table', $info)) {
            $join_table = $table['alias'] . '.';
          }
          elseif (isset($info['table'])) {
            // If we're aware of a table alias for this table, use the table
            // alias instead of the table name.
            if (isset($left) && $left['table'] == $info['table']) {
              $join_table = $left['alias'] . '.';
            }
            else {
              $join_table = $info['table'] . '.';
            }
          }

          // Convert a single-valued array of values to the single-value case,
          // and transform from IN() notation to = notation.
          if (is_array($info['value']) && count($info['value']) == 1) {
            if (empty($info['operator'])) {
              $operator = '=';
            }
            else {
              $operator = $info['operator'] == 'NOT IN' ? '!=' : '=';
            }
            $info['value'] = array_shift($info['value']);
          }

          if (is_array($info['value'])) {
            // With an array of values, we need multiple placeholders and the
            // 'IN' operator is implicit.
            foreach ($info['value'] as $value) {
              $placeholder_i = ':views_join_condition_' . $select_query->nextPlaceholder();
              $arguments[$placeholder_i] = $value;
            }

            $operator = !empty($info['operator']) ? $info['operator'] : 'IN';
            $placeholder = '( ' . implode(', ', array_keys($arguments)) . ' )';
          }
          else {
            // With a single value, the '=' operator is implicit.
            $operator = !empty($info['operator']) ? $info['operator'] : '=';
            $placeholder = ':views_join_condition_' . $select_query->nextPlaceholder();
            $arguments[$placeholder] = $info['value'];
          }

          $extras[] = "$join_table$info[field] $operator $placeholder";
        }

        if ($extras) {
          if (count($extras) == 1) {
            $condition .= ' AND ' . array_shift($extras);
          }
          else {
            $condition .= ' AND (' . implode(' ' . $this->extra_type . ' ', $extras) . ')';
          }
        }
      }
      elseif ($this->extra && is_string($this->extra)) {
        $condition .= " AND ($this->extra)";
      }
    }

    $select_query->addJoin($this->type, $right_table, $table['alias'], $condition, $arguments);
  }
}
