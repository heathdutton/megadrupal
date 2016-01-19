<?php

/**
 * @file
 * Contains a wrapper that modifies the way the clients connection behaves so
 * that tests will actually run on drupal.org. Bummer.
 */

class clients_connection_wrapper extends clients_connection_new_relic_insights_query {

  protected function setUp() {
    $entity_info = new_relic_insights_entity_info(TRUE);
    $this->entityInfo = $entity_info['insight'];
    $this->idKey = $this->entityInfo['entity keys']['id'];
    $this->nameKey = isset($this->entityInfo['entity keys']['name']) ? $this->entityInfo['entity keys']['name'] : $this->idKey;
    $this->statusKey = empty($info['entity keys']['status']) ? 'status' : $info['entity keys']['status'];
  }

  function get_credentials_storage_plugin($plugin_id = NULL) {
    return new fake_credentials_storage_plugin();
  }

}

class fake_credentials_storage_plugin {

  function credentialsLoad($connection) {
    // Get the list of properties which are credentials.
    $credentials_properties = $connection->credentialsProperties();
    // Copy them to the credentials array, where connection classes expect to
    // find them.
    foreach ($credentials_properties as $property_name) {
      $connection->credentials[$property_name] = $connection->configuration[$property_name];
    }
  }

}
