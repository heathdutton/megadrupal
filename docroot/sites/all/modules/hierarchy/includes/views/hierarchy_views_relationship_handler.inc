<?php

/**
 * @file
 * Provide relationship handler for hierarchy position fields.
 */
class hierarchy_views_relationship_handler extends views_handler_relationship {

  function option_definition() {
    $options = parent::option_definition();
    $options['delta'] = array('default' => -1);
    $options['root'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  /**
   * Add a delta selector for multiple fields.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $field = field_info_field($this->definition['field_name']);

    $form['root'] = array(
      '#type' => 'checkbox',
      '#title' => t('Root items only'),
      '#description' => t('Enable to select only items that do not have parents in the hierarchy.'),
      '#default_value' => !empty($this->options['root']),
    );

    // Only add the delta selector if the field is multiple.
    if ($field['cardinality']) {
      $max_delta = ($field['cardinality'] == FIELD_CARDINALITY_UNLIMITED) ? 10 : $field['cardinality'];

      $options = array('-1' => t('All'));
      for ($i = 0; $i < $max_delta; $i++) {
        $options[$i] = $i + 1;
      }
      $form['delta'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => $this->options['delta'],
        '#title' => t('Delta'),
        '#description' => t('The delta allows you to select which item in a multiple value field to key the relationship off of. Select "1" to use the first item, "2" for the second item, and so on. If you select "All", each item in the field will create a new row, which may appear to cause duplicates.'),
      );
    }
  }

  function ensure_my_table() {
    $field = field_info_field($this->definition['field_name']);
    $parent_type_column = _field_sql_storage_columnname($this->definition['field_name'], 'parent_type');

    if (!isset($this->table_alias)) {
      $join = $this->get_join();

      if ($this->options['delta'] != -1 && $field['cardinality']) {
        $join->extra[] = array(
          'field' => 'delta',
          'value' => $this->options['delta'],
          'numeric' => TRUE,
        );
      }

// Incorrect for top level elements.
//      if (empty($this->options['root']) && isset($this->definition['parent_type'])) {
//        $join->extra[] = array(
//          'field' => $parent_type_column,
//          'value' => $this->definition['parent_type'],
//        );
//      }

      if (!empty($this->options['root'])) {
        $join->extra[] = array(
          'field' => $parent_type_column,
          'operator' => 'IS NULL',
          'value' => NULL,
        );
      }

      $this->table_alias = $this->query->add_table($this->table, $this->relationship, $join);
    }

    return $this->table_alias;
  }

}
