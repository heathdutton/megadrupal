<?php

/**
 * @file
 *   Commerce Product Display Node migration.
 *   This is a dynamic migration, reused for every product type
 *   (so that products of each type can be imported separately)
 */
class CommerceMigrateMagentoProductDisplayMigration extends Migration {
  // An array mapping D6 format names to this D7 databases formats.
  public $filter_format_mapping = array();

  public function __construct(array$arguments) {
    parent::__construct($arguments);
    // Source fields
    $source_fields = array(
      'products' => t('Product Options (commerce_product_reference).'),
      'status' => t('Boolean indicating whether the product is active or disabled.'),
    );
    $store_id = $arguments['store_id'];
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
      MigrateDestinationNode::getKeySchema()
    );

    // Create a MigrateSource object, which manages retrieving the input data.
    $connection = commerce_migrate_magento_get_source_connection();
    // check if the flat table exists
    $table_name = 'catalog_product_flat_' . $store_id;
    if ($connection->schema()->tableExists($table_name)) {
      $query = $connection->select($table_name, 'cpfl');
      $query->leftJoin('catalog_product_entity', 'cpe', 'cpe.entity_id = cpfl.entity_id');
      $query->leftJoin('catalog_product_relation', 'cpr', 'cpr.parent_id = cpe.entity_id');
      $query->fields('cpfl')->distinct();
      $query->condition('cpfl.type_id', array('configurable', 'grouped'), 'IN');
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
    // The product_display content type is created by commerce install profiles,
    // not the module itself, so hardcoding this might be unwise.
    $this->destination = new MigrateDestinationNode($arguments['type']);
    // Properties
    // Default uid to 1 if we're not mapping it.
    $owner = variable_get('commerce_migrate_magento_product_display_owner', 1);
    $this->addFieldMapping('uid', NULL)->defaultValue($owner);
    $this->addFieldMapping('title', 'name');
    $this->addFieldMapping('created', 'created_at');
    $this->addFieldMapping('changed', 'updated_at');
    $this->addFieldMapping('status', 'status')->defaultValue(1);
    $this->addFieldMapping('language', NULL)->defaultValue(LANGUAGE_NONE);
    // This gets mapped later on in prepareRow as $row->path.
    $this->addFieldMapping('path', 'url_path')->defaultValue(NULL);
    // Fields
    $this->addFieldMapping('field_prods', 'products')->sourceMigration($arguments['products']);
    // $this->addFieldMapping('title_field', 'name');
    $this->addFieldMapping('body', 'short_description')->defaultValue('');
    // $this->addFieldMapping('field_style_no', 'sku');
  }

  public function prepareRow($row) {
    $row->status = commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'catalog_product', 'status');
    // product displays
    $connection = commerce_migrate_magento_get_source_connection();
    $query = $connection->select('catalog_product_relation', 'cpr');
    $query->fields('cpr');
    $query->condition('cpr.parent_id', $row->entity_id, '=');
    $result = $query->execute();
    $products = array();
    while ($record = $result->fetchAssoc()) {
      $products[] = $record['child_id'];
    }
    $row->products = $products;
    $row->created_at = strtotime($row->created_at);
    $row->updated_at = strtotime($row->updated_at);
  }
}

