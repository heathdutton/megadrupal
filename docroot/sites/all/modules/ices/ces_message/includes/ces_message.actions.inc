<?php

/**
 * @file
 * Implements the ces_message actions functionality.
 */

/**
 * @defgroup ces_message_actions Ces Message Actions
 * @ingroup ces_message
 * @{
 * creating a core Entity API entity.
 */

/**
 * Entity uri callback.
 */
function ces_message_action_uri($action) {
  return array(
    'path' => 'ces/bank/exchange/offerswants/action/' . $action->id,
  );
}
/**
 * Helper.
 */
function _ces_message_get_actions_options() {
  $options = array();
  $actions = ces_message_get_actions();
  foreach ($actions as $id => $cat) {
    $options[$id] = $cat->name;
  }
  return $options;
}

/**
 * Get actions entities.
 */
function ces_message_get_actions() {
  $ids = ces_message_get_actions_ids();
  return ces_message_action_load_multiple($ids);
}
/**
 * Return the list of all actions.
 */
function ces_message_get_actions_ids() {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'ces_action')
         ->propertyOrderBy('name', 'ASC');
  $result = $query->execute();
  if (isset($result['ces_action'])) {
    return array_keys($result['ces_action']);
  }
  return array();
}

/**
 * Implements hook_entity_load().
 */
function ces_message_action_load($id = NULL, $reset = FALSE) {
  $ids = (isset($id) ? array($id) : array());
  $conditions = array();
  $action = ces_message_action_load_multiple($ids, $conditions, $reset);
  return $action ? reset($action) : FALSE;
}
/**
 * Laod multiple categories.
 */
function ces_message_action_load_multiple($ids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('ces_action', $ids, $conditions, $reset);
}
/**
 * Delete action.
 */
function ces_message_action_delete($id) {
  $controller = entity_get_controller('ces_action');
  return $controller->delete($id);
}
/**
 * Access action.
 */
function ces_message_action_access($permission, $action) {
  return ces_bank_access($permission, 'exchange', $action->exchange);
}

/**
 * Implements hook_entity_save().
 */
function ces_message_action_save($action) {
  $controller = entity_get_controller('ces_action');
  return $controller->save($action);
}

/**
 * Entity controller.
 */
class CesMessageActionEntityController extends DrupalDefaultEntityController {
  /**
   * Save entity.
   */
  public function save($action) {
    if (!isset($action->id)) {
      // Insert.
      drupal_write_record('ces_action', $action);
      field_attach_insert('ces_action', $action);
      module_invoke_all('entity_insert', $action, 'ces_action');
    }
    else {
      drupal_write_record('ces_action', $action, 'id');
      field_attach_update('ces_action', $action);
      module_invoke_all('entity_update', $action, 'ces_action');
    }
    return $action;
  }
  /**
   * Delete entity.
   */
  public function delete($id) {
    // Load full entity.
    $action = ces_message_action_load($id);
    // Change the parent of child categories.
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'ces_action')
          ->propertyCondition('parent', $action->id);
    $result = $query->execute();
    if (isset($result['ces_action'])) {
      $ids = array_keys($result['ces_action']);
      $children = ces_message_action_load_multiple($ids);
      foreach ($children as $cat) {
        $cat->parent = $action->parent;
        $this->save($cat);
      }
    }
    // Change the action of offers and wants assigned to this action.
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'ces_offerwant')
          ->propertyCondition('action', $action->id);
    $result = $query->execute();
    if (isset($result['ces_action'])) {
      $ids = array_keys($result['ces_action']);
      $offerwants = ces_offerwant_load_multiple($ids);
      foreach ($offerwants as $offerwant) {
        $offerwant->action = $action->parent;
        ces_offerwant_save($offerwant);
      }
    }
    // Finally delete action from database.
    db_delete('ces_action')->condition('id', $action->id)->execute();
    field_attach_delete('ces_action', $action);
  }
}
/**
 * @} End of "defgroup ces_message_actions".
 */
