<?php

class WebformValidationRuleComponents extends Migration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $simple_fields = array('ruleid');
    $complex_fields = array('cid');
    $fields = array_merge($simple_fields, $complex_fields);

    $query = $this->query($fields);
    $table_name = 'webform_validation_rule_components';

    $this->source = new MigrateSourceSQL($query, $fields, NULL,  array('map_joinable' => FALSE, 'skip_count' => FALSE));

    $this->destination = new MigrateDestinationTable($table_name);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'ruleid' => array('type' => 'int',
                       'unsigned' => TRUE,
                       'not null' => TRUE,
                       'description' => 'Source rule ID',
                       'alias' => 'r',
                      ),
        'cid' => array('type' => 'int',
                       'unsigned' => TRUE,
                       'not null' => TRUE,
                       'description' => 'Source component ID',
                       'alias' => 'c',
                      ),
      ),
      MigrateDestinationTable::getKeySchema($table_name)
    );


    $this->addSimpleMappings($simple_fields);

    // @todo WebformComponents sourceMigration doesn't work (keys hard to line up)
    $this->addFieldMapping('cid', 'cid');
  }

  protected function query($fields) {
    $connection = migrate_webform_get_source_connection();
    $query = $connection
           ->select('webform_validation_rule_components', 'wvrc') 
           ->fields('wvrc', $fields)
           ->orderBy('ruleid', 'ASC');
    return $query;
  }

  public function prepareRow($row) {

    // skip?
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    $result = db_select("webform_validation_rule", "vr")->fields("vr", array("ruleid"))->condition("ruleid", $row->ruleid)->execute()->fetchField();
    if(!$result) {
        return FALSE;
    }
  }

  public function prepare($entity, $row) {
  }

}





