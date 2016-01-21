<?php

class ContentTextMigration extends Migration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Import of text contents.');

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'fancy_migrate') . '/migrations/import/content/texts.csv',
      array(),
      array('delimiter' => ',', 'header_rows' => 1)
    );

    $this->destination = new MigrateDestinationEntityAPI('fancy_content', 'text');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'guid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('fancy_content', 'text')
    );

    $this->addFieldMapping('title', 'title')->defaultValue('');
    $this->addFieldMapping('field_fancy_text', 'text');
  }
}
