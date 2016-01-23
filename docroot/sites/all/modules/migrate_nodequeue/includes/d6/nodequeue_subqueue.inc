<?php

/**
 * Migrate subqueues.
 */
class NodequeueSubqueueDrupal6Migration extends DrupalMigration {
  protected $source_qids;

  public function __construct($args = array()) {
    $args['description'] = 'Migration of subqueues from Drupal 6';
    parent::__construct($args);
    $this->source_qids = empty($args['source_qids']) ? array() : $args['source_qids'];

    // Declare source and destination objects.
    $options = array('fix_field_names' => array(), 'map_joinable' => FALSE);
    $this->source      = new MigrateDrupal6SourceSql($this->query(), array(), NULL, $options);
    $this->destination = new NodequeueSubqueueDestination();

    // Map source with destination. Schema is the same in D6 and D7.
    $key_schema = $this->destination->getKeySchema();
    $this->map = new MigrateSQLMap($this->machineName, $key_schema, $key_schema);

    // Declare field mappings.
    isset($args['migration_queues'])
      ? $this->addFieldMapping('qid', 'qid')->sourceMigration($args['migration_queues'])
      : $this->addFieldMapping('qid', 'qid');
    $this->addSimpleMappings(array('reference', 'title'));
  }

  /**
   * @override
   *
   * Get {nodequeue_subqueue} data.
   */
  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('nodequeue_subqueue', 'sq')
      ->fields('sq', array('sqid', 'qid', 'reference', 'title'));
    if (!empty($this->source_qids)) {
      $query = $query->condition('qid', $this->source_qids, 'IN');
    }
    return $query;
  }

}
