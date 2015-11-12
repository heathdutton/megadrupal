<?php
/**
 * @file
 * Base Query class.
 */

namespace Drupal\clients_suitecrm\RemoteEntity\Query;

/**
 * SuiteCRM REST base select query builder.
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
class SuiteCrmRestBaseQuery extends \RemoteEntityQuery implements \QueryPlaceholderInterface {

  /**
   * The related clients resource.
   *
   * @var clients_resource
   */
  protected $clientsResource;

  /**
   * A unique identifier for this query object.
   */
  protected $uniqueIdentifier;

  /**
   * The placeholder counter.
   */
  protected $nextPlaceholder = 0;

  /**
   * {@inheritdoc}
   */
  public function __construct($connection) {
    parent::__construct($connection);
    // Set unique identifier.
    $this->uniqueIdentifier = uniqid('', TRUE);
  }

  /**
   * Returns the related clients resource.
   *
   * This needs the entity type set to work.
   *
   * @return clients_resource
   *   The related clients resource.
   */
  public function getClientsResource() {
    if (!$this->clientsResource) {
      $this->clientsResource = clients_resource_load($this->base_entity_type);
    }
    return $this->clientsResource;
  }

  /**
   * Returns a unique identifier for this object.
   */
  public function uniqueIdentifier() {
    return $this->uniqueIdentifier;
  }

  /**
   * Gets the next placeholder value for this query object.
   *
   * @return int
   *   Next placeholder value.
   */
  public function nextPlaceholder() {
    return $this->nextPlaceholder++;
  }

  /**
   * Builds a SQL query from a given conditions array.
   *
   * @param array $conditions
   *   Array of conditions.
   *
   * @return string
   *   The SQL query with inserted placeholders.
   */
  protected function buildConditionQuery($conditions) {
    $table_name = $this->getClientsResource()->configuration['fields']['table_name'];

    // Build conditions object.
    $condition = db_and();
    foreach ($conditions as $remote_condition) {
      $condition->condition($table_name . '.' . $remote_condition['field'], $remote_condition['value'], $remote_condition['operator']);
    }
    // @TODO How can we ensure this connection fits the requirements of the db
    // suiteCRM runs on?
    $connection = \Database::getConnection();

    // Compile condition to create query syntax.
    $condition->compile($connection, $this);

    // Compile the arguments as well. They need to be quoted before they can be
    // used.
    $arguments = array();
    foreach ((array) $condition->arguments() as $key => $val) {
      $arguments[$key] = $connection->quote($val);
    }
    // Fetch compiled query and replace placeholders.
    $query = strtr((string) $condition, $arguments);

    return $query;
  }
}
