<?php

class ContentMapMigration extends Migration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Import of map contents.');

    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'fancy_migrate') . '/migrations/import/content/maps.csv',
      array(),
      array('delimiter' => ',', 'header_rows' => 1)
    );

    $this->destination = new MigrateDestinationEntityAPI('fancy_content', 'map');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'guid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('fancy_content', 'map')
    );

    $this->addFieldMapping('title', 'title')->defaultValue('');
    $this->addFieldMapping('field_fancy_location')->defaultValue(FALSE);
    $this->addFieldMapping('field_fancy_location:name', 'name');
    $this->addFieldMapping('field_fancy_location:city', 'city');
    $this->addFieldMapping('field_fancy_location:country', 'country');
    $this->addFieldMapping('field_fancy_location:latitude', 'latitude');
    $this->addFieldMapping('field_fancy_location:longitude', 'longitude');
  }
}
