<?php

class SectionMigration extends Migration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Import of sections.');
    $this->dependencies = array(
      'ContentImage',
      'ContentHeadline',
      'ContentMap',
      'ContentRichText',
      'ContentText',
    );

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'fancy_migrate') . '/migrations/import/sections.csv',
      array(),
      array('delimiter' => ',', 'header_rows' => 1)
    );

    $this->destination = new MigrateDestinationEntityAPI('fancy_section', 'responsive');

    //Source and destination relation for rollbacks
    $this->map = new MigrateSQLMap(
      $this->machineName,
      array(
        'guid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationEntityAPI::getKeySchema('fancy_section', 'responsive')
    );

    $this->addFieldMapping('title')->defaultValue('');
    $this->addFieldMapping('field_fancy_view_mode_selector', 'view mode')->defaultValue('default');
    $this->addFieldMapping('field_fancy_rel_contents', 'contents')
      ->separator(',')
      ->sourceMigration(array(
        'ContentImage',
        'ContentHeadline',
        'ContentMap',
        'ContentRichText',
        'ContentText',
    ));

    $this->addUnmigratedDestinations(array('uid', 'created', 'changed'));
  }
}
