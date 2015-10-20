<?php

/**
 * @file
 * Contains the column selection handler.
 */

/**
 * Handler to show column selection widget and apply the selection to the adjustable table style plugin.
 *
 * @ingroup views_handlers
 */
class views_adjustable_table_handler_selection extends views_handler {

  function can_expose() {
    return (get_class($this->view->style_plugin) === 'views_adjustable_table_plugin_style');
  }

  function is_exposed() {
    return TRUE;
  }

  function exposed_form(&$form, &$form_state) {
    $style_plugin = $this->view->style_plugin;

    if (get_class($style_plugin) !== 'views_adjustable_table_plugin_style') {
      return;
    }

    $options = $style_plugin->options;

    if (empty($options['info'])) {
      return;
    }

    $fields = $this->view->field;
    $columns = $style_plugin->sanitize_columns($options['columns'], $fields);
    $field_nos = array();
    $field_options = array();

    foreach (array_keys($fields) as $field_no => $field) {
      $field_nos[$field] = $field_no;
    }

    foreach ($columns as $field => $column) {
      if ($field == $column && empty($fields[$field]->options['exclude'])) {
        $id = 'f' . $field_nos[$field];
        $field_options[$id] = $fields[$field]->label();
      }
    }

    if (empty($field_options)) {
      return;
    }

    $input = $this->view->get_exposed_input();
    $selected_options = array();
    if (!empty($input['columns'])) {
      foreach ($input['columns'] as $key) {
        if (array_key_exists($key, $field_options)) {
          $selected_options[$key] = $field_options[$key];
          unset($field_options[$key]);
        }
      }
      $field_options = $selected_options + $field_options;
    }

    $form['columns'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#title' => t('Columns'),
      '#options' => $field_options,
    );

    if ((isset($options['column_selection_widget'])) && ($options['column_selection_widget'] == 'bsmselect')) {
      $path = drupal_get_path('module', 'views_adjustable_table');
      $form['columns']['#attached']['library'][] = array('system', 'ui.draggable');
      $form['columns']['#attached']['library'][] = array('system', 'ui.droppable');
      $form['columns']['#attached']['library'][] = array('system', 'ui.sortable');
      $form['columns']['#attached']['js'][] = $path . '/bsmselect/jquery.bsmselect.js';
      $form['columns']['#attached']['js'][] = $path . '/bsmselect/jquery.bsmselect.sortable.js';
      $form['columns']['#attached']['css'][] = $path . '/bsmselect/jquery.bsmselect.css';
      $form['columns']['#pre_render'] = array('views_adjustable_table_pre_render_bsmselect');
    }
  }

  function exposed_info() {
    return array();
  }

}
