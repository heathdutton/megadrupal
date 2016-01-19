<?php
/**
 * @file
 * SuiteCrmRestInsertQuery.php
 */

namespace Drupal\clients_suitecrm\RemoteEntity\Query;

/**
 * SuiteCRM insert query builder.
 *
 * This should only be used via
 * \Drupal\Clients\Connection\SuiteCrm::remote_entity_save().
 */
class SuiteCrmRestInsertQuery extends SuiteCrmRestUpdateQuery {

  /**
   * Return the remote service method to call.
   *
   * Client Resources can overwrite this in the configuration to allow to create
   * custom calls. That way we can handled enhanced rest service functions.
   */
  protected function getRemoteMethod() {
    $clients_resource = $this->getClientsResource();
    if (!empty($clients_resource->configuration['remote_methods']['insert'])) {
      return $clients_resource->configuration['remote_methods']['insert'];
    }
    return variable_get('clients_suitecrm_default_insert_callback', 'set_entry');
  }

  /**
   * Execute the query.
   *
   * The entity must already have been set with setEntity().
   */
  public function execute() {
    // Just ensures the id isn't set then give it to the update handler.
    $this->entity->remote_id = NULL;
    return parent::execute();
  }

}
