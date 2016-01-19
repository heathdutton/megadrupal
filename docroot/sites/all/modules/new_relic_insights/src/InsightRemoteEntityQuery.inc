<?php

/**
 * @file
 * Contains the remote entity query class for New Relic Insights.
 */


class InsightRemoteEntityQuery extends RemoteEntityQuery {

  /**
   * The maximum supported query limit on New Relic Insights.
   */
  const MAX_LIMIT = 1000;

  /**
   * The raw NRQL query string for this entity query.
   *
   * @var string
   */
  protected $nrql = '';

  /**
   * The raw response from New Relic Insights.
   */
  protected $response;

  /**
   * An array of conditions on the query.
   *
   * @var array
   */
  protected $conditions = array();

  /**
   * An array of "in" or "out" conditions on the query.
   *
   * @var array
   */
  protected $inOutConditions = array(
    'in' => array(),
    'out' => array(),
  );

  /**
   * An array representing the time constraints for this particular query. By
   * default, the past day's events are returned, but the ability to query back
   * further and specify date ranges is supported.
   *
   * @see InsightRemoteEntityQuery::timeCondition()
   *
   * @var array
   */
  protected $timeCondition = array(
    'since' => NULL,
    'until' => NULL,
  );

  /**
   * An array representing the range for this particular query. Note that,
   * unlike EntityFieldQuery, the start field is always ignored and assumed 0
   * because Insights does not support explicit starts / paging. Range defaults
   * to 100.
   *
   * @var array
   */
  protected $range = array(
    'start' => 0,
    'length' => 100,
  );

  /**
   * TRUE if this is a count query. FALSE if it isn't.
   *
   * @var bool
   */
  protected $count = FALSE;

  /**
   * TRUE if this is a manual query.
   *
   * @var bool
   */
  protected $manualQuery = FALSE;

  /**
   * @todo Document this.
   * @param $name
   * @param $value
   * @param string $operator
   */
  function entityCondition($name, $value, $operator = '=') {
    switch ($name) {
      case 'entity_id':
        $this->propertyCondition('iid', $value, $operator);
        break;

      // Ignore the entity_type condition.
      case 'entity_type':
        break;

      // Primary case for this is the bundle.
      case 'bundle':
        $this->remote_base = $value;
        break;

      // Default to treating this the same as a property condition.
      default:
        $this->propertyCondition($name, $value, $operator);

    }
  }

  /**
   * @todo Document this.
   * @param $property_name
   * @param $value
   * @param string $operator
   */
  function propertyCondition($property_name, $value, $operator = '=') {
    // Special handling for operators.
    switch ($operator) {
      // NRQL prefers != over <>.
      case '<>':
        $operator = '!=';
        break;

      case 'CONTAINS':
        $operator = 'LIKE';
        $value = '%' . $value . '%';
        break;

      case 'STARTS_WITH':
        $operator = 'LIKE';
        $value = $value . '%';
        break;

      case 'IN':
        $this->inOutConditions['in'][$property_name] = array();
        foreach ($value as $val) {
          $this->inOutConditions['in'][$property_name][] = array(
            'field' => $property_name,
            'value' => $val,
            'operator' => '=',
          );
        }
        return;
        break;

      case 'NOT IN':
        foreach ($value as $val) {
          $this->inOutConditions['out'][] = array(
            'field' => $property_name,
            'value' => $val,
            'operator' => '!=',
          );
        }
        return;
        break;
  }

    // Special handling for the timestamp field.
    if ($property_name == 'timestamp') {
      $this->timeCondition($value, $operator);
      return;
    }

    // Passing an entity, field, or property condition of "NRQL" overrides the
    // NRQL query used to query Insights.
    if ($property_name == 'NRQL') {
      $this->setNRQL($value);
      return;
    }

    $this->conditions[] = array(
      'field' => $property_name,
      'value' => $value,
      'operator' => $operator,
    );
  }

  /**
   * @todo Document this.
   * @param $value
   * @param $operator
   */
  function timeCondition($value, $operator) {
    // Normalize values.
    if (!is_array($value)) {
      $value = array($value);
    }
    foreach ($value as &$val) {
      // Strings will probably be of the form "x days ago"
      if (!is_numeric($val)) {
        $val = strtotime($val);
      }
    }

    // Apply appropriate time conditions based on the provided operator.
    switch ($operator) {
      case '>':
        $this->timeCondition['since'] = REQUEST_TIME - $value[0];
        break;

      case '>=':
        $this->timeCondition['since'] = REQUEST_TIME - $value[0] + 1;
        break;

      case '<':
        $this->timeCondition['until'] = REQUEST_TIME - $value[0];
        break;

      case '<=':
        $this->timeCondition['until'] = REQUEST_TIME - $value[0] - 1;
        break;

      case '=':
        // We can fake equality by looking for events within a single second.
        $this->timeCondition['since'] = REQUEST_TIME - $value[0] + 1;
        $this->timeCondition['until'] = REQUEST_TIME - $value[0] - 1;
        break;

      case 'BETWEEN':
        $this->timeCondition['since'] = REQUEST_TIME - min($value);
        $this->timeCondition['until'] = REQUEST_TIME - max($value);
        break;

      case 'NOT BETWEEN':
      case '!=':
      default:
        // Not supported.
        break;
    }
  }

  /**
   * @todo Document this. There's no differentiation in Insights of what is a
   * property vs. what is a field, so just treat them the same.
   * @param $property_name
   * @param $column
   * @param $value
   * @param string $operator
   */
  function fieldCondition($property_name, $column, $value, $operator = '=') {
    $this->propertyCondition($property_name, $value, $operator);
  }

  /**
   * @todo Document this, particularly differences from EntityFieldQuery:range()
   * @param null $start
   * @param null $length
   */
  public function range($start = NULL, $length = NULL) {
    $this->range = array(
      'start' => empty($start) ? 0 : $start,
      'length' => empty($length) ? 100 : min($length, self::MAX_LIMIT),
    );
  }

  /**
   * Sets the query to be a count query only.
   */
  public function count() {
    $this->count = TRUE;
  }

  /**
   * Run the query and return a result.
   *
   * @return array|int
   *   Remote entity objects as retrieved from the remote connection. If this is
   *   a count query, the integer count will be returned instead.
   */
  function execute() {
    // If no NRQL query has yet been built, build it.
    if (empty($this->nrql)) {
      $this->buildNRQL();
    }

    // Execute the query via the remote connection.
    $this->response = $this->connection->callMethodArray('query', array('GET', array('nrql' => $this->nrql)));

    // If this is a simple count query, return the count.
    if ($this->count) {
      return $this->response->results[0]->count;
    }
    // If the query was manually written, return the full response body.
    elseif ($this->manualQuery) {
      return $this->response;
    }
    // Otherwise, return the array of events.
    else {
      return $this->response->results[0]->events;
    }
  }

  /**
   * Build the query from an EntityFieldQuery object.
   *
   * @param $efq
   *   The built-up EntityFieldQuery object.
   *
   * @return
   *   The current object. Helpful for chaining methods.
   */
  function buildFromEFQ($efq) {
    // Copy each of the property conditions.
    foreach ($efq->propertyConditions as $condition) {
      $this->propertyCondition($condition['column'], $condition['value'], $condition['operator']);
    }

    // Copy each entity condition.
    foreach ($efq->entityConditions as $property => $condition) {
      $this->entityCondition($property, $condition['value'], $condition['operator']);
    }

    // Copy each field condition.
    foreach ($efq->fields as $condition) {
      $this->fieldCondition($condition['field'], 'value', $condition['value']);
    }

    // Copy the count value.
    if ($efq->count) {
      $this->count();
    }

    // Copy the range value.
    if (!empty($efq->range)) {
      $this->range($efq->range['start'], $efq->range['length']);
    }

    // There's no way to easily apply a LIMIT through the Views UI, but we can
    // infer a limit based on the pager options, if they're set.
    if (isset($efq->metaData['view']->query->pager->options['items_per_page'])) {
      $limit = $efq->metaData['view']->query->pager->options['items_per_page'];
    }
    if (isset($efq->metaData['view']->query->pager->options['total_pages'])) {
      $total_pages = $efq->metaData['view']->query->pager->options['total_pages'];
      if (!empty($total_pages)) {
        $limit = isset($limit) ? $limit * $total_pages : $total_pages;
      }
    }
    if (isset($limit)) {
      $this->range(NULL, $limit);
    }

    return $this;
  }

  /**
   * Builds a NRQL query, based on conditions provided.
   *
   * @return string
   *   A NRQL query suitable for use in Insights' Query API.
   */
  public function buildNRQL() {
    // Default to the current remote base.
    $bundle = empty($this->remote_base) ? 'Transaction' : $this->remote_base;

    // Define the query base.
    if (is_array($bundle)) {
      $bundle = '`' . implode('`, `', $bundle) . '`';
    }
    else {
      $bundle = $this->quote($bundle);
    }

    if ($this->count) {
      $nrql = 'SELECT COUNT(*) FROM ';
    }
    else {
      $nrql = 'SELECT * FROM ';
    }

    $nrql .= check_plain($bundle);

    // Iterate through all provided conditions.
    if (!empty($this->conditions)) {
      $nrql .= ' WHERE ';

      foreach ($this->conditions as $id => $condition) {
        if ($id > 0) {
          $nrql .= ' AND ';
        }

        if (empty($condition['operator'])) {
          $condition['operator'] = '=';
        }

        $nrql .= $this->quote(check_plain($condition['field'])) . ' ';
        $nrql .= check_plain($condition['operator']) . ' ';
        $nrql .= "'" . check_plain($condition['value']) . "'";
      }
    }

    // Iterate through "in" conditions.
    if (!empty($this->inOutConditions['in'])) {
      if (strpos($nrql, ' WHERE ') === FALSE) {
        $nrql .= ' WHERE ';
      }
      else {
        $nrql .= ' AND ';
      }

      $in_count = 0;
      foreach ($this->inOutConditions['in'] as $field => $conditions) {
        if ($in_count > 0) {
          $nrql .= ' AND ';
        }

        $nrql .= '(';
        foreach ($conditions as $id => $condition) {
          if ($id > 0) {
            $nrql .= ' OR ';
          }

          $nrql .= $this->quote(check_plain($field)) . ' ';
          $nrql .= check_plain($condition['operator']) . ' ';
          $nrql .= "'" . check_plain($condition['value']) . "'";
        }

        $nrql .= ')';
        $in_count++;
      }
    }

    // Also iterate through any "out" conditions.
    if (!empty($this->inOutConditions['out'])) {
      if (strpos($nrql, ' WHERE ') === FALSE) {
        $nrql .= ' WHERE ';
      }
      else {
        $nrql .= ' AND ';
      }

      $nrql .= '(';
      foreach ($this->inOutConditions['out'] as $id => $condition) {
        if ($id > 0) {
          $nrql .= ' AND ';
        }

        $nrql .= $this->quote(check_plain($condition['field'])) . ' ';
        $nrql .= check_plain($condition['operator']) . ' ';
        $nrql .= "'" . check_plain($condition['value']) . "'";
      }
      $nrql .= ')';
    }

    // Apply time constraints, if any where provided.
    if (!empty($this->timeCondition['since'])) {
      $nrql .= ' SINCE ' . $this->timeCondition['since'] . ' seconds ago';

      if (!empty($this->timeCondition['until'])) {
        $nrql .= ' UNTIL ' . $this->timeCondition['until'] . ' seconds ago';
      }
    }

    $nrql .= ' LIMIT ' . check_plain($this->range['length']);

    // Set and return the query.
    $this->nrql = $nrql;
    return $this->nrql;
  }

  /**
   * Returns the NRQL query string for this entity query.
   *
   * @return string
   *   The NRQL query string for this entity query.
   */
  public function getNRQL() {
    return $this->nrql;
  }

  /**
   * Manually set the NRQL query string for this entity query. Useful for cases
   * when aggregation (beyond counts) is desired.
   *
   * @param string $nrql
   *   The NRQL query string to be used when executed.
   */
  public function setNRQL($nrql) {
    $this->manualQuery = TRUE;
    $this->nrql = $nrql;
  }

  /**
   * Returns a NRQL quoted attribute.
   *
   * @param string $attribute
   *   The attribute to be quoted.
   *
   * @return string
   *   The provided attribute, quoted per NRQL syntax.
   */
  protected function quote($attribute) {
    return '`' . $attribute . '`';
  }

}
