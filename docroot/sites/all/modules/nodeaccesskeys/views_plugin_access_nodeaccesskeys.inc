<?php

/**
 * @file
 * Access plugin that provides Node Access Key support.
 */

/**
 * Allow Views' pages to be controlled by Node Access Keys.
 *
 * @ingroup views_access_plugins
 */

class ViewsPluginAccessNodeAccessKeys extends views_plugin_access {

  /**
   * Determine if the current user has access or not.
   */
  function access($account) {
    return nodeaccesskeys_get_protection($this->options['node_type'], NODE_ACCESS_ALLOW);
  }

  /**
   * Tell Views the access callback and arguments.
   */
  function get_access_callback() {
    return array(
      'nodeaccesskeys_get_protection',
      array(
        $this->options['node_type'], NODE_ACCESS_ALLOW,
      ),
    );
  }

  /**
   * Tell Views what content type the View is set to.
   */
  function summary_title() {
    return $this->options['node_type'];
  }

  /**
   * Tell Views what options this plugin can store.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['node_type'] = array(
      'default' => variable_get('nodeaccesskeys_default_node_type', NULL),
    );
    return $options;
  }

  /**
   * Provide the form for setting options.
   */
  function options_form(&$form, &$form_state) {
    $node_types = array_map('check_plain', node_type_get_names());
    $form['node_type'] = array(
      '#type' => 'select',
      '#options' => $node_types,
      '#title' => t('Content Type'),
      '#default_value' => $this->options['node_type'],
      '#description' => t('Select a content type for this View to behave like.'),
    );
  }
}
