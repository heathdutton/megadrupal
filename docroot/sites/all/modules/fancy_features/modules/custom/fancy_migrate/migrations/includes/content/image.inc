<?php

class ContentImageMigration extends Migration {
  public function __construct() {
    parent::__construct();

    $this->dependencies = array('MediaImage');
    $this->description = t('Import of image contents.');

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'fancy_migrate') . '/migrations/import/content/images.csv',
      array(),
      array('delimiter' => ',', 'header_rows' => 1)
    );

    $this->destination = new MigrateDestinationEntityAPI('fancy_content', 'image');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'guid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('fancy_content', 'image')
    );

    $this->addFieldMapping('title')->defaultValue('');

    $this->addFieldMapping('field_fancy_image', 'filename')
      ->sourceMigration(array('MediaImage'));

    $this->addFieldMapping('field_fancy_image:file_class')
      ->defaultValue('MigrateFileFid');;
  }
}
