<?php

/**
 * @file
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 *
 * avail_roles filter handler.
 */
class imagepicker_views_handler_filter_avail_roles extends views_handler_filter {

  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  /**
   * Determine if a filter can be exposed.
   */
  function can_expose() { return FALSE; }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['value'] = array(
      '#type' => 'value',
      '#value' => $this->value,
    );
  }

  function query() {
    if (imagepicker_variable_get('imagepicker_publicroles_enabled', 0)) {
      global $user;
      $this->ensure_my_table();
      $field = $this->table_alias . '.' . $this->real_field;
      $roles = $user->roles;
      $rolesql = FALSE;
      if (count($roles)) {
        $ct = 0;
        foreach ($roles AS $role) {
          $role = '%' . db_like($role) . '%';
          if (! $ct) {
            // first time
            $rolesql = db_or()
              ->condition($field, 'all', '=')
              ->condition($field, $role, 'LIKE');
          }
          else {
            $rolesql->condition($field, $role, 'LIKE');
          }
          $ct++;
        }
      }
      if (is_object($rolesql)) {
        $this->query->add_where($this->options['group'], $rolesql);
      }
    }
  }
}
