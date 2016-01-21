<?php

/**
 * @file
 * Definition of views_has_item_handler_filter_boolean.
 */


/**
 * Simple filter to handle matching of boolean values.
 *
 * This handler checks to see if a field has an item or not.
 *
 * Definition items:
 * - label: (REQUIRED) The label for the checkbox.
 * - field_name: (REQUIRED) The field name
 *
 * @ingroup views_filter_handlers
 */
class views_has_item_handler_filter_boolean_operator extends views_handler_filter_boolean_operator {
  // The name of field
  public $field_name;

  function construct() {
    parent::construct();
    $this->field_name = $this->definition['field_name'];
  }

  function option_definition() {
    $options = parent::option_definition();
    $field_info = field_info_field($this->definition['field_name']);
    $options['column'] = array();
    if (isset($field_info['columns'])) {
      $columns = array_keys($field_info['columns']);
      // Set default value for each column of this field
      foreach ($field_info['columns'] as $column => $info) {
        $option = $this->field_name . '_' . $column;
        $options['column'][$option]['use_it'] = array(
          'default' => 0,
        );
        $options['column'][$option]['no_value'] = array(
          'default' => '',
        );
      }
    }

    return $options;
  }


  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $field_info = field_info_field($this->field_name);
    $columns = array_keys($field_info['columns']);

    $collapsed = TRUE;
    if ($this->options['column']) {
      // See if any column is in use to open the fieldset.
      foreach ($this->options['column'] as $option => $values) {
        if ($values['use_it'] == 1) {
          $collapsed = FALSE;
          break;
        }
      }
    }

    $form['column'] = array(
      '#type'         => 'fieldset',
      '#title'        => t('No has an item if...'),
      '#description'  => t('Select one or more columns and define what value means that no has an item'),
      '#collapsible'  => TRUE,
      '#collapsed'    => $collapsed,
    );
    foreach ($field_info['columns'] as $column => $info) {
      $option = $this->field_name . '_' . $column;
      $form['column'][$option]['use_it'] = array(
        '#type'           => 'checkbox',
        '#title'          => $option,
        '#default_value'  => $this->options['column'][$option]['use_it'],
        '#description'    => isset($info['description']) ? $info['description'] : '',
      );
      $source = 'edit-options-column-' . drupal_html_id($option) . '-use-it';

      $form['column'][$option]['no_value'] = array(
        '#type' => 'textfield',
        '#title' => t('No item if you have this value'),
        '#size' => 30,
        '#default_value' => $this->options['column'][$option]['no_value'],
        '#dependency' => array($source => array(1)),
      );

      if ($this->get_operator_for($info['type']) == 'numeric') {
        $form['column'][$option]['no_value']['#element_validate'][] = 'views_has_item_validate_options_numeric';
      }
      else {
        $form['column'][$option]['no_value']['#element_validate'][] = 'views_has_item_validate_options';
      }
    }
  }

  /**
   * This function help to get a standard data type.
   *
   * For each column of this field validator attached by data type.
   */
  function get_operator_for($type) {
    switch ($type) {
      case 'text':
      case 'varchar':
        return 'string';
      case 'int':
      case 'float':
        return 'numeric';
    }
  }


  function query() {
    $this->ensure_my_table();
    // We consulted by those entities that do not have an item.
    if ($this->value == '0') {
      $no_column = TRUE;
      // We started an "and" in case of using the columns.
      $and = db_and();
      if ($this->options['column']) {
        // We see if we have any settings for the columns of the field.
        foreach ($this->options['column'] as $column => $info) {
          // If we are using at least one column to define  when there is no
          // an item, then the condition attached to the variable $ and
          if ($info['use_it'] == 1) {
            $no_column = FALSE;
            $field = "$this->table_alias.$column";
            // We add the condition that the column should have the value set
            // in configuration to consider that has no item.
            $and->condition($field, $info['no_value']);
          }
        }
      }

      $field_entity_id = "$this->table_alias.entity_id";

      // If no use some column of this field, we add the condition
      // that entity identifier is null.
      if ($no_column) {
        $this->query->add_where($this->options['group'], $field_entity_id, NULL, "IS NULL");
      }
      // Otherwise, mix the conditions in an "OR".
      else {
        $or = db_or()
          ->condition($and)
          ->condition($field_entity_id, NULL, "IS NULL");
        $this->query->add_where($this->options['group'], $or);
      }
    }
    // If you query for entities that if it have an item.
    elseif ($this->value == '1') {
      $no_column = TRUE;
      if ($this->options['column']) {
        // We see if we have any settings for the columns of the field.
        foreach ($this->options['column'] as $column => $info) {
          if ($info['use_it'] == 1) {
            $no_column = FALSE;
            $field = "$this->table_alias.$column";
            // If we find any columns, then we add the condition that has an
            // item provided they do not have a specific value.
            $this->query->add_where($this->options['group'], $field, $info['no_value'], "<>");
          }
        }
      }

      // If no use some column of this field, we add the condition
      // that entity identifier is not null.
      if ($no_column) {
        $field_entity_id = "$this->table_alias.entity_id";
        $this->query->add_where($this->options['group'], $field_entity_id, NULL, "IS NOT NULL");
      }
    }
  }
}
