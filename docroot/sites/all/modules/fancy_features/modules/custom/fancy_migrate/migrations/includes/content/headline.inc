<?php

class ContentHeadlineMigration extends Migration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Import of headline contents.');

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'fancy_migrate') . '/migrations/import/content/headlines.csv',
      array(),
      array('delimiter' => ',', 'header_rows' => 1)
    );

    $this->destination = new MigrateDestinationEntityAPI('fancy_content', 'headline');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'guid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('fancy_content', 'headline')
    );

    $this->addFieldMapping('title', 'title')->defaultValue('');
    $this->addFieldMapping('field_headline', 'headline');
    $this->addFieldMapping('field_fancy_style', 'style');
    $this->addFieldMapping('field_fancy_textcolor', 'textcolor')->defaultValue('');
  }
}
