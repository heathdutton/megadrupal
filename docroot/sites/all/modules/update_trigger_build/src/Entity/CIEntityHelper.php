<?php

/**
 * @file
 *  A wrapper class for working with this module's entities.
 */

/**
 * Provides a wrapper for retrieving and formatting CI entities. This avoids 
 * duplicating code across the codebase. 
 */
class CIEntityHelper {

  /**
   * Utility function to get all CI Server objects.
   * 
   * @param boolean $options
   *  If true then convert the list of entities to a list of options for a select.
   *
   * @return array $ci_servers
   *   CI Server objects.
   */
  public function getCIServers($options = FALSE) {

    $bundles = field_info_bundles('ci_server');
    $ci_servers_array = array();

    foreach($bundles as $bundle_id => $bundle) {

      // Ignore the generic CI Server bundle.  All user created CI Servers will be of a
      // specific type. E.g. Jenkins.
      // A CI Server bundle is created automatically when we created the ci_server entity
      // because Drupal does not allow an entity without a bundle.  
      if($bundle_id == 'ci_server') {
        continue;
      }

      $efq = new EntityFieldQuery();
      $efq->entityCondition('entity_type', $bundle_id);

      // Optionally restrict the list of jobs to those that belong to a specific
      // CI server.
      if(isset($ci_server_type_id)) {
        $efq->propertyCondition('ci_server_type', $ci_server_type_id, '=');
      }

      $ci_servers = $efq->execute();

      if(empty($ci_servers)) {
        continue;
      }

      // Optionally convert the entities to a list of options for a select.
      if($options) {
        $ci_servers = $this->loadEntities($ci_servers, $bundle_id);
        $ci_servers = $this->convertToOptions($ci_servers, array('entity_type', 'id'), 'label', ':');
      }

      $ci_servers_array += $ci_servers;
    }

    return $ci_servers_array;
  }

  /**
   * Retrieves a list of CI Server Types.  
   * 
   * @return array $ci_server_types
   *  Array of entity info, keyed by entity Id.
   */
  function getCIServerTypes() {

    // All CI Server Types exist as a bundle of the CI Server entity.
    $bundles = field_info_bundles('ci_server');
    $ci_server_types = array();

    foreach($bundles as $entity_id => $bundle) {

      // Ignore the generic CI Server bundle.  All user created CI Servers will be of a
      // specific type. E.g. Jenkins.
      if($entity_id == 'ci_server') {
        continue;
      }

      $ci_server_types[$entity_id] = entity_get_info($entity_id);
    }

    return $ci_server_types;    
  }
  /**
   * Retrieve a list of the admin URLs for each of the the Server types.
   * The admin URL is where CI Server entities can be created, edited,
   * deleted, exported etc.
   *
   * @return array $admin_ui_links
   *  An array of admin UI links in a format that's suitable for the theme_links function..
   */
  public function getCIServerAdminLinks() {

    $admin_ui_links = array();    
    $ci_servers = $this->getCIServerTypes();

    foreach($ci_servers as $ci_server_type => $entity_info) {

      // Add to the array in a format that's suitable for Drupal's theme_links function.
      $admin_ui_links[] = array(
        'title' => $entity_info['label'],
        'href' => $entity_info['admin ui']['path'],
      );
    }

    return $admin_ui_links;
  }  

  /**
   * Loads a full list of entity objects.  EntityFieldQuery only returns a list 
   * of entity Ids, so this function will convert into a list of full entity 
   * objects that contain properties and fields.
   *
   * @param array $entities
   *  The list of entity Ids.
   *
   * @param string $entity_type
   *  The type of entity.  Required by the entity_load function.
   *
   * @return array $entities
   *  An array containing the full entity objects.
   */
  public function loadEntities($entities, $entity_type) {

    $loaded_entities = array();

    // Load the full entity object into the array.
    foreach($entities[$entity_type] as $entity) {

      // Load the full entity object.
      $entity = array_shift((entity_load($entity_type, array($entity->id))));

      // Explicitly set the entity type.  There is a bug in Drupal core, which 
      // makes it difficult to retrieve the entity type from a loaded entity. 
      // See https://www.drupal.org/node/1042822.
      $entity->entity_type = $entity_type;

      $loaded_entities[] = $entity;
    }

    return $loaded_entities;    
  }

  /**
   * Converts a list of retrieved entities to an associative array for use with a 
   * form API select element.
   * 
   * @parame array $entities
   *  An array of fully loaded Entity objects. 
   *
   * @param string|array $key
   *  The property that should be used as the key of the associative array.  If an 
   *  array is passed then multiple properties will be concatenated.
   *  
   * @param string|array $value
   *  The property that should be used as the value of the associative array. If an 
   *  array is passed then multiple properties will be concatenated.
   *  
   * @param string $separator
   *  If $value or $key should be concatenated then this will be used as a separator.
   * 
   * @return array $entity_assoc
   *  An associative array containing the options.
   */
  public function convertToOptions($entities, $key = 'id', $value = 'label', $separator = ' ') {

    $entity_assoc = array();

    foreach($entities as $entity) {
      $entity = (object) $entity;

      // Set the associative array key, concatenating if required.
      if(is_array($key)) {
        $str_key = $this->concatenateProperties($entity, $key, $separator);
      }
      else {
        $str_key = isset($entity->{$key}) ? $entity->{$key} : ''; 
      }

      // Set the associative array value, concatenating if required.
      if(is_array($value)) {
        $str_value = $this->concatenateProperties($entity, $value, $separator);
      }
      else {
        $str_value = isset($entity->{$value}) ? $entity->{$value} : ''; 
      }

      // Add the completed item to the associative array.
      $entity_assoc[$str_key] = $str_value;
    }

    return $entity_assoc;
  }

  /**
   * Concatenates multiple entity properties into a single string.
   * 
   * @param Entity $entity
   *  An entity object.
   * 
   * @param array $properties
   *  The entity properties to be concatenated.
   * 
   * @param string $separator
   *  The separator that will be placed in between the properties.
   */
  private function concatenateProperties($entity, $properties, $separator) {

    $concatenated = '';

    // Concatenate the properties.
    foreach($properties as $property) {
      if(isset($entity->{$property})) {
        $concatenated .= $entity->{$property} . $separator;
      }
    }

    // Remove the trailing separator.
    $concatenated = trim($concatenated, $separator);

    return $concatenated;
  }
}
