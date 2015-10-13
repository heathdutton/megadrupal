<?php

/**
 * @file Views Argument Handler class implementation.
 */

/**
 * Argument handler to compare a views_menu_reference field against a given path.
 */
class views_menu_reference_handler_argument_current_path extends views_handler_argument_string {

  /**
   * This filter cannot be exposed.
   */
  function can_expose() {
    return FALSE;
  }

  /**
   * Add the filtering to the query.
   *
   * Potentially redundant if the parent class could be replaced with an IN
   * handler.
   */
  public function query($group_by = FALSE) {
    $this->ensure_my_table();

    // The given path.
    $value = $this->argument;

    // !!!!!!! IMPORTANT !!!!!! FROM HERE ON THIS IS COMPLETELY IDENTICALLY
    // TO THE ARGUMENT IMPLEMENTATION IN "views_menu_reference_handler_argument_current_path.inc".
    // IF YOU CHANGE SOMETHING HERE, ALSO CHANGE IT THERE! ;)
    // Get the parents hierarchy of the given path, which is a simple array,
    // keyed by the depth and a + Separator for elements including children.
    // These keys are the depth value the field has to match together with the MLID:
    // The values are the MLIDs the field has to match together with the depth.
    $path_parents_hierarchy = views_menu_reference_get_link_path_parents_hierarchy($value);    
    $field_mlid = $this->get_field();
    $field_depth = str_replace('mlid', 'depth', $this->get_field());
    if (!empty($path_parents_hierarchy)) {
      // Each level is compared by an OR, because if just one level matches,
      // the related entity is linked to the menu item.
      $level_conditions = db_or();
      // Add the Only-Level-Condition (No below menu items included)
      foreach ($path_parents_hierarchy as $level => $mlids) {
        // DEPTH AND MLID have to match in one field. If both match, the related
        // entity is linked to the menu item.
        $condition = db_and()
            // On a level there may be several MLIDs that are allowed to match
            // Because there may be several menu items for one path.
            ->condition($field_mlid, $mlids, 'IN')
            ->condition($field_depth, $level);
        $level_conditions->condition($condition);
      }

      // Add the conditions to the query.
      $this->query->add_where(
          0, $level_conditions
      );
    }
    else {
      // There is no hierarchy for the path that might match, so there can
      // not be a matching related node. We represnt this knowledge by setting
      // the condition totally false (1=0).
      $this->query->add_where_expression(
          0, '1=0');
    }
  }

}
