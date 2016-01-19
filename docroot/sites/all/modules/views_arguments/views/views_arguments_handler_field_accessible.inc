<?php

class views_arguments_handler_field_accessible extends views_handler_field {

  public $has_access = NULL;

  /**
   * Overrides views_handler_field::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['access_type'] = array('default' => 'permissions');
    $options['permissions'] = array('default' => array());
    $options['roles'] = array('default' => array());
    $options['and_or'] = array('default' => 'or');

    return $options;
  }

  /**
   * Get #options for Permissions.
   */
  protected function get_permissions() {
    $module_info = system_get_info('module');

    $perms = array();
    foreach (module_implements('permission') as $module) {
      $permissions = module_invoke($module, 'permission');
      foreach ($permissions as $name => $perm) {
        $perms[$module_info[$module]['name']][$name] = strip_tags($perm['title']);
      }
    }

    ksort($perms);
    return $perms;
  }

  /**
   * Get #options for Roles.
   */
  protected function get_roles() {
    return db_query('
      SELECT name
      FROM {role}
      ORDER BY weight, name
    ')->fetchAllKeyed(0, 0);
  }

  /**
   * Overrides views_handler_field::options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['access_type'] = array(
      '#type' => 'radios',
      '#title' => t('Access type'),
      '#options' => array(
        'permissions' => t('Permissions'),
        'roles' => t('Roles'),
      ),
      '#default_value' => $this->options['access_type'],
    );

    $form['permissions'] = array(
      '#type' => 'select',
      '#title' => t('Permissions'),
      '#options' => $this->get_permissions(),
      '#default_value' => $this->options['permissions'],
      '#multiple' => TRUE,
      '#size' => 10,
    );
    $form['permissions']['#states']['visible']['input[name="options[access_type]"]']['value'] = 'permissions';

    $form['roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Roles'),
      '#options' => $this->get_roles(),
      '#default_value' => $this->options['roles'],
    );
    $form['roles']['#states']['visible']['input[name="options[access_type]"]']['value'] = 'roles';

    $form['and_or'] = array(
      '#type' => 'radios',
      '#title' => t('Multiples resolution'),
      '#options' => array(
        'and' => t('Require all'),
        'or' => t('Require any'),
      ),
      '#default_value' => $this->options['and_or'],
    );
  }

  /**
   * Overrides views_handler_field::options_form().
   */
  function options_submit(&$form, &$form_state) {
    // Remove unselected options.
    $form_state['values']['options']['permissions'] = array_filter($form_state['values']['options']['permissions']);
    $form_state['values']['options']['roles'] = array_filter($form_state['values']['options']['roles']);
  }

  /**
   * Find out if the current user has the configured access.
   */
  protected function _has_access() {
    global $user;

    $want = $have = array();

    // Check roles.
    if ($this->options['access_type'] == 'roles') {
      $want = $this->options['roles'];
      $have = array_intersect($user->roles, $wanteds);
    }

    // Check permissions.
    elseif ($this->options['access_type'] == 'permissions') {
      $want = $this->options['permissions'];
      $have = array_filter(array_map('user_access', $want));
    }

    if ($want) {
      // Only if they all match.
      if ($this->options['and_or'] == 'and') {
        return count($have) == count($want);
      }

      // Fine if any match.
      return TRUE;
    }
  }

  /**
   * Fetch user's access and cache in handler, because every row will be the same.
   */
  protected function has_access() {
    if ($this->has_access === NULL) {
      $this->has_access = (bool) $this->_has_access();
    }

    return $this->has_access;
  }

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    return $this->has_access() ? '1' : '';
  }

  /**
   * Overrides views_handler_field::query().
   */
  function query() {}

}
