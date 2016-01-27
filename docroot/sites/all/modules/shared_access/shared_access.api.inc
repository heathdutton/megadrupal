<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */


/**
 * Allows modules to provide extra data that can be used by shared_access
 * controllers to determine access to an entity.
 *
 * @param $default_params
 *   An array of default parameters that can be used when determining the access
 *   for a given entity. The contents of the $default_params array are:
 *     - op: The operation being performed.
 *     - entity_type: The type of entity for which access is being checked,
 *     - entity: The entity object for which access is being checked.
 *     - field_name: The name of the current shared_access field instance being
 *         used to determine access for the entity.
 *     - instance: The current instance of the shared_access field being used. 
 *     - account: The user for whom to check access.
 *
 * @return An array containing extra info. The array that is returned will be
 *   combined with the default_params array and passed into every realm's access
 *   controller for determining access to the entity by the given user.
 *
 * The example below is from the shared_access module's own implementation of
 * this hook.
 *
 * @see shared_access()
 * @see shared_access_shared_access_params()
 */
function hook_shared_access_params($default_params) {
  $params = &drupal_static(__FUNCTION__, array());
  $id = $default_params['instance']['id']];
  if (!isset($params[$id ])) {
    $params[$id] = array();
    // Add the query string to the params if the 'mail' realm is enabled for this instance.
    if (in_array('mail', $default_params['instance']['settings']['realms'])) {
      $query_parameters = drupal_get_query_parameters();
      if (!empty($query_parameters['sa_key'])) {
        $params[$id]['sa_key'] = $query_parameters['sa_key'];
      }
    }
  }
  // only return an array if there is something to return.
  if ($params[$id]) {
    return $params[$id];
  }
}

/**
 * Allows modules to provide additional operations that can be used by shared
 * access fields to determine what kind of access a user may have to the entity
 * data. An example would be if an entity defining module used different
 * operations or permissions then the standard view, update, and delete.
 * This hook could be implemented to define the operations an entity uses and
 * then call shared_access($op, ...) from within its own access functions to
 * determine if a user has access to perform the operation on that entity. Also
 * this hook may be used to define a grouped set of operations that if granted
 * the user would have access to all of those sub operations. The example
 * provided groups the view, update and delete operation into an all encompasing
 * 'manage' operation.
 *
 * @return $ops
 *   An array of operations that can be controlled by shared access fields.
 *   The returned array contains key/value pairs where the key is the operation
 *   that would typically be used in an access function such as 'view' and the
 *   value is an array of information used by shared access to perform access
 *   functionality. The value array has the following structure:
 *   
 *   'label' - A label for the operation used in shared access forms.
 *   'description' - A description of what the operation is used for.
 *   'sub ops' - Optional: An array whose values are the keys of other shared
 *               access operations. These sub ops will inherit access to realms
 *               granted to this one.
 */
function hook_shared_access_ops() {
  $ops = array(
    'manage' => array(
      'label' => t('Manage'),
      'description' => t('Grouped set of access operations for viewing, updating and deleting an entity.'),
      'sub ops' => array('view', 'update', 'delete'),
    ),
  );
  
  return $ops;
}
