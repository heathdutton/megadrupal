<?php

/**
 * @file
 * Views field handler.
 */

class diff_views_handler_field_diff_radio extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['node_vid'] = array('table' => 'node_revision', 'field' => 'vid');
    $this->additional_fields['node_nid'] = array('table' => 'node_revision', 'field' => 'nid');
  }

  function access() {
    return user_access('view revisions');
  }

  function form_element_row_id($row_id) {
    return "diff-$row_id";
  }

  function form_element_name() {
    return $this->options['id'];
  }

  function render($values) {
    return '<!--form-item-' . $this->form_element_name() . '--' . $this->form_element_row_id($this->view->row_index) . '-->';
  }

  /**
   * Views form callback.
   */
  function views_form(&$form, &$form_state) {
    if (count($this->view->result) < 2) {
      return;
    }

    $field_name = $this->form_element_name();
    $form_state['storage']['diff_field_name'] = $field_name;

    foreach ($this->view->result as $row_id => $row) {
      $value = $row->node_revision_nid . ':' . $row->vid;
      $form_element_row_id = $this->form_element_row_id($row_id);
      $form[$field_name][$form_element_row_id] = array(
        '#prefix' => '<div class="diff-views">',
        '#suffix' => '</div>',
      );
      $form[$field_name][$form_element_row_id]['old'] = array(
        '#type' => 'radio',
        '#parents' => array($field_name, 'old'),
        '#return_value' => $value,
        '#states' => array(
          'disabled' => array(
            ':input[name="' . $field_name . '[new]"]' => array('value' => $value),
          ),
        ),
      );
      $form[$field_name][$form_element_row_id]['new'] = array(
        '#type' => 'radio',
        '#parents' => array($field_name, 'new'),
        '#return_value' => $value,
        '#states' => array(
          'disabled' => array(
            ':input[name="' . $field_name . '[old]"]' => array('value' => $value),
          ),
        ),
      );
    }

    $form['#attached']['css'][] = drupal_get_path('module', 'diff_views') . '/diff_views.css';
  }

  function get_revision_entity($values, $op) {
    $vid = $this->get_value($values, 'node_vid');
    $node = $this->get_value($values);
    // Unpublished nodes ignore access control.
    $node->status = 1;
    // Ensure user has access to perform the operation on this node.
    if (!node_access($op, $node)) {
      return array($node, NULL);
    }
    return array($node, $vid);
  }
}