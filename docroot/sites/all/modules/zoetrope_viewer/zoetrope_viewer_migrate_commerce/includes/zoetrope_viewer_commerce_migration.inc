<?php

/**
 * @file
 * Zoetrope image migration for importing in to commerce.
 */

/**
 * Imports Zoetrope Feeds into a field on drupal commerce inventories.
 */
class ZoetropeViewerCommerceMigration extends Migration {

  /**
   * Fetch the settings and init the migration.
   */
  public function __construct(array $arguments) {
    $this->arguments = $arguments;
    parent::__construct();
    $this->description = t('Import Zoetrope image to your commerce site, keyed on SKU, from your Zoetrope image feed.');

    // Create a MigrateSource object, which manages retrieving the input data.
    $source_file = variable_get('zoetrope_viewer_migrate_commerce_source_url', FALSE);
    if (!$source_file) {
      throw new MigrateException(t('You need to set your import url for Zoetrope images on the @link',
          array('@link' => l(t('settings page'), 'admin/content/zoetrope_commerce'))));
    }

    // Get the configured destination field.
    $this->destination_field = variable_get('zoetrope_viewer_migrate_commerce_dest_field', FALSE);
    if (!$this->destination_field) {
      throw new MigrateException(t('You need to set up a field to import to your Zoetrope images to on the @link',
          array('@link' => l(t('settings page'), 'admin/content/zoetrope_commerce'))));
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

    $this->addFieldMapping('product_id', 'product_id');
    $this->addFieldMapping($this->destination_field, 'zoetrope_uuid');
    $this->addFieldMapping($this->destination_field . ':start_position',
            'zoetrope_start_position');
    $this->addFieldMapping($this->destination_field . ':gallery_image_positions',
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
   * Look up the product_id of the commerce product by SKU or skip.
   */
  public function prepareRow($row) {

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'commerce_product');
    $query->propertyCondition('sku', $row->sku);
    $result = $query->execute();

    if (isset($result['commerce_product'])) {
      $product_id = array_pop(array_keys($result['commerce_product']));
      $row->product_id = $product_id;
    }
    else {
      // We need to skip this row because it doesn't match a product.
      $this->queueMessage(t("Couldn't match '%sku' in the feed to an ubercart product, skipping row.",
               array('%sku' => $row->sku)));
      return FALSE;
    }
  }
}
