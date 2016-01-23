<?php

/**
 * @file
 * Handler class for drupal_database_table_external storage plugin.
 */

class SamplerStorageHandlerDrupalDatabaseTableExternal extends SamplerStorageHandlerDrupalDatabaseTable {

  public function reportSchemaToDrupal() {
    return FALSE;
  }

  protected function setupDatabase() {
    $this->setupDatabaseExternal();
  }

}

