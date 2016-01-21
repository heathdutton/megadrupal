<?php
/**
 * @file
 * SuiteCrmRestUpdateQuery.php
 */

namespace Drupal\clients_suitecrm\RemoteEntity\Query;

/**
 * SuiteCRM SOAP update query builder.
 *
 * This should only be used via
 * @see \Drupal\clients_suitecrm\Clients\Connection\SuiteCrm::remote_entity_save().
 */
class SuiteCrmRestUpdateQuery extends \RemoteEntityUpdateQuery {

  /**
   * The related clients resource.
   *
   * @var clients_resource
   */
  protected $clientsResource;

  /**
   * Returns the related clients resource.
   *
   * This needs the entity type set to work.
   *
   * @return clients_resource
   *   The related clients resource.
   */
  public function getClientsResource() {
    if (!$this->clientsResource) {
      $this->clientsResource = clients_resource_load($this->base_entity_type);
    }
    return $this->clientsResource;
  }

  /**
   * Return the remote service method to call.
   *
   * Client Resources can overwrite this in the configuration to allow to create
   * custom calls. That way we can handled enhanced rest service functions.
   */
  protected function getRemoteMethod() {
    $clients_resource = $this->getClientsResource();
    if (!empty($clients_resource->configuration['remote_methods']['update'])) {
      return $clients_resource->configuration['remote_methods']['update'];
    }
    return variable_get('clients_suitecrm_default_update_callback', 'set_entry');
  }

  /**
   * Execute the query.
   *
   * The entity must already have been set with setEntity().
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

    /** @var \EntityDrupalWrapper $wrapper */
    $wrapper = entity_metadata_wrapper($this->base_entity_type, $this->entity);

    // If no properties are explicitly set simply store all remote properties.
    $remote_fields = (!empty($this->update_fields)) ? $this->update_fields : $this->entity_info['property map'];
    // Ensure a consistent order of the fields.
    ksort($remote_fields);

    // Build values array.
    foreach ($remote_fields as $local_field => $remote_field) {
      // Skip properties that aren't set in the raw data.
      if (isset($wrapper->entity_data->raw()->{$remote_field}) || isset($wrapper->raw()->{$remote_field})) {
        $method_args['name_value_list'][] = array(
          'name' => $remote_field,
          'value' => $wrapper->{$local_field}->raw(),
        );
      }
    }

    // Store entry.
    // @link http://support.sugarcrm.com/02_Documentation/04_Sugar_Developer/Sugar_Developer_Guide_6.5/02_Application_Framework/Web_Services/05_Method_Calls/set_entry/
    $response = $this->connection->callMethodArray($this->getRemoteMethod(), $method_args);

    // There was an error. Throw an exception!
    if (!isset($response->id)) {
      $message = 'Remote save failed.';
      $error_code = 0;
      // Try to collect as much information as possible.
      if (isset($response->number)) {
        $error_code = $response->number;
      }
      if (isset($response->name)) {
        $message .= ' ' . $response->name;
      }
      if (isset($response->name)) {
        $message .= ' ' . $response->description;
      }
      // Give a random failure id to avoid further saving issues.
      $this->entity->remote_id = uniqid('remote-save-failed-', TRUE);
      $e = new \Exception($message, $error_code);
      $uri = entity_uri($this->base_entity_type, $this->entity);
      watchdog_exception('SuiteCRM', $e, NULL, array(), WATCHDOG_ERROR, l(t('Entity'), $uri['path'], $uri['options']));
      throw $e;
    }

    return (isset($response->id)) ? $response->id : FALSE;
  }

}
