<?php

/**
 * @file
 * Handler class for drupal_database_table_per_metric_external storage plugin.
 */

class SamplerStorageHandlerDrupalDatabaseTablePerMetricExternal extends SamplerStorageHandlerDrupalDatabaseTablePerMetric {

  public function reportSchemaToDrupal() {
    return FALSE;
  }

  protected function setupDatabase() {
    $this->setupDatabaseExternal();
  }

}

