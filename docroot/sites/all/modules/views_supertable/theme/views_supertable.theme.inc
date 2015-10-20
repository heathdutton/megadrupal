<?php

/**
 * Display a view as a jQuery supertable.
 */
function template_preprocess_views_supertable(&$vars) {
  $view     = $vars['view'];

  // We need the raw data for this grouping which is passed in as $vars['rows'].
  // However, the template also needs to use for the rendered fields.  We
  // therefore swap the raw data out to a new variable and reset $vars['rows']
  // so that it can get rebuilt.
  // Store rows so that they may be used by further preprocess functions.
  $result   = $vars['result'] = $vars['rows'];
  $vars['rows'] = array();
  $vars['field_classes'] = array();

  $options  = $view->style_plugin->options;
  $handler  = $view->style_plugin;

  $fields   = &$view->field;
  $columns  = $handler->sanitize_columns($options['columns'], $fields);

  $active   = !empty($handler->active) ? $handler->active : '';
  $order    = !empty($handler->order) ? $handler->order : 'asc';

  $query    = tablesort_get_query_parameters();
  if (isset($view->exposed_raw_input)) {
    $query += $view->exposed_raw_input;
  }

  $search_form = drupal_get_form('supertable_search_form');
  $vars['page_header'] = render($search_form);

  $header = array();

  // Fields must be rendered in order as of Views 2.3, so we will pre-render
  // everything.
  $renders = $handler->render_fields($result);

  foreach ($columns as $field => $column) {
    // Create a second variable so we can easily find what fields we have
    // and what the CSS classes should be.
    $vars['fields'][$field] = drupal_clean_css_identifier($field);
    if ($active == $field) {
      $vars['fields'][$field] .= ' active';
    }

    // Render the header labels.
    if ($field == $column && empty($fields[$field]->options['exclude'])) {
      $label = check_plain(!empty($fields[$field]) ? $fields[$field]->label() : '');
      if (empty($options['info'][$field]['sortable']) || !$fields[$field]->click_sortable()) {
        $vars['header'][$field] = $label;
      }
      else {
        $initial = !empty($options['info'][$field]['default_sort_order']) ? $options['info'][$field]['default_sort_order'] : 'asc';

        if ($active == $field) {
          $initial = ($order == 'asc') ? 'desc' : 'asc';
        }

        $title = t('sort by @s', array('@s' => $label));
        if ($active == $field) {
          $label .= theme('tablesort_indicator', array('style' => $initial));
        }

        $query['order'] = $field;
        $query['sort'] = $initial;
        $link_options = array(
          'html' => TRUE,
          'attributes' => array('title' => $title),
          'query' => $query,
        );
        $vars['header'][$field] = l($label, $_GET['q'], $link_options);
      }

      $vars['header_classes'][$field] = '';
      // Set up the header label class.
      if ($fields[$field]->options['element_default_classes']) {
        $vars['header_classes'][$field] .= "views-field views-field-" . $vars['fields'][$field];
      }
      $class = $fields[$field]->element_label_classes(0);
      if ($class) {
        if ($vars['header_classes'][$field]) {
          $vars['header_classes'][$field] .= ' ';
        }
        $vars['header_classes'][$field] .= $class;
      }

      // Add a header label wrapper if one was selected.
      if ($vars['header'][$field]) {
        $element_label_type = $fields[$field]->element_label_type(TRUE, TRUE);
        if ($element_label_type) {
          $vars['header'][$field] = '<' . $element_label_type . '>' . $vars['header'][$field] . '</' . $element_label_type . '>';
        }
      }

    }

    // Add a CSS align class to each field if one was set.
    if (!empty($options['info'][$field]['align'])) {
      $vars['fields'][$field] .= ' ' . drupal_clean_css_identifier($options['info'][$field]['align']);
    }

    // Render each field into its appropriate column.
    foreach ($result as $num => $row) {
      // Add field classes.
      $vars['field_classes'][$field][$num] = '';
      if ($fields[$field]->options['element_default_classes']) {
        $vars['field_classes'][$field][$num] = "views-field views-field-" . $vars['fields'][$field];
      }
      if ($classes = $fields[$field]->element_classes($num)) {
        if ($vars['field_classes'][$field][$num]) {
          $vars['field_classes'][$field][$num] .= ' ';
        }

        $vars['field_classes'][$field][$num] .= $classes;
      }
      $vars['field_attributes'][$field][$num] = array();

      if (!empty($fields[$field]) && empty($fields[$field]->options['exclude'])) {
        $field_output = $renders[$num][$field];
        $element_type = $fields[$field]->element_type(TRUE, TRUE);
        if ($element_type) {
          $field_output = '<' . $element_type . '>' . $field_output . '</' . $element_type . '>';
        }

        // Don't bother with separators and stuff if the field does not show up.
        if (empty($field_output) && !empty($vars['rows'][$num][$column])) {
          continue;
        }

        // Place the field into the column, along with an optional separator.
        if (!empty($vars['rows'][$num][$column])) {
          if (!empty($options['info'][$column]['separator'])) {
            $vars['rows'][$num][$column] .= filter_xss_admin($options['info'][$column]['separator']);
          }
        }
        else {
          $vars['rows'][$num][$column] = '';
        }

        $vars['rows'][$num][$column] .= $field_output;
      }
    }

    // Remove columns if the option "hide empty column"
    // is checked and the field is not empty.
    if (!empty($options['info'][$field]['empty_column'])) {
      $empty = TRUE;
      foreach ($vars['rows'] as $num => $columns) {
        $empty &= empty($columns[$column]);
      }
      if ($empty) {
        foreach ($vars['rows'] as $num => &$columns) {
          unset($columns[$column]);
          unset($vars['header'][$column]);
        }
      }
    }
  }

  $count = 0;
  foreach ($vars['rows'] as $num => $row) {
    $vars['row_classes'][$num][] = ($count++ % 2 == 0) ? 'odd' : 'even';
    if ($row_class = $handler->get_row_class($num)) {
      $vars['row_classes'][$num][] = $row_class;
    }
  }

  $vars['row_classes'][0][] = 'views-row-first';
  $vars['row_classes'][count($vars['row_classes']) - 1][] = 'views-row-last';

  $vars['classes_array'] = array('views-table', 'matrix');
  if (empty($vars['rows']) && !empty($options['empty_table'])) {
    $vars['rows'][0][0] = $view->display_handler->render_area('empty');
    // Calculate the amounts of rows with output.
    $vars['field_attributes'][0][0]['colspan'] = count($vars['header']);
    $vars['field_classes'][0][0] = 'views-empty';
  }

  if (!empty($options['sticky'])) {
    drupal_add_js('misc/tableheader.js');
    $vars['classes_array'][] = "sticky-enabled";
  }
  $vars['classes_array'][] = 'cols-' . count($vars['header']);
  if (!empty($handler->options['summary'])) {
    $vars['attributes_array'] = array('summary' => $handler->options['summary']);
  }
}

function supertable_search_form($form, &$form_state) {
  $module_path = drupal_get_path('module', 'views_supertable');

  $form['fieldset'] = array(
    '#type' => 'fieldset',
  );

  $form['fieldset']['id_search'] = array(
    '#title' => t('Search table'),
    '#type' => 'textfield',
    '#action' => '#',
    '#field_suffix' => '<span class="loading">' . t('Loading...') . '</span>',
  );

  // Add jquery.quicksearch library.
  // Have to use rawgit.com since github doesn't serve file MIME info anymore
  // and newer browsers need text/javascript type.
  $form['#attached']['js'][] = array(
    'data' => '//rawgit.com/riklomas/quicksearch/f984f7f92ed8f24ef6d8aec0d47724eaf6f246ce/jquery.quicksearch.js',
    'type' => 'external',
  );

  // Add module js.
  $form['#attached']['js'][] = array(
    'data' => $module_path . '/js/supertable.js',
    'type' => 'file',
  );

  // Add module css.
  $form['#attached']['css'][] = array(
    'data' => $module_path . '/css/views_supertable.css',
    'type' => 'file',
  );

  return $form;
}
