<?php

/**
 * @file
 * Contains views access plugin for Clientcert permissions
 */

/**
 * Allow views to allow access to only users with a specified permission in the
 * current group.
 */
class clientcert_plugin_access_perm extends views_plugin_access {

  /**
   * Retrieve the options when this is a new access
   * control plugin
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['perm'] = array('default' => 'update');
    $options['clientcert_id_arg'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    $perms = array(
      'update' => 'update',
      'view' => 'view',
    );

    $form['perm'] = array(
      '#type' => 'select',
      '#options' => $perms,
      '#title' => t('Clientcert permission'),
      '#default_value' => $this->options['perm'],
      '#description' => t('Only users with the selected permission flag on this clientcert item will be able to access this display.')
    );

    $current_display = $this->view->current_display;
    if ($this->view->display[$current_display]->handler->has_path()) {
      // Show the group ID argument position only for "Page" displays.
      $form['clientcert_id_arg'] = array(
        '#type' => 'select',
        '#title' => t('Argument position for clientcert ID'),
        '#default_value' => $this->options['clientcert_id_arg'],
        '#options' => array(NULL => t('None')) + range(0, 9),
        '#description' => t('Clientcert ID argument position with arg() function. e.g. if your dynamic path is "clientcert/item/%/test" and you are using the second "%" for group IDs you have to choose "2" like "arg(2)".'),
      );
    }
  }

  /**
   * Return a string to display as the clickable title for the
   * access control.
   */
  function summary_title() {
    $current_display = $this->view->current_display;
    if ($this->options['clientcert_id_arg'] === FALSE || $this->view->display[$current_display]->display_plugin != 'page') {
      return $this->options['perm'];
    }

    $params = array(
      '@perm' => $this->options['perm'],
      '@arg' => $this->options['clientcert_id_arg'],
    );

    return t('@perm, getting the clientcert ID from argument position @arg', $params);
  }

  /**
   * Determine if the current user has access or not.
   */
  function access($account) {
    return user_access('administer clientcert');
  }

  /**
   * Determine the access callback and arguments.
   */
  function get_access_callback() {
    return array('_clientcert_views_page_access', array($this->options['perm'], $this->options['clientcert_id_arg']));
  }
}
