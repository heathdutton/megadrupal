<?php

/**
 * Migrate nodequeue roles configuration.
 */
class NodequeueRoleDrupal6Migration extends DrupalMigration {
  protected $source_qids;

  public function __construct($args = array()) {
    $args['description'] = 'Migration of nodequeue role configuration from Drupal 6';
    parent::__construct($args);
    $this->source_qids = empty($args['source_qids']) ? array() : $args['source_qids'];

    // Declare source and destination objects.
    $options = array('fix_field_names' => array(), 'map_joinable' => FALSE);
    $this->source      = new MigrateDrupal6SourceSql($this->query(), array(), NULL, $options);
    $this->destination = new NodequeueRoleDestination();

    // Map source with destination. Assume table schema is the same in D6 and D7.
    $key_schema = $this->destination->getKeySchema();
    $this->map = new MigrateSQLMap($this->machineName, $key_schema, $key_schema);

    // Declare field mappings.
    isset($args['migration_queues'])
      ? $this->addFieldMapping('qid', 'qid')->sourceMigration($args['migration_queues'])
      : $this->addFieldMapping('qid', 'qid');
    isset($args['migrations_roles'])
      ? $this->addFieldMapping('rid', 'rid')->sourceMigration($args['migrations_roles'])
      : $this->addFieldMapping('rid', 'rid');
  }

  /**
   * @override
   *
   * Get {nodequeue_roles} data.
   */
  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('nodequeue_roles', 'r')
      ->fields('r', array('qid', 'rid'));
    if (!empty($this->source_qids)) {
      $query = $query->condition('qid', $this->source_qids, 'IN');
    }
    return $query;
  }

}
