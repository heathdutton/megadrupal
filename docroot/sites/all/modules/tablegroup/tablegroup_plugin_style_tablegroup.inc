<?php

class tablegroup_plugin_style_tablegroup extends views_plugin_style {
  function option_definition() {
    $options = parent::option_definition();

    $options['row_grouping'] = array('default' => NULL);
    $options['row_sort'] = array('default' => 0);
    $options['col_grouping'] = array('default' => NULL);
    $options['col_sort'] = array('default' => 0);
    $options['cell_placeholder'] = array('default' => '&nbsp;');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // grabbed from views_plugin_style_table, slight edits {{{
    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#value' => t('You need at least one field before you can configure your tablegroup settings'),
        '#prefix' => '<div class="error form-item description">',
        '#suffix' => '</div>',
      );
      return;
    }

    // Create an array of fields for use as options
    foreach ($handlers as $field => $handler) {
      if ($label = $handler->label()) {
        $field_names[$field] = $label;
      }
      else {
        $field_names[$field] = $handler->ui_name();
      }
    }
    // }}} end grab

    $form['row_grouping'] = array(
      '#type' => 'select',
      '#title' => t('Rows'),
      '#options' => $field_names,
      '#default_value' => $this->options['row_grouping'],
    );
    $form['row_sort'] = array(
      '#type' => 'radios',
      '#title' => t('Row sort'),
      '#options' => array(0 => 'none', 'asc' => 'asc', 'desc' => 'desc'),
      '#default_value' => $this->options['row_sort'],
    );
    $form['col_grouping'] = array(
      '#type' => 'select',
      '#title' => t('Columns'),
      '#options' => $field_names,
      '#default_value' => $this->options['col_grouping'],
    );
    $form['col_sort'] = array(
      '#type' => 'radios',
      '#title' => t('Column sort'),
      '#options' => array(0 => 'none', 'asc' => 'asc', 'desc' => 'desc'),
      '#default_value' => $this->options['col_sort'],
    );
    $form['cell_placeholder'] = array(
      '#type' => 'textfield',
      '#title' => t('Cell placeholder'),
      '#default_value' => $this->options['cell_placeholder'],
    );
  }

  function render() {
    $output = '';

    // Figure out what functions to use for sorting rows and columns.
    // @see http://php.net/asort
    // @see http://php.net/ksort
    $col_sort = ($this->options['col_sort'] == 'asc' ? 'asort' : ($this->options['col_sort'] == 'desc' ? 'arsort' : ''));
    $row_sort = ($this->options['row_sort'] == 'asc' ? 'ksort' : ($this->options['row_sort'] == 'desc' ? 'krsort' : ''));

    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);

    foreach ($sets as $title => $set) {
      $table['header'] = array();
      $table['rows'] = array();
      $table['raw'] = array();

      // group by row
      $rows = $this->render_grouping($set, $this->options['row_grouping']);

        foreach ($rows as $row_header => $row) {
          // group by column
          $columns = $this->render_grouping($row, $this->options['col_grouping']);
          $table['raw'][$row_header] = $columns;

          // collect the column headers
          foreach (array_keys($columns) as $col_header) {
            if (!in_array($col_header, $table['header'])) {
              $table['header'][] = $col_header;
            }
          }
        }

        // Order rows and columns.
        if ($col_sort) { $col_sort($table['header']); }
        if ($row_sort) { $row_sort($table['raw']); }

        // step through and fill each cell
        foreach (array_keys($table['raw']) as $row_header) {
          foreach ($table['header'] as $col_header) {
            // if there are items in this cell, render them
            if (isset($table['raw'][$row_header][$col_header])) {
              foreach ($table['raw'][$row_header][$col_header] as $row_index => $item) {
                // the view's row_index property is used by template_preprocess_views_view_fields()
                // in Views' theme.inc to match figure out which view result is
                // being rendered (despite the fact that we are passing in the
                // object we want rendered!!)
                $this->view->row_index = $row_index;
                if ( !isset($table['rows'][$row_header][$col_header]) ) {
                  $table['rows'][$row_header][$col_header] = '';
                }
                $table['rows'][$row_header][$col_header] .= theme('tablegroup_item', array('item' => $this->row_plugin->render($item)));
              }
            }
            // empty cell, use a placeholder
            else {
              $table['rows'][$row_header][$col_header] = filter_xss_admin($this->options['cell_placeholder']);
            }
          }
        }

      $variables = array(
        'view' => $this->view,
        'options' => $this->options,
        'rows' => $table,
        'title' => $title,
      );

      // Finally, render using the theme function.
      $output .= theme($this->theme_functions(), $variables);
    }

    unset($this->view->row_index);
    return $output;
  }

  function validate() {
    $errors = parent::validate();

    if (!isset($this->options['row_grouping']) || !isset($this->options['col_grouping'])) {
      $errors[] = t('You must set both "Row" and "Column" values in the style options.');
    }

    return $errors;
  }

}
