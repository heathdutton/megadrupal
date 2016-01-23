<?php

/**
 * @file
 * Views access plugin that provides node-type and role-based access control.
 */

class views_node_access_plugin_access_node_type_role extends views_plugin_access_role {
  function get_access_callback() {
    $callback = parent::get_access_callback();
    return array('views_node_access_check_node_type', array($this->options['node_types'], $callback));
  }

  function summary_title() {
    $title = parent::summary_title();
    $count = count($this->options['node_types']);
    if ($count < 1) {
      return t('No node type(s) selected and @title', array('@title' => $title));
    }
    elseif ($count > 1) {
      return t('Multiple node types and @title', array('@title' => $title));
    }
    else {
      $ids = node_type_get_names();
      $id = reset($this->options['node_types']);
      return check_plain($ids[$id]) . ' ' . t('and') . ' ' . $title;
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['node_types'] = array('default' => array());
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $node_types = node_type_get_names();
    $form['node_types'] = array(
      '#type' => 'checkboxes',
      '#options' => array_map('check_plain', $node_types),
      '#title' => t('Node types'),
      '#default_value' => $this->options['node_types'],
      '#description' => t('Only nodes with this types can display the view as tab.'),
    );
    parent::options_form($form, $form_state);
  }

  function options_validate(&$form, &$form_state) {
    if (!array_filter($form_state['values']['access_options']['node_types'])) {
      form_error($form['node_types'], t('You must select at least one node type'));
    }
    parent::options_validate($form, $form_state);
  }

  function options_submit(&$form, &$form_state) {
    $form_state['values']['access_options']['node_types'] = array_filter($form_state['values']['access_options']['node_types']);
    parent::options_submit($form, $form_state);
  }
}
