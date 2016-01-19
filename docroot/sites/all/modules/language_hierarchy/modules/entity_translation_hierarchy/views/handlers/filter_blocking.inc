<?php

/**
 * @file
 * Definition of entity_translation_hierarchy_views_handler_filter_blocking.
 */

/**
 * Filter by blocking column.
 *
 * @ingroup views_filter_handlers
 */
class entity_translation_hierarchy_views_handler_filter_blocking extends views_handler_filter_boolean_operator {

  /**
   * Add condition to select only part of the tree that is under argument's id.
   *
   * @see entity_translation_hierarchy_views_post_execute()
   */
  function query() {
    // Behave as normal SQL query if user want to display blocking translations.
    if ($this->value) {
      parent::query();
    }
    else {
      $table = $this->ensure_my_table();

      // Add weight, depth and parent fields.
      $this->query->add_field($table, 'blocking', 'entity_translation_hierarchy_blocking');
      $this->query->add_field($table, 'entity_id', 'entity_translation_hierarchy_entity_id');
    }
  }

  /**
   * Remove parent content from views for translations that are set as blocking.
   */
  function post_execute(&$values) {
    if (!isset($values[0]->entity_translation_hierarchy_blocking)) {
      return;
    }

    $blocked_ids = array();

    // Get entity ids of blocked translations.
    foreach ($values as $index => $row) {
      if ($row->entity_translation_hierarchy_blocking) {
        $blocked_ids[] = $row->entity_translation_hierarchy_entity_id;
      }
    }

    // Remove entities from view that have blocked translations.
    foreach ($values as $index => $row) {
      if (in_array($row->entity_translation_hierarchy_entity_id, $blocked_ids)) {
        unset($values[$index]);
      }
    }
  }
}
