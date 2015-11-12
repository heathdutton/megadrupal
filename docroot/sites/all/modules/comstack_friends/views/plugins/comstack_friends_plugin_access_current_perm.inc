<?php

/**
 * @file
 * Contains comstack_friends_plugin_access_current_perm.
 */

/**
 * Access plugin that provides permission-based access control.
 *
 * @ingroup views_access_plugins
 */
class comstack_friends_plugin_access_current_perm extends views_plugin_access {
  function access($account) {
    return comstack_friends_views_check_perm($this->options['bypass_perm'], $this->options['perm'], $account);
  }

  function get_access_callback() {
    $callback = parent::get_access_callback();
    return array('comstack_friends_views_check_perm', array($this->options['bypass_perm'], $this->options['perm']));
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['perm'] = array('default' => 'access content');
    $options['bypass_perm'] = array('default' => 'administer users');

    return $options;
  }

  function summary_title() {
    $permissions = module_invoke_all('permission');
    if (isset($permissions[$this->options['perm']])) {
      return $permissions[$this->options['perm']]['title'];
    }

    return t($this->options['perm']);
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $perms = array();
    $module_info = system_get_info('module');

    // Get list of permissions
    foreach (module_implements('permission') as $module) {
      $permissions = module_invoke($module, 'permission');
      foreach ($permissions as $name => $perm) {
        $perms[$module_info[$module]['name']][$name] = strip_tags($perm['title']);
      }
    }

    ksort($perms);

    $form['perm'] = array(
      '#type' => 'select',
      '#options' => $perms,
      '#title' => t('Permission'),
      '#default_value' => $this->options['perm'],
      '#description' => t('Only users with the selected permission flag will be able to access this display. Note that users with "access all views" can see any view, regardless of other permissions.'),
    );

    $form['bypass_perm'] = array(
      '#type' => 'select',
      '#options' => $perms,
      '#title' => t('Bypass permission'),
      '#default_value' => $this->options['bypass_perm'],
      '#description' => t('Users with this permission will be allowed to access this view even if they are not the user in question.'),
      '#weight' => -1,
    );
  }
}
