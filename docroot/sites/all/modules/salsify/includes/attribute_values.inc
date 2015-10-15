<?php

/**
 * @file
 * includes/attribute_values.inc
 */

/**
 * Imports attribute values from Salsify.
 */
class SalsifyAttributeValuesMigration extends SalsifyMigration {

  /**
   * {@inheritdoc}
   */
  protected $enabled = FALSE;

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $urls = array();
    $source_fields = $this->salsifyAttributes();
    foreach ($arguments['channel_ids'] as $channel_id) {
      $run = $this->arguments['channels'][$channel_id];
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
        // Save attribute value attributes.
        if (in_array('attribute_values', $attribute->{'salsify:entity_types'})) {
          $source_fields[$attribute->{'salsify:id'}] = $attribute->{'salsify:name'};
        }
        // Enable this migration if our attribute still exists.
        if ($attribute->{'salsify:id'} == $arguments['attribute']->{'salsify:id'}) {
          $this->enabled = TRUE;
        }
      }
    }
    // Skip mappings for fields which no longer exist.
    foreach ($this->getStoredFieldMappings() as $key => $mapping) {
      $source_field = $mapping->getSourceField();
      if ($source_field && !isset($source_fields[$source_field])) {
        unset($this->storedFieldMappings[$key]);
      }
    }
    $this->description = t('Import %name attribute values from Salsify.', array('%name' => $arguments['attribute']->{'salsify:name'}));
    // Map source rows to destination items.
    $this->map = new MigrateSQLMap($this->machineName, array(
      'salsify:id' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'description' => 'Value ID',
      ),
    ), MigrateDestinationTerm::getKeySchema());
    $this->source = new MigrateSourceJSON($urls, $arguments['attribute']->{'salsify:id'}, $source_fields, array(
      'reader_class' => 'SalsifyAttributeValuesJSONReader',
      'cache_counts' => TRUE,
      'cache_key' => $this->cacheKey,
    ));
    $this->destination = new MigrateDestinationTerm($arguments['vocabulary']);
    // The MigrateFieldMapping class must be constructed at least once to
    // populate the priorities property.
    new MigrateFieldMapping('dummy', 'dummy');
  }

  /**
   * Creates a taxonomy term stub.
   */
  protected function createStub($migration, $source_key) {
    $term = new stdClass();
    if (!empty($source_key[0])) {
      migrate_instrument_start('SalsifyAttributeValuesMigration::createStub');
      $vocabulary = taxonomy_vocabulary_machine_name_load($this->arguments['vocabulary']);
      $term->vid = $vocabulary->vid;
      $term->name = t('Stub: !key', array('!key' => $source_key[0]));
      taxonomy_term_save($term);
      migrate_instrument_stop('SalsifyAttributeValuesMigration::createStub');
    }
    return isset($term->tid) ? array($term->tid) : FALSE;
  }

  /**
   * {@inheritdoc}
   */
  static public function salsifyAttributes() {
    return array(
      'salsify:attribute_id' => t('Attribute ID'),
      'salsify:id' => t('Value ID'),
      'salsify:parent_id' => t('Parent value ID'),
      'salsify:name' => t('Value name'),
    ) + parent::salsifyAttributes();
  }

}

/**
 * Loads attribute values from a Salsify channel.
 */
class SalsifyAttributeValuesJSONReader extends SalsifyJSONReader {

  protected $attributeId;

  /**
   * {@inheritdoc}
   */
  public function __construct($json_url, $id_field) {
    // We added the attribute ID to the $id_field argument because that was the
    // only way to get it to our reader without writing our own source class.
    $this->attributeId = $id_field;
    parent::__construct($json_url, 'salsify:id');
  }

  /**
   * {@inheritdoc}
   */
  public function next() {
    // Load attribute values until we find one of ours.
    do {
      $this->salsifyNext('attribute_values');
    } while (!empty($this->currentElement) && $this->currentElement->{'salsify:attribute_id'} != $this->attributeId);
  }

}
