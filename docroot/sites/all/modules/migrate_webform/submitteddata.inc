<?php

class WebformSubmittedData extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    $simple_fields = array('no', 'data');
    $complex_fields = array('nid', 'sid', 'cid');
    $fields = array_merge($simple_fields, $complex_fields);

    $query = $this->query($fields);
    $table_name = 'webform_submitted_data';

    $this->source = new MigrateSourceSQL($query, $fields, NULL,  array('map_joinable' => FALSE, 'skip_count' => FALSE));

    $this->destination = new MigrateDestinationTable($table_name);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'nid' => array('type' => 'int',
                       'unsigned' => TRUE,
                       'not null' => TRUE,
                       'description' => 'Source node ID',
                       'alias' => 'n',
                      ),
        'sid' => array('type' => 'int',
                       'unsigned' => TRUE,
                       'not null' => TRUE,
                       'description' => 'Source submission ID',
                       'alias' => 's',
                      ),
        'cid' => array('type' => 'int',
                       'unsigned' => TRUE,
                       'not null' => TRUE,
                       'description' => 'Source component ID',
                       'alias' => 'c',
                      ),
        'no' => array('type' => 'varchar',
                       'length' => 128,
                       'not null' => TRUE,
                       'description' => 'Source component instance ID?',
                       'alias' => 'o',
                      ),
      ),
      MigrateDestinationTable::getKeySchema($table_name)
    );

    $this->addSimpleMappings($simple_fields);

    $this->addFieldMapping('nid', 'nid')
      ->sourceMigration($arguments['node_migrations']);

    $this->addFieldMapping('sid', 'sid')
          ->sourceMigration('WebformSubmissions');
    $this->addFieldMapping('cid', 'cid'); // @todo consult WebformComponents
    // ... but first see https://drupal.org/node/2093073
  }

  protected function query($fields) {
    $connection = migrate_webform_get_source_connection();
    $query = $connection
      ->select('webform_submitted_data', 'wsd')
      ->fields('wsd', $fields)
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

    // find the type of the component we are transferring
    // cannot expand the base query because cid is part of the uniqueness
    $type = db_select('webform_component', 'wc')
      ->fields('wc', array('type', 'name'))
      ->condition('cid', $row->cid)
      ->condition('nid', $row->nid)
      ->execute()
      ->fetchField();

    // mangle the file ID to be the new version
    if ($type == 'file' || $type == 'private_file') {
      $filemigrations = explode(' ', variable_get('migrate_webform_file_migration_class', ""));
      foreach ($filemigrations as $key => $filemigration) {
        $destfile = db_select('migrate_map_' . strtolower($filemigration), 'map')
          ->fields('map', array('destid1'))
          ->condition('map.sourceid1', $row->data)
          ->execute()
          ->fetchField();
        if ($destfile != NULL) {
          $row->data = $destfile;
          file_usage_add(file_load($destfile), 'webform', 'submission', $row->sid);
        }
      }
    }
  }

  public function prepare($entity, $row) {
  }
}

