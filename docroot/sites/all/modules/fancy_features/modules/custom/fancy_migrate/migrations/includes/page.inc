<?php

class PageMigration extends Migration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Import of pages.');
    $this->dependencies = array('Section');

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'fancy_migrate') . '/migrations/import/pages.csv',
      array(),
      array('delimiter' => ',', 'header_rows' => 1)
    );

    $this->destination = new MigrateDestinationNode('fancy_page');

    //Source and destination relation for rollbacks
    $this->map = new MigrateSQLMap(
      $this->machineName,
      array(
        'guid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('field_fancy_rel_sections', 'sections')
      ->separator(',')
      ->sourceMigration('Section');

    $this->addUnmigratedDestinations(array('uid', 'created', 'changed',
      'status', 'promote', 'sticky', 'revision', 'log', 'language',
      'tnid', 'translate', 'revision_uid', 'is_new', 'path', 'comment', 'pathauto'));

  }
}
