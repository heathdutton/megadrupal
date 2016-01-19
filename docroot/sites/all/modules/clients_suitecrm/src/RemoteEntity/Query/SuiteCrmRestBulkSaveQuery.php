<?php
/**
 * @file
 * SuiteCrmRestBulkSaveQuery.php
 */

namespace Drupal\clients_suitecrm\RemoteEntity\Query;

/**
 * SuiteCRM SOAP bulk save query builder.
 *
 * This should only be used via
 * \Drupal\Clients\Connection\SuiteCrm::remote_entity_save_multiple().
 */
class SuiteCrmRestBulkSaveQuery extends \RemoteEntityBulkSaveQuery {

  /**
   * Execute the bulk save query.
   *
   * The entities must already have been set with setEntities().
   *
   * @return array
   *   An array of the GUIDs that were created by the bulk save, keyed by the
   *   corresponding entity IDs.
   */
  public function execute() {
    // Make the initial connection.
    $this->connection->connect();

    // Prepare arguments. This ensures the order of the keys is as required by
    // the service even if the values are filled later on.
    $method_args = array(
      'module_name' => '',
      'name_value_list' => array(),
    );
    if (!empty($this->getClientsResource()->configuration['module']['module_key'])) {
      $method_args['module_name'] = $this->getClientsResource()->configuration['module']['module_key'];
    }

    $wrapper = entity_metadata_wrapper($this->base_entity_type, $this->entity);

    // If no properties are explicitly set simply store all remote properties.
    $remote_fields = (!empty($this->update_fields)) ? $this->update_fields : $this->entity_info['property map'];

    foreach ($this->entities as $entity) {
      $name_value_list = array();
      // Build values array.
      foreach ($remote_fields as $local_field => $remote_field) {
        $name_value_list[] = array(
          'name' => $remote_field,
          'value' => $wrapper->{$local_field}->raw(),
        );
      }
      $method_args['name_value_list'][] = $name_value_list;
    }

    // Store entries.
    // @link http://support.sugarcrm.com/02_Documentation/04_Sugar_Developer/Sugar_Developer_Guide_6.5/02_Application_Framework/Web_Services/05_Method_Calls/set_entries/
    $response = $this->connection->callMethodArray('set_entries', $method_args);

    return $response->ids;
  }


}
