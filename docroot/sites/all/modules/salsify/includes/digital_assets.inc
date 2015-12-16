<?php

/**
 * @file
 * includes/digital_assets.inc
 */

/**
 * Imports digital assets from Salsify.
 */
class SalsifyDigitalAssetsMigration extends SalsifyMigration {

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
    $source_fields = $this->salsifyAttributes();
    foreach ($this->arguments['channels'] as $channel_id => &$run) {
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
        // Save digital asset attributes.
        if (in_array('digital_assets', $attribute->{'salsify:entity_types'})) {
          $source_fields[$attribute->{'salsify:id'}] = $attribute->{'salsify:name'};
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
    $this->description = t('Import digital assets from Salsify.');
    // Map source rows to destination items.
    $this->map = new MigrateSQLMap($this->machineName, array(
      'salsify:id' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'description' => 'Digital asset ID',
      ),
    ), MigrateDestinationFile::getKeySchema());
    $this->source = new MigrateSourceJSON($urls, array(), $source_fields, array(
      'reader_class' => 'SalsifyDigitalAssetsJSONReader',
      'cache_counts' => TRUE,
      'cache_key' => $this->cacheKey,
    ));
    $this->destination = new MigrateDestinationFile($arguments['bundle']);
    // The MigrateFieldMapping class must be constructed at least once to
    // populate the priorities property.
    new MigrateFieldMapping('dummy', 'dummy');
  }

  /**
   * {@inheritdoc}
   */
  static public function salsifyAttributes() {
    return array(
      'salsify:id' => t('Asset ID'),
      'salsify:name' => t('A brief name for the digital asset'),
      'salsify:url' => t('A URL that the digital asset can be fetched from'),
      'salsify:source_url' => t('The URL that the digital asset was originally fetched from'),
      'salsify:is_primary_image' => t('Digital asset to be shown when displaying product images'),
      'salsifyUrlEncode' => t('Whether the URL needs to be encoded. This is set to FALSE if we retrieve the file from a .ZIP download.'),
    ) + parent::salsifyAttributes();
  }

  /**
   * {@inheritdoc}
   */
  protected function preImport() {
    parent::preImport();
    foreach ($this->arguments['channels'] as $run) {
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

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $return = parent::prepareRow($row);
    if ($return) {
      // Find downloaded .ZIP file for active URL.
      $url = $this->getSource()->activeUrl();
      // Check for digital asset in downloaded .ZIP file.
      if (!empty($this->archiver[$url]) && $this->archiver[$url] instanceof ArchiverZip) {
        $name = $row->{'salsify:name'};
        $zip = $this->archiver[$url]->getArchive();
        if ($zip->locateName($name) !== FALSE) {
          $row->{'salsify:url'} = "zip://$zip->filename#$name";
          $row->salsifyUrlEncode = FALSE;
        }
      }
    }
    return $return;
  }

  /**
   * Creates a file stub.
   */
  protected function createStub($migration, $source_key) {
    $file = new stdClass();
    if (!empty($source_key[0])) {
      migrate_instrument_start('SalsifyDigitalAssetsMigration::createStub');
      $file = file_save_data('', "temporary://stub_$source_key[0]");
      migrate_instrument_stop('SalsifyDigitalAssetsMigration::createStub');
    }
    return isset($file->fid) ? array($file->fid) : FALSE;
  }

  /**
   * {@inheritdoc}
   */
  protected function postRollback() {
    parent::postRollback();
    $mappings = $this->getFieldMappings();
    if (isset($mappings['destination_dir'])) {
      $destination_dir = $mappings['destination_dir']->getDefaultValue();
      if (is_dir($destination_dir)) {
        $dir = dir($destination_dir);
        while (($entry = $dir->read()) !== FALSE) {
          if ($entry != '.' && $entry != '..') {
            break;
          }
        }
        if ($entry === FALSE) {
          drupal_rmdir($destination_dir);
        }
      }
    }
  }

}

/**
 * Loads digital assets from a Salsify channel.
 */
class SalsifyDigitalAssetsJSONReader extends SalsifyJSONReader {

  protected $currentProduct;
  protected $seenIds = array();

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
    // If a product is loaded, load next new digital asset.
    if (!empty($this->currentProduct)) {
      do {
        $this->currentElement = next($this->currentProduct->{'salsify:digital_assets'});
        if (!empty($this->currentElement)) {
          $this->currentId = $this->currentElement->{'salsify:id'};
        }
      } while (!empty($this->currentElement) && in_array($this->currentId, $this->seenIds));
    }
    // If the current product has no new digital assets, load the next product
    // that does.
    if (empty($this->currentElement)) {
      do {
        $this->salsifyNext('products');
        $this->currentProduct = $this->currentElement;
        $this->currentElement = NULL;
        $this->currentId = NULL;
        if (isset($this->currentProduct->{'salsify:digital_assets'})) {
          foreach ($this->currentProduct->{'salsify:digital_assets'} as $digital_asset) {
            if (!in_array($digital_asset->{'salsify:id'}, $this->seenIds)) {
              $this->currentElement = $digital_asset;
              $this->currentId = $digital_asset->{'salsify:id'};
              break;
            }
          }
        }
      } while (!empty($this->currentProduct) && !isset($this->currentElement));
    }
    $this->seenIds[] = $this->currentId;
  }

}
