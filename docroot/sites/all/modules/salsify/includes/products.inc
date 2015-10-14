<?php

/**
 * @file
 * includes/products.inc
 */

/**
 * Imports products from Salsify.
 */
class SalsifyProductsMigration extends SalsifyMigration {

  /**
   * Source product ID field.
   *
   * @var string
   */
  protected $productId = 'id';

  /**
   * Tells us whether digital assets are being downloaded in this migration.
   *
   * @var boolean
   */
  protected $downloadAssets = FALSE;

  /**
   * Archive handler for .ZIP file. Archiver objects are keyed by URL.
   *
   * @var array
   */
  protected $archiver = array();

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $urls = array();
    $source_fields = array(
      'products' => $this->salsifyAttributes(),
      'digital_assets' => SalsifyDigitalAssetsMigration::salsifyAttributes(),
      'relations' => array(
        'salsify:id' => t('Unique ID within the scope of all related relationships'),
        'salsify:target_product_id' => t('ID of related product'),
      ),
      'bundled_products' => array(
        'salsify:id' => t('Unique ID within the scope of all bundle relationships'),
        'salsify:bundled_product_id' => t('ID of a bundled product'),
      ),
    );
    foreach ($arguments['channel_ids'] as $channel_id) {
      $run = &$this->arguments['channels'][$channel_id];
      // Refresh the run in case the URL has expired.
      $result = drupal_http_request(url(self::BASE_URL . "/channels/$channel_id/runs/$run[id]", array(
        'query' => array(
          'auth_token' => $this->arguments['api_key'],
        ),
      )));
      $run = drupal_json_decode($result->data);
      $urls[] = $run['product_export_url'];
      // Load attributes from this channel.
      $reader = new SalsifyAttributesJSONReader($run['product_export_url'], 'salsify:id');
      foreach ($reader as $attribute) {
        if (isset($attribute->{'salsify:role'})) {
          switch ($attribute->{'salsify:role'}) {
            case 'product_id':
              // Save product ID attribute.
              $this->productId = $attribute->{'salsify:id'};
              break;

            case 'relation_type':
              // For some reason, the relation type attribute does not list
              // relations as its entity type.
              if (!in_array('relations', $attribute->{'salsify:entity_types'})) {
                $source_fields['relations'][$attribute->{'salsify:id'}] = $attribute->{'salsify:name'};
              }
              break;
          }
        }
        // Save source fields.
        foreach ($attribute->{'salsify:entity_types'} as $entity_type) {
          if (isset($source_fields[$entity_type])) {
            $source_fields[$entity_type][$attribute->{'salsify:id'}] = $attribute->{'salsify:name'};
          }
        }
      }
    }
    // Add bundled products attributes.
    foreach ($source_fields['bundled_products'] as $name => $desription) {
      $source_fields['products']["salsify:bundled_products::$name"] = t('Bundled products: @description', array('@description' => $desription));
    }
    // Add relations attributes.
    foreach ($source_fields['relations'] as $name => $description) {
      $source_fields['products']["salsify:relations::$name"] = t('Relations: @description', array('@description' => $description));
    }
    // Add digital asset attributes.
    foreach ($source_fields['digital_assets'] as $name => $description) {
      $source_fields['products']["salsify:digital_assets::$name"] = t('Digital assets: @description', array('@description' => $description));
    }
    $product_reference_fields = array_keys(field_read_fields(array('type' => 'commerce_product_reference')));
    foreach ($this->getStoredFieldMappings() as $key => $mapping) {
      $source_field = $mapping->getSourceField();
      if ($source_field) {
        if (!isset($source_fields['products'][$source_field])) {
          // Skip mappings for fields which no longer exist.
          unset($this->storedFieldMappings[$key]);
        }
        elseif (in_array($key, $product_reference_fields) && in_array($this->machineName, (array) $mapping->getSourceMigration())) {
          // The product mapping uses both the product ID and the revision ID
          // to match source records with destination records. Only the
          // product ID is used in a product reference field, so we need to
          // remove the revision ID from the mapping.
          $mapping->callbacks('salsify_product_ids');
        }
        elseif (!$this->downloadAssets) {
          $destination_field = $mapping->getDestinationField();
          if (isset($destination_field) && $source_field == 'salsify:digital_assets::salsify:url') {
            $this->downloadAssets = TRUE;
          }
        }
      }
    }
    $this->description = format_plural(count($arguments['channel_ids']), 'Import products from Salsify channel %channel_ids.', 'Import products from Salsify channels %channel_ids', array(
      '%channel_ids' => implode(', ', $arguments['channel_ids']),
    ));
    // Map source rows to destination items.
    $this->map = new MigrateSQLMap($this->machineName, array(
      $this->productId => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'description' => 'Product ID',
      ),
    ), MigrateDestinationCommerceProduct::getKeySchema('commerce_product'));
    if (isset($arguments['variation']) && $arguments['variation']['type'] == 'relation') {
      if (isset($arguments['variation']['relation_type'])) {
        $id_field = $arguments['variation'];
        $id_field['product_id'] = $this->productId;
        $reader_class = 'SalsifyProductsRelationsTypeJSONReader';
      }
      else {
        $id_field = $this->productId;
        $reader_class = 'SalsifyProductsRelationsJSONReader';
      }
    }
    else {
      $id_field = $this->productId;
      $reader_class = 'SalsifyProductsJSONReader';
    }
    $this->source = new MigrateSourceJSON($urls, $id_field, $source_fields['products'], array(
      'reader_class' => $reader_class,
      'cache_counts' => TRUE,
      'cache_key' => $this->cacheKey,
    ));
    // The MigrateFieldMapping class must be constructed at least once to
    // populate the priorities property.
    new MigrateFieldMapping('dummy', 'dummy');
  }

  /**
   * {@inheritdoc}
   */
  protected function preImport() {
    parent::preImport();
    if ($this->downloadAssets) {
      foreach ($this->arguments['channel_ids'] as $channel_id) {
        $run = $this->arguments['channels'][$channel_id];
        // Set up archivers for our channels.
        if (isset($run['digital_asset_file_export_url']) && empty($this->archiver[$run['product_export_url']])) {
          // Save .ZIP file to temporary directory.
          $file = drupal_tempnam('temporary://', 'salsify') . '.zip';
          $remote = fopen($run['digital_asset_file_export_url'], 'r');
          $local = fopen($file, 'w');
          while (!feof($remote)) {
            fwrite($local, fread($remote, 1024));
          }
          fclose($remote);
          fclose($local);
          $this->archiver[$run['product_export_url']] = archiver_get_archiver($file);
        }
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $return = parent::prepareRow($row);
    if ($return) {
      foreach (array('salsify:relations', 'salsify:digital_assets') as $property) {
        // We need to save each field property in a separate array.
        if (isset($row->$property)) {
          foreach ($row->$property as $delta => $attachment) {
            foreach ($attachment as $key => $value) {
              $row->{"$property::$key"}[$delta] = $value;
            }
          }
        }
      }
      if (isset($row->{'salsify:digital_assets'})) {
        if ($this->downloadAssets) {
          // Find downloaded .ZIP file for active URL.
          $url = $this->getSource()->activeUrl();
          // Check for digital asset in downloaded .ZIP file.
          if (!empty($this->archiver[$url]) && $this->archiver[$url] instanceof ArchiverZip) {
            foreach ($row->{'salsify:digital_assets::salsify:name'} as $delta => $name) {
              $zip = $this->archiver[$url]->getArchive();
              if ($zip->locateName($name) !== FALSE) {
                $row->{'salsify:digital_assets::salsify:url'}[$delta] = "zip://$zip->filename#$name";
                $row->{'salsify:digital_assets::salsifyUrlEncode'}[$delta] = FALSE;
              }
            }
          }
        }
      }
    }
    return $return;
  }

  /**
   * {@inheritdoc}
   */
  protected function applyMappings() {
    parent::applyMappings();
    $destination = $this->getDestination();
    $entity_type = $destination->getEntityType();
    $bundle = $destination->getBundle();
    $instances = field_info_instances($entity_type, $bundle);
    $entity = $this->destinationValues;
    foreach ($instances as $machine_name => $instance) {
      if (property_exists($entity, $machine_name)) {
        $values = &$entity->$machine_name;
        $field_info = field_info_field($machine_name);
        switch ($field_info['type']) {
          case 'commerce_product_reference':
            if (isset($values['arguments'])) {
              $arguments = array_filter($values['arguments']);
              unset($values['arguments']);
              foreach ($values as $key => $value) {
                // If we have requested a specific relation type, filter out
                // other relation types.
                if (isset($arguments['filter_value']) && isset($arguments['filter_attribute'])) {
                  if (is_array($arguments['filter_value'])) {
                    $filter = $arguments['filter_value'][$key];
                  }
                  else {
                    $filter = $arguments['filter_value'];
                  }
                  if (is_array($arguments['filter_attribute'])) {
                    $attribute = $arguments['filter_attribute'][$key];
                  }
                  else {
                    $attribute = $arguments['filter_attribute'];
                  }
                  if ($filter != $attribute) {
                    unset($values[$key]);
                  }
                }
              }
            }
            break;
        }
      }
    }
  }

}

/**
 * Class for generating dummy migration objects.
 *
 * Required to add product reference subfields in the migration wizard.
 *
 * @see SalsifyCommerceProductReferenceFieldHandler::fields()
 */
class SalsifyDummyProductsMigration extends SalsifyProductsMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments = NULL) {
    // This is empty to avoid the overhead of constructing a real migration.
  }

}

/**
 * Imports new products.
 */
class SalsifyNewProductsMigration extends SalsifyProductsMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->destination = new MigrateDestinationCommerceProduct('commerce_product', $arguments['product_type']);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    // Skip this record if there is an existing product.
    if (commerce_product_validate_sku_unique($row->{$this->productId}, NULL)) {
      $return = parent::prepareRow($row);
    }
    else {
      $return = FALSE;
    }
    return $return;
  }

  /**
   * Creates a product stub.
   */
  protected function createStub($migration, $source_key) {
    if (!empty($source_key[0]) && commerce_product_validate_sku_unique($source_key[0], NULL)) {
      migrate_instrument_start('SalsifyProductsMigration::createStub');
      $product = commerce_product_new($this->arguments['product_type']);
      $product->sku = $source_key[0];
      $product->title = t('Stub: !key', array('!key' => $source_key[0]));
      commerce_product_save($product);
      migrate_instrument_stop('SalsifyProductsMigration::createStub');
      $return = isset($product->product_id) ? array(
        $product->product_id,
        $product->revision_id,
      ) : FALSE;
    }
    else {
      $return = FALSE;
    }
    return $return;
  }

}

/**
 * Updates existing products.
 */
class SalsifyExistingProductsMigration extends SalsifyProductsMigration {

  /**
   * {@inheritdoc}
   */
  protected $systemOfRecord = Migration::DESTINATION;

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->destination = new SalsifyDestinationCommerceProduct('commerce_product', $arguments['product_type']);
  }

  /**
   * {@inheritdoc}
   */
  static public function salsifyAttributes() {
    return array(
      'salsify:id' => t('The internal numeric ID of the product retrieved by SKU.'),
    ) + parent::salsifyAttributes();
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $product = commerce_product_load_by_sku($row->{$this->productId});
    if ($product != FALSE) {
      // Add the Drupal product ID to the record.
      $row->{'salsify:id'} = $product->product_id;
      $return = parent::prepareRow($row);
    }
    else {
      // Skip this record if there is no existing product.
      $return = FALSE;
    }
    return $return;
  }

  /**
   * Creates a product stub.
   */
  protected function createStub($migration, $source_key) {
    $product = empty($source_key[0]) ? FALSE : commerce_product_load_by_sku($source_key[0]);
    return $product != FALSE ? array(
      $product->product_id,
      $product->revision_id,
    ) : FALSE;
  }

}

/**
 * Adds fields for updating existing products.
 */
class SalsifyDestinationCommerceProduct extends MigrateDestinationCommerceProduct {

  /**
   * {@inheritdoc}
   */
  public function fields($migration = NULL) {
    $properties = entity_get_property_info('commerce_product');
    $fields = array(
      'product_id' => $properties['properties']['product_id']['description'],
      'type' => $properties['properties']['type']['description'],
      'revision' => t('Create new revision'),
      'log' => t('Revision log message'),
    );
    $fields += parent::fields($migration);
    return $fields;
  }

}

/**
 * Adds subfields for product relations.
 */
class SalsifyCommerceProductReferenceFieldHandler extends MigrateFieldHandler {

  /**
   * {@inheritdoc}
   */
  public function __construct() {
    $this->dependencies = array('MigrateCommerceProductReferenceFieldHandler');
    $this->registerTypes(array('commerce_product_reference'));
  }

  /**
   * Add subfields to map product relations to product references.
   */
  public function fields($type, $instance, $migration = NULL) {
    $fields = array();
    // Only operate on instances of SalisfyProductsMigration.
    if (isset($migration) && $migration instanceof SalsifyProductsMigration) {
      $fields['filter_attribute'] = t('Option: Relation type attribute');
      $fields['filter_value'] = t('Option: Relation type allowed in this field');
    }
    return $fields;
  }

}

/**
 * Loads products which do not have a specified relation type from a Salsify
 * channel.
 */
class SalsifyProductsRelationsTypeJSONReader extends SalsifyJSONReader {

  protected $attributeId;
  protected $relationType;

  /**
   * {@inheritdoc}
   */
  public function __construct($json_url, $id_field) {
    // We added the attribute ID and relation type to the $id_field argument
    // because that was the only way to get it to our reader without writing our
    // own source class.
    $this->attributeId = $id_field['attribute_id'];
    $this->relationType = $id_field['relation_type'];
    parent::__construct($json_url, $id_field['product_id']);
  }

  /**
   * {@inheritdoc}
   */
  public function next() {
    do {
      $this->salsifyNext('products');
      // If this product has the specified relation type, load the next product.
      $display = FALSE;
      if (isset($this->currentElement->{'salsify:relations'})) {
        foreach ($this->currentElement->{'salsify:relations'} as $relation) {
          if (isset($relation->{$this->attributeId}) && $relation->{$this->attributeId} == $this->relationType) {
            $display = TRUE;
            break;
          }
        }
      }
    } while ($display);
  }

}

/**
 * Loads products which do not have relations from a Salsify channel.
 */
class SalsifyProductsRelationsJSONReader extends SalsifyJSONReader {

  /**
   * {@inheritdoc}
   */
  public function next() {
    // If a product has relations, load the next product.
    do {
      $this->salsifyNext('products');
    } while (!empty($this->currentElement->{'salsify:relations'}));
  }

}

/**
 * Loads products from a Salsify channel.
 */
class SalsifyProductsJSONReader extends SalsifyJSONReader {

  /**
   * {@inheritdoc}
   */
  public function next() {
    $this->salsifyNext('products');
  }

}
