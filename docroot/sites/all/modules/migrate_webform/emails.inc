<?php

class WebformEmails extends Migration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $simple_fields = array('eid', 'email', 'subject', 'from_name', 'from_address', 'template', 'excluded_components', 'html', 'attachments');
    $complex_fields = array('nid');
    $fields = array_merge($simple_fields, $complex_fields);

    $query = $this->query($fields);
    $table_name = 'webform_emails';

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
        'eid' => array('type' => 'int',
                       'unsigned' => TRUE,
                       'not null' => TRUE,
                       'description' => 'Source component ID',
                       'alias' => 'e',
                      ),
      ),
      MigrateDestinationTable::getKeySchema($table_name)
    );

    $this->addSimpleMappings($simple_fields);

    $this->addFieldMapping('nid', 'nid')
      ->sourceMigration($arguments['node_migrations']);
    $this->addFieldMapping('extra', NULL)
         ->defaultValue(serialize(array())); // is this ok empty?
  }

  protected function query($fields) {
    $connection = migrate_webform_get_source_connection();
    $query = $connection
           ->select('webform_emails', 'we') 
           ->fields('we', $fields)
           ->orderBy('eid', 'ASC');
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

