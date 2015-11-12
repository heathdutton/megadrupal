<?php

/**
 * @file
 * Migrate Aggregator from D6 to D7.
 */

/**
 * Basic handler to migrate a table.
 */
abstract class AggregatorBasicMigration extends DrupalMigration {
  protected $table;

  public function __construct($arguments, $table) {
    parent::__construct($arguments);
    $this->table = $table;
    
    // Declare source and destination objects.
    $options = array('fix_field_names' => array(), 'map_joinable' => FALSE);
    $this->source      = new MigrateDrupal6SourceSql($this->query(), array(), NULL, $options);
    $this->destination = new MigrateDestinationTable($this->table);

    // Map source with destination. Aggregator keys have the same schema in D6 and D7.
    $key_schema = $this->destination->getKeySchema($this->table);
    $this->map = new MigrateSQLMap($this->machineName, $key_schema, $key_schema);
  }

  /**
   * @override
   */
  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select($this->table, 'alias')
      ->fields('alias');
    return $query;
  }

}

/**
 * Handler to migrate Aggregator categories.
 */
class AggregatorCategoryMigration extends AggregatorBasicMigration {
  public function __construct($arguments) {
    parent::__construct($arguments, 'aggregator_category');
    $this->addSimpleMappings(array('cid', 'title', 'description', 'block'));
  }
}

/**
 * Handler to migrate Aggregator category/feed joins.
 */
class AggregatorCategoryFeedMigration extends AggregatorBasicMigration {
  public function __construct($arguments) {
    parent::__construct($arguments, 'aggregator_category_feed');
    $this->addFieldMapping('cid', 'cid')->sourceMigration(array('AggregatorCategoryMigration', 'AggregatorCategoryItemMigration'));
    $this->addFieldMapping('fid', 'fid')->sourceMigration(array('AggregatorFeedMigration'));
  }
}

/**
 * Handler to migrate Aggregator category/item joins.
 */
class AggregatorCategoryItemMigration extends AggregatorBasicMigration {
  public function __construct($arguments) {
    parent::__construct($arguments, 'aggregator_category_item');
    $this->addFieldMapping('cid', 'cid')->sourceMigration(array('AggregatorCategoryMigration', 'AggregatorCategoryFeedMigration'));
    $this->addFieldMapping('iid', 'iid')->sourceMigration(array('AggregatorItemMigration', 'AggregatorCategoryItemMigration'));
  }
}

/**
 * Handler to migrate Aggregator feeds.
 */
class AggregatorFeedMigration extends AggregatorBasicMigration {
  public function __construct($arguments) {
    parent::__construct($arguments, 'aggregator_feed');
    $this->addSimpleMappings(array('fid', 'title', 'url', 'refresh', 'checked', 'queued', 'link', 'description', 'image', 'hash', 'etag', 'modified', 'block'));
  }
}

/**
 * Handler to migrate Aggregator items.
 */
class AggregatorItemMigration extends AggregatorBasicMigration {
  public function __construct($arguments) {
    parent::__construct($arguments, 'aggregator_item');
    $this->addSimpleMappings(array('iid', 'title', 'link', 'author', 'description', 'timestamp', 'guid'));
    $this->addFieldMapping('fid', 'fid')->sourceMigration(array('AggregatorFeedMigration', 'AggregatorCategoryFeedMigration'));
  }
}
