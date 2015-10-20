<?php

/**
 * @file
 * Mock test class for MaPS Import mapping class.
 */

use Drupal\maps_import\Mapping\Media as MediaMapping;

/**
 * Mock class.
 */
class MapsImportMappingMediaMock extends MediaMapping {
  
  /**
   * @inheritdoc
   */
  public function getFileUri($path, $preset, $filename) {
    return drupal_get_path('module', 'maps_import') . "/tests/files/medias/{$path}/{$preset}{$filename}";
  }
  
}
