<?php

/**
 * Controller class for node_tab entities.
 */
class NodeTabController extends EntityAPIControllerExportable {

  /**
   * Overrides EntityAPIControllerExportable::buildQuery().
   */
  protected function buildQuery($ids, $conditions = array(), $revision_id = FALSE) {
    $query = parent::buildQuery($ids, $conditions, $revision_id);
    $query->orderBy('base.weight');
    $query->orderBy('base.name');
    return $query;
  }

  /**
   * Overrides EntityAPIControllerExportable::delete().
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $entities = $ids ? $this->load($ids) : FALSE;
    if ($entities) {
      // Delete the tabs.
      parent::delete($ids, $transaction);

      // Save the node type and machine name of every deleted tab, so that its
      // path aliases can be purged later. Then mark the node type for path
      // alias rebuild, so that the purge can happen.
      $deleted_tabs = variable_get('node_tab_deleted', array());
      $rebuild = variable_get('node_tab_rebuild', array());
      foreach ($entities as $entity) {
        $deleted_tabs[$entity->node_type][] = $entity->name;
        $rebuild[$entity->node_type] = $entity->node_type;
      }
      variable_set('node_tab_deleted', $deleted_tabs);
      variable_set('node_tab_rebuild', $rebuild);
    }
  }

  /**
   * Overrides EntityAPIControllerExportable::delete().
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    // Load the original tab, if this is an update.
    if (!empty($entity->tab_id) && !isset($entity->original)) {
      $entity->original = entity_load_unchanged('node_tab', $entity->tab_id);
    }

    // Mark its node type for path alias rebuild.
    // Done for new tabs and for updated tabs whose machine name changed.
    if (empty($entity->tab_id) || $entity->original->name != $entity->name) {
      $rebuild = variable_get('node_tab_rebuild', array());
      $rebuild[$entity->node_type] = $entity->node_type;
      variable_set('node_tab_rebuild', $rebuild);
      // In case the tab was renamed, the rebuild will create a path alias for
      // the new tab name, so the old tab path alias needs to be removed.
      if (isset($entity->original) && $entity->original->name != $entity->name) {
        $deleted_tabs = variable_get('node_tab_deleted', array());
        $deleted_tabs[$entity->node_type][] = $entity->name;
        variable_set('node_tab_deleted', $deleted_tabs);
      }
    }

    // Go ahead and save the tab.
    parent::save($entity, $transaction);
  }
}
