<?php

/**
 * @file
 *   Commerce Product migration.
 *   This is a dynamic migration, reused for every product type
 *   (so that products of each type can be imported separately)
 */
class CommerceMigrateMagentoProductMigration extends Migration {

  public function __construct(array$arguments) {
    parent::__construct($arguments);
    // Source fields
    $source_fields = array(
      'status' => t('Boolean indicating whether the product is active or disabled.'),
    );
    $store_id = $arguments['store_id'];
    $store_code = $arguments['store_code'];

    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'entity_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Magento Entity ID',
          'alise' => 'cpfl'
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('commerce_product', $arguments['type'])
    );

    // Create a MigrateSource object, which manages retrieving the input data.
    $connection = commerce_migrate_magento_get_source_connection();
    // check if the flat table exists
    $table_name = 'catalog_product_flat_' . $store_id;
    if ($connection->schema()->tableExists($table_name)) {
      $query = $connection->select($table_name, 'cpfl');
      $query->leftJoin('catalog_product_entity', 'cpe', 'cpe.entity_id = cpfl.entity_id');
      $query->leftJoin('catalog_product_relation', 'cpr', 'cpr.child_id = cpe.entity_id');
      $query->fields('cpfl')->distinct();
      $query->condition('cpfl.type_id', 'simple', '=');
      // set categories if they exists
      $categories = variable_get('commerce_migrate_magento_' . $arguments['type'] . '_categries', array());
      if (!empty($categories)) {
        $query->leftJoin('catalog_category_product', 'ccp', 'ccp.product_id = cpr.parent_id');
        $query->condition('ccp.category_id', $categories, 'IN');
      }
    }
    else {
      $query = $connection->select('catalog_product_entity', 'cpe');
      $query->fields('cpe');
    }

    $this->source = new MigrateSourceSQL($query, $source_fields, NULL, array('map_joinable' => FALSE));
    $this->destination = new MigrateDestinationEntityAPI('commerce_product', $arguments['type']);

    // Properties
    $this->addFieldMapping('sku', 'sku')->dedupe('commerce_product', 'sku');
    $this->addFieldMapping('type', NULL)->defaultValue($arguments['type']);
    $this->addFieldMapping('title', 'name');
    $this->addFieldMapping('created', 'created_at');
    $this->addFieldMapping('changed', 'updated_at');
    // Default uid to 0
    $owner = variable_get('commerce_migrate_magento_product_owner', 0);
    $this->addFieldMapping('uid', NULL)->defaultValue($owner);
    // Probably unnecessary but we should respect the status of the old products.
    $this->addFieldMapping('status', 'status')->defaultValue(1);
    $this->addFieldMapping('language', NULL)->defaultValue(LANGUAGE_NONE);
    $this->addFieldMapping('path', 'url_path');
    // Fields
    $this->addFieldMapping('commerce_price', 'price');
    $this->addFieldMapping('commerce_price:currency_code', NULL)->defaultValue('USD');
    // $this->addFieldMapping('field_prod_imgs', 'thumbnail');
    // $this->addFieldMapping('field_prod_imgs:source_dir')->defaultValue('http://BASEFILEPATH');
    // weight
    // $this->addFieldMapping('field_prod_wt', 'weight');
    // $this->addFieldMapping('field_prod_wt:unit', NULL)->defaultValue('lb');
  }

  public function prepareRow($row) {
    $row->status = commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'catalog_product', 'status');
    $row->created_at = strtotime($row->created_at);
    $row->updated_at = strtotime($row->updated_at);
  }
}

