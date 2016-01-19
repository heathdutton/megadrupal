<?php
/**
 * @file
 * SuiteCRM REST select query builder.
 */

namespace Drupal\clients_suitecrm\RemoteEntity\Query;

/**
 * SuiteCRM REST select query builder.
 *
 * Somewhat based on EntityFieldQuery, and also the Views query builder.
 *
 * Note that this returns raw results. To get remote entities packed into
 * Drupal entities, use this class via the Remote Entity resource.
 *
 * Usage:
 * @code
 *  $query = $controller->getRemoteEntityQuery('select');
 * @endcode
 */
class SuiteCrmRestSelectQuery extends SuiteCrmRestBaseQuery {

  /**
   * Determines whether the query is RetrieveMultiple or Retrieve.
   *
   * The query is Multiple by default, until an ID condition causes it to be
   * single.
   */
  public $retrieveMultiple = TRUE;

  /**
   * An array of conditions on the query. These are grouped by the table they
   * are on.
   */
  public $conditions = array();

  /**
   * The fields to retrieve.
   */
  public $fields = array();

  /**
   * Relationships.
   */
  public $relationships = array();

  /**
   * The pager details.
   */
  public $pager = array(
    'limit' => NULL,
    'page'  => 1,
    'paging_cookie' => '',
  );

  /**
   * Whether to return a total record count with a query.
   *
   * Defaults to FALSE.
   */
  public $returnRecordCount = FALSE;

  /**
   * The total record count returned by the query.
   *
   * This is populated after execution if $returnRecordCount was set to TRUE.
   */
  public $totalRecordCount = NULL;

  /**
   * Add a field to retrieve.
   *
   * @param string $field_name
   *   The name of the field on the remote entity.
   */
  public function addField($field_name) {
    $this->fields[] = $field_name;
  }

  /**
   * Add multiple fields to retrieve.
   *
   * @param array $field_names
   *   An array of the names of the field on the remote entity.
   */
  public function addFields($field_names) {
    foreach ($field_names as $field_name) {
      $this->fields[] = $field_name;
    }
  }

  /**
   * Relationships are an abstraction of joins.
   *
   * These should be defined in hook_remote_entity_query_table_info().
   *
   * @param string $base
   *   The base table of the relationship. This can either be the base table of
   *   the query, or a table that has already been added to the query by a prior
   *   relationship.
   * @param string $relationship_name
   *   The name of the relationship, as defined by table data defined by modules
   *   in hook_remote_entity_query_table_info().
   *
   * @throws Exception
   *  Throws an exception if the $base is not already present in the query.
   */
  public function addRelationship($base, $relationship_name) {
    // TODO: cache this!
    // Ensure we use the original name if this is a substitution.
    $connection_name = clients_connection_get_substituted_name($this->connection->name);
    $table_info = remote_entity_get_query_table_info($connection_name);
    // Figure out the table this relationship joins onto.
    $relationship_definition = $table_info[$base]['relationships'][$relationship_name];
    $join_table = $relationship_definition['join']['table'];

    if ($base == $this->remote_base) {
      // If the new relationship is on the base of this query, just add it to
      // the top level of the relationship tree.
      // We assume (famous last words!) that we only join to a particular
      // table once, and key by the join table.
      $this->relationships[$join_table] = array(
        'base' => $base,
        'relationship_name' => $relationship_name,
      );
    }
    else {
      // If the new relationship is on another table, we expect it to already
      // be here (and throw an exception if it's not).
      if (isset($this->relationships[$base])) {
        $this->relationships[$base]['relationships'][$join_table] = array(
          'base' => $base,
          'relationship_name' => $relationship_name,
        );
      }
      else {
        throw new Exception(t('Base for new relationship not set on query.'));
      }
    }

    // TODO: eventually have to deal with table aliases and all that fun.
  }

  /**
   * Add a condition to the query.
   *
   * Sets the query to be a single retrieve if the condition is for a single
   * entity ID.
   *
   * Based on EntityFieldQuery::entityCondition().
   *
   * @param string $name
   *   The name of the entity property. We only support 'entity_id' for now.
   */
  public function entityCondition($name, $value, $operator = NULL) {
    $this->retrieveMultiple = is_array($value);

    if ($name == 'entity_id') {
      $field = $this->entity_info['remote entity keys']['remote id'];
      $this->conditions[$this->remote_base][] = array(
        'field' => $field,
        'value' => $value,
        'operator' => $operator,
      );
    }
    else {
      $this->conditions[$this->remote_base][] = array(
        'field' => $name,
        'value' => $value,
        'operator' => $operator,
      );
    }
  }

  /**
   * Add a condition to the query, using local property keys.
   *
   * Works only with properties defined in the property map.
   *
   * @see EntityFieldQuery::fieldCondition()
   *
   * @param string $property_name
   *   A local property defined in the $entity_info 'property map' array.
   * @param mixed $value
   *   The value to test the column value against.
   * @param string $operator
   *   The operator to be used to test the given value.
   *
   * @throws \Exception
   *   Throws Exception if the property isn't available in the property map.
   */
  public function propertyCondition($property_name, $value, $operator = NULL) {
    if (!isset($this->entity_info)) {
      return;
    }
    if (!isset($this->entity_info['property map'][$property_name])) {
      throw new \EntityFieldQueryException('Invalid query property. Only properties in the property map are allowed');
    }

    // Adding a field condition (probably) automatically makes this a multiple.
    $this->retrieveMultiple = TRUE;

    $remote_field_name = $this->entity_info['property map'][$property_name];

    $this->conditions[$this->remote_base][] = array(
      'field' => $remote_field_name,
      'value' => $value,
      'operator' => $operator,
    );
  }

  /**
   * Add a condition to the query, using remote property keys.
   *
   * Based on EntityFieldQuery::fieldCondition().
   *
   * @param string|array $field_name
   *   The name of the field, if the field is on the query base table. If the
   *   field is on a relationship table, an array of the form
   *   (TABLE, FIELDNAME)
   *   In this case the relationship table must already have been added to the
   *   query.
   * @param mixed $value
   *   The value of the condition. May be an array for certain operators, e.g.
   *   'Between'. May be NULL for an operator which does not require a value,
   *   e.g. 'NotNull'.
   * @param string $operator
   *   The operator for the condition. Defaults to '=' for a single value, and
   *   'in' for a multiple value. Possible values include 'Between',
   *   'BeginsWith', 'NotNull'. Full reference can be found at
   *  http://msdn.microsoft.com/en-us/library/microsoft.xrm.sdk.query.conditionoperator.aspx
   */
  public function fieldRemoteCondition($field_name, $value, $operator = NULL) {
    // Adding a field condition (probably) automatically makes this a multiple.
    // TODO: figure this out for sure!
    $this->retrieveMultiple = TRUE;

    if (is_array($field_name)) {
      // The condition is on a relationship table.
      list($condition_table, $condition_field) = $field_name;
      // TODO: throw an exception if the table is not already present as a
      // relationship.
      $this->conditions[$condition_table][] = array(
        'field' => $condition_field,
        'value' => $value,
        'operator' => $operator,
      );
    }
    else {
      $this->conditions[$this->remote_base][] = array(
        'field' => $field_name,
        'value' => $value,
        'operator' => $operator,
      );
    }
  }

  /**
   * Set the pager options.
   *
   * @param int $limit
   *   The number of items to return per request. This defaults to none. Anything
   *  empty (0, NULL) means no limit.
   * @param int $page
   *   The current page number. 1 is the first page of results.
   */
  public function setPager($limit, $page = NULL) {
    $this->pager['limit'] = $limit;
    if (!empty($page)) {
      $this->pager['page'] = $page;
    }
  }

  /**
   * Return the remote service method to call.
   *
   * Client Resources can overwrite this in the configuration to allow to create
   * custom calls. That way we can handled enhanced rest service functions.
   */
  protected function getRemoteMethod() {
    $clients_resource = $this->getClientsResource();
    if (!empty($clients_resource->configuration['remote_methods']['select'])) {
      return $clients_resource->configuration['remote_methods']['select'];
    }
    return 'get_entry_list';
  }

  /**
   * Run the query and return a result.
   *
   * @return array
   *   Remote entity objects as retrieved from the remote connection.
   */
  public function execute() {
    // Make the initial connection.
    $this->connection->connect();

    // Prepare arguments. This ensures the order of the keys is as required by
    // the service even if the values are filled later on.
    $method_args = array(
      'module_name' => '',
      'query' => '',
      'order_by' => '',
      'offset' => '0',
      'select_fields' => array(),
      'link_name_to_fields_array' => array(),
      'max_results' => '0',
      'deleted' => '0',
      'favorites' => FALSE,
    );
    if (!empty($this->getClientsResource()->configuration['module']['module_key'])) {
      $method_args['module_name'] = $this->getClientsResource()->configuration['module']['module_key'];
    }

    // Build conditions.
    if (!empty($this->conditions[$this->remote_base])) {
      $method_args['query'] = $this->buildConditionQuery($this->conditions[$this->remote_base]);
    }

    // Set pager if enabled.
    if (!empty($this->pager['limit'])) {
      $method_args['max_results'] = (int) $this->pager['limit'];
      $method_args['offset'] = ((int) $this->pager['limit'] * (int) $this->pager['page']);
    }

    // Set fields if defined.
    if (!empty($this->fields)) {
      // @TODO Set select_fields in $method_args.
    }

    // Fetch entries.
    // @link http://support.sugarcrm.com/02_Documentation/04_Sugar_Developer/Sugar_Developer_Guide_6.5/02_Application_Framework/Web_Services/05_Method_Calls/get_entry_list/
    $response = $this->connection->callMethodArray($this->getRemoteMethod(), $method_args);

    // Parse the response into entities.
    $remote_entities = $this->parseResponse($response);
    return $remote_entities;
  }

  /**
   * Helper for query() parse the response.
   *
   * May also set the $totalRecordCount property on the query, if applicable.
   *
   * The results will be post processed by
   * @see RemoteEntityAPIDefaultController::process_remote_entities()
   *
   * @param object $response
   *   The JKSON response from REST.
   *
   * @return array
   *   An array of entity objects, keyed numerically. An empty array is returned
   *   if the response contains no entities.
   */
  public function parseResponse($response) {
    $remote_entities = array();
    if (!empty($response->result_count)) {
      $this->totalRecordCount = (int) $response->total_count;
      foreach ($response->entry_list as $entry) {
        $entity = array(
          'remote_id' => $entry->id,
          'type' => $this->base_entity_type,
        );

        foreach ($entry->name_value_list as $property => $list) {
          $entity[$property] = $list->value;
        }
        $remote_entities[$entry->id] = (object) $entity;
      }
    }
    return $remote_entities;
  }

}
