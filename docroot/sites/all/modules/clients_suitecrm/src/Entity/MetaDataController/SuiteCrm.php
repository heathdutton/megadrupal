<?php
/**
 * @file
 * Provides a generic entity class for SuiteCRM entities.
 */

namespace Drupal\clients_suitecrm\Entity\MetaDataController;

/**
 * Generic entity class for suiteCRM remote entities.
 */
class SuiteCrm extends \RemoteEntityAPIDefaultMetadataController {

  /**
   * Return a set of properties for an entity based on the schema definition.
   */
  protected function convertSchema() {
    $properties = parent::convertSchema($this->info['base table']);
    // Add the options list handler from enum fields.
    $schema = drupal_get_schema($this->info['base table']);
    foreach ($schema['fields'] as $name => $info) {
      if (isset($properties[$name]) && isset($info['suite_options'])) {
        $properties[$name]['options list'] = 'clients_suitecrm_enum_options_list';
      }
    }
    return $properties;
  }

  /**
   * {@inheritdoc}
   */
  public function entityPropertyInfo() {
    // Let remote entity do it's job.
    $parent_info = parent::entityPropertyInfo();

    $properties = &$parent_info[$this->type]['properties'];

    // Add shadowing information to the properties.
    $shadowing_properties = $this->info['property map'];
    $entity_resource = clients_resource_get_for_component('remote_entity', $this->type);

    // Don't add auto shadowing for additional properties. If one needs
    // shadowing for additional properties it can be added using the alter hook.
    if (!empty($entity_resource->configuration['additional_properties'])) {
      foreach ($entity_resource->configuration['additional_properties'] as $additional_property) {
        unset($shadowing_properties[$additional_property['local_property']]);
      }
    }

    foreach ($shadowing_properties as $mirrored_remote_property => $remote_property) {
      if ($remote_property != 'id') {
        $local_property = 'crm_' . $remote_property;
        if (isset($properties[$mirrored_remote_property])) {
          $properties[$mirrored_remote_property] += array(
            'remote property shadowing' => array(
              'local property' => $local_property,
            ) + $this->getPropertyShadowingHandler($entity_resource, $remote_property, $local_property),
          );
          if (isset($properties[$local_property]['options list'])) {
            $properties[$mirrored_remote_property]['options list'] = $properties[$local_property]['options list'];
          }
        }

        // Ensure the local property has a setter callback too.
        if (isset($properties[$local_property])) {
          $properties[$local_property] += array(
            'setter callback' => 'entity_property_verbatim_set',
          );
        }
      }
    }

    // Ensure the bundle property always has a label.
    if (!empty($this->info['entity keys']['bundle']) && ($key = $this->info['entity keys']['bundle']) && !isset($parent_info[$this->type]['properties'][$key]['label'])) {
      $parent_info[$this->type]['properties'][$key]['label'] = t('Type');
    }

    return $parent_info;
  }

  /**
   * Return the property shadowing handler for certain suiteCRM data types.
   *
   * - datetime: Converted to Unix timestamp.
   * - date: Converted to Unix timestamp.
   *
   * @param clients_resource_remote_entity $entity_resource
   *   The clients resource to handle.
   * @param string $remote_property
   *   The remote property to handle.
   * @param string $local_property
   *   The related local property.
   *
   * @return array
   *   Array with the local to remote and remote to local callbacks for the
   *   shadowing handling.
   */
  protected function getPropertyShadowingHandler($entity_resource, $remote_property, $local_property) {
    if (isset($entity_resource->configuration['fields']['module_fields'][$remote_property])) {
      switch ($entity_resource->configuration['fields']['module_fields'][$remote_property]['type']) {
        // Date fields.
        case 'date':
          return array(
            'local to remote callback' => 'clients_suitecrm_shadowing_schema_property_verbatim_date',
            'remote to local callback' => 'clients_suitecrm_shadowing_schema_property_verbatim_date',
          );

        // Datetime fields.
        case 'datetime':
          return array(
            'local to remote callback' => 'clients_suitecrm_shadowing_schema_property_verbatim_datetime',
            'remote to local callback' => 'clients_suitecrm_shadowing_schema_property_verbatim_datetime',
          );
      }
    }

    // Default to the standard handler from remote entity.
    return array(
      'local to remote callback' => 'remote_entity_shadowing_schema_property_verbatim_named',
      'remote to local callback' => 'remote_entity_shadowing_schema_property_verbatim_named',
    );
  }
}
