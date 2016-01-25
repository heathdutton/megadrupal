<?php

/**
 * @file
 * Handles test for MaPS Import Fetcher classes.
 */

use Drupal\maps_import\Fetcher\Configuration;
use Drupal\maps_import\Fetcher\Objects;

/**
 * Handles tests for configuration import operation.
 */
class MapsImportConfigurationFetcherMock extends Configuration {

  protected function fetch(&$context = NULL) {
    $this->data = MapsImportRequestMock::getData($this->profile, array('file' => $this->getType()));
    return (FALSE !== $this->data);
  }

}

/**
 * Handles tests for objects and links import operation.
 */
class MapsImportObjectsFetcherMock extends Objects {

  /**
   * Get the total number of operations.
   */
  public function getTotalOperations(array $args = array()) {
    return 1;
  }

  protected function fetch(&$context = NULL) {
    $this->data = MapsImportRequestMock::getData($this->profile, array('file' => $this->getType()));
    return (FALSE !== $this->data);
  }

}
