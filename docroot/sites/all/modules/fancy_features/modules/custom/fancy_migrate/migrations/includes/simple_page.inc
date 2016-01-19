<?php

class SimplePageMigration extends Migration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Import of simple pages.');
    $this->dependencies = array(
      'ContentImage',
      'ContentHeadline',
      'ContentMap',
      'ContentRichText',
      'ContentText',
    );

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'fancy_migrate') . '/migrations/import/simple_pages.csv',
      array(),
      array('delimiter' => ',', 'header_rows' => 1)
    );

    $this->destination = new MigrateDestinationNode('fancy_simple_page');

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
    $this->addFieldMapping('field_fancy_rel_contents', 'contents')
      ->separator(',')
      ->sourceMigration(array(
        'ContentImage',
        'ContentHeadline',
        'ContentMap',
        'ContentRichText',
        'ContentText',
      ));

    $this->addUnmigratedDestinations(array('uid', 'created', 'changed',
      'status', 'promote', 'sticky', 'revision', 'log', 'language',
      'tnid', 'translate', 'revision_uid', 'is_new', 'path', 'comment', 'pathauto'));

  }
}
