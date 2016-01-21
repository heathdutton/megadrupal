<?php

/**
 * @file
 * Definition of views_plugin_access_role.
 */

/**
 * Access plugin that provides role-based access control.
 *
 * @ingroup views_access_plugins
 */
class vaogr_plugin_access_role extends views_plugin_access {

  /**
   * Validates user if have access.
   */
  function access($account) {
    return vaogr_check_roles(array_filter($this->options['role']), $account);

  }

  /**
   * Call function to validate access.
   */
  function get_access_callback() {
    return array('vaogr_check_roles', array(array_filter($this->options['role'])));

  }

  /**
   * Display a title.
   */
  function summary_title() {
    $count = count($this->options['role']);
    if ($count < 1) {
      return t('No OG role(s) selected');
    }
    elseif ($count > 1) {
      return t('Multiple OG roles');
    }
    else {
      $rids = views_ui_get_roles();
      $rid = reset($this->options['role']);
      return check_plain($rids[$rid]);
    }

  }

  /**
   * Initialize default options.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['role'] = array('default' => array());

    return $options;

  }

  /**
   * Displays the form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['role'] = array(
      '#type' => 'checkboxes',
      '#title' => t('OG Role'),
      '#default_value' => $this->options['role'],
      '#options' => array_map('check_plain', vaogr_get_roles()),
      '#description' => t('Only the checked OG roles will be able to access this display. Note that users with "access all views" can see any view, regardless of the OG role.'),
    );

  }

  /**
   * Validates the form.
   */
  function options_validate(&$form, &$form_state) {
    if (!array_filter($form_state['values']['access_options']['role'])) {
      form_error($form['role'], t('You must select at least one OG role if type is "by OG role"'));
    }

  }

  /**
   * Submits the form.
   */
  function options_submit(&$form, &$form_state) {
    $form_state['values']['access_options']['role'] = array_filter($form_state['values']['access_options']['role']);

  }
}
