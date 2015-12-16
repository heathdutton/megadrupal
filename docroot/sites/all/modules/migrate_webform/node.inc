<?php

/**
 * Handling specific to a Drupal 6 source for nodes.
 */
class WebformNode extends DrupalNode6Migration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);
    $this->destination_type = $arguments['destination_type'];
    if (isset($arguments['roles'])) {
      $this->roles = $arguments['roles'];
    }
    $this->source_db = variable_get('migrate_webform_source_database', 'default');

    // Keep old NIDs.
    $this->addFieldMapping('nid', 'nid')
         ->description('Let\'s reuse the old NIDs');
    $this->removeFieldMapping('is_new');
    $this->addFieldMapping('is_new')->defaultvalue(TRUE);

    // Get users if configured
    // Default uid to 1 if we're not mapping it.
    if (variable_get('migrate_webform_user_map_ok', FALSE)) {
      $this->removeFieldMapping('uid');
      $this->addFieldMapping('uid', 'uid')->sourceMigration(variable_get('migrate_webform_user_migration_class', ""))->defaultValue(1);
    }
    else {
      $this->removeFieldMapping('uid');
      $this->addFieldMapping('uid', 'uid')->defaultValue(1);
    }

  }

  public function prepareRow($row) {
    // skip?
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    db_set_active($this->source_db);
      $result = db_query("select * from webform_roles where nid=:nid", array(':nid' => $row->nid));
    db_set_active();

    // deal with role mappings
    foreach ($result as $item) {
      if (isset($this->roles[$item->rid]['new'])) {
        $row->roles[] = $this->roles[$item->rid]['new'];
      }
      else {
        $row->roles = array( 1 , 2 );
      }
    }
    if (sizeof($row->roles == 0)) {
      $row->roles = array( 1 , 2 );
    }
  }

  public function prepare($entity, $row) {
    $entity->webform['roles'] = $row->roles;
  }
}
