<?php

class MongoEmbeddedEntityFieldQuery extends EntityFieldQuery {

  /**
   * List of field conditions for parent entity.
   *
   * @var array
   *
   * @see MongoEmbeddedEntityFieldQuery::parentFieldCondition()
   */
  public $parentFieldConditions = array();

  /**
   * List of property conditions for parent entity.
   *
   * @var array
   *
   * @see MongoEmbeddedEntityFieldQuery::parentPropertyCondition()
   */
  public $parentPropertyConditions = array();

  /**
   * Adds a condition on parent field values.
   *
   * Note that entities with empty field values will be excluded from the
   * EntityFieldQuery results when using this method.
   *
   * @param $field
   *   Either a field name or a field array.
   * @param $column
   *   The column that should hold the value to be matched.
   * @param $value
   *   The value to test the column value against.
   * @param $operator
   *   The operator to be used to test the given value.
   * @param $delta_group
   *   An arbitrary identifier: conditions in the same group must have the same
   *   $delta_group.
   * @param $language_group
   *   An arbitrary identifier: conditions in the same group must have the same
   *   $language_group.
   *
   * @return EntityFieldQuery
   *   The called object.
   *
   * @see EntityFieldQuery::addFieldCondition
   * @see EntityFieldQuery::deleted
   */
  public function parentFieldCondition($field, $column = NULL, $value = NULL, $operator = NULL, $delta_group = NULL, $language_group = NULL) {
    return $this->addParentFieldCondition($this->parentFieldConditions, $field, $column, $value, $operator, $delta_group, $language_group);
  }

  /**
   * Adds the given condition to the proper condition array.
   *
   * @param $conditions
   *   A reference to an array of conditions.
   * @param $field
   *   Either a field name or a field array.
   * @param $column
   *   A column defined in the hook_field_schema() of this field. If this is
   *   omitted then the query will find only entities that have data in this
   *   field, using the entity and property conditions if there are any.
   * @param $value
   *   The value to test the column value against. In most cases, this is a
   *   scalar. For more complex options, it is an array. The meaning of each
   *   element in the array is dependent on $operator.
   * @param $operator
   *   Possible values:
   *   - '=', '<>', '>', '>=', '<', '<=', 'STARTS_WITH', 'CONTAINS': These
   *     operators expect $value to be a literal of the same type as the
   *     column.
   *   - 'IN', 'NOT IN': These operators expect $value to be an array of
   *     literals of the same type as the column.
   *   - 'BETWEEN': This operator expects $value to be an array of two literals
   *     of the same type as the column.
   *   The operator can be omitted, and will default to 'IN' if the value is an
   *   array, or to '=' otherwise.
   * @param $delta_group
   *   An arbitrary identifier: conditions in the same group must have the same
   *   $delta_group. For example, let's presume a multivalue field which has
   *   two columns, 'color' and 'shape', and for entity id 1, there are two
   *   values: red/square and blue/circle. Entity ID 1 does not have values
   *   corresponding to 'red circle', however if you pass 'red' and 'circle' as
   *   conditions, it will appear in the  results - by default queries will run
   *   against any combination of deltas. By passing the conditions with the
   *   same $delta_group it will ensure that only values attached to the same
   *   delta are matched, and entity 1 would then be excluded from the results.
   * @param $language_group
   *   An arbitrary identifier: conditions in the same group must have the same
   *   $language_group.
   *
   * @return EntityFieldQuery
   *   The called object.
   */
  protected function addParentFieldCondition(&$conditions, $field, $column = NULL, $value = NULL, $operator = NULL, $delta_group = NULL, $language_group = NULL) {
    // The '!=' operator is deprecated in favour of the '<>' operator since the
    // latter is ANSI SQL compatible.
    if ($operator == '!=') {
      $operator = '<>';
    }
    if (is_scalar($field)) {
      $field_definition = field_info_field($field);
      if (empty($field_definition)) {
        throw new EntityFieldQueryException(t('Unknown field: @field_name', array('@field_name' => $field)));
      }
      $field = $field_definition;
    }
    // Ensure the same index is used for field conditions as for fields.
    $index = count($this->fields);
    $this->fields[$index] = $field;
    if (isset($column)) {
      $conditions[$index] = array(
        'field' => $field,
        'column' => $column,
        'value' => $value,
        'operator' => $operator,
        'delta_group' => $delta_group,
        'language_group' => $language_group,
      );
    }
    return $this;
  }

  /**
   * Adds a condition on an entity-specific property of the parent entity.
   *
   * An $entity_type must be specified by calling
   * EntityFieldCondition::entityCondition('entity_type', $entity_type) before
   * executing the query. Also, by default only entities stored in SQL are
   * supported; however, EntityFieldQuery::executeCallback can be set to handle
   * different entity storage.
   *
   * @param $column
   *   A column defined in the hook_schema() of the base table of the entity.
   * @param $value
   *   The value to test the field against. In most cases, this is a scalar. For
   *   more complex options, it is an array. The meaning of each element in the
   *   array is dependent on $operator.
   * @param $operator
   *   Possible values:
   *   - '=', '<>', '>', '>=', '<', '<=', 'STARTS_WITH', 'CONTAINS': These
   *     operators expect $value to be a literal of the same type as the
   *     column.
   *   - 'IN', 'NOT IN': These operators expect $value to be an array of
   *     literals of the same type as the column.
   *   - 'BETWEEN': This operator expects $value to be an array of two literals
   *     of the same type as the column.
   *   The operator can be omitted, and will default to 'IN' if the value is an
   *   array, or to '=' otherwise.
   *
   * @return EntityFieldQuery
   *   The called object.
   */
  public function parentPropertyCondition($column, $value, $operator = NULL) {
    // The '!=' operator is deprecated in favour of the '<>' operator since the
    // latter is ANSI SQL compatible.
    if ($operator == '!=') {
      $operator = '<>';
    }
    $this->parentPropertyConditions[] = array(
      'column' => $column,
      'value' => $value,
      'operator' => $operator,
    );
    return $this;
  }
}
