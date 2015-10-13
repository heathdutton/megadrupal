<?php

class equi_join_handler_relationship extends views_handler_relationship {
  
  function option_definition() {
    $options = parent::option_definition();
    $options['equi_join_left'] = array('default' => !empty($this->definition['equi_join_left']) ? $this->definition['equi_join_left'] : '');
    $options['equi_join_right'] = array('default' => !empty($this->definition['equi_join_right']) ? $this->definition['equi_join_right'] : '');
    return $options;
  }

  /**
   * Default options form that provides the label widget that all fields
   * should have.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $bundle = $this->definition['base'];
    $field_options = array();
    $fields = field_info_fields();
    foreach ($fields as $field) {
      if (array_key_exists($bundle, $field['bundles'])) {
        $field_options[$field['field_name']] = $field['field_name'];
      }
    }
    $form['equi_join_help'] = array(
      '#type' => 'markup',
      '#markup' => t('The resulting query will include the two tables belonging to the fields and a conditional similar to "WHERE left_field = right_field"'),
    );
    $form['equi_join_left'] = array(
      '#type' => 'select',
      '#title' => t('Left field'),
      '#options' => $field_options,
      '#default_value' => $this->options['equi_join_left'],
    );
    $form['equi_join_right'] = array(
      '#type' => 'select',
      '#title' => t('Right field'),
      '#options' => $field_options,
      '#default_value' => $this->options['equi_join_right'],
    );
    // Remove the 'require':
    unset($form['required']);
  }

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    // Figure out what base table this relationship brings to the party.
    $table_data = views_fetch_data($this->definition['base']);
    $base_field = empty($this->definition['base field']) ? $table_data['table']['base']['field'] : $this->definition['base field'];

    $this->ensure_my_table();

    $def = $this->definition;
    $def['table'] = $this->definition['base'];
    $def['field'] = $base_field;
    $def['left_table'] = $this->table_alias;
    $def['left_field'] = $this->field;

    // The join must be always an inner join!    
    $def['type'] = 'INNER';

    if (!empty($def['join_handler']) && class_exists($def['join_handler'])) {
      $join = new $def['join_handler'];
    }
    else {
      $join = new equi_join();
    }

    $join->definition = $def;
    $join->options = $this->options;
    $join->construct();
    $join->adjusted = TRUE;

    // use a short alias for this:
    $alias = $def['table'] . '_' . $this->table;

    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);

    // Add the equality comparison that makes it an equi join
    $left = field_info_field($this->options['equi_join_left']);
    $right = field_info_field($this->options['equi_join_right']);
    
    $left_table = key($left['storage']['details']['sql'][FIELD_LOAD_CURRENT]);
    $right_table = key($right['storage']['details']['sql'][FIELD_LOAD_CURRENT]);

    $left_field = $left['storage']['details']['sql'][FIELD_LOAD_CURRENT][$left_table]['value'];
    $right_field = $right['storage']['details']['sql'][FIELD_LOAD_CURRENT][$right_table]['value'];
    
    $left_table_join = new views_join();
    $left_table_join->table = $left_table;
    $left_table_join->field = 'entity_id';
    $left_table_join->left_table = $this->definition['base'];
    $left_table_join->left_field = $this->definition['field'];
    $left_table_join->type = 'LEFT';
    $left_table_alias = $this->query->add_relationship($this->options['equi_join_left'], $left_table_join, $this->definition['base']);

    $right_table_join = new views_join();
    $right_table_join->table = $right_table;
    $right_table_join->field = 'entity_id';
    $right_table_join->left_table = $this->alias;
    $right_table_join->left_field = $this->real_field;
    $right_table_join->type = 'LEFT';
    $right_table_alias = $this->query->add_relationship($this->options['equi_join_right'], $right_table_join, $this->alias);

    $this->query->add_where_expression(0, $left_table_alias . '.' . $left_field . ' = ' . $right_table_alias . '.' . $right_field);
  }
}
