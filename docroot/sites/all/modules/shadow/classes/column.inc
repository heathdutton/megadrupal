<?php

class ShadowColumn extends ShadowStateMachine {
  /**
   * @var array
   *   array(
   *     array('left_table', 'left_field', 'right_table', 'right_field', array(
   *       array('field', array(values)),
   *       ...
   *     ), optional),
   *     ...
   *   )
   */
  protected $joins;

  /**
   * @var string
   */
  protected $table;

  /**
   * @var string
   */
  protected $field;

  /**
   * @var bool
   */
  protected $inverted;

  /**
   * @var int
   */
  protected $join_index;

  public function __construct($definition = NULL) {
    $this->joins = array();
    $this->table = NULL;
    $this->field = NULL;
    $this->inverted = FALSE;

    $this->join_index = 0;

    $transitions = array(
      'start' => array(
        array('[a-z][a-z0-9_]*', 'tablename', 'tablename', FALSE),
      ),
      'tablename' => array(
        array('.', 'dot', NULL, TRUE),
      ),
      'dot' => array(
        array('[a-z][a-z0-9_]*', 'fieldname', NULL, FALSE),
      ),
      'fieldname' => array(
        array('', 'end', 'fieldname', FALSE),
        array('*', 'inverted', 'fieldname', TRUE),
        array('=', 'join_right_table', 'fieldname', TRUE),
      ),
      'inverted' => array(
        array('', 'end', 'invert', FALSE),
      ),
      'join_right_table' => array(
        array('[a-z0-9_]+', 'join_right_table', NULL, FALSE),
        array('.', 'join_right_dot', 'right_table', TRUE),
      ),
      'join_right_dot' => array(
        array('[a-z]+', 'join_right_field', NULL, FALSE),
      ),
      'join_right_field' => array(
        array('[a-z0-9_]+', 'join_right_field', NULL, FALSE),
        array(',', 'join_separator', 'right_field', TRUE),
        array('[', 'join_cond_start', 'right_field', TRUE),
        array('*', 'join_optional', 'right_field', FALSE),
      ),
      'join_separator' => array(
        array('[a-z][a-z0-9_]*', 'tablename', 'tablename', FALSE),
      ),
      'join_optional' => array(
        array(',', 'join_separator', 'join_optional', TRUE),
      ),
      'join_cond_start' => array(
        array('[a-z]+', 'join_cond_field', NULL, FALSE),
      ),
      'join_cond_field' => array(
        array('[a-z0-9_]+', 'join_cond_field', NULL, FALSE),
        array(':', 'join_cond_start_value', 'join_cond_field', TRUE),
      ),
      'join_cond_start_value' => array(
        array('\'', 'join_cond_string', NULL, TRUE),
        array('[0-9\\.\\-]', 'join_cond_numeric', NULL, FALSE),
      ),
      'join_cond_start_value_or_field' => array(
        array('\'', 'join_cond_string', NULL, TRUE),
        array('[0-9.\\-]', 'join_cond_numeric', NULL, FALSE),
        array('[a-z]+', 'join_cond_field', NULL, FALSE),
      ),
      'join_cond_string' => array(
        array('[^\\\']', 'join_cond_string', NULL, FALSE),
        array('\\', 'join_cond_backslashed', NULL, TRUE),
        array('\'', 'join_cond_after_value', 'join_cond_string', TRUE),
      ),
      'join_cond_backslashed' => array(
        array('(.)', 'join_cond_string', NULL, FALSE),
      ),
      'join_cond_numeric' => array(
        array('[0-9.\\-]', 'join_cond_numeric', NULL, FALSE),
        array(',', 'join_cond_start_value_or_field', 'join_cond_numeric', TRUE),
        array(']', 'join_cond_end', 'join_cond_numeric', TRUE),
      ),
      'join_cond_after_value' => array(
        array(']', 'join_cond_end', NULL, TRUE),
        array(',', 'join_cond_start_value_or_field', NULL, TRUE),
      ),
      'join_cond_end' => array(
        array('*', 'join_optional', NULL, FALSE),
        array(',', 'join_separator', NULL, TRUE),
      ),
    );

    $this->regex_allow_whitespace = FALSE;
    $this->regex_case_sensitive = FALSE;
    
    parent::__construct($transitions);

    if ($definition) {
      $this->parse($definition);
    }
  }

  public function parse($input) {
    parent::parse($input);

    $this->joins = array();
    $this->table = NULL;
    $this->field = NULL;
    $this->inverted = FALSE;

    $this->join_index = 0;

    // Contains joins and the table / fieldname (mixed).
    $fields_and_tables = array();

    $join_id = -1;

    $token_count = count($this->state_holder->token_types);
    for ($i = 0; $i < $token_count; ++$i) {
      $type = $this->state_holder->token_types[$i];
      $value = $this->state_holder->token_values[$i];
      switch ($type) {
        case 'tablename':
          ++$join_id;
          $join_cond_id = -1;
          $fields_and_tables[$join_id] = array(
            'join_conditions' => array(),
            'join_optional' => FALSE,
          );
        case 'fieldname':
        case 'right_table':
        case 'right_field':
          $fields_and_tables[$join_id][$type] = $value;
          break;
        case 'join_cond_field':
          ++$join_cond_id;
          $fields_and_tables[$join_id]['join_conditions'][$join_cond_id] = array(
            'field' => $value,
            'values' => array(),
          );
          break;
        case 'join_optional':
          $fields_and_tables[$join_id]['join_optional'] = TRUE;
          break;
        case 'join_cond_string':
        case 'join_cond_numeric':
          $value = $type == 'join_cond_numeric' ? (double) $value : (string) $value;
          $fields_and_tables[$join_id]['join_conditions'][$join_cond_id]['values'][] = $value;
          break;
        case 'inverted':
          $this->inverted = TRUE;
          break;
      }
    }
    $field = array_pop($fields_and_tables);
    $this->joins = $fields_and_tables;
    $this->table = $field['tablename'];
    $this->field = $field['fieldname'];
  }

  /**
   * Check if there is a next join in the joins iterator.
   *
   * @return bool
   */
  public function hasNextJoin() {
    return isset($this->joins[$this->join_index]);
  }

  /**
   * Generate SQL code for next join (iterator).
   *
   * @param string $left_alias Alias used for the left table.
   * @param string $right_alias Alias used for the right table.
   * @return array
   *   array('sql' => string, 'args' => array)
   */
  public function nextJoin($left_alias, $right_alias) {
    if (!$this->hasNextJoin()) {
      return FALSE;
    }
    extract($this->joins[$this->join_index]);

    $conditions = array();
    $args = array();

    $join_type = $join_optional ? 'LEFT JOIN' : 'JOIN';
    $left_field = $fieldname;
    $conditions[] = "$left_alias.$left_field = $right_alias.$right_field";

    foreach ($join_conditions as $join_condition) {
      $placeholders = array();
      foreach ($join_condition['values'] as $value) {
        $argument_name = _shadow_argument_name();
        $placeholders[] = $argument_name;
        $args[$argument_name] = $value;
      }
      $placeholders = implode(', ', $placeholders);
      $conditions[] = "$right_alias.{$join_condition['field']} IN ($placeholders)";
    }

    $sql = "$join_type {{$right_table}} $right_alias ON (" . implode(' AND ', $conditions) . ')';
    
    ++$this->join_index;
    return array('sql' => $sql, 'args' => $args);
  }

  /**
   * Rewind the joins iterator.
   */
  public function rewindJoins() {
    $this->join_index = 0;
  }

  /**
   * Get the tablename.
   *
   * @return string
   */
  public function getTableName() {
    return $this->table;
  }

  /**
   * Get the fieldname.
   *
   * @return string
   */
  public function getFieldName() {
    return $this->field;
  }

  /**
   * Check if the value is inverted.
   *
   * @return bool
   */
  public function isInverted() {
    return $this->inverted;
  }

  /**
   * Get a SchemaAPI compatible schema for this field.
   *
   * @return array
   */
  public function getSchema() {
    if (!$table_schema = drupal_get_schema($this->table)) {
      return FALSE;
    }
    if (!isset($table_schema['fields'][$this->field])) {
      return FALSE;
    }
    $field_schema = $table_schema['fields'][$this->field];
    $field_type = $field_schema['type'] == 'serial' ? 'int' : $field_schema['type'];
    $not_null = !empty($field_schema['not null']);
    foreach ($this->joins as $join) {
      if ($join['join_optional']) {
        $not_null = FALSE;
      }
    }
    $output = array(
      'type' => $field_type,
      'not null' => $not_null,
    );
    if (!empty($field_schema['size'])) {
      $output['size'] = $field_schema['size'];
    }
    if (!empty($field_schema['length'])) {
      $output['length'] = $field_schema['length'];
    }
    if (!empty($field_schema['unsigned'])) {
      $output['unsigned'] = $field_schema['unsigned'];
    }
    return $output;
  }

  /**
   * Check if this column can have multiple rows for a single row in the base table.
   *
   * @return bool
   */
  public function isMultiple() {
    foreach ($this->joins as $join) {
      $left_table = $join['tablename'];
      $left_field = $join['fieldname'];
      $right_table = $join['right_table'];
      $right_field = $join['right_field'];
      if (!$table_schema = drupal_get_schema($right_table)) {
        // We don't know the answer, assume TRUE.
        return TRUE;
      }
      $is_unique = FALSE;
      if (!empty($table_schema['primary key'])) {
        $key_field = $table_schema['primary key'][0];
        $key_field = is_array($key_field) ? $key_field[0] : $key_field;
        if (count($table_schema['primary key']) != 1 || $key_field != $right_field) {
          $is_unique = TRUE;
        }
      }
      isset($table_schema['unique keys']) or $table_schema['unique keys'] = array();
      foreach ($table_schema['unique keys'] as $unique) {
        $key_field = $unique[0];
        $key_field = is_array($key_field) ? $key_field[0] : $key_field;
        if (count($unique) != 1 || $key_field != $right_field) {
          $is_unique = TRUE;
        }
      }
      if (!$is_unique) {
        return TRUE;
      }
    }
    return FALSE;
  }
}
