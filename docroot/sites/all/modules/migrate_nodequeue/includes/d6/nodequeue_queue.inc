<?php

/**
 * Migrate nodequeues.
 */
class NodequeueQueueDrupal6Migration extends DrupalMigration {
  protected $source_qids;

  public function __construct($args = array()) {
    $args['description'] = 'Migration of nodequeues from Drupal 6';
    parent::__construct($args);
    $this->source_qids = empty($args['source_qids']) ? array() : $args['source_qids'];

    // Declare source and destination objects.
    $options = array('fix_field_names' => array(), 'map_joinable' => FALSE);
    $this->source      = new MigrateDrupal6SourceSql($this->query(), array(), NULL, $options);
    $this->destination = new NodequeueQueueDestination();

    // Map source with destination. Schema is the same in D6 and D7.
    $key_schema = $this->destination->getKeySchema();
    $this->map = new MigrateSQLMap($this->machineName, $key_schema, $key_schema);

    // Declare field mappings.
    $this->addSimpleMappings(array('name', 'title', 'subqueue_title', 'size', 'link', 'link_remove', 'owner', 'show_in_ui', 'show_in_tab', 'show_in_links', 'reference', 'reverse', 'i18n'));
  }

  /**
   * @override
   *
   * Get {nodequeue_queue} data.
   */
  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('nodequeue_queue', 'q')
      ->fields('q', array('qid', 'name', 'title', 'subqueue_title', 'size', 'link', 'link_remove', 'owner', 'show_in_ui', 'show_in_tab', 'show_in_links', 'reference', 'reverse', 'i18n'));
    if (!empty($this->source_qids)) {
      $query = $query->condition('qid', $this->source_qids, 'IN');
    }
    return $query;
  }

  /**
   * @override
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Make sure the queue does not exist already.
    if (nodequeue_load($row->name)) {
      $this->queueMessage(t('Queue %qname already exists on destination.', array('%qname' => $row->name)), self::MESSAGE_ERROR);
      return FALSE;
    }

    // Populate $row->types.
    $row->types = Database::getConnection('default', $this->sourceConnection)
      ->select('nodequeue_types', 't')
      ->fields('t', array('type'))
      ->condition('qid', $row->qid)
      ->execute()
      ->fetchCol();
  }

  /**
   * @override
   */
  public function prepare($d7queue, $d6queue) {
    // Set roles.
    // Empty array - NodequeueRole migrations will take care of values.
    $d7queue->roles = array();

    // Set types.
    // Assume content types have the same machine name in D6 and D7.
    // Make sure content types exist in D7.
    $d7queue->types = array();
    foreach ($d6queue->types as $type) {
      if (!node_type_load($type)) {
        $this->queueMessage(t('Type %type does not exist on destination. Queue %qname will not accept %type nodes.', array('%type' => $type, '%qname' => $d7queue->name)), self::MESSAGE_WARNING);
      }
      else {
        $d7queue->types[] = $type;
      }
    }
  }

}
