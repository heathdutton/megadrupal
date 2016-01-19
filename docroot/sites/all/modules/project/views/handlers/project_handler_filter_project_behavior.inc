<?php
/**
 * @file
 * A views filter that limits node.type based on project behavior settings.
 */

/**
 * Filter by project behavior.
 */
class project_handler_filter_project_behavior extends views_handler_filter {

  function construct() {
    parent::construct();
    $this->no_operator = TRUE;
  }

  function value_form(&$form, &$form_state) {
    // @todo Let the modules define the options instead of hardcoding here.
    $form['value'] = array(
      '#type' => 'select',
      '#title' => t('Options'),
      '#options' => array(
        'project' => t('Used for projects'),
        'project_issue' => t('Used for issues'),
        'project_release' => t('Used for releases'),
      ),
      '#default_value' => $this->value,
    );
  }

  function query() {
    // Put together a set of node types to filter on.
    $node_types = node_type_get_types();
    $allowed = array();
    foreach ($node_types as $node_type) {
      $behavior = variable_get('project_behavior_' . $node_type->type, '');
      if ($behavior == $this->value) {
        $allowed[] = $node_type->type;
      }
    }

    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field", $allowed, 'IN');
  }
}
