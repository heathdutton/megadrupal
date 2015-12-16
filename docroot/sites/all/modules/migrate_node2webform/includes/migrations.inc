<?php

/**
 * Migrates D6 nodes into D7 webform submissions.
 */
class Node2WebformSubmissionMigration extends DrupalMigration {
  public function __construct(array $args) {
    parent::__construct($args);

    // Populate source fields.
    $this->sourceFields += $this->version->getSourceFields('node', $this->sourceType, TRUE);  

    // Declare source + destination + map.
    $key_schema = MigrateDestinationTable::getKeySchema('node');
    $options = array('fix_field_names' => array(), 'map_joinable' => FALSE);
    $this->source      = new MigrateDrupal6SourceSQL($this->query(), $this->sourceFields, NULL, $options);
    $this->destination = new WebformSubmissionD7Destination($args['destination_nid']);
    $this->map         = new MigrateSQLMap($this->machineName, $key_schema, $key_schema);

    // Declare field mappings.
    $this->addFieldMapping('is_draft', 'status')->callbacks(array($this, 'negate'))->defaultValue(FALSE);
    $this->addFieldMapping('submitted', 'changed');

    if (isset($args['user_migration'])) {
      $this->addFieldMapping('uid', 'uid')->sourceMigration($args['user_migration'])->defaultValue(1);
    }
    else {
      $this->addFieldMapping('uid', 'uid')->defaultValue(1);
    }

    // Declare unmigrated fields.
    $this->addUnmigratedDestinations(array('nid'));
  }

  /**
   * Negate a value.
   *
   * @param $value
   *   Input value (e.g. TRUE).
   * @return
   *   Negation (e.g. FALSE).
   */
  public function negate($value) {
    return !$value;
  }

  /**
   * @override
   *
   * @see DrupalNode6Migration::query().
   */
  public function query() {
    // Get core fields.
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('node', 'n')
      ->fields('n', array('nid', 'vid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed', 'comment', 'promote', 'moderate', 'sticky', 'tnid', 'translate'))
      ->condition('n.type', $this->sourceType)
      ->orderBy('n.changed');

    // Get basic CCK fields.
    $cck_table = 'content_type_' . $this->sourceType;
    if (Database::getConnection('default', $this->sourceConnection)->schema()->tableExists($cck_table)) {
      $query->leftJoin($cck_table, 'f', 'n.vid = f.vid');
      $field_info = $this->version->getSourceFieldInfo();
      foreach ($field_info as $field_name => $info) {
        if (isset($info['columns']) && !$info['multiple'] && $info['db_storage']) {
          // Webform components store a single value. Migrate only the main CCK column.
          // Example:
          // - d6: a CCK     file field stores the fid + file attributes.
          // - d7: a webform file field stores only the fid.
          // We retrieve only the fid, no need to get attributes.
          $field_column = array_shift($info['columns']);
          $query->addField('f', $field_column, $field_name);
        }
      }
    }
    return $query;
  }

  /**
   * @override
   *
   * Populate multiple-value CCK fields.
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    $field_info = $this->version->getSourceFieldInfo();
    foreach ($field_info as $field_name => $info) {
      if (isset($info['columns']) && $info['multiple'] && !$info['db_storage']) {
        $cck_table = 'content_' . $field_name;
        if (Database::getConnection('default', $this->sourceConnection)->schema()->tableExists($cck_table)) {
          // Same as basic fields: we migrate only the main CCK column.
          $field_column = array_shift($info['columns']);
          $query = Database::getConnection('default', $this->sourceConnection)->select($cck_table, 'f');
          $query->addField('f', $field_column, $field_name);
          $query->condition('vid', $row->vid);
          $row->$field_name = $query->execute()->fetchCol();
        }
      }
    }
  }

}

