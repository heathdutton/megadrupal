<?php

class WebformComponents extends Migration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $simple_fields = array('cid', 'pid', 'form_key', 'name', 'type', 'value', 'extra', 'weight');
    $complex_fields = array('nid', 'mandatory');
    $fields = array_merge($simple_fields, $complex_fields);

    $query = $this->query($fields);
    $table_name = 'webform_component';

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

    $this->addFieldMapping('required', 'mandatory');

    $this->addFieldMapping('nid', 'nid')
      ->sourceMigration($arguments['node_migrations']);

    // is private_file enabled on the d6 site?
    $connection = Database::getConnection('default', $arguments['source_connection']);
    $query = $connection->select('system', 's')->fields('s', array('status'));
    $query->condition('s.name', 'private_files', '=');
    $this->private_file = $query->execute()->fetchField();

  }

  protected function query($fields) {
   $connection = migrate_webform_get_source_connection();
   $query = $connection
           ->select('webform_component', 'wc') 
           ->fields('wc', $fields)
           ->orderBy('cid', 'ASC');
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

    // Users may have put tokens in the default values.
    // Prompt user if mapping uknown.
    // May be possible to use _token_upgrade_token_list()
    // from token.install, returns array('old' => 'new') without % prefix.
    if (strpos($row->value, '%') === 0) {
      switch($row->value) {
        case '%useremail':
          $row->value = '[current-user:mail]';
          break;
        default:
          drupal_set_message(t('The default value of :value in node :nid has not been remapped yet. Please submit a patch or !edit on that node!', array(':value' => $row->value, ':nid' => $row->nid, '!edit' => l('edit component', 'node/' . $row->nid . '/webform/components/' . $row->cid))), 'warning');
      }
    }

    if ($this->private_file == TRUE) {
      if ($row->type == 'private_file') {
        $row->type = 'file';
        $extra = unserialize($row->extra);
        $extra['scheme'] = 'private';
        $row->extra = serialize($extra);
      }
    }
  }

  public function prepare($entity, $row) {
  }

}
