<?php

class ContentRichTextMigration extends Migration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Import of rich text contents.');

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'fancy_migrate') . '/migrations/import/content/rich_text.csv',
      array(),
      array('delimiter' => ',', 'header_rows' => 1)
    );

    $this->destination = new MigrateDestinationEntityAPI('fancy_content', 'rich_text');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'guid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('fancy_content', 'rich_text')
    );

    $this->addFieldMapping('title', 'title')->defaultValue('');
    $this->addFieldMapping('field_fancy_text', 'rich_text');
    $this->addFieldMapping('field_fancy_text:format')->defaultValue('wysiwyg');
  }
}
