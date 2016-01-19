<?php

class MediaImageMigration extends Migration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Import of image media.');

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'fancy_migrate') . '/migrations/import/media/images.csv',
      array(),
      array('delimiter' => ',', 'header_rows' => 1)
    );

    $this->destination = new MigrateDestinationMedia('image');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'filename' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Image filename',
        )
      ),
      MigrateDestinationMedia::getKeySchema()
    );

    $this->addFieldMapping('source_dir')
      ->defaultValue(drupal_get_path('module', 'fancy_migrate') . '/migrations/import/media/images');

    $this->addFieldMapping('value', 'filename');

    $this->addFieldMapping('uid')
      ->defaultValue(1);
  }
}
