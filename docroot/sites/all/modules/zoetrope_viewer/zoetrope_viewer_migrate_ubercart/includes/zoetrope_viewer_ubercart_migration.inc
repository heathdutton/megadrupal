<?php

/**
 * @file
 * Zoetrope image migration for importing in to ubercart.
 */

/**
 * Imports Zoetrope Feeds into a field on drupal commerce inventories.
 */
class ZoetropeViewerUbercartMigration extends Migration {

  /**
   * Fetch the settings and init the migration.
   */
  public function __construct(array $arguments) {
    $this->arguments = $arguments;
    parent::__construct();
    $this->description = t('Import Zoetrope image to your ubercart site, keyed on SKU, from your Zoetrope image feed.');

    // Create a MigrateSource object, which manages retrieving the input data.
    $source_file = variable_get('zoetrope_viewer_migrate_ubercart_source_url', FALSE);
    if (!$source_file) {
      throw new MigrateException(t('You need to set your import url for Zoetrope images on the @link',
          array('@link' => l(t('settings page'), 'admin/content/zoetrope_ubercart'))));
    }

    // Get the configured destination field.
    $destination_field = variable_get('zoetrope_viewer_migrate_ubercart_dest_field', FALSE);
    if (!$destination_field) {
      throw new MigrateException(t('You need to set up a field to import to your Zoetrope images to on the @link',
          array('@link' => l(t('settings page'), 'admin/content/zoetrope_ubercart'))));
    }

    // Create a map object for tracking the relationships between source rows.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sku' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'SKU',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );
    $this->destination = new MigrateDestinationNode('product');

    $this->source = new MigrateSourceJSON($source_file, 'sku', $this->JSONFields());

    // See https://www.drupal.org/node/1117454 - we want to update not replace.
    $this->systemOfRecord = Migration::DESTINATION;

    $this->addFieldMapping('nid', 'nid');
    $this->addFieldMapping($destination_field, 'zoetrope_uuid');
    $this->addFieldMapping($destination_field . ':start_position',
              'zoetrope_start_position');
    $this->addFieldMapping($destination_field . ':gallery_image_positions',
              'zoetrope_gallery_image_positions');
  }

  /**
   * Descriptions of the fields from the JSON objects.
   */
  public function JSONFields() {
    return array(
      'sku' => 'product SKU',
      'zoetrope_uuid' => 'the UUID of the Zoetrope image being imported',
      'zoetrope_start_position' => 'The initial position of this Zoetrope image',
      'zoetrope_gallery_image_positions' => 'the gallery image indices of this image',
    );
  }

  /**
   * Look up the nid of the node which matches the SKU or skip row.
   */
  public function prepareRow($row) {
    // Look up the nid based on the model.
    $nid = db_query("SELECT nid FROM {uc_products} WHERE model=:model",
              array(':model' => $row->sku))->fetchObject();
    if (isset($nid->nid)) {
      $row->nid = $nid->nid;
    }
    else {
      // We need to skip this row because it doesn't match a product.
      $this->queueMessage(t("Couldn't match '%sku' in the feed to an ubercart product, skipping row.",
               array('%sku' => $row->sku)));
      return FALSE;
    }
  }
}
