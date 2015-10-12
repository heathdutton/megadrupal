<?php

/**
 * @file
 * includes/displays.inc
 */

/**
 * Handles the tables for field variations.
 */
class SalsifyDisplaysMigrateSQLMap extends MigrateSQLMap {

  /**
   * Name of table that tracks product variations.
   *
   * @var string
   */
  protected $variationsTable;

  /**
   * {@inheritdoc}
   */
  public function __construct($machine_name, array $source_key, array $destination_key, $connection_key = 'default', $options = array()) {
    // Default generated table name, limited to 63 characters.
    $prefix_length = strlen(Database::getConnection('default', $connection_key)->tablePrefix());
    $this->variationsTable = 'salsify_variations_' . drupal_strtolower($machine_name);
    $this->variationsTable = drupal_substr($this->variationsTable, 0, 63 - $prefix_length);
    parent::__construct($machine_name, $source_key, $destination_key, $connection_key, $options);
  }

  /**
   * Gets the name of the product variations table.
   *
   * @return string
   *   Variations table name
   */
  public function getVariationsTable() {
    return $this->variationsTable;
  }

  /**
   * {@inheritdoc}
   */
  protected function ensureTables() {
    if (!$this->ensured) {
      if (!$this->connection->schema()->tableExists($this->variationsTable)) {
        $schema = array(
          'description' => t('IDs for shared fields product variations.'),
          'fields' => array(
            'display_id' => array(
              'description' => t('A unique ID for a group of product variations.'),
              'type' => 'int',
              'unsigned' => TRUE,
              'not null' => TRUE,
            ),
            'product_id' => array(
              'description' => t('Product ID of a member of a group of product variations.'),
              'type' => 'varchar',
              'length' => 255,
              'not null' => TRUE,
            ),
          ),
          'primary key' => array('display_id', 'product_id'),
        );
        $this->connection->schema()->createTable($this->variationsTable, $schema);
      }
    }
    parent::ensureTables();
  }

  /**
   * {@inheritdoc}
   */
  public function delete(array $source_key, $messages_only = FALSE) {
    if (!$messages_only) {
      db_delete($this->variationsTable)
        ->condition('display_id', $source_key)
        ->execute();
    }
    parent::delete($source_key, $messages_only);
  }

  /**
   * {@inheritdoc}
   */
  public function deleteBulk(array $source_keys) {
    db_delete($this->variationsTable)
      ->condition('display_id', $source_keys)
      ->execute();
    parent::deleteBulk($source_keys);
  }

  /**
   * {@inheritdoc}
   */
  public function destroy() {
    $this->connection->schema()->dropTable($this->variationsTable);
    parent::destroy();
  }

}

/**
 * Imports product displays from Salsify.
 */
class SalsifyDisplaysMigration extends SalsifyProductsMigration {

  /**
   * Source product ID field.
   *
   * @var string
   */
  protected $productId = 'id';

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    SalsifyMigration::__construct($arguments);
    $urls = array();
    $attributes = array();
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
        // Save attributes.
        $attributes[$attribute->{'salsify:id'}] = $attribute;
        // Save source fields.
        foreach ($attribute->{'salsify:entity_types'} as $entity_type) {
          if (isset($source_fields[$entity_type])) {
            $source_fields[$entity_type][$attribute->{'salsify:id'}] = $attribute->{'salsify:name'};
          }
        }
      }
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
        elseif (in_array($key, $product_reference_fields)) {
          foreach ((array) $mapping->getSourceMigration() as $source_migration) {
            if (str_replace(array(
              'NewProductsChannel',
              'ExistingProductsChannel',
            ), '', $source_migration) != $source_migration) {
              // The product mapping uses both the product ID and the revision
              // ID to match source records with destination records. Only the
              // product ID is used in a product reference field, so we need to
              // remove the revision ID from the mapping.
              $mapping->callbacks('salsify_product_ids');
            }
          }
        }
        elseif (!$this->downloadAssets) {
          $destination_field = $mapping->getDestinationField();
          if (isset($destination_field) && $source_field == 'salsify:digital_assets::salsify:url') {
            $this->downloadAssets = TRUE;
          }
        }
      }
    }
    $entity_info = entity_get_info($arguments['entity_type']);
    // Select the most specific destination class available. Destination classes
    // don't seem to be registered anywhere, so we list the ones we know here.
    switch ($arguments['entity_type']) {
      case 'comment':
        $dest_class = 'MigrateDestinationComment';
        $dest_args = array($arguments['bundle']);
        $schema_args = array();
        break;

      case 'file':
        $dest_class = 'MigrateDestinationFile';
        $dest_args = array($arguments['bundle']);
        $schema_args = array();
        break;

      case 'node':
        $dest_class = 'MigrateDestinationNode';
        $dest_args = array($arguments['bundle']);
        $schema_args = array();
        break;

      case 'taxonomy_term':
        $dest_class = 'MigrateDestinationTerm';
        $dest_args = array($arguments['bundle']);
        $schema_args = array();
        break;

      case 'user':
        $dest_class = 'MigrateDestinationUser';
        $dest_args = array();
        $schema_args = array();
        break;

      default:
        $dest_class = 'MigrateDestinationEntityAPI';
        $dest_args = array(
          $arguments['entity_type'],
          $arguments['bundle'],
        );
        $schema_args = array($arguments['entity_type']);
        break;
    }
    $this->description = format_plural(count($arguments['channel_ids']), 'Import %entity product displays from Salsify channel %channel_ids.', 'Import %entity product displays from Salsify channels %channel_ids', array(
      '%entity' => $arguments['entity_type'],
      '%channel_ids' => implode(', ', $arguments['channel_ids']),
    ));
    // Map source rows to destination items.
    if (isset($arguments['variation'])) {
      switch ($arguments['variation']['type']) {
        case 'relation':
          $key_fields = array($this->productId);
          if (isset($arguments['variation']['attribute_id']) && isset($arguments['variation']['relation_type'])) {
            $id_field = $arguments['variation'];
            $id_field['product_id'] = $this->productId;
          }
          else {
            $id_field = $this->productId;
          }
          $reader_class = 'SalsifyDisplaysRelationsJSONReader';
          break;

        case 'fields':
          $key_fields = array('salsify:display_id');
          $id_field = array_diff(array_keys($source_fields['products']), $arguments['unmigrated_sources'], array($this->productId));
          $id_field['product_id'] = $this->productId;
          $reader_class = 'SalsifyDisplaysFieldsJSONReader';
          break;
      }
    }
    else {
      $key_fields = $id_field = array($this->productId);
      $reader_class = 'SalsifyProductsJSONReader';
    }
    $source_key = array();
    foreach ($key_fields as $name) {
      $source_key[$name] = array(
        'not null' => TRUE,
        'description' => $name == 'salsify:display_id' ? t('Local display ID') : $source_fields['products'][$name],
      );
      if (isset($attributes[$name]->{'salsify:data_type'})) {
        $data_type = $attributes[$name]->{'salsify:data_type'};
      }
      else {
        // Set data types for special attributes.
        switch (end(explode('::', $name))) {
          case 'salsify:digital_assets::salsifyUrlEncode':
            $data_type = 'boolean';
            break;

          default:
            $data_type = 'string';
            break;
        }
      }
      // Convert Salsify data types to Drupal data types.
      switch ($data_type) {
        case 'integer':
          $source_key[$name]['type'] = 'int';
          break;

        case 'float':
          $source_key[$name]['type'] = 'float';
          break;

        case 'boolean':
          $source_key[$name]['type'] = 'int';
          $source_key[$name]['size'] = 'tiny';
          $source_key[$name]['unsigned'] = TRUE;
          break;

        default:
          $source_key[$name]['type'] = 'varchar';
          $source_key[$name]['length'] = 255;
          break;
      }
    }
    if (isset($arguments['variation']) && $arguments['variation']['type'] == 'fields') {
      $map_class = 'SalsifyDisplaysMigrateSQLMap';
    }
    else {
      $map_class = 'MigrateSQLMap';
    }
    $this->map = new $map_class($this->machineName, $source_key, call_user_func_array(array(
      $dest_class,
      'getKeySchema',
    ), $schema_args));
    $this->source = new MigrateSourceJSON($urls, $id_field, $source_fields['products'], array(
      'reader_class' => $reader_class,
      'cache_counts' => TRUE,
      'cache_key' => $this->cacheKey,
    ));
    $class = new ReflectionClass($dest_class);
    $this->destination = $class->newInstanceArgs($dest_args);
    // The MigrateFieldMapping class must be constructed at least once to
    // populate the priorities property.
    new MigrateFieldMapping('dummy', 'dummy');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareKey($source_key, $row) {
    if (isset($this->arguments['variation']) && $this->arguments['variation']['type'] == 'fields') {
      // Look for display ID that maps to one of our products.
      $result = db_select($this->map->getVariationsTable(), 'v')
        ->fields('v', array('display_id'))
        ->condition('v.product_id', $row->{$this->productId})
        ->range(0, 1)
        ->execute();
      $row->{'salsify:display_id'} = $result->fetchField();
      if ($row->{'salsify:display_id'} === FALSE) {
        // Generate unique ID.
        $row->{'salsify:display_id'} = db_next_id();
        // Get list of new products.
        $variations = (array) $row->{$this->productId};
      }
      else {
        // Delete products that are no longer part of this display.
        db_delete($this->map->getVariationsTable())
          ->condition('display_id', $row->{'salsify:display_id'})
          ->condition('product_id', (array) $row->{$this->productId}, 'NOT IN')
          ->execute();
        // Get list of new products.
        $result = db_select($this->map->getVariationsTable(), 'v')
          ->fields('v', array('product_id'))
          ->condition('display_id', $row->{'salsify:display_id'})
          ->execute();
        $variations = array_diff((array) $row->{$this->productId}, $result->fetchCol());
      }
      // Add new products to display.
      $query = db_insert($this->map->getVariationsTable())
        ->fields(array('display_id', 'product_id'));
      foreach ($variations as $product_id) {
        $query->values(array(
          'display_id' => $row->{'salsify:display_id'},
          'product_id' => $product_id,
        ));
      }
      $query->execute();
    }
    return parent::prepareKey($source_key, $row);
  }

  /**
   * Remove entry from variations table.
   *
   * @param int $display_id
   *   Node ID of product display.
   */
  public function completeRollback($display_id) {
    if (isset($this->arguments['variation']) && $this->arguments['variation']['type'] == 'fields') {
      db_delete($this->map->getVariationsTable())
        ->condition('display_id', $display_id)
        ->execute();
    }
  }

  /**
   * Creates a product display stub.
   */
  protected function createStub($migration, $source_key) {
    $node = new stdClass();
    if (isset($this->arguments['variation']) && $this->arguments['variation']['type'] == 'fields') {
      $key_field = 'salsify:display_id';
    }
    else {
      $key_field = $this->productId;
    }
    if (!empty($source_key[$key_field])) {
      migrate_instrument_start('SalsifyDisplaysMigration::createStub');
      $node->type = $this->getDestination()->getBundle();
      node_object_prepare($node);
      $node->title = t('Stub: !key', array('!key' => $source_key[$key_field]));
      node_save($node);
      migrate_instrument_stop('SalsifyDisplaysMigration::createStub');
    }
    return isset($node->nid) ? $node->nid : FALSE;
  }

}

/**
 * Loads a specified type of product relation from a Salsify channel.
 */
class SalsifyRelationsTypeJSONReader extends SalsifyJSONReader {

  protected $attributeId;
  protected $relationType;
  protected $currentProduct;
  protected $relations = array();

  /**
   * {@inheritdoc}
   */
  public function __construct($json_url, $id_field) {
    // We added the attribute ID and relation type to the $id_field argument
    // because that was the only way to get it to our reader without writing our
    // own source class.
    $this->attributeId = $id_field['attribute_id'];
    $this->relationType = $id_field['relation_type'];
    parent::__construct($json_url, array());
  }

  /**
   * {@inheritdoc}
   */
  public function rewind() {
    // Start with an empty list of relations.
    $this->relations = array();
    parent::rewind();
  }

  /**
   * {@inheritdoc}
   */
  public function next() {
    // If a product is loaded, load next new relation.
    if (!empty($this->currentProduct)) {
      do {
        $this->currentElement = next($this->currentProduct->{'salsify:relations'});
        if (!empty($this->currentElement)) {
          $this->currentId = $this->currentElement->{'salsify:target_product_id'};
        }
      } while (!empty($this->currentElement) && (in_array($this->currentId, $this->relations) || $this->currentElement->{$this->attributeId} != $this->relationType));
    }
    // If the current product has no new relations, load the next product that
    // does.
    if (empty($this->currentElement)) {
      do {
        $this->salsifyNext('products');
        $this->currentProduct = $this->currentElement;
        $this->currentElement = NULL;
        $this->currentId = NULL;
        if (isset($this->currentProduct->{'salsify:relations'})) {
          foreach ($this->currentProduct->{'salsify:relations'} as $relation) {
            if (!in_array($relation->{'salsify:target_product_id'}, $this->relations) && $relation->{$this->attributeId} == $this->relationType) {
              $this->currentElement = $relation;
              $this->currentId = $relation->{'salsify:target_product_id'};
              break;
            }
          }
        }
      } while (!empty($this->currentProduct) && !isset($this->currentElement));
    }
    $this->relations[] = $this->currentId;
  }

}

/**
 * Loads all product relations from a Salsify channel.
 */
class SalsifyRelationsJSONReader extends SalsifyJSONReader {

  protected $currentProduct;
  protected $relations = array();

  /**
   * {@inheritdoc}
   */
  public function rewind() {
    // Start with an empty list of relations.
    $this->relations = array();
    parent::rewind();
  }

  /**
   * {@inheritdoc}
   */
  public function next() {
    // If a product is loaded, load next new relation.
    if (!empty($this->currentProduct)) {
      do {
        $this->currentElement = next($this->currentProduct->{'salsify:relations'});
        if (!empty($this->currentElement)) {
          $this->currentId = $this->currentElement->{'salsify:target_product_id'};
        }
      } while (!empty($this->currentElement) && in_array($this->currentId, $this->relations));
    }
    // If the current product has no new relations, load the next product that
    // does.
    if (empty($this->currentElement)) {
      do {
        $this->salsifyNext('products');
        $this->currentProduct = $this->currentElement;
        $this->currentElement = NULL;
        $this->currentId = NULL;
        if (isset($this->currentProduct->{'salsify:relations'})) {
          foreach ($this->currentProduct->{'salsify:relations'} as $relation) {
            if (!in_array($relation->{'salsify:target_product_id'}, $this->relations)) {
              $this->currentElement = $relation;
              $this->currentId = $relation->{'salsify:target_product_id'};
              break;
            }
          }
        }
      } while (!empty($this->currentProduct) && !isset($this->currentElement));
    }
    $this->relations[] = $this->currentId;
  }

}

/**
 * Loads displays from a Salsify channel. Skips product relations.
 */
class SalsifyDisplaysRelationsJSONReader extends SalsifyJSONReader {

  protected $attributeId;
  protected $relationType;
  protected $relations = array();

  /**
   * {@inheritdoc}
   */
  public function __construct($json_url, $id_field) {
    if (is_array($id_field)) {
      // Skip a specified type of product relation.
      $this->attributeId = $id_field['attribute_id'];
      $this->relationType = $id_field['relation_type'];
      $reader = new SalsifyRelationsTypeJSONReader($json_url, $id_field);
      $id_field = $id_field['product_id'];
    }
    else {
      // Skip all product relations.
      $reader = new SalsifyRelationsJSONReader($json_url, array());
    }
    foreach ($reader as $relation) {
      $this->relations[] = $relation->{'salsify:target_product_id'};
    }
    parent::__construct($json_url, $id_field);
  }

  /**
   * {@inheritdoc}
   */
  public function next() {
    // If the current product is the relation of another product, load the next
    // product.
    do {
      $this->salsifyNext('products');
    } while (isset($this->currentId) && in_array($this->currentId[0], $this->relations));
    if (isset($this->currentElement)) {
      // Add standalone products as their own relations.
      $standalone = TRUE;
      if (!empty($this->currentElement->{'salsify:relations'})) {
        // Products with a variation relation are not standalone.
        if (isset($this->attributeId)) {
          foreach ($this->currentElement->{'salsify:relations'} as $relation) {
            if (isset($relation->{$this->attributeId}) && $relation->{$this->attributeId} == $this->relationType) {
              $standalone = FALSE;
              break;
            }
          }
        }
        else {
          $standalone = FALSE;
        }
      }
      if ($standalone) {
        $relation = new stdClass();
        $relation->{'salsify:target_product_id'} = $this->currentElement->{$this->idField[0]};
        if (isset($this->attributeId)) {
          $relation->{$this->attributeId} = $this->relationType;
        }
        $this->currentElement->{'salsify:relations'} = array($relation);
      }
    }
  }

}

/**
 * Loads one display per set of shared field values from a Salsify channel.
 */
class SalsifyDisplaysFieldsJSONReader extends SalsifyJSONReader {

  protected $productId;
  protected $variations = array();
  protected $seenIds = array();

  /**
   * {@inheritdoc}
   */
  public function __construct($json_url, $id_field) {
    $this->productId = $id_field['product_id'];
    unset($id_field['product_id']);
    $reader = new SalsifyProductsJSONReader($json_url, $id_field);
    foreach ($reader as $key => $variation) {
      $this->variations[$key][] = $variation->{$this->productId};
    }
    parent::__construct($json_url, $id_field);
  }

  /**
   * {@inheritdoc}
   */
  public function rewind() {
    // Start with an empty list of seen IDs.
    $this->seenIds = array();
    parent::rewind();
  }

  /**
   * {@inheritdoc}
   */
  public function next() {
    // If the current display ID has been seen before, load the next new
    // product.
    do {
      $this->salsifyNext('products');
      $hash = $this->key();
    } while (isset($hash) && in_array($hash, $this->seenIds));
    if (isset($hash)) {
      // Add all products sharing field values to this display.
      $this->currentElement->{$this->productId} = $this->variations[$hash];
      $this->seenIds[] = $hash;
    }
  }

}
