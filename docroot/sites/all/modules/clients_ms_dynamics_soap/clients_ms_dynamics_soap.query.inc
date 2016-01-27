<?php

/**
 * MS Dynamics SOAP select query builder.
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
 *
 * This creates a Microsoft RetrieveMultiple request with a QueryExpression type
 * query.
 */
class MSDynamicsSoapSelectQuery extends RemoteEntityQuery {
  // TODO: make vars protected once no longer developing.

  /**
   * Determines whether the query is RetrieveMultiple or Retrieve.
   *
   * The query is Multiple by default, until an ID condition causes it to be
   * single.
   */
  public $retrieve_multiple = TRUE;

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
   * The paging cookie returned for the query.
   *
   * Callers that need to make queries that return more than 5000 records will
   * need to obtain it to pass it back to the next iteration of the query.
   * Queries of fewer than 5000 records can get away with just setting the
   * pager.
   */
  public $paging_cookie;

  /**
   * Whether to return a total record count with a query.
   *
   * Defaults to FALSE.
   */
  public $return_record_count = FALSE;

  /**
   * The total record count returned by the query.
   *
   * This is populated after execution if $return_record_count was set to TRUE.
   */
  public $total_record_count = NULL;

  // function addProperty()
  // SHould support adding fields by property name and magically converting them.

  /**
   * Add a field to retrieve.
   *
   * @param $field_name
   *  The name of the field on the remote entity.
   */
  function addField($field_name) {
    // TODO: eventually make this private and use addProperty().
    $this->fields[] = $field_name;
  }

  /**
   * Add multiple fields to retrieve.
   *
   * @param $field_names
   *  An array of the names of the field on the remote entity.
   */
  function addFields($field_names) {
    foreach ($field_names as $field_name) {
      $this->fields[] = $field_name;
    }
  }

  /**
   * Relationships are an abstraction of joins.
   *
   * These should be defined in hook_remote_entity_query_table_info().
   *
   * @param $base
   *  The base table of the relationship. This can either be the base table of
   *  the query, or a table that has already been added to the query by a prior
   *  relationship.
   * @param $relationship_name
   *  The name of the relationship, as defined by table data defined by modules
   *  in hook_remote_entity_query_table_info().
   *
   * @throws Exception
   *  Throws an exception if the $base is not already present in the query.
   */
  function addRelationship($base, $relationship_name) {
    // TODO: cache this!
    $table_info = remote_entity_get_query_table_info($this->connection->name);
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
   * @param $name
   *  The name of the entity property. We only support 'entity_id' for now.
   */
  function entityCondition($name, $value, $operator = NULL) {
    if ($name == 'entity_id') {
      $field = $this->entity_info['remote entity keys']['remote id'];

      if (is_array($value)) {
        // If we have a multiple value, then this will be a multiple query: add
        // the condition for that to use.
        $this->conditions[$this->remote_base][] = array(
          'field' => $field,
          'value' => $value,
          'operator' => $operator,
        );
      }
      else {
        // If we're querying for a single value, set the query to be single.
        $this->retrieve_multiple = FALSE;

        // This is a total hack because querying on an id is special in
        // MSD SOAP.
        // TODO: clean this up.
        $this->id_condition = $value;
      }
    }
    else {
      $this->conditions[$this->remote_base][] = array(
        'field' => $field,
        'value' => $value,
        'operator' => $operator,
      );
    }
  }

  /**
   * Add a condition to the query, using local property keys.
   *
   * Based on EntityFieldQuery::fieldCondition().
   *
   * @param $property_name
   *  A local property. Ie, a key in the $entity_info 'property map' array.
   */
  function propertyCondition($property_name, $value, $operator = NULL) {
    if (!isset($this->entity_info)) {
      // TODO: exception, base not set.
      return;
    }
    if (!isset($this->entity_info['property map'][$property_name])) {
      // TODO: exception, bad property.
      return;
    }

    // Adding a field condition (probably) automatically makes this a multiple.
    // TODO: figure this out for sure!
    $this->retrieve_multiple = TRUE;

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
   * @param $field_name
   *  The name of the field, if the field is on the query base table. If the
   *  field is on a relationship table, an array of the form (TABLE, FIELDNAME).
   *  In this case the relationship table must already have been added to the
   *  query.
   * @param $value
   *  The value of the condition. May be an array for certain operators, e.g.
   *  'Between'. May be NULL for an operator which does not require a value,
   *  e.g. 'NotNull'.
   * @param $operator = NULL
   *  The operator for the condition. Defaults to '=' for a single value, and
   *  'in' for a multiple value. Possible values include 'Between',
   *  'BeginsWith', 'NotNull'. Full reference can be found at
   *  http://msdn.microsoft.com/en-us/library/microsoft.xrm.sdk.query.conditionoperator.aspx
   */
  function fieldRemoteCondition($field_name, $value, $operator = NULL) {
    // Adding a field condition (probably) automatically makes this a multiple.
    // TODO: figure this out for sure!
    $this->retrieve_multiple = TRUE;

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
   * @param $limit
   *  The number of items to return per request. This defaults to none. Anything
   *  empty (0, NULL) means no limit.
   * @param $page
   *  The current page number. 1 is the first page of results.
   * @param $paging_cookie
   *  (optional) The paging cookie from the previous result. This is required if
   *  the values of $limit and $page are requesting records beyond the 5000th in
   *  the result set (i.e., if $limit * $page > 5000).
   */
  function setPager($limit, $page = NULL, $paging_cookie = '') {
    $this->pager['limit'] = $limit;
    if (!empty($page)) {
      $this->pager['page'] = $page;
    }
    if (!empty($paging_cookie)) {
      $this->pager['paging_cookie'] = $paging_cookie;
    }
  }

  /**
   * Run the query and return a result.
   *
   * @return
   *  Remote entity objects as retrieved from the remote connection.
   */
  function execute() {
    // TEMP!
    $entity_info = $this->entity_info;
    //dsm($this);

    // Make the initial connection.
    $this->connection->connect();
    // Get the query header.
    $action = $this->get_action();
    $query_pieces['header'] = $this->connection->get_query_header($action);

    // SINGLE LOAD BODY IS TOTALLY DIFFERENT!!
    // TODO: 2 classes????
    if ($this->retrieve_multiple) {
      $query_pieces['body'] = $this->get_query_body_multiple();
    }
    else {
      $query_pieces['body'] = $this->get_query_body_single();
    }

    $query_pieces['footer'] = '</s:Envelope>';

    $requestXML =
      $query_pieces['header'] .
      $query_pieces['body'] .
      $query_pieces['footer'];

    //dsm($requestXML, '$requestXML');

    // Debug mode output.
    $this->connection->debug($requestXML);

    // ARGGGH!!!
    $response = $this->connection->GetSOAPResponse($this->connection->endpoint, $requestXML);
    //dsm($response, 'responseXML');

    // Debug mode output.
    $this->connection->debug($response);

    // Parse the response XML into entities.
    $remote_entities = $this->parse_response($response);

    return $remote_entities;
  }

  /**
   * Get the query body XML for a single query.
   */
  function get_query_body_single() {
    $body_xml = '<s:Body>
	                  <Retrieve xmlns="http://schemas.microsoft.com/xrm/2011/Contracts/Services">
                      <entityName>' . strip_tags($this->remote_base) . '</entityName>
	                      <id>' . strip_tags($this->id_condition) . '</id>';

    //$body_xml .= $this->get_query_fields();
    // AAAARGH die die die Microsoft and SOAP. This should be refactorable, but
    // it's not for FKW reasons.
    // Take the fields from the entity info property map. The values are the
    // remote properties.
    foreach ($this->entity_info['property map'] as $field_name) {
      $this->fields[] = $field_name;
    }

    $body_xml .= '
            <columnSet xmlns:colset="http://schemas.microsoft.com/xrm/2011/Contracts" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
              <colset:AllColumns>false</colset:AllColumns>
              <colset:Columns xmlns:cols="http://schemas.microsoft.com/2003/10/Serialization/Arrays">';

    foreach ($this->fields as $field_name) {
      $body_xml .= '<cols:string>' . strip_tags($field_name) . '</cols:string>';
    }

    $body_xml .= '
              </colset:Columns>
            </columnSet>';
    // END AAAARGH die die.

	  $body_xml .= '
	                  </Retrieve>
	                </s:Body>';

	  return $body_xml;
  }

  /**
   * Get the query body XML for a multiple query.
   */
  function get_query_body_multiple() {
    // Get the table info from hook_remote_entity_query_table_info().
    $table_info = remote_entity_get_query_table_info($this->connection->name);

    // REFACTOR NEEDED
    $body_xml = '<s:Body>
        <RetrieveMultiple xmlns="http://schemas.microsoft.com/xrm/2011/Contracts/Services" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
          <query i:type="a:QueryExpression" xmlns:a="http://schemas.microsoft.com/xrm/2011/Contracts">';

    // ====================== Fields
    $body_xml .= $this->get_query_fields();

    // ====================== Conditions
    $conditions = isset($this->conditions[$this->remote_base]) ? $this->conditions[$this->remote_base] : array();
    $body_xml .= '
            <a:Criteria>' .
            $this->get_query_conditions($this->remote_base, $conditions) . '
            </a:Criteria>';

    $body_xml .= '
            <a:Distinct>false</a:Distinct>
            <a:EntityName>' . strip_tags($this->remote_base) . '</a:EntityName>';

    // Link entities.
    $body_xml .= $this->get_query_link_entities($this->relationships);

    $pager_limit = $this->pager['limit'];
    if (empty($pager_limit)) {
      // Doesn't matter what we pass as the count, but we have to pass something.
      $pager_limit = -1;
    }

    // The ReturnTotalRecordCount property tells MS Dynamics whether to include
    // a total count of records in the returned information. It is suspected
    // that including this incurs a performance cost, so this defaults to FALSE.
    $return_record_count_string = $this->return_record_count ? 'true' : 'false';

    $body_xml .= '
            <a:Orders />
            <a:PageInfo>
              <a:Count>' . intval($pager_limit) . '</a:Count>
              <a:PageNumber>' . intval($this->pager['page']) . '</a:PageNumber>';

    if (empty($this->pager['paging_cookie'])) {
      $body_xml .= '
              <a:PagingCookie i:nil="true" />';
    }
    else {
      $body_xml .= '
              <a:PagingCookie>' . htmlspecialchars($this->pager['paging_cookie']) . '</a:PagingCookie>';
    }

    $body_xml .= '
              <a:ReturnTotalRecordCount>' . $return_record_count_string . '</a:ReturnTotalRecordCount>
            </a:PageInfo>
            <a:NoLock>false</a:NoLock>
          </query>';
    $body_xml .= '
        </RetrieveMultiple>';
    $body_xml .= '
      </s:Body>';

    return $body_xml;
  }

  /**
   * Helper for get_query_body_*(): get the fields list.
   *
   * Except: ARGH the XML is subtly different if single/multiple due to the
   * plethora of namespaces all over. FAIL.
   *
   * @return
   *  A string containing the <a:ColumnSet> XML element.
   */
  private function get_query_fields() {
    // If no fields have been added, we add all the fields declared for the
    // entity.
    if (empty($this->fields)) {
      foreach ($this->entity_info['property map'] as $field_name) {
        $this->fields[] = $field_name;
      }
    }

    $body_xml = '
            <a:ColumnSet>
              <a:AllColumns>false</a:AllColumns>
              <a:Columns xmlns:b="http://schemas.microsoft.com/2003/10/Serialization/Arrays">';

    foreach ($this->fields as $field_name) {
      $body_xml .= '<b:string>' . strip_tags($field_name) . '</b:string>';
    }

    $body_xml .= '
              </a:Columns>
            </a:ColumnSet>';

    return $body_xml;
  }

  /**
   * Generate query XML for conditions.
   *
   * These may be applied at the base level of the XML query, or within a
   * LinkEntities element as conditions on the join.
   *
   * @param $table
   *  The table that these conditions are on.
   * @param $conditions
   *  An array of conditions.
   *
   * @return
   *  XML text for the query, containing the elements:
   *    - a:Conditions (which may be empty)
   *    - a:FilterOperator
   *    - a:Filters (which may be empty)
   */
  function get_query_conditions($table, $conditions) {
    $table_info = remote_entity_get_query_table_info($this->connection->name);

    $condition_xml = '';
    if (count($conditions)) {
      $condition_xml .= '
              <a:Conditions>';

      // Get info on fields, to check their type.
      if (isset($table_info[$table]['fields'])) {
        $field_info = $table_info[$table]['fields'];
      }
      else {
        $field_info = array();
      }

      foreach ($conditions as $condition) {
        if (is_array($condition['value'])) {
          // The value may be an array for certain operations, such as 'Between'.
          $values = $condition['value'];
        }
        elseif (is_null($condition['value'])) {
          // The value may be NULL if it is not required for the operator, such
          // as 'NotNull'. In this case, set the values to an empty array so
          // no values are set in the condition.
          $values = array();
        }
        else {
          // Turn the values into an array in all cases.
          $values = array($condition['value']);
        }

        // Get the operator.
        if (is_null($condition['operator'])) {
          // If not specified, we set a default of either 'Equal' or 'In',
          // depending on how many values we were passed.
          if (count($values) == 1) {
            $operator = 'Equal';
          }
          else {
            $operator = 'In';
          }
        }
        else {
          $operator = $condition['operator'];
        }

        // Set the type from hook_remote_entity_query_table_info(), if defined
        // there. Otherwise, default to 'string'.
        if (isset($field_info[$condition['field']])) {
          $type = $field_info[$condition['field']]['type'];
        }
        else {
          $type = 'string';
        }

        $condition_xml .= '
                <a:ConditionExpression>
                  <a:AttributeName>' . strip_tags($condition['field']) . '</a:AttributeName>
                  <a:Operator>' . strip_tags($operator) . '</a:Operator>
                  <a:Values xmlns:b="http://schemas.microsoft.com/2003/10/Serialization/Arrays">';
        foreach ($values as $value) {
          // $type comes from hook data so is not user input.
          $condition_xml .= '
                    <b:anyType i:type="c:' . $type . '" xmlns:c="http://www.w3.org/2001/XMLSchema">' . strip_tags($value) . '</b:anyType>';
        }
        $condition_xml .= '
                  </a:Values>
                </a:ConditionExpression>';
      }

      $condition_xml .= '
              </a:Conditions>
              <a:FilterOperator>And</a:FilterOperator>
              <a:Filters />';
    }
    else {
      // No conditions: empty elements.
      $condition_xml .= '
              <a:Conditions />
              <a:FilterOperator>And</a:FilterOperator>
              <a:Filters />';
    }

    return $condition_xml;
  }

  /**
   * Generate recursive relationship query XML.
   *
   * @param $relationships
   *  An array of relationship data. Passed in so we can call this recursively
   *  with further relationships on relatioships.
   *
   * @return
   *  XML text for the query, including the outer 'LinkEntities' entities
   *  element, which may be empty if there are no relationships to add at the
   *  current level.
   */
  function get_query_link_entities($relationships) {
    $table_info = remote_entity_get_query_table_info($this->connection->name);

    $link_xml = '';
    if (count($relationships)) {
      $link_xml .= '
            <a:LinkEntities>';

      foreach ($relationships as $relationship_data) {
        // The base of the relationship is the left hand table we join to with
        // this relationship.
        // This must be either the base of the whole query, or a table we
        // have already joined onto.
        $relationship_base = $relationship_data['base'];
        $relationship_name = $relationship_data['relationship_name'];
        $relationship_info = $table_info[$relationship_base]['relationships'][$relationship_name];
        $join_table = $relationship_info['join']['table'];

        // Fill in the LHS base.
        $relationship_info['base']['table'] = $relationship_base;

        $link_xml .= '
              <a:LinkEntity>
                <a:Columns>
                  <a:AllColumns>false</a:AllColumns>
                  <a:Columns xmlns:b="http://schemas.microsoft.com/2003/10/Serialization/Arrays" />
                </a:Columns>
                <a:EntityAlias i:nil="true" />
                <a:JoinOperator>Inner</a:JoinOperator>';
        // Pass at least an empty array in all circumstances; the helper will
        // produce the correct XML if there are no conditions.
        $conditions = isset($this->conditions[$join_table]) ? $this->conditions[$join_table] : array();
        $link_xml .= '
                <a:LinkCriteria>' .
                $this->get_query_conditions($join_table, $conditions) . '
                </a:LinkCriteria>';

        // Add relationships on this one if there are any.
        if (!empty($relationship_data['relationships'])) {
          $link_xml .= $this->get_query_link_entities($relationship_data['relationships']);
        }
        else {
          $link_xml .= '
                <a:LinkEntities />';
        }
        $link_xml .= '
                <a:LinkFromAttributeName>' . $relationship_info['base']['field'] . '</a:LinkFromAttributeName>
                <a:LinkFromEntityName>' . $relationship_info['base']['table'] . '</a:LinkFromEntityName>
                <a:LinkToAttributeName>' . $relationship_info['join']['field'] . '</a:LinkToAttributeName>
                <a:LinkToEntityName>' . $relationship_info['join']['table'] . '</a:LinkToEntityName>
              </a:LinkEntity>';

      }

      $link_xml .= '
            </a:LinkEntities>';
    }
    else {
      $link_xml .= '
            <a:LinkEntities />';
    }

    return $link_xml;
  }

  /**
   * Determine the action, based on the value of $this->retrieve_multiple.
   *
   * @return
   *  The string to use for the SOAP action.
   */
  private function get_action() {
    if ($this->retrieve_multiple) {
      $action = 'RetrieveMultiple';
    }
    else {
      $action = 'Retrieve';
    }
    return $action;
  }

  /**
   * Helper for query(). Parse the XML response.
   *
   * May also set the $total_record_count property on the query, if applicable.
   *
   * @param $response
   *  The XML response from SOAP.
   *
   * @return
   *  An array of entity objects, keyed numerically. An empty array is returned
   *  if the response contains no entities.
   *
   * @throws
   *  Exception if a fault is received from XRM.
   */
  function parse_response($response) {
    $entities = array();

    if (empty($response)) {
      watchdog('clients_ms_dynamics_soap', 'Response is empty for MS Dynamics select query.', array(), WATCHDOG_CRITICAL);
      throw new Exception('Response is empty for MS Dynamics select query.');
    }

    $response_dom = new DomDocument();
    $response_dom->loadXML($response);

    // First see if there is an error.
    $dom_entities_fault = $response_dom->getElementsByTagName('Fault');
    if ($dom_entities_fault->length) {
      $xpath = new DOMXPath($response_dom);
      $fault_text_node_list = $xpath->query("//*[local-name()='Reason']/*[local-name()='Text']");
      $conditions = $this->debug_get_conditions();
      $text = "Remote query returned a fault. Conditions were: $conditions. Remote fault text is: " . $fault_text_node_list->item(0)->textContent;
      watchdog('clients_ms_dynamics_soap', $text, array(), WATCHDOG_CRITICAL);
      throw new Exception($text);
    }

    if ($this->retrieve_multiple) {
      $dom_entities = $response_dom->getElementsbyTagName("Entity");

      // Get the paging cookie. There's always one present, even if no pager
      // was set.
      $dom_entities_paging_cookie = $response_dom->getElementsbyTagName("PagingCookie");
      $this->paging_cookie = $dom_entities_paging_cookie->item(0)->nodeValue;
    }
    else {
      $dom_entities = $response_dom->getElementsbyTagName("RetrieveResult");
    }

    foreach ($dom_entities as $entity_dom) {
      // TODO: move to an unpacking helper!?
      $key_value_pairs = $entity_dom->getElementsbyTagName("KeyValuePairOfstringanyType");
      $entity = new StdClass;
      foreach ($key_value_pairs as $key_value_pair) {
        $key = $key_value_pair->getElementsbyTagName("key")->item(0)->textContent;

        $value_element = $key_value_pair->getElementsbyTagName("value")->item(0);
        $value_type = $value_element->getAttribute('i:type');

        if ($value_type == 'b:EntityReference') {
          // By convention, an entity reference is stored in the field as
          // the GUID, and pseudofields are created for:
          // - the type
          // - the name
          $id = $value_element->getElementsbyTagName("Id")->item(0)->textContent;
          $name = $value_element->getElementsbyTagName("Name")->item(0)->textContent;
          $type = $value_element->getElementsbyTagName("LogicalName")->item(0)->textContent;

          $entity->{$key} = $id;
          // Name.
          $name_key = $key . '_name';
          $entity->{$name_key} = $name;
          // Type.
          $type_key = $key . '_type';
          $entity->{$type_key} = $type;
        }
        else {
          $value = $key_value_pair->getElementsbyTagName("value")->item(0)->textContent;
          $entity->{$key} = $value;
        }
      }
      $entities[] = $entity;
    }

    // If a total record count was requested, extract that from the response.
    if ($this->return_record_count) {
      $xpath = new DOMXPath($response_dom);
      $count_node_list = $xpath->query("//*[local-name()='TotalRecordCount']");
      $text = $count_node_list->item(0)->textContent;

      $this->total_record_count = (int) $text;
    }

    return $entities;
  }

  /**
   * Get the conditions in a summarized format for debug output.
   *
   * @return
   *  A human-readable string describing the conditions on the query.
   */
  function debug_get_conditions() {
    $output = array();
    foreach ($this->conditions as $table => $conditions) {
      foreach ($conditions as $condition) {
        if (is_array($condition['value'])) {
          $value = implode(', ', $condition['value']);
          $operator = empty($condition['operator']) ? 'IN' : $condition['operator'];
        }
        else {
          $value = $condition['value'];
          $operator = empty($condition['operator']) ? '=' : $condition['operator'];
        }

        $output[] = check_plain("$table.$condition[field] $operator $value");
      }
    }

    return implode(', ', $output);
  }

}

/**
 * Query builder for a FetchExpression query.
 *
 * This creates a Microsoft RetrieveMultiple request with a FetchExpression type
 * query. Because Microsoft had to invent two different XML query languages!
 *
 * Because the RemoteEntityAPIDefaultController doesn't know about these sorts
 * of queries, queries of this type need to be requested directly from the
 * controller object.
 * TODO: add more docs here, or improve the DX.
 */
class MSDynamicsSoapSelectFetchExpressionQuery extends MSDynamicsSoapSelectQuery {

  protected $fetchXML;

  /**
   * Set the FetchXML query string.
   *
   * This should not have a pager, as this is added by the query builder later
   * on.
   *
   * Any attributes are replaced with the full list of remote properties, to
   * ensure complete entities are returned.
   */
  public function addFetchXMLQuery($fetch_xml) {
    $this->fetchXMLBuilder = new MSDynamicsSoapFetchXMLBuilder($this->base_entity_type, $this->remote_base);
    $this->fetchXMLBuilder->setFetchXML($fetch_xml);
  }

  /**
   * Get the DOMDocument for the FetchXML query.
   *
   * This allows custom manipulation of the FetchXML prior to executing the
   * query.
   *
   * @return
   *  The DOMDocument object for the FetchXML query.
   */
  public function getDom() {
    if (!is_object($this->fetchXMLBuilder) || !is_object($this->fetchXMLBuilder->dom)) {
      throw new Exception(t("The query XML has not yet been set."));
    }
    return $this->fetchXMLBuilder->dom;
  }

  /**
   * Get the query body XML for a multiple query.
   */
  function get_query_body_multiple() {
    // Get the table info from hook_remote_entity_query_table_info().
    $table_info = remote_entity_get_query_table_info($this->connection->name);

    // Prepare the FetchXML query.
    // Pass the pager details in: paging is done in the inner FetchXML.
    $this->fetchXMLBuilder->setPager($this->pager);
    // Set our full list of remote properties into the FetchXML, replacing any
    // currently there.
    $this->fetchXMLBuilder->setQueryAttributes();

    $body_xml = '<s:Body>
        <RetrieveMultiple xmlns="http://schemas.microsoft.com/xrm/2011/Contracts/Services" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
          <query i:type="a:FetchExpression" xmlns:a="http://schemas.microsoft.com/xrm/2011/Contracts">';

    $body_xml .= '
            <a:Query>' .
            htmlspecialchars($this->fetchXMLBuilder->getFetchXML()) .
            '</a:Query>';

    $body_xml .= '
          </query>';
    $body_xml .= '
        </RetrieveMultiple>';
    $body_xml .= '
      </s:Body>';

    return $body_xml;
  }

}

/**
 * MS Dynamics SOAP insert query builder.
 *
 * This should only be used via
 * clients_connection_ms_dynamics_soap::remote_entity_save().
 *
 * TODO: updateFields method to set which fields to send?
 */
class MSDynamicsSoapInsertQuery extends RemoteEntityInsertQuery {

  /**
   * Execute the query.
   *
   * The entity must already have been set with setEntity().
   *
   * @return
   *  The GUID for the newly created remote entity.
   */
  function execute() {
    // Make the initial connection.
    $this->connection->connect();
    // Get the query header.
    $action = 'Create';
    $query_pieces['header'] = $this->connection->get_query_header($action);

    $query_pieces['body'] = $this->get_query_body();

    $query_pieces['footer'] = '</s:Envelope>';

    $requestXML =
      $query_pieces['header'] .
      $query_pieces['body'] .
      $query_pieces['footer'];

    //dsm($requestXML, '$requestXML');

    // Debug mode output.
    $this->connection->debug($requestXML);

    $response = $this->connection->GetSOAPResponse($this->connection->endpoint, $requestXML);

    // Debug mode output.
    $this->connection->debug($response);

    // Parse the response XML.
    $guid = $this->parse_response($response);
    return $guid;
  }

  /**
   * Get the query body XML.
   */
  function get_query_body() {
    $body_xml = '';
    $body_xml .= '
      <s:Body>
        <Create xmlns="http://schemas.microsoft.com/xrm/2011/Contracts/Services"  xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
          <entity xmlns:a="http://schemas.microsoft.com/xrm/2011/Contracts">
            <a:Attributes xmlns:b="http://schemas.datacontract.org/2004/07/System.Collections.Generic">';

    $body_xml .= $this->get_query_attributes();

    // 'Id' might not be needed. TODO: test this out.
    $body_xml .= '
            </a:Attributes>
            <a:EntityState i:nil="true" />
            <a:FormattedValues xmlns:b="http://schemas.datacontract.org/2004/07/System.Collections.Generic" />
            <a:Id>00000000-0000-0000-0000-000000000000</a:Id>
            <a:LogicalName>' . strip_tags($this->remote_base) . '</a:LogicalName>
            <a:RelatedEntities xmlns:b="http://schemas.datacontract.org/2004/07/System.Collections.Generic" />
          </entity>
        </Create>
      </s:Body>';

    return $body_xml;
  }

  /**
   * Helper for get_query_body().
   *
   * Build the XML string for the attributes to insert.
   */
  function get_query_attributes() {
    $property_map = $this->entity_info['property map'];

    $attribute_builder = new ClientMSDynamicsQueryAttributeBuilder($this->base_entity_type, $this->remote_base, $this->entity, $this->table_info);
    $attributes_xml = $attribute_builder->get_query_attributes($property_map);

    return $attributes_xml;
  }

  /**
   * Helper for execute(). Parse the XML response and handle errors.
   *
   * @param $response
   *  The XML response from SOAP.
   *
   * @return
   *  The GUID for the saved remote entity.
   *
   * @throws Exception
   *  Throws an exception if the remote response contains an error message.
   */
  function parse_response($response) {
    $response_dom = new DomDocument();
    $response_dom->loadXML($response);

    // First see if there is an error.
    $dom_entities_fault = $response_dom->getElementsByTagName('Fault');
    if ($dom_entities_fault->length) {
      // Yes, use of SimpleXML here is a total mishmash.
      // TODO: cleanup. Ideally by someone who knows about XML parsing!
      $xml = simplexml_load_string($response);
      $result = $xml->xpath('//s:Reason/s:Text');
      $text = (string) $result[0];

      watchdog('clients_ms_dynamics_soap', $text, array(), WATCHDOG_CRITICAL);
      throw new Exception($text);
    }

    $dom_entities = $response_dom->getElementsbyTagName("CreateResult");
    // There is only one, but we appear to have to foreach this anway!? WTF.
    foreach ($dom_entities as $dom_entity) {
      $guid = $dom_entity->nodeValue;
    }

    return $guid;
  }

}

/**
 * MS Dynamics SOAP update query builder.
 *
 * This should only be used via
 * clients_connection_ms_dynamics_soap::remote_entity_save().
 */
class MSDynamicsSoapUpdateQuery extends RemoteEntityUpdateQuery {

  /**
   * Execute the query.
   *
   * The entity must already have been set with setEntity().
   */
  function execute() {
    // Make the initial connection.
    $this->connection->connect();
    // Get the query header.
    $action = 'Update';
    $query_pieces['header'] = $this->connection->get_query_header($action);

    $query_pieces['body'] = $this->get_query_body();

    $query_pieces['footer'] = '</s:Envelope>';

    $requestXML =
      $query_pieces['header'] .
      $query_pieces['body'] .
      $query_pieces['footer'];

    //dsm($requestXML, '$requestXML');

    // Debug mode output.
    $this->connection->debug($requestXML);

    $response = $this->connection->GetSOAPResponse($this->connection->endpoint, $requestXML);

    // Debug mode output.
    $this->connection->debug($response);

    // Parse the response XML to check for an error. There is nothing useful
    // in the response to return.
    $this->parse_response($response);
  }

  /**
   * Get the query body XML.
   */
  function get_query_body() {
    $body_xml = '';
    $body_xml .= '
      <s:Body>
        <Update xmlns="http://schemas.microsoft.com/xrm/2011/Contracts/Services"  xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
          <entity xmlns:a="http://schemas.microsoft.com/xrm/2011/Contracts">
            <a:Attributes xmlns:b="http://schemas.datacontract.org/2004/07/System.Collections.Generic">';

    $body_xml .= $this->get_query_attributes();

    $body_xml .= '
            </a:Attributes>
            <a:EntityState i:nil="true" />
            <a:FormattedValues xmlns:b="http://schemas.datacontract.org/2004/07/System.Collections.Generic" />
            <a:Id>' . strip_tags($this->entity->remote_id) . '</a:Id>
            <a:LogicalName>' . strip_tags($this->remote_base) . '</a:LogicalName>
            <a:RelatedEntities xmlns:b="http://schemas.datacontract.org/2004/07/System.Collections.Generic" />
          </entity>
        </Update>
      </s:Body>';

    return $body_xml;
  }

  /**
   * Helper for get_query_body().
   *
   * Build the XML string for the attributes to insert.
   */
  function get_query_attributes() {
    $property_map = $this->entity_info['property map'];

    // If the fields to update have been specified, then filter the property
    // map to just these. Contrarily, if no fields were specified with
    // $update_fields, we update them all.
    // See addField() / addFields() in the parent class.
    if (count($this->update_fields)) {
      $property_map = array_intersect($property_map, $this->update_fields);
    }

    $attribute_builder = new ClientMSDynamicsQueryAttributeBuilder($this->base_entity_type, $this->remote_base, $this->entity, $this->table_info);
    $attributes_xml = $attribute_builder->get_query_attributes($property_map);

    return $attributes_xml;
  }

  /**
   * Helper for execute(). Parse the XML response and handle errors.
   *
   * @param $response
   *  The XML response from SOAP.
   *
   * @throws Exception
   *  Throws an exception if the remote response contains an error message.
   */
  function parse_response($response) {
    $response_dom = new DomDocument();
    $response_dom->loadXML($response);

    // See if there is an error.
    $dom_entities_fault = $response_dom->getElementsByTagName('Fault');
    if ($dom_entities_fault->length) {
      // Yes, use of SimpleXML here is a total mishmash.
      // TODO: cleanup. Ideally by someone who knows about XML parsing!
      $xml = simplexml_load_string($response);
      $result = $xml->xpath('//s:Reason/s:Text');
      $text = (string) $result[0];

      watchdog('clients_ms_dynamics_soap', $text, array(), WATCHDOG_CRITICAL);
      throw new Exception($text);
    }

    // There is nothing to do if there is no error: nothing useful is returned
    // from XRM.
  }

}

/**
 * MS Dynamics SOAP bulk save query builder.
 *
 * This creates an Execute SOAP query.
 *
 * This should only be used via
 * clients_connection_ms_dynamics_soap::remote_entity_save_multiple().
 */
class MSDynamicsSoapBulkSaveQuery extends RemoteEntityBulkSaveQuery {

  /**
   * An index of the entity ids that are saved.
   *
   * This is keyed by the order that XRM receives them in. Values are the entity
   * ids. This allows matching up with what is returned from XRM.
   */
  public $entities_index = array();

  /**
   * Execute the bulk save query.
   *
   * The entities must already have been set with setEntities().
   *
   * @return
   *  An array of the GUIDs that were created by the bulk save, keyed by the
   *  corresponding entity IDs.
   */
  function execute() {
    // Make the initial connection.
    $this->connection->connect();
    // Get the query header.
    $action = 'Execute';
    $query_pieces['header'] = $this->connection->get_query_header($action);

    $query_pieces['body'] = $this->get_query_body();

    $query_pieces['footer'] = '</s:Envelope>';

    $requestXML =
      $query_pieces['header'] .
      $query_pieces['body'] .
      $query_pieces['footer'];

    // Debug mode output.
    $this->connection->debug($requestXML);

    $response = $this->connection->GetSOAPResponse($this->connection->endpoint, $requestXML);

    // Debug mode output.
    $this->connection->debug($response);

    // Parse the response to get the GUIDs for the entities that were inserted
    // remotely.
    $insert_guids = $this->parse_response($response);

    return $insert_guids;
  }

  /**
   * Get the query body XML for bulk save.
   */
  function get_query_body() {
    $body_xml = '';
    $body_xml .= '
      <s:Body>
        <Execute xmlns="http://schemas.microsoft.com/xrm/2011/Contracts/Services" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
          <request i:type="a:ExecuteMultipleRequest" xmlns:a="http://schemas.microsoft.com/xrm/2011/Contracts">
            <a:Parameters xmlns:b="http://schemas.datacontract.org/2004/07/System.Collections.Generic">
              <a:KeyValuePairOfstringanyType>
                <b:key>Requests</b:key>
                <b:value i:type="c:OrganizationRequestCollection" xmlns:c="http://schemas.microsoft.com/xrm/2012/Contracts">' . "\n";

    // Add each entity to the query XML.
    foreach ($this->entities as $entity) {
      $body_xml .= $this->get_query_entity_request($entity);

      // Keep track of the order in which we pass these to XRM, so we can match
      // up the returned GUIDs in parse_response().
      $this->entities_index[] = $entity->identifier();
    }

    $body_xml .= '
                </b:value>
              </a:KeyValuePairOfstringanyType>
              <a:KeyValuePairOfstringanyType>
                <b:key>Settings</b:key>
                <b:value i:type="c:ExecuteMultipleSettings" xmlns:c="http://schemas.microsoft.com/xrm/2012/Contracts">
                  <c:ContinueOnError>false</c:ContinueOnError>
                  <c:ReturnResponses>true</c:ReturnResponses>
                </b:value>
              </a:KeyValuePairOfstringanyType>
            </a:Parameters>
            <a:RequestId i:nil="true" />
            <a:RequestName>ExecuteMultiple</a:RequestName>
          </request>
        </Execute>
      </s:Body>';

    //dsm($body_xml);

    return $body_xml;
  }

  /**
   * Get the query body XML for a single entity in the bulk save.
   *
   * @param $entity
   *  A single entity to generate XML for.
   *
   * @return
   *  The OrganizationRequest XML element for the entity.
   */
  function get_query_entity_request($entity) {
    // Get the complete property map for the entity.
    $property_map = $this->entity_info['property map'];

    //dsm($entity, 'entity in query');
    // Determine whether this is an insert or an update. An entity not yet
    // saved remotely will have no remote ID property.
    if (empty($entity->remote_id)) {
      // This is an entity which is being created remotely, as it has not
      // been previously saved remotely.
      $request = 'CreateRequest';
      $requestname = 'Create';

      // Set an empty remote ID.
      $remote_id = '00000000-0000-0000-0000-000000000000';

      // For a create operation, we ignore a passed in list of fields to
      // use, and use the whole property map.
    }
    else {
      // This is an entity which is being updated remotely, as it has previously
      // been saved remotely.
      $request = 'UpdateRequest';
      $requestname = 'Update';

      $remote_id = $entity->remote_id;

      // If the fields to update have been specified, then filter the property
      // map to just these. Contrarily, if no fields were specified with
      // $update_fields, we update them all.
      if (count($this->update_fields)) {
        $property_map = array_intersect($property_map, $this->update_fields);
      }
    }

    $body_entity_xml = '';
    $body_entity_xml .= '
                  <c:OrganizationRequest i:type="a:' . $request . '">
                    <a:Parameters>
                      <a:KeyValuePairOfstringanyType>
                        <b:key>Target</b:key>
                        <b:value i:type="a:Entity">
                          <a:Attributes>' . "\n";

    // Build the XML attributes for the entity.
    $attribute_builder = new ClientMSDynamicsQueryAttributeBuilder($this->base_entity_type, $this->remote_base, $entity, $this->table_info);
    $attributes_xml = $attribute_builder->get_query_attributes($property_map);
    //dsm($attributes_xml, '$attributes_xml');

    $body_entity_xml .= $attributes_xml;

    // Close the XML for the single entity.
    $body_entity_xml .= '
                          </a:Attributes>
                          <a:EntityState i:nil="true" />
                          <a:FormattedValues />
                          <a:Id>' . $remote_id . '</a:Id>
                          <a:LogicalName>' . strip_tags($this->remote_base) . '</a:LogicalName>
                          <a:RelatedEntities />
                        </b:value>
                      </a:KeyValuePairOfstringanyType>
                    </a:Parameters>
                    <a:RequestId i:nil="true" />
                    <a:RequestName>' . $requestname . '</a:RequestName>
                  </c:OrganizationRequest>' . "\n";

    return $body_entity_xml;
  }

  /**
   * Helper for execute(). Parse the XML response and handle errors.
   *
   * @param $response
   *  The XML response from SOAP.
   *
   * @return
   *  An array of guids for remote entities that have been created as a result
   *  of the bulk operation. These are keyed by the entity ID of the entity they
   *  are for.
   *
   * @throws Exception
   *  Throws an exception if the remote response contains an error message.
   */
  function parse_response($response) {
    $response_dom = new DomDocument();
    $response_dom->loadXML($response);
    $xpath = new DOMXPath($response_dom);

    // First check whether there was an overall error.
    /*
    // This crashes when there is a specific error with an entity.
    // TODO: investigate further.
    $dom_entities_fault = $response_dom->getElementsByTagName('Fault');
    if ($dom_entities_fault->length) {
      $xpath = new DOMXPath($response_dom);
      $fault_text_node_list = $xpath->query("//*[local-name()='Reason']/*[local-name()='Text']");
      $text = $fault_text_node_list->item(0)->textContent;
      watchdog('clients_ms_dynamics_soap', $text, array(), WATCHDOG_CRITICAL);
      throw new Exception($text);
    }
    */

    // The meat of the response is the ExecuteResult element, which has a child
    // Results, which contains two key-value pairs...
    $execute_result_entities = $xpath->query("//*[local-name()='ExecuteResult']/*[local-name()='Results']/*[local-name()='KeyValuePairOfstringanyType']");

    // The first one has information about whether the whole request has any
    // faults.
    $result_faulted_node = $execute_result_entities->item(0);
    $result_faulted_value_node = $result_faulted_node->getElementsByTagName('value');
    if ($result_faulted_value_node->item(0)->textContent == 'true') {
      // There may be multiple error messages in a bulk update query: assemble
      // them all.
      $messages = array();
      foreach ($xpath->query("//*[local-name()='Fault']//*[local-name()='Message']") as $message_node) {
        $messages[] = $message_node->textContent;
      }

      $text = implode(', ', $messages);
      watchdog('clients_ms_dynamics_soap', $text, array(), WATCHDOG_CRITICAL);
      throw new Exception($text);
    }

    // Move on to the second key-value pair in the results. This has the
    // actual result data!
    // Each entity we passed in gets a ExecuteMultipleResponseItem item with
    // response data related to that entity.
    $result_data_node_list = $execute_result_entities->item(1)->getElementsByTagName('ExecuteMultipleResponseItem');

    $guids = array();
    foreach ($result_data_node_list as $result_data_node) {
      $result_type = $result_data_node->getElementsByTagName('ResponseName')->item(0)->textContent;

      // Only return something for a CreateResponse.
      if ($result_type == 'Create') {
        $result_index = $result_data_node->getElementsByTagName('RequestIndex')->item(0)->textContent;
        $guid = $result_data_node->getElementsByTagName('value')->item(0)->textContent;

        // Get the entity ID for this entity from the index.
        $entity_id = $this->entities_index[$result_index];

        // Key by entity ID in the guid array.
        $guids[$entity_id] = $guid;
      }
    }

    return $guids;
  }

}

/**
 * MS Dynamics SOAP SetStateRequest query builder.
 *
 * MS Dynamics uses a special type of Execute query for a request to set a state
 * on an MSD entity.
 *
 * This inherits from RemoteEntitySaveQuery as it fundamentally is a sort of
 * save operation on an entity, and that has methods for setting the entity.
 *
 * There's no API in the resource to use this, as it's somewhat special to MS
 * Dynamics. The following is an example of how to use this:
 * @code
 * $resource = clients_resource_get_for_component('remote_entity', $entity_type);
 * $connection = $resource->getConnection();
 * $query = $connection->getRemoteEntityQuery('set_state');
 * $query->setEntity($entity_type, $entity);
 * $query->requestParameter(PARAMETER, VALUE);
 * $success = $query->execute();
 * @endcode
 */
class MSDynamicsSoapSetStateRequestQuery extends RemoteEntitySaveQuery {

  public $parameters = array();

  /**
   * Set a parameter on the query.
   */
  public function requestParameter($parameter_name, $value) {
    $this->parameters[$parameter_name] = $value;
  }

  /**
   * Execute the query.
   *
   * The entity must already have been set with setEntity().
   *
   * @return
   *  TRUE if the request executed successfully.
   */
  function execute() {
    // Make the initial connection.
    $this->connection->connect();
    // Get the query header.
    $action = 'Execute';
    $query_pieces['header'] = $this->connection->get_query_header($action);

    $query_pieces['body'] = $this->get_query_body();

    $query_pieces['footer'] = '</s:Envelope>';

    $requestXML =
      $query_pieces['header'] .
      $query_pieces['body'] .
      $query_pieces['footer'];

    // Debug mode output.
    $this->connection->debug($requestXML);

    $response = $this->connection->GetSOAPResponse($this->connection->endpoint, $requestXML);

    // Debug mode output.
    $this->connection->debug($response);

    // Parse the response XML to check for an error. There is nothing useful
    // in the response to return.
    return $this->parse_response($response);
  }

  /**
   * Get the query body XML.
   */
  function get_query_body() {
    $body_xml = '';
    $body_xml .= '
      <s:Body>
        <Execute xmlns="http://schemas.microsoft.com/xrm/2011/Contracts/Services" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
          <request i:type="b:SetStateRequest" xmlns:a="http://schemas.microsoft.com/xrm/2011/Contracts" xmlns:b="http://schemas.microsoft.com/crm/2011/Contracts">
            <a:Parameters xmlns:b="http://schemas.datacontract.org/2004/07/System.Collections.Generic">';

    $body_xml .= $this->get_query_parameters();

    $body_xml .= '
            </a:Parameters>
            <a:RequestId i:nil="true" />
            <a:RequestName>SetState</a:RequestName>
          </request>
        </Execute>
      </s:Body>';

    return $body_xml;
  }

  /**
   * Get the parameters for the execute query.
   */
  function get_query_parameters() {
    // This can't use ClientMSDynamicsQueryAttributeBuilder because we're not
    // setting actual entity properties, but parameters for the request.
    $parameters_xml = '
              <a:KeyValuePairOfstringanyType>
                <b:key>EntityMoniker</b:key>
                <b:value i:type="a:EntityReference">
                  <a:Id>' . $this->entity->remote_id . '</a:Id>
                  <a:LogicalName>' . $this->remote_base . '</a:LogicalName>
                  <a:Name i:nil="true" />
                </b:value>
              </a:KeyValuePairOfstringanyType>';

    foreach ($this->parameters as $parameter_name => $value) {
      $parameters_xml .= '
              <a:KeyValuePairOfstringanyType>
                <b:key>' . $parameter_name . '</b:key>
                <b:value i:type="a:OptionSetValue">
                  <a:Value>' . $value . '</a:Value>
                </b:value>
              </a:KeyValuePairOfstringanyType>';
    }
    return $parameters_xml;
  }

  /**
   * Helper for execute(). Parse the XML response and handle errors.
   *
   * @param $response
   *  The XML response from SOAP.
   *
   * @return
   *  TRUE if the request completed successfully.
   *
   * @throws Exception
   *  Throws an exception if the remote response contains an error message.
   */
  function parse_response($response) {
    $response_dom = new DomDocument();
    $response_dom->loadXML($response);
    $xpath = new DOMXPath($response_dom);

    $fault_text_node_list = $xpath->query("//*[local-name()='Fault']/*[local-name()='Reason']/*[local-name()='Text']");
    $fault_text_node_list_item = $fault_text_node_list->item(0);
    if (!empty($fault_text_node_list_item)) {
      $text = "Remote query returned a fault. Remote fault text is: " . $fault_text_node_list->item(0)->textContent;
      throw new Exception($text);
    }

    $response_name = $xpath->query("//*[local-name()='ExecuteResult']/*[local-name()='ResponseName']");
    $response_name_text = $response_name->item(0)->textContent;
    if ($response_name_text != 'SetState') {
      throw new Exception("Something went wrong with the SetStateRequest.");
    }

    return TRUE;
  }

}

/**
 * Helper for building attributes XML for insert and update queries.
 *
 * Should be instantiated to make calling variable methods easier. Given a
 * property map, returns the XML for those attributes.
 *
 * Example usage:
 * @code
 * $attribute_builder = new ClientMSDynamicsQueryAttributeBuilder($this->base_entity_type, $this->remote_base, $this->entity, $this->table_info);
 * $attributes_xml = $attribute_builder->get_query_attributes($property_map);
 * @endcode
 *
 * TODO: change MSDynamicsSoapInsertQuery to make use of this.
 *
 * @see ClientMSDynamicsQueryAttributeBuilder::get_query_attributes()
 */
class ClientMSDynamicsQueryAttributeBuilder {

  /**
   * Constructor.
   *
   * @param $entity_type
   *  The entity type to work with.
   * @param $remote_base
   *  The remote base table.
   * @param $entity
   *  The entity to take values from.
   * @param $table_info
   *  The remote base's table info.
   */
  function __construct($entity_type, $remote_base, $entity, $table_info) {
    $this->entityType = $entity_type;
    $this->remote_base = $remote_base;
    $this->entity = $entity;
    $this->table_info = $table_info;

    $this->entity_info = entity_get_info($entity_type);
  }

  /**
   * Build the XML string for the attributes to insert.
   *
   * Values for attributes are taken from the entity's metadata wrapper, using
   * the property map defined in hook_entity_info().
   *
   * If a property's value is NULL, it is not included in the attributes.
   *
   * @param $property_map
   *  A property map of properties to add. This should be in the same format as
   *  the 'property map' in the entity info, but need not contain all of the
   *  items from hook_entity_info().
   *
   * @return
   *  An XML string.
   */
  function get_query_attributes($property_map) {
    // Get info on fields, to check their type.
    if (isset($this->table_info[$this->remote_base]['fields'])) {
      $field_info = $this->table_info[$this->remote_base]['fields'];
    }
    else {
      $field_info = array();
    }

    $wrapper = entity_metadata_wrapper($this->entityType, $this->entity);

    $attributes_xml = '';

    $id_property = $this->entity_info['remote entity keys']['remote id'];

    foreach ($property_map as $property_name => $remote_property_name) {
      // Skip the remote id property: we don't set this on insert.
      if ($remote_property_name == $id_property) {
        continue;
      }

      // Get the value from the wrapper.
      $value = $wrapper->{$property_name}->raw();

      // Skip NULL values.
      if (is_null($value)) {
        continue;
      }

      // Get the type from hook_remote_entity_query_table_info(), if defined
      // there. Otherwise, default to 'string'.
      if (isset($field_info[$remote_property_name])) {
        $type = $field_info[$remote_property_name]['type'];
      }
      else {
        $type = 'string';
        // Fake this into the field info, since it wasn't there, so we can
        // pass it to get_query_attribute_TYPE().
        $field_info[$remote_property_name] = array(
          'type' => $type,
        );
      }

      $attributes_xml .= $this->method_invoke_get_query_attribute($field_info[$remote_property_name], $remote_property_name, $value);
    } // foreach ($property_map

    return $attributes_xml;
  }

  /**
   * Hand over production of the attribute to the right method.
   *
   * @param $property_field_info
   *  The info for this field from hook_remote_entity_query_table_info(). This
   *  should have the 'type' property set.
   * @param $remote_property_name
   *  The name of the remote property.
   * @param $value
   *  The value to set.
   *
   * @return
   *  An XML string containing the KeyValuePairOfstringanyType element.
   */
  function method_invoke_get_query_attribute($property_field_info, $remote_property_name, $value) {
    $type = $property_field_info['type'];
    // Farm out to helper methods of the form get_query_attribute_TYPE().
    $method = 'get_query_attribute_' . $type;
    if (!method_exists($this, $method)) {
      // Fall back to the generic method if there is not a specific one.
      $method = 'get_query_attribute_generic';
    }
    $attribute_xml = $this->$method($property_field_info, $remote_property_name, $value);
    return $attribute_xml;
  }

  /**
   * Implements get_query_attribute_TYPE().
   *
   * Get query attribute XML for a value of generic type.
   *
   * @param $property_field_info
   *  The info for this field from hook_remote_entity_query_table_info(). This
   *  should have the 'type' property set.
   * @param $remote_property_name
   *  The name of the remote property.
   * @param $value
   *  The value to set.
   *
   * @return
   *  An XML string containing the KeyValuePairOfstringanyType element.
   */
  function get_query_attribute_generic($property_field_info, $remote_property_name, $value) {
    // Handles 'string' and 'int'.
    $attribute_xml = '
              <a:KeyValuePairOfstringanyType>
                <b:key>' . $remote_property_name . '</b:key>
                <b:value i:type="c:' . $property_field_info['type'] . '" xmlns:c="http://www.w3.org/2001/XMLSchema">' . strip_tags($value) . '</b:value>
              </a:KeyValuePairOfstringanyType>';
    return $attribute_xml;
  }

  /**
   * Implements get_query_attribute_TYPE().
   *
   * Get query attribute XML for a value of type 'OptionSetValue'.
   */
  function get_query_attribute_OptionSetValue($property_field_info, $remote_property_name, $value) {
    $attribute_xml = '
              <a:KeyValuePairOfstringanyType>
                <b:key>' . $remote_property_name . '</b:key>
                <b:value i:type="a:OptionSetValue">
                  <a:Value>' . strip_tags($value) . '</a:Value>
                </b:value>
              </a:KeyValuePairOfstringanyType>';
    return $attribute_xml;
  }

  /**
   * Implements get_query_attribute_TYPE().
   *
   * Get query attribute XML for a value of type 'dateTime'.
   *
   * The value is expected to be of the form: '2013-02-25T00:00:00'; dates
   * formatted with the PHP date format constant DATE_W3C also work.
   */
  function get_query_attribute_dateTime($property_field_info, $remote_property_name, $value) {
    $attribute_xml = '
              <a:KeyValuePairOfstringanyType>
                <b:key>' . $remote_property_name . '</b:key>
                <b:value i:type="c:dateTime" xmlns:c="http://www.w3.org/2001/XMLSchema">' . strip_tags($value) . '</b:value>
              </a:KeyValuePairOfstringanyType>';
    return $attribute_xml;
  }

  /**
   * Implements get_query_attribute_TYPE().
   *
   * Get query attribute XML for a value of type 'EntityReference'.
   *
   * Expects the field info for the remote field to have 'entity type' set.
   *
   * @throws Exception
   *  Throws an exception if the value is empty.
   */
  function get_query_attribute_EntityReference($property_field_info, $remote_property_name, $value) {
    // Check the value for the GUID is non-empty.
    if (empty($value)) {
      list($id) = entity_extract_ids($this->entityType, $this->entity);
      throw new Exception(t("Empty value for GUID in entity reference remote field @field-name on !entity-type id !id", array(
        '@field-name' => $remote_property_name,
        '!entity-type' => $this->entityType,
        '!id' => $id,
      )));
    }

    // For now, we only allow setting an entity reference field where the
    // type of the entity is fixed in the field info.
    // TODO?: allow the type to come in with the data, if/when needed.
    $attribute_xml = '
              <a:KeyValuePairOfstringanyType>
                <b:key>' . $remote_property_name . '</b:key>
                <b:value i:type="a:EntityReference">
                  <a:Id>' . strip_tags($value) . '</a:Id>
                  <a:LogicalName>' . $property_field_info['entity type'] . '</a:LogicalName>
                  <a:Name i:nil="true" />
                </b:value>
              </a:KeyValuePairOfstringanyType>';
    return $attribute_xml;
  }

  /**
   * Implements get_query_attribute_TYPE().
   *
   * Get query attribute XML for a value of type 'EntityCollection'.
   *
   * Expects the field info for the remote field to have 'entity type' set.
   */
  function get_query_attribute_EntityCollection($property_field_info, $remote_property_name, $value) {
    // This is horrendous. Works for now, will need an overhaul if it needs
    // to be used in other cases! Main roblem is I only have one example to go
    // on and so don't understand how this can vary. In particular:
    // Does this always have a nested entity?
    $nested_type = $property_field_info['nested']['entity type'];
    // Is the nested GUID always zero?
    $nested_guid = '00000000-0000-0000-0000-000000000000';
    // Get the property on the nested entity that we need to set.
    $nested_entity_property = $property_field_info['nested']['value field'];
    // Get the table info for the nested entity's table.
    $nested_type_table_info = $this->table_info[$nested_type];

    $attribute_xml = '';
    $attribute_xml .= '
              <a:KeyValuePairOfstringanyType>
                <b:key>' . $remote_property_name . '</b:key>
                <b:value i:type="a:EntityCollection">
                  <a:Entities>
                    <a:Entity>
                      <a:Attributes>';

    // Next question: do we ever need to set more than one property on the
    // nested entity?
    // For now, assume not, and just get the first out.
    $value = array_shift($value);
    // Create the attribute XML for the nested entity's property.
    $attribute_xml .= $this->method_invoke_get_query_attribute($nested_type_table_info['fields'][$nested_entity_property], $nested_entity_property, $value);

    $attribute_xml .= '
                      </a:Attributes>
                      <a:EntityState i:nil="true" />
                      <a:FormattedValues />
                      <a:Id>' . $nested_guid . '</a:Id>
                      <a:LogicalName>' . $nested_type . '</a:LogicalName>
                      <a:RelatedEntities />
                    </a:Entity>
                  </a:Entities>
                  <a:EntityName i:nil="true" />
                  <a:MinActiveRowVersion i:nil="true" />
                  <a:MoreRecords>false</a:MoreRecords>
                  <a:PagingCookie i:nil="true" />
                  <a:TotalRecordCount>0</a:TotalRecordCount>
                  <a:TotalRecordCountLimitExceeded>false</a:TotalRecordCountLimitExceeded>
                </b:value>
              </a:KeyValuePairOfstringanyType>';
    return $attribute_xml;
  }

}
