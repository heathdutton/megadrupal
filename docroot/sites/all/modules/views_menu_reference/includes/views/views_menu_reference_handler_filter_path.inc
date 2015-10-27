<?php

/**
 * @file Views filter handler class implementation.
 */

/**
 * Filter to compare a views_menu_reference field against a given path.
 */
class views_menu_reference_handler_filter_path extends views_handler_filter_equality {

  /**
   * Provide simple equality operator.
   */
  function operator_options() {
    return array(
      // Only matches makes sense here, because it's more logic behind.
      '=' => t('matches')
    );
  }

  /**
   * Provide a text field to enter the path to filter on.
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => empty($form_state['exposed']) ? t('Path') : '',
      '#size' => 20,
      '#default_value' => $this->value,
    );
  }

  /**
   * Display the filter on the administrative summary.
   */
  function admin_summary() {
    $operators = $this->operator_options();
    return check_plain($operators[$this->operator] . ' Path: ' . $this->value);
  }

  /**
   * This filter cannot be exposed to users!
   */
  function can_expose() {
    return FALSE;
  }

  /**
   * Add the filtering to the query.
   * This is quite complex, because we have to find all matching fields by
   * the given path, which seems to be the best and fastest solution.
   */
  public function query() {
    $this->ensure_my_table();

    // The given path
    $value = $this->value;

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
          $this->options['group'], $level_conditions
      );
    }
    else {
      // There is no hierarchy for the path that might match, so there can
      // not be a matching related node. We represnt this knowledge by setting
      // the condition totally false (1=0).
      $this->query->add_where_expression(
          $this->options['group'], '1=0');
    }
  }

}

