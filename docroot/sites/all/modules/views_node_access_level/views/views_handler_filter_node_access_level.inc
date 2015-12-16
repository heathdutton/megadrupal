<?php

/**
 * @file
 * Definition of views_handler_filter_node_access.
 */

/**
 * Filter by node_access records.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_node_access_level extends views_handler_filter {
  function admin_summary() { }
  function operator_form(&$form, &$form_state) { }
  function can_expose() {
    return FALSE;
  }

  function options_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'select',
      '#title' => t('Node access level'),
      '#description' => t('Level of access to a node'),
      '#options' => array(
        'view' => t('View'),
        'update' => t('Edit'),
        'delete' => t('Delete'),
      ),
    );
    $form['caution'] = array(
      '#markup' => t('Note: for Edit and Delete access, <strong>only node access grants</strong> are checked. Modules can override node access grants to allow or deny actions, so this check does not necessarily reflect whether the user can actually perform the Edit or Delete action.'),
    );
  }

  /**
   * See _node_access_where_sql() for a non-views query based implementation.
   */
  function query() {
    if (!user_access('administer nodes') && module_implements('node_grants')) {
      $access = !empty($this->value) ? $this->value : 'view';
      $table = $this->ensure_my_table();
      $grants = db_or();
      foreach (node_access_grants($access) as $realm => $gids) {
        foreach ($gids as $gid) {
          $grants->condition(db_and()
            ->condition($table . '.gid', $gid)
            ->condition($table . '.realm', $realm)
          );
        }
      }

      $this->query->add_where('AND', $grants);
      $this->query->add_where('AND', $table . '.grant_'. $access, 1, '>=');
    }
  }
}
