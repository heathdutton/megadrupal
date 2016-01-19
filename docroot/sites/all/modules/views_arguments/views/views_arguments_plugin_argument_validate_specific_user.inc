<?php

class views_arguments_plugin_argument_validate_specific_user extends views_plugin_argument_validate {

  function option_definition() {
    $options = parent::option_definition();
    $options['permission'] = array('default' => '');
    $options['restrict_roles'] = array('default' => FALSE, 'bool' => TRUE);
    $options['roles'] = array('default' => array());

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $perms = array();
    $module_info = system_get_info('module');

    // Get list of permissions.
    foreach (module_implements('permission') as $module) {
      $permissions = module_invoke($module, 'permission');
      foreach ($permissions as $name => $perm) {
        $perms[$module_info[$module]['name']][$name] = strip_tags($perm['title']);
      }
    }

    ksort($perms);

    $form['permission'] = array(
      '#type' => 'select',
      '#options' => $perms,
      '#title' => t('Permission'),
      '#default_value' => $this->options['permission'],
    );

    $form['restrict_roles'] = array(
      '#type' => 'checkbox',
      '#title' => t('Restrict user based on role'),
      '#default_value' => $this->options['restrict_roles'],
    );
    $form['roles'] = array(
      '#type' => 'checkboxes',
      '#prefix' => '<div id="edit-options-validate-options-user-specific-roles-wrapper">',
      '#suffix' => '</div>',
      '#title' => t('Restrict to the selected roles'),
      '#options' => array_map('check_plain', user_roles(TRUE)),
      '#default_value' => $this->options['roles'],
      '#description' => t('If no roles are selected, users from any role will be allowed.'),
      '#dependency' => array(
        'edit-options-validate-options-user-specific-restrict-roles' => array(1),
      ),
    );
  }

  function options_submit(&$form, &$form_state, &$options = array()) {
    // filter trash out of the options so we don't store giant unnecessary arrays
    $options['roles'] = array_filter($options['roles']);
  }

  function validate_argument($argument) {
    global $user;
    $account = user_load($argument);

    $valid = (bool) $user->uid;
    $valid &= $user->uid == $argument || user_access($this->options['permission']);
    $valid &= empty($this->options['restrict_roles']) || empty($this->options['roles']) || array_intersect_key($this->options['roles'], $account->roles);

    return $valid;
  }

}
