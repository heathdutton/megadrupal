<?php

/**
 * @file
 * Contains the adjustable table style plugin.
 */

/**
 * Style plugin to render each item as a row in a table with adjustable columns.
 *
 * @ingroup views_style_plugins
 */
class views_adjustable_table_plugin_style extends views_plugin_style_table {

  var $original_fields = NULL;
  var $original_columns = NULL;

  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options);

    $this->original_fields = isset($view->field) ? $view->field : array();
    $this->original_columns = isset($this->options['columns']) ? $this->options['columns'] : array();

    $handler = _views_create_handler(array('handler' => 'views_adjustable_table_handler_selection'));
    $info = $handler->option_definition();
    $handler->init($view, $info);
    $view->display_handler->handlers['adjustable_table']['selection'] = $handler;
    $view->adjustable_table = array('selection' => $handler);
    $input = $view->get_exposed_input();
    if (empty($input['columns'])) {
      $view->exposed_input['columns'] = $this->get_preselected_columns();
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['column_selection_widget'] = array('default' => 'bsmselect');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    if (empty($form['info'])) {
      return;
    }

    foreach (array_keys($form['info']) as $field) {
      $safe = str_replace(array('][', '_', ' '), '-', $field);
      // the $id of the column for dependency checking.
      $id = 'edit-style-options-columns-' . $safe;

      $form['info'][$field]['preselected_column'] = array(
        '#type' => 'checkbox',
        '#default_value' => isset($this->options['info'][$field]['preselected_column']) ? $this->options['info'][$field]['preselected_column'] : FALSE,
        '#dependency' => array($id => array($field)),
      );
    }

    $form['#theme'] = 'views_ui_adjustable_table_style_plugin';

    $form['description_markup']['#markup'] .= '<div class="description form-item">' .
      t('Choose preselected columns which will be shown initially and when the user selects empty set of columns. If no preselected columns are chosen, the first column will be shown.') .
      '</div>';

    $selection_widgets = array(
      'bsmselect' => 'bsmSelect',
    );

    $form['column_selection_widget'] = array(
      '#type' => 'select',
      '#title' => t('Column selection widget'),
      '#description' => t('Widget for selecting columns by the end user.'),
      '#options' => $selection_widgets,
      '#default_value' => !empty($this->options['column_selection_widget']) ? $this->options['column_selection_widget'] : 'bsmselect',
    );
  }

  function get_preselected_columns() {
    $options = $this->options;

    if (empty($options['info'])) {
      return array();
    }

    $fields = $this->original_fields;
    $columns = $this->sanitize_columns($this->original_columns, $fields);
    $field_nos = array();
    $ids = array();
    $preselected = array();

    foreach (array_keys($fields) as $field_no => $field) {
      $field_nos[$field] = $field_no;
    }

    foreach ($columns as $field => $column) {
      if ($field == $column && empty($fields[$field]->options['exclude'])) {
        $id = 'f' . $field_nos[$field];
        $ids[] = $id;
        if (!empty($options['info'][$field]['preselected_column'])) {
          $preselected[] = $id;
        }
      }
    }

    if (empty($preselected)) {
      if (!empty($ids)) {
        reset($ids);
        $preselected = array(current($ids));
      }
    }

    return $preselected;
  }

  function query() {
    parent::query();
    $input = $this->view->get_exposed_input();
    if (empty($input['columns'])) {
      return;
    }

    $options = $this->options;

    if (empty($options['info'])) {
      return;
    }

    $fields = $this->original_fields;
    if (empty($fields)) {
      return;
    }
    $columns = $this->sanitize_columns($this->original_columns, $fields);
    $selected_column_fields = array();

    $fields_by_key = array();
    foreach (array_keys($fields) as $no => $field) {
      $fields_by_key['f' . $no] = $field;
    }
    foreach ($input['columns'] as $field_key) {
      if (!array_key_exists($field_key, $fields_by_key)) {
        continue;
      }
      $selected_column_fields[$fields_by_key[$field_key]] = $fields_by_key[$field_key];
    }

    $new_fields = array();
    $new_columns = array();

    foreach ($this->original_fields as $field => $field_handler) {
      if (!empty($field_handler->options['exclude'])) {
        $new_fields[$field] = $field_handler;
      }
    }

    foreach ($selected_column_fields as $selected_field) {
      foreach ($this->original_fields as $field => $field_handler) {
        if ((empty($field_handler->options['exclude'])) && (isset($columns[$field]) && ($columns[$field] == $selected_field))) {
          $new_columns[$field] = $columns[$field];
          if (!array_key_exists($field, $new_fields)) {
            $new_fields[$field] = $field_handler;
          }
        }
      }
    }
    if (empty($new_fields)) {
      return;
    }

    $this->view->field = $new_fields;
    $this->options['columns'] = $new_columns;
  }

}
