<?php
/**
 * @file
 * Suite CRM integration API documentation.
 */

/**
 * Allows to modify the entity schema of clients resources when it's created.
 *
 * This is an own hook and not the regular hook_schema_alter() because the
 * schema for clients_suitecrm entities is pre-generated and later on set
 * using hook_schema().
 *
 * @see clients_suitecrm_crm_to_entity_schema()
 *
 * @param array $schema
 *   The schema definition to create the entity info of (fields / properties).
 * @param object $clients_resource
 *   The clients resource this entity info is created of.
 */
function hook_clients_suitecrm_schema_alter(&$schema, $clients_resource) {
  // No example yet.
}


/**
 * Allows to modify the entity info of clients resources when it's created.
 *
 * This is an own hook and not the regular hook_entity_info_alter() because the
 * entity info for clients_suitecrm entities are pre-generated and later on set
 * using hook_entity_info().
 * But you can use hook_entity_info_alter() as well, however this hook provides
 * more metadata.
 *
 * @see clients_suitecrm_crm_to_entity_info()
 * @see remote_entity_hook_entity_info()
 * @see hook_entity_info()
 * @see entity_crud_hook_entity_info()
 *
 * @param array $entity_info
 *   The generated entity info as array.
 * @param clients_resource_remote_entity $clients_resource
 *   The clients resource this entity info is created of.
 * @param array $schema
 *   The schema definition to create the entity info of (fields / properties).
 */
function hook_clients_suitecrm_entity_info_alter(&$entity_info, $clients_resource, $schema) {
  if (isset($entity_info['suitecrm_leads'])) {
    $entity_info['suitecrm_leads']['controller class'] = '\\Drupal\\clients_suitecrm\\Controller\\SuiteCrmControllerLeads';
  }
}
