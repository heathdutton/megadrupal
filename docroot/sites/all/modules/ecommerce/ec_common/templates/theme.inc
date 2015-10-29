<?php

/**
 * @file
 * Provide preprocess functions for all the theme templates.
 */

/**
 * Default theme function for all filter forms.
 */
function template_preprocess_ec_views_exposed_form(&$vars) {
  views_add_css('views');
  $form = &$vars['form'];

  // Put all single checkboxes together in the last spot.
  $checkboxes = '';

  if (!empty($form['q'])) {
    $vars['q'] = drupal_render($form['q']);
  }

  $vars['widgets'] = array();
  foreach ($form['#info'] as $id => $info) {
    // Set aside checkboxes.
    if (isset($form[$info['value']]['#type']) && $form[$info['value']]['#type'] == 'checkbox') {
      $checkboxes .= drupal_render($form[$info['value']]);
      continue;
    }
    $widget = new stdClass;
    // set up defaults so that there's always something there.
    $widget->label = $widget->operator = $widget->widget = NULL;

    if (!empty($info['label'])) {
      $widget->label = $info['label'];
    }
    if (!empty($info['operator'])) {
      $widget->operator = drupal_render($form[$info['operator']]);
    }
    $widget->widget = drupal_render($form[$info['value']]);
    $vars['widgets'][$id] = $widget;
  }

  // Wrap up all the checkboxes we set aside into a widget.
  if ($checkboxes) {
    $widget = new stdClass;
    // set up defaults so that there's always something there.
    $widget->label = $widget->operator = $widget->widget = NULL;
    $widget->widget = $checkboxes;
    $vars['widgets']['checkboxes'] = $widget;
  }

  // Don't render these:
  unset($form['form_id']);
  unset($form['form_build_id']);
  unset($form['form_token']);

  // This includes the submit button.
  $vars['button'] = drupal_render($form['submit']);

  $vars['extra'] = drupal_render($form);
}

function template_preprocess_ec_views_exposed_form_fieldset(&$vars) {
  $form = &$vars['form'];

  // Put all single checkboxes together in the last spot.
  $checkboxes = '';

  $vars['widgets'] = array();
  foreach ($form['#info'] as $id => $info) {
    // Set aside checkboxes.
    if (isset($form[$info['value']]['#type']) && $form[$info['value']]['#type'] == 'checkbox') {
      $checkboxes .= drupal_render($form[$info['value']]);
      continue;
    }
    $widget = new stdClass;
    // set up defaults so that there's always something there.
    $widget->label = $widget->operator = $widget->widget = NULL;

    if (!empty($info['label'])) {
      $widget->label = $info['label'];
    }
    if (!empty($info['operator'])) {
      $widget->operator = drupal_render($form[$info['operator']]);
    }
    $widget->widget = drupal_render($form[$info['value']]);
    $vars['widgets'][$id] = $widget;
  }

  // Wrap up all the checkboxes we set aside into a widget.
  if ($checkboxes) {
    $widget = new stdClass;
    // set up defaults so that there's always something there.
    $widget->label = $widget->operator = $widget->widget = NULL;
    $widget->widget = $checkboxes;
    $vars['widgets']['checkboxes'] = $widget;
  }
}

/**
 * Display a view as a table style.
 */
function template_preprocess_ec_view_table_multi(&$vars) {
  $view     = $vars['view'];

  // We need the raw data for this grouping, which is passed in as $vars['rows'].
  // However, the template also needs to use for the rendered fields.  We
  // therefore swap the raw data out to a new variable and reset $vars['rows']
  // so that it can get rebuilt.
  // Store rows so that they may be used by further preprocess functions.
  $result   = $vars['result'] = $vars['rows'];
  $vars['rows'] = array();
  $vars['rows_attributes'] = array();
  $vars['fields_attributes'] = array();

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

  $header = array();

  // Fields must be rendered in order as of Views 2.3, so we will pre-render
  // everything.
  $renders = $handler->render_fields($result);

  $vars['lines'] = array();
  foreach ($columns as $field => $column) {
    $line = $options['info'][$field]['row'];
    if (!isset($vars['lines'][$line])) {
      $vars['lines'][$line] = array();
    }
    $vars['lines'][$line][] = $field;
  }
  ksort($vars['lines']);
  $first_line = reset(array_keys($vars['lines']));
  $last_line = end(array_keys($vars['lines']));
  
  foreach ($vars['lines'] as $line => $cols) {
    foreach ($cols as $field) {
      $column = $columns[$field];
      // Create a second variable so we can easily find what fields we have and what the
  
      // CSS classes should be.
      $vars['fields'][$field] = drupal_clean_css_identifier($field);
      if ($active == $field) {
        $vars['fields'][$field] .= ' active';
      }

      // render the header labels
      if ($field == $column && $line == $first_line && empty($fields[$field]->options['exclude'])) {
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

      // Add a CSS align class to each field if one was set
      if (!empty($options['info'][$field]['align'])) {
        $vars['fields'][$field] .= ' ' . drupal_clean_css_identifier($options['info'][$field]['align']);
      }
      
      if (!isset($vars['fields_attributes'][$field])) {
        $vars['fields_attributes'][$field] = array(
          'class' => array(),
        );
        if ($fields[$field]->options['element_default_classes']) {
          $vars['fields_attributes'][$field]['class'][] = "views-field";
          $vars['fields_attributes'][$field]['class'][] = "views-field-" . $vars['fields'][$field];
        }
        if ($active == $field) {
          $vars['fields_attributes'][$field]['class'][] = ' active';
        }
        if (!empty($options['info'][$field]['rowspan'])) {
          $rowspan = $options['info'][$field]['rowspan'];
          $vars['fields_attributes'][$field]['rowspan'] = $rowspan;
        }
        if (!empty($options['info'][$field]['colspan'])) {
          $vars['fields_attributes'][$field]['colspan'] = $options['info'][$field]['colspan'];
        }
      }

      // Render each field into its appropriate column.
      foreach ($result as $num => $row) {
        if (!empty($fields[$field]) && empty($fields[$field]->options['exclude'])) {
          $field_output = $renders[$num][$field];
          $element_type = $fields[$field]->element_type(TRUE, TRUE);
          if ($element_type) {
            $field_output = '<' . $element_type . '>' . $field_output . '</' . $element_type . '>';
          }

          // Don't bother with separators and stuff if the field does not show up.
          if (empty($field_output) && !empty($vars['rows'][$num][$line][$column])) {
            continue;
          }

          // Place the field into the column, along with an optional separator.
          if (!empty($vars['rows'][$num][$line][$column])) {
            if (!empty($options['info'][$column]['separator'])) {
              $vars['rows'][$num][$line][$column] .= filter_xss_admin($options['info'][$column]['separator']);
            }
          }
          else {
            $vars['rows'][$num][$line][$column] = '';
          }

          $vars['rows'][$num][$line][$column] .= $field_output;
        }
      }

      // Remove columns if the option is hide empty column is checked and the field is not empty.
      if (!empty($options['info'][$field]['empty_column'])) {
        $empty = TRUE;
        foreach ($vars['rows'] as $num => $columns) {
          $empty &= empty($columns[$line][$column]);
        }
        if ($empty) {
          foreach ($vars['rows'] as $num => &$columns) {
            unset($columns[$line][$column]);
            unset($vars['header'][$line][$column]);
          }
        }
      }
    }
  }

  $count = 0;
  foreach ($vars['rows'] as $num => $row) {
    foreach ($row as $line => $attributes) {
      $vars['rows_attributes'][$num][$line]['class'][] = ($count % 2 == 0) ? 'odd' : 'even';
      if ($line == $first_line) {
        $vars['rows_attributes'][$num][$line]['class'][] = 'first-line';
      }
      if ($line == $last_line) {
        $vars['rows_attributes'][$num][$line]['class'][] = 'last-line';
      }
    }
    $count++;
  }

  foreach ($vars['rows_attributes'][0] as $line => $attributes) {
    $vars['rows_attributes'][0][$line]['class'][] = 'views-row-first';
  }
  $last = count($vars['rows_attributes']) - 1;
  foreach ($vars['rows_attributes'][$last] as $line => $attributes) {
    $vars['rows_attributes'][$last][$line]['class'][] = 'views-row-last';
  }

  // Remove all the row spans from the header attributes;
  $vars['header_attributes'] = $vars['fields_attributes'];
  foreach ($vars['header_attributes'] as $field) {
    unset($vars['header_attributes']['rowspan']);
  }
  
  // Go through the columns and make sure that none of the rowspans move outside the group of lines that it is in.
  $count = 0;
  foreach ($vars['lines'] as $line => $cols) {
    foreach ($cols as $field) {
      if (isset($vars['fields_attributes'][$field]['rowspan']) && is_numeric($vars['fields_attributes'][$field]['rowspan']) && $vars['fields_attributes'][$field]['rowspan'] > count($lines)) {
        $vars['fields_attributes'][$field]['rowspan'] += count($lines) - ($vars['fields_attributes'][$field]['rowspan'] + $count);
      }
    }
    $count++;
  }
  
  $vars['classes_array'] = array('views-table');
  if (empty($vars['rows']) && !empty($options['empty_table'])) {
    $vars['rows'][0][$first_line][0] = $view->display_handler->render_area('empty');
    // Calculate the amounts of rows with output.
    $vars['fields_attributes'][0][$first_line][0]['colspan'] = count($vars['header']);
    $vars['fields_attributes'][0][$first_line][0]['class'][] = 'views-empty';
  }


  if (!empty($options['sticky'])) {
    drupal_add_js('misc/tableheader.js');
    $vars['classes_array'][] = "sticky-enabled";
  }
  $vars['classes_array'][] = 'cols-'. count($vars['header']);

  if (!empty($handler->options['summary'])) {
    $vars['attributes_array'] = array('summary' => $handler->options['summary']);
  }
}
