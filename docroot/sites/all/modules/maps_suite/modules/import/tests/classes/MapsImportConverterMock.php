<?php

/**
 * @file
 * Mock test class for MaPS Import converter class.
 */

use Drupal\maps_import\Converter\Media as MediaConverter;

/**
 * Mock class.
 */
class MapsImportMediaConverterMock extends MediaConverter {
  
  /**
   * @inheritdoc
   */
  public function getMapping() {
    if (!isset($this->mapping)) {
      $this->mapping = new MapsImportMappingMediaMock($this);
    }

    return $this->mapping;
  }
  
}
