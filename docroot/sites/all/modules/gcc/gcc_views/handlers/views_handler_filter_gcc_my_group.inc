<?php

/**
 * @file
 * Definition of views_handler_filter_workflow_state.
 */

/**
 * Filter by node type.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_gcc_my_group extends views_handler_filter_in_operator {

  /**
   * TODO.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['entity_type'] = NULL;

    return $options;
  }

  /**
   * TODO.
   */
  public function get_value_options() {

    if (!isset($this->value_options)) {

      $this->value_title = t('Groups');
      $groups = gcc_membership_get_user_memberships($GLOBALS['user']->uid);

      $groups_ids = array();
      foreach ($groups as $id => $group) {
        if ($group->entity_type == $this->definition['entity_type']) {
          $groups_ids[] = $group->entity_id;
        }
      }

      $options = array();
      $groups = entity_load($this->definition['entity_type'], $groups_ids);
      foreach ($groups as $id => $group) {
        $options[$id] = entity_label($this->definition['entity_type'], $group);
      }

      $this->value_options = $options;
    }
  }
}
