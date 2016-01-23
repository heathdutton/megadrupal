<?php

class WebformSubmissions extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    $simple_fields = array('sid', 'submitted', 'remote_addr', 'is_draft');
    $complex_fields = array('nid', 'uid');
    $fields = array_merge($simple_fields, $complex_fields);

    $query = $this->query($fields);
    $table_name = 'webform_submissions';

    $this->source = new MigrateSourceSQL($query, $fields, NULL,  array('map_joinable' => FALSE, 'skip_count' => FALSE));

    $this->destination = new MigrateDestinationTable($table_name);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sid' => array('type' => 'int',
                       'unsigned' => TRUE,
                       'not null' => TRUE,
                       'description' => 'Source component ID',
                       'alias' => 's',
                      ),
      ),
      MigrateDestinationTable::getKeySchema($table_name)
    );

    $this->addSimpleMappings($simple_fields);

    $this->addFieldMapping('nid', 'nid')
      ->sourceMigration($arguments['node_migrations']);

    // Get users if configured
    // Default uid to 1 if we're not mapping it.
    if (variable_get('migrate_webform_user_map_ok', FALSE)) {
      $this->removeFieldMapping('uid');
      $this->addFieldMapping('uid', 'uid')->sourceMigration(variable_get('migrate_webform_user_migration_class', ""))->defaultValue(0);
    }
    else {
      $this->removeFieldMapping('uid');
      $this->addFieldMapping('uid', 'uid')->defaultValue(0);
    }
  }

  protected function query($fields) {
    $connection = migrate_webform_get_source_connection();
    $query = $connection
           ->select('webform_submissions', 'wss') 
           ->fields('wss', $fields)
           ->orderBy('sid', 'ASC');
    return $query;
  }

  public function prepareRow($row) {
    // skip?
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    $result = db_select("node", "n")->fields("n", array("nid"))->condition("nid", $row->nid)->execute()->fetchField();
    if(!$result) {
      return FALSE;
    }
  }

  public function prepare($entity, $row) {
  }
}

