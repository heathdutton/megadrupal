<?php

/**
 * @file
 * Class to aid querying parties and attached entities.
 */

/**
 * Query extender for party attached entities.
 *
 * This allows you to extend select query to hadd helper methods for querying
 * attached entities.
 *
 * To respect party access permissions, add the 'party_access' tag to the query
 * using SelectQuery::addTag().
 */
class PartyQuery extends SelectQueryExtender {

  /**
   * The alias of the base party table to build from. Defaults to party.
   */
  protected $base_alias = 'party';

  /**
   * An array of entity table aliases keyed by data_set_name:delta:type that we
   * have joined to.
   *
   * Although this may result in duplicate joins to a single entitiy table,
   * that may be necessary if you are querying fields on the entity itself
   * across different data sets.
   *
   * @see PartyQuery::party_attached_entity.
   */
  protected $data_sets = array();

  /**
   * An array of field table aliases keyed by
   * data_set_name:data_set_delta:field_name:field_delta:type that we have
   * joined to.
   *
   * @see PartyQuery::party_attached_entity.
   */
  protected $fields = array();

  /**
   * Attempt to sniff out the party base table.
   */
  public function __construct(SelectQueryInterface $query, DatabaseConnection $connection) {
    parent::__construct($query, $connection);

    $tables = $this->getTables();
    foreach ($tables as $alias => $table) {
      if ($table['table'] == 'party') {
        $this->base_alias = $alias;
        break;
      }
    }
  }

  /**
   * Get or set the alias for the base party table.
   *
   * @param string $alias
   *   Optionally a string containing the base party table alias. If FALSE
   *   we return the alias without changing it.
   *
   * @return string
   *   The alias for the base party table.
   */
  public function alias($alias = FALSE) {
    if ($alias) {
      $this->base_alias = $alias;
    }

    return $alias;
  }

  /**
   * Create a join to an attached entity.
   *
   * @param $data_set string|array
   *   Either a data set name or a data set array.
   * @param $delta int
   *   Optionally provide a delta to join to.
   * @param $type string
   *   The type of join. Typically one of INNER, LEFT OUTER and RIGHT OUTER.
   *   Defaults to INNER.
   *
   * @return string
   *   The table alias for the attached entity.
   */
  public function joinAttachedEntity($data_set, $alias = NULL, $delta = NULL, $type = 'INNER') {
    // Get hold of our data set definition.
    if (is_scalar($data_set)) {
      $data_set_definition = party_get_data_set_info($data_set);
      if (empty($data_set_definition)) {
        throw new Exception(t('Unknown data set: @data_set', array('@data_set' => $data_set)));
      }
      $data_set = $data_set_definition;
    }

    // Find or create our join to the entity table.
    $data_set_key = implode(':', array(
      $data_set['set_name'],
      $delta,
      $type,
    ));

    if (!isset($alias)) {
      $alias = $data_set['set_name'];
    }

    // Find or create our join.
    if (!isset($this->data_sets[$data_set_key])) {
      // Start with the party_attached_entity table.
      $data_set_placeholder = ':data_set_' . $this->nextPlaceholder();
      $conditions = "%alias.pid = {$this->base_alias}.pid AND %alias.data_set = {$data_set_placeholder}";
      $arguments = array($data_set_placeholder => $data_set['set_name']);

      // If we're joining a specific delta, we need some additional conditions.
      if (isset($delta)) {
        $delta_placeholder = ':delta_' . $this->nextPlaceholder();
        $conditions .= " AND %alias.delta = {$delta_placeholder}";
        $arguments[$delta_placeholder] = $delta;
      }

      $pae_alias = $this->addJoin($type, 'party_attached_entity', 'pae', $conditions, $arguments);

      // Get hold of our entity info for building our join.
      $entity_info = entity_get_info($data_set['entity type']);
      if (empty($entity_info)) {
        throw new Exception(t('Unknown entity: @entity_type', array('@entity_type' => $data_set['entity type'])));
      }

      // Create our join.
      $this->data_sets[$data_set_key] = $this->addJoin($type,
        $entity_info['base table'],
        $alias,
        "%alias.{$entity_info['entity keys']['id']} = {$pae_alias}.eid");
    }

    return $this->data_sets[$data_set_key];
  }

  /**
   * Create a join to an field on an attached entity.
   *
   * @param $data_set string|array
   *   Either a data set name or a data set array.
   * @param $field string|array
   *   Either a field name or a field info array.
   * @param $field_delta int
   *   Optionally provide a delta to join to for the field.
   * @param $data_set_delta int
   *   Optionally provide a delta to join to for the data set.
   * @param $type string
   *   The type of join. Typically one of INNER, LEFT OUTER and RIGHT OUTER.
   *   Defaults to INNER.
   *
   * @return string
   *   The table alias for the attached entity.
   */
  public function joinField($data_set, $field, $alias = NULL, $field_delta = NULL, $data_set_delta = NULL, $type = 'INNER') {
    // Get hold of our data set definition.
    if (is_scalar($data_set)) {
      if ($data_set == 'party') {
        $data_set = array(
          'set_name' => 'party',
          'entity type' => 'party',
        );
      }
      else {
        $data_set_definition = party_get_data_set_info($data_set);
        if (empty($data_set_definition)) {
          throw new Exception(t('Unknown data set: @data_set', array('@data_set' => $data_set)));
        }
        $data_set = $data_set_definition;
      }
    }

    // Get hold of the field information.
    if (is_scalar($field)) {
      $field_definition = field_info_field($field);
      if (empty($field_definition)) {
        throw new Exception(t('Unknown field: @field_name', array('@field_name' => $field)));
      }
      $field = $field_definition;
    }

    if (!isset($alias)) {
      $alias = $field['field_name'];
    }

    if ($data_set['set_name'] == 'party') {
      // We don't need to join as we're on party.
      $entity_alias = $this->base_alias;
    }
    else {
      // Get our join to the entity table.
      $entity_alias = $this->joinAttachedEntity($data_set, NULL, $data_set_delta, $type);
    }

    // Find or create our join to the field table.
    $field_key = implode(':', array(
      $data_set['set_name'],
      $data_set_delta,
      $field['field_name'],
      $field_delta,
      $type,
    ));

    if (!isset($this->fields[$field_key])) {
      $entity_info = entity_get_info($data_set['entity type']);
      $table_name = _field_sql_storage_tablename($field);

      // Build our conditions as an array which we'll implode with ANDs.
      $entity_type_placeholder = ':entity_type_' . $this->nextPlaceholder();
      $conditions = array(
        "%alias.entity_type = {$entity_type_placeholder}",
        "%alias.entity_id = {$entity_alias}.{$entity_info['entity keys']['id']}",
      );

      // Build our arguments array.
      $arguments = array(
        $entity_type_placeholder => $data_set['entity type'],
      );

      // If we're joining a specific delta, we need some additional conditions.
      if (isset($field_delta)) {
        $delta_placeholder = ':delta_' . $this->nextPlaceholder();
        $conditions[] = "%alias.delta = {$delta_placeholder}";
        $arguments[$delta_placeholder] = $field_delta;
      }

      // Create our join.
      $this->fields[$field_key] = $this->addJoin($type,
        $table_name,
        $alias,
        implode(' AND ', $conditions),
        $arguments);
    }

    return $this->fields[$field_key];
  }

  /**
   * Set a simple property condition on an attached entity.
   *
   * This doesn't handle more complicated condition sets. If you need that
   * functionality, use PartyQuery::joinAttachedEntity() and set your
   * conditions using the normal SelectQuery methods.
   *
   * @param $data_set
   *   Either a data set name or a data set array.
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
   * @param $delta
   *   Optionally specify an attached entity delta you want to query against.
   * @param $type string
   *   The type of join. Typically one of INNER, LEFT OUTER and RIGHT OUTER.
   *   Defaults to INNER.
   *
   * @see PartyQuery::joinAttachedEntity().
   *
   * @return PartyQuery
   *  The build PartyQuery.
   */
  public function propertyCondition($data_set, $column, $value, $operator = NULL, $delta = NULL, $type = 'INNER') {
    // Get our entity table alias.
    $entity_alias = $this->joinAttachedEntity($data_set, NULL, $delta, $type);

    // Set our condition.
    $this->condition("{$entity_alias}.{$column}", $value, $operator);

    return $this;
  }

  /**
   * Set a simple property condition on an attached entity.
   *
   * This doesn't handle more complicated condition sets. If you need that
   * functionality, use PartyQuery::joinAttachedEntity() and set your
   * conditions using the normal SelectQuery methods.
   *
   * @param $data_set
   *   Either a data set name or a data set array.
   * @param $field string|array
   *   Either a field name or a field info array.
   * @param $column
   *   A column defined in the hook_field_schema() of this field. If this is
   *   omitted then the query will find only entities that have data in this
   *   field, using the entity and property conditions if there are any.
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
   * @param $field_delta
   *   Optionally specify an field delta you want to query against.
   * @param $data_set_delta
   *   Optionally specify an attached entity delta you want to query against.
   * @param $type string
   *   The type of join. Typically one of INNER, LEFT OUTER and RIGHT OUTER.
   *   Defaults to INNER.
   *
   * @see PartyQuery::joinAttachedEntity().
   *
   * @return PartyQuery
   *  The build PartyQuery.
   */
  public function fieldCondition($data_set, $field, $column, $value, $operator = NULL, $field_delta = NULL, $data_set_delta = NULL, $type = 'INNER') {
    // Get hold of the field information.
    if (is_scalar($field)) {
      $field_definition = field_info_field($field);
      if (empty($field_definition)) {
        throw new Exception(t('Unknown field: @field_name', array('@field_name' => $field)));
      }
      $field = $field_definition;
    }

    // Get our entity table alias.
    $field_alias = $this->joinField($data_set, $field, NULL, $field_delta, $data_set_delta, $type);

    // Set our condition.
    $this->condition("{$field_alias}.{$field['field_name']}_{$column}", $value, $operator);

    return $this;
  }

  /**
   * Execute a special case where we want a list of party ids.
   *
   * This is a shortcut method to select the party id, group by party id to
   * remove any duplicates, execute and return an array of pids.
   *
   * @return array
   *   An array of party pids.
   */
  public function fetchPids() {
    // Make sure we're not selecting all fields from any tables.
    $tables =& $this->getTables();
    foreach ($tables as &$table) {
      $table['all_fields'] = FALSE;
    }

    // Make sure we only select party.pid.
    $fields =& $this->getFields();
    $fields = array(
      'pid' => array(
        'field' => 'pid',
        'table' => $this->base_alias,
        'alias' => 'pid',
      ),
    );

    // Make sure we only group by party.pid.
    $group_by =& $this->getGroupBy();
    $group_by = drupal_map_assoc(array("{$this->base_alias}.pid"));

    // Execute and return our pids.
    $result = $this->execute();
    return $result->fetchCol();
  }
}
