<?php
/**
 * @file
 * Views integration of tablegroups module.
 */

/**
 * This class holds all the funtionality for the unformatted tablegroup plugin.
 */
class TablegroupPluginStyleTablegroup extends views_plugin_style {

  /**
   * Set default options.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['row_grouping'] = array('default' => NULL);
    $options['row_sort'] = array('default' => 0);
    $options['row_sort_key'] = array('default' => 'none');
    $options['row_all'] = array('default' => 0);
    $options['col_grouping'] = array('default' => NULL);
    $options['col_sort'] = array('default' => 0);
    $options['col_sort_key'] = array('default' => 'none');
    $options['col_all'] = array('default' => 0);
    $options['cell_placeholder'] = array('default' => '&nbsp;');

    return $options;
  }

  /**
   * Configuration form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Grabbed from views_plugin_style_table, slight edits {{{
    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#value' => t('You need at least one field before you can configure your tablegroup settings'),
        '#prefix' => '<div class="error form-item description">',
        '#suffix' => '</div>',
      );
      return;
    }

    // Create an array of fields for use as options.
    foreach ($handlers as $field => $handler) {
      if ($label = $handler->label()) {
        $field_names[$field] = $label;
      }
      else {
        $field_names[$field] = $handler->ui_name();
      }
    }
    // }}} end grab.
    $form['row_grouping'] = array(
      '#type' => 'select',
      '#title' => t('Rows'),
      '#options' => $field_names,
      '#default_value' => $this->options['row_grouping'],
    );
    $form['row_sort'] = array(
      '#type' => 'radios',
      '#title' => t('Row sort'),
      '#options' => array(
        0 => t('none'),
        'asc' => t('asc'),
        'desc' => t('desc'),
      ),
      '#default_value' => $this->options['row_sort'],
    );
    $form['row_sort_key'] = array(
      '#type' => 'checkbox',
      '#title' => t('Row sort by key value/term weight'),
      '#default_value' => $this->options['row_sort_key'],
      '#description' => t("Works currently with taxobmoy terms and integer lists."),
    );
    $form['row_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Preload all terms/allowed values.'),
      '#default_value' => $this->options['row_all'],
      '#description' => t("Warning: Additional database usage."),
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
      '#options' => array(
        0 => t('none'),
        'asc' => t('asc'),
        'desc' => t('desc'),
      ),
      '#default_value' => $this->options['col_sort'],
    );
    $form['col_sort_key'] = array(
      '#type' => 'checkbox',
      '#title' => t('Column sort by key value/term weight'),
      '#default_value' => $this->options['col_sort_key'],
      '#description' => t("Works currently with taxobmoy terms and integer lists."),
    );
    $form['col_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Preload all terms/allowed values for collumns.'),
      '#default_value' => $this->options['col_all'],
      '#description' => t("Warning: Additional database usage."),
    );
    $form['cell_placeholder'] = array(
      '#type' => 'textfield',
      '#title' => t('Cell placeholder'),
      '#default_value' => $this->options['cell_placeholder'],
    );
  }

  /**
   * Render function.
   */
  public function render() {
    $opt = $this->options;
    $output = '';

    // Figure out what functions to use for sorting rows and columns.
    // @see http://php.net/asort
    // @see http://php.net/ksort
    $col_sort_cmd = ($opt['col_sort'] == 'asc' ? 'asort' : ($opt['col_sort'] == 'desc' ? 'arsort' : ''));
    $col_sort_keys_cmd = ($opt['col_sort'] == 'asc' ? 'ksort' : ($opt['col_sort'] == 'desc' ? 'krsort' : ''));
    $row_sort_cmd = ($opt['row_sort'] == 'asc' ? 'ksort' : ($opt['row_sort'] == 'desc' ? 'krsort' : ''));

    // Arrays to be filled for loading all values or key based sorting.
    $row_values = array();
    $col_values = array();

    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($this->view->result, $opt['grouping']);

    if (($opt['col_all'] == 1)
      || ($opt['col_sort_key'] == 1)
      || ($opt['row_all'] == 1)
      || ($opt['row_sort_key'] == 1)) {
      $fields = field_info_fields();
    }

    if (($opt['row_sort_key'] == 1)
      || ($opt['col_sort_key'] == 1)) {

      // Collect values and/or terms if all isn't used.
      if ((($opt['col_all'] == 0) && ($opt['col_sort_key'] == 1))
        || (($opt['row_all'] == 0) && ($opt['row_sort_key'] == 1))) {

        foreach ($this->view->result as $value) {

          if ($opt['row_sort_key'] == 1) {
            $row_field_view = "field_" . $opt['row_grouping'];
            $field_data = $value->$row_field_view;
            if ($fields[$opt['row_grouping']]['type'] == 'taxonomy_term_reference') {
              if (isset($field_data[0]['raw']['taxonomy_term'])) {
                $term = $field_data[0]['raw']['taxonomy_term'];
                $row_values[$term->weight] = $field_data[0]['rendered']['#markup'];
              }
            }
            else {
              if (isset($field_data[0]['raw']['value'])) {
                $string = $field_data[0]['raw']['value'];
                $string = (string) $string;
                $row_values[$string] = $field_data[0]['rendered']['#markup'];
              }
            }
          }

          if ($opt['col_sort_key'] == 1) {
            $col_field_view = "field_" . $opt['col_grouping'];
            $field_data = $value->$col_field_view;
            if ($fields[$opt['col_grouping']]['type'] == 'taxonomy_term_reference') {
              if (isset($field_data[0]['raw']['taxonomy_term'])) {
                $term = $field_data[0]['raw']['taxonomy_term'];
                $row_values[$term->weight] = $field_data[0]['rendered']['#markup'];
              }
            }
            else {
              if (isset($field_data[0]['raw']['value'])) {
                $string = $field_data[0]['raw']['value'];
                $string = (string) $string;
                $row_values[$string] = $field_data[0]['rendered']['#markup'];
              }
            }
          }

        }
      }
    }

    if ($opt['row_all'] == 1) {
      if ($fields[$opt['row_grouping']]['type'] == 'taxonomy_term_reference') {
        $vocab_name = $fields[$opt['row_grouping']]['settings']['allowed_values'][0]['vocabulary'];
        $vocabulary = taxonomy_vocabulary_machine_name_load($vocab_name);
        $terms = taxonomy_get_tree($vocabulary->vid);

        foreach ($terms as $term) {
          if ($opt['row_sort_key'] == 1) {
            $row_values[$term->weight] = $term->name;
          }
          else {
            $row_values[$term->name] = $term->name;
          }
        }
      }
      else {
        $row_values = list_allowed_values($fields[$opt['row_grouping']]);
      }
    }

    if ($opt['col_all'] == 1) {
      if ($fields[$opt['col_grouping']]['type'] == 'taxonomy_term_reference') {
        $vocab_name = $fields[$opt['col_grouping']]['settings']['allowed_values'][0]['vocabulary'];
        $vocabulary = taxonomy_vocabulary_machine_name_load($vocab_name);
        $terms = taxonomy_get_tree($vocabulary->vid);

        foreach ($terms as $term) {
          if ($opt['col_sort_key'] == 1) {
            $col_values[$term->weight] = $term->name;
          }
          else {
            $col_values[$term->name] = $term->name;
          }
        }
      }
      else {
        $col_values = list_allowed_values($fields[$opt['col_grouping']]);
      }
    }

    foreach ($sets as $title => $set) {
      $table['header'] = array();
      $table['rows'] = array();
      $table['raw'] = array();
      $row_headers = array();

      // Group by row:
      $rows = $this->render_grouping($set, $opt['row_grouping']);

      foreach ($rows as $row_header => $row) {
        // Group by column:
        $columns = $this->render_grouping($row, $opt['col_grouping']);
        $table['raw'][$row_header] = $columns;
      }

      // Order rows and columns:
      if ($row_values == array()) {
        $row_headers = array_keys($table['raw']);
        if ($row_sort_cmd) {
          $row_sort_cmd($row_headers);
        }
      }
      else {
        if ($row_sort_cmd) {
          $row_sort_cmd($row_values);
        }
        $row_headers = array_values($row_values);
      }

      if ($col_values == array()) {
        // Collect the column headers:
        foreach (array_keys($columns) as $col_header) {
          if (!in_array($col_header, $table['header'])) {
            $table['header'][] = $col_header;
          }
        }
        if ($col_sort_cmd) {
          $col_sort_cmd($table['header']);
        }
      }
      else {
        if ($col_sort_keys_cmd) {
          $col_sort_keys_cmd($col_values);
        }
        $table['header'] = array_values($col_values);
      }

      // Step through and fill each cell:
      foreach ($row_headers as $row_header) {
        foreach ($table['header'] as $col_header) {
          // If there are items in this cell, render them.
          if (isset($table['raw'][$row_header][$col_header])) {
            foreach ($table['raw'][$row_header][$col_header] as $row_index => $item) {
              // The view's row_index property is used by
              // template_preprocess_views_view_fields() in Views' theme.inc to
              // match figure out which view result is being rendered (despite
              // the fact that we are passing in the object we want rendered!!).
              $this->view->row_index = $row_index;
              if (!isset($table['rows'][$row_header][$col_header])) {
                $table['rows'][$row_header][$col_header] = '';
              }
              $table['rows'][$row_header][$col_header] .= theme('tablegroup_item', array('item' => $this->row_plugin->render($item)));
            }
          }
          // Empty cell, use a placeholder:
          else {
            $table['rows'][$row_header][$col_header] = filter_xss_admin($opt['cell_placeholder']);
          }
        }
      }

      $variables = array(
        'view' => $this->view,
        'options' => $opt,
        'rows' => $table,
        'title' => $title,
      );

      // Finally, render using the theme function.
      $output .= theme($this->theme_functions(), $variables);
    }

    unset($this->view->row_index);
    return $output;
  }

  /**
   * Validation function of configuration form.
   */
  public function validate() {
    $errors = parent::validate();

    if (!isset($this->options['row_grouping']) || !isset($this->options['col_grouping'])) {
      $errors[] = t('You must set both "Row" and "Column" values in the style options.');
    }

    return $errors;
  }

}
