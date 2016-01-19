<?php
/**
 * @file
 * Menu callbacks and forms used by the Webform Charts module.
 */

/**
 * Implements hook_webform_analysis_alter().
 *
 * Internal callback for the public function
 * webform_charts_webform_analysis_alter() in the main module.
 */
function _webform_charts_webform_analysis_alter(&$analysis) {
  $node = $analysis['#node'];

  // If this is a component-specific page, load the chart configuration form.
  if (!empty($analysis['#component'])) {
    $component = $analysis['#component'];
    $cid = $component['cid'];
    $data = $analysis['data'][$cid]['#data'];
    $analysis['data'][$cid]['chart'] = drupal_get_form('webform_charts_edit_chart', $node, $component, $data);
    $analysis['data'][$cid]['chart']['#weight'] = -1;
  }
  // Otherwise if showing all components' analysis, loop and print each one.
  else {
    $analysis['#attached']['library'][] = array('webform_charts', 'webform_charts.admin');

    // Add charts to each component's analysis.
    foreach (element_children($analysis['data']) as $cid) {
      $component = $node->webform['components'][$cid];
      $data = $analysis['data'][$cid]['#data'];
      if (!empty($data['table_rows'])) {
        // Wrapper around both chart and edit link.
        $analysis['data'][$cid]['chart'] = array(
          '#weight' => -1,
          '#theme_wrappers' => array('container'),
          '#attributes' => array('class' => array('webform-chart')),
        );

        // Create and add the chart.
        $chart = _webform_charts_component_chart($node, $component, $data);
        $chart['#title'] = NULL;
        $chart['#legend_position'] = 'bottom';
        // Hide axis labels on the summary, there's not enough room.
        if (isset($chart['xaxis'])) {
          $chart['xaxis']['#labels'] = array_fill(0, count($chart['xaxis']['#labels']), '');
        }

        $analysis['data'][$cid]['chart']['chart'] = $chart;

        // Add an edit chart link to the other table.
        $edit_chart = l(t('Chart options »'), 'node/' . $node->nid . '/webform-results/analysis/' . $component['cid'], array('attributes' => array('class' => array('webform-chart-configure'))));
        $analysis['data'][$cid]['chart']['edit'] = array(
          '#markup' => $edit_chart,
        );

        // Modify the table to remove redundant information.
        //unset($analysis['data'][$cid]['basic']['#data']['table_rows']);
        //unset($analysis['data'][$cid]['basic']['#data']['table_header']);
      }
    }
  }
}

/**
 * @param object $node
 *   The node whose components are being analyzed.
 * @param array $component
 *   The component whose data is being analyzed.
 * @param array $data
 *   The data array from a components _webform_analysis_component() callback.
 *   Usually contains the keys "table_rows" and "table_header" which is used
 *   to generate the chart.
 *
 * @return array
 *   A renderable chart array.
 */
function _webform_charts_component_chart($node, $component, $analysis_data) {
  // Safety check to prevent attempting to render data-less components.
  if (empty($analysis_data['table_rows'])) {
    return array(
      '#markup' => t('No chartable data'),
    );
  }

  $header = !empty($analysis_data['table_header']) ? $analysis_data['table_header'] : array();
  $data = $analysis_data['table_rows'];
  $chart_config = isset($component['extra']['chart']) ? $component['extra']['chart'] : array();
  $chart_config += webform_charts_default_settings($node, $component, $analysis_data);

  // Ensure stacking isn't enabled on an invalid chart type. Otherwise the data
  // may end up getting reversed unnecessarily.
  $chart_type_info = chart_get_type($chart_config['type']);
  if ($chart_config['stacking'] && !$chart_type_info['stacking']) {
    $chart_config['stacking'] = FALSE;
  }

  $chart = array(
    '#type' => 'chart',
    '#id' => 'webform_component_chart__' . $node->nid . '__' . $component['cid'],
    '#title' => check_plain(filter_xss($component['name'])),
    '#chart_type' => $chart_config['type'],
    '#chart_library' => empty($chart_config['library']) ? NULL : $chart_config['library'],
    '#legend' => $chart_config['legend'],
    '#legend_position' => $chart_config['legend_position'],
    '#data_labels' => $chart_config['data_labels'],
    '#tooltips' => $chart_config['tooltips'],
    '#stacking' => $chart_config['stacking'],
    '#colors' => array_values($chart_config['colors']),
  );
  if (empty($header)) {
    $labels = array();
    $nonzero = FALSE;
    foreach ($data as $row_id => $row) {
      $label = array_shift($row);
      $labels[] = html_entity_decode($label);
      $data[$row_id] = floatval(array_shift($row));
      if (!$nonzero && $data[$row_id]) {
        $nonzero = TRUE;
      }
    }

    // Bail out if we have no non-zero data.
    if (!$nonzero) {
      return array(
        '#markup' => t('No chartable data.'),
      );
    }

    $chart['data'] = array(
      '#type' => 'chart_data',
      '#title' => t('Choice'),
      '#data' => $data,
    );
    $labels = webform_charts_wrap_labels($labels);
    $chart['data']['#labels'] = $labels;

    $chart['xaxis'] = array(
      '#type' => 'chart_xaxis',
      '#title' => $chart_config['xaxis_title'],
      '#labels' => $labels,
      '#labels_rotation' => $chart_config['xaxis_labels_rotation'],
    );
    $chart['yaxis'] = array(
      '#type' => 'chart_yaxis',
      '#title' => $chart_config['yaxis_title'],
      '#labels_rotation' => $chart_config['yaxis_labels_rotation'],
    );
  }
  else {
    // Remove the top-left empty header cell if any.
    if (empty($header[0])) {
      array_shift($header);
    }

    // If stacked, reverse the series so that first values are on bottom.
    if ($chart_config['stacking']) {
      $chart['#colors'] = array_reverse($chart['#colors']);
      $header = array_reverse($header);
    }

    // Extract all the labels from the data and cast all data to numeric values.
    $labels = array();
    foreach ($data as $row_id => $row_data) {
      $label = array_shift($row_data);
      $labels[] = html_entity_decode($label);
      $data[$row_id] = array_map('floatval', array_values($row_data));

      // After extracting labels, also reverse data to match for stacked charts.
      if ($chart_config['stacking']) {
        $data[$row_id] = array_reverse($data[$row_id]);
      }
    }

    $series_data = array();
    foreach ($header as $col_id => $series_title) {
      foreach ($data as $row_id => $row_data) {
        $series_data[$col_id][$row_id] = $row_data[$col_id];
      }
      $chart["data_$col_id"] = array(
        '#type' => 'chart_data',
        '#title' => html_entity_decode($series_title),
        '#data' => $series_data[$col_id],
        '#labels' => $labels,
      );
    }
    $chart['xaxis'] = array(
      '#type' => 'chart_xaxis',
      '#title' => $chart_config['xaxis_title'],
      '#labels' => $labels,
      '#labels_rotation' => $chart_config['xaxis_labels_rotation'],
    );
    $chart['yaxis'] = array(
      '#type' => 'chart_yaxis',
      '#title' => $chart_config['yaxis_title'],
      '#labels_rotation' => $chart_config['yaxis_labels_rotation'],
      '#min' => $chart_config['yaxis_min'],
      '#max' => $chart_config['yaxis_max'],
    );
  }

  return $chart;
}

/**
 * Wrap the contents of an array of legend text to fit nicely on our page.
 */
function webform_charts_wrap_labels($legends) {
  foreach ($legends as &$legend) {
    $legend = wordwrap($legend, 40, '<br />');
  }
  return $legends;
}

/**
 * Form callback; Display the form for editing an individual chart.
 */
function webform_charts_edit_chart($form, $form_state, $node, $component, $data) {
  // Populate settings and defaults.
  form_load_include($form_state, 'inc', 'charts', 'includes/charts.pages');
  $options = isset($component['extra']['chart']) ? $component['extra']['chart'] : array();
  $options += webform_charts_default_settings($node, $component, $data);

  $form['#node'] = $node;
  $form['#component'] = $component;
  $form['#data'] = $data;
  $form['#attached']['library'][] = array('webform_charts', 'webform_charts.admin');
  if (module_exists('charts_highcharts')) {
    $form['#attached']['library'][] = array('webform_charts', 'webform_charts.admin.highcharts');
  }

  $form['title'] = array(
    '#markup' => t('Chart configuration'),
  );
  $form['preview'] = _webform_charts_component_chart($node, $component, $data);
  $form['preview']['#weight'] = -100;
  $form['preview']['#title'] = FALSE;

  $parents = webform_component_parent_keys($node, $component);
  $form_key = implode(':', $parents);
  $form['help'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t('This chart may be embedded into the <a href="!url">confirmation page</a> by using the token !token.', array('!url' => url('node/' . $node->nid . '/webform/configure'), '!token' => "<code>[node:webform-charts:$form_key]</code>")) . '</p>',
  );

  // Get a list of available chart libraries.
  $charts_info = charts_info();
  $library_options = array(
    '' => t('- Default -'),
  );
  foreach ($charts_info as $library_name => $library_info) {
    $library_options[$library_name] = $library_info['label'];
  }
  $form['library'] = array(
    '#title' => t('Charting library'),
    '#type' => 'select',
    '#options' => $library_options,
    '#default_value' => $options['library'],
    '#access' => count($library_options) > 1,
    '#attributes' => array('class' => array('chart-library-select')),
    '#weight' => -20,
  );

  $chart_types = charts_type_info();
  $type_options = array();
  foreach ($chart_types as $chart_type => $chart_type_info) {
    $type_options[$chart_type] = $chart_type_info['label'];
  }
  $form['type'] = array(
    '#title' => t('Chart type'),
    '#title_display' => 'invisible',
    '#type' => 'radios',
    '#default_value' => $options['type'],
    '#options' => $type_options,
    '#required' => TRUE,
    '#weight' => -15,
    '#attributes' => array('class' => array('chart-type-radios', 'container-inline')),
  );

  $form['legend'] = array(
    '#title' => t('Show legend'),
    '#type' => 'checkbox',
    '#default_value' => $options['legend'],
    '#weight' => -10,
  );
  $form['tooltips'] = array(
    '#title' => t('Show tooltips'),
    '#type' => 'checkbox',
    '#default_value' => $options['tooltips'],
    '#weight' => -9,
  );
  $form['data_labels'] = array(
    '#title' => t('Show data labels'),
    '#type' => 'checkbox',
    '#default_value' => $options['data_labels'],
    '#weight' => -8,
  );
  $form['stacking'] = array(
    '#title' => t('Stacked'),
    '#type' => 'checkbox',
    '#default_value' => $options['stacking'],
    '#weight' => -6,
    // Only shown if dealing with a multi-axis chart.
    '#access' => !isset($form['preview']['data']),
  );

  // Set data attributes to identify special properties of different types.
  foreach ($chart_types as $chart_type => $chart_type_info) {
    if ($chart_type_info['axis_inverted']) {
      $form['type'][$chart_type]['#attributes']['data-axis-inverted'] = TRUE;
    }
    if ($chart_type_info['stacking']) {
      $form['type'][$chart_type]['#attributes']['data-stacking'] = TRUE;
    }
    if ($chart_type_info['axis'] === CHARTS_SINGLE_AXIS) {
      $form['type'][$chart_type]['#attributes']['data-axis-single'] = TRUE;
      if (!empty($data['table_header'])) {
        $form['type'][$chart_type]['#access'] = FALSE;
      }
    }
  }

  // Try various approaches to determine the labels used for colors.
  $fields = array();

  // If a header is present that will be used for colors.
  if (!empty($data['table_header'])) {
    $fields = $data['table_header'];
    if (isset($fields[0]) && strlen($fields[0]) === 0) {
      array_shift($fields);
    }
    $fields = array_values($fields);
  }
  // Otherwise pull the first column from the data for single-series charts.
  elseif ($data['table_rows']) {
    $fields = array();
    foreach ($data['table_rows'] as $row_data) {
      if (is_string($row_data[0])) {
        $fields[] = $row_data[0];
      }
    }
  }

  $form['colors'] = array(
    '#theme' => 'webform_charts_colors_element',
    '#theme_wrappers' => array('form_element'),
    '#title' => t('Colors'),
  );
  $color_count = 0;
  foreach ($fields as $field_name => $field_label) {
    $form['colors'][$field_name] = array(
      '#title' => webform_filter_xss($field_label),
      '#type' => 'textfield',
      '#attributes' => array('TYPE' => 'color'),
      '#size' => 10,
      '#maxlength' => 7,
      '#default_value' => !empty($options['colors'][$field_name]) ? $options['colors'][$field_name] : $options['colors'][$color_count],
      '#parents' => array('colors', $field_name),
    );
    $color_count++;
  }

  $form['xaxis'] = array(
    '#title' => t('Horizontal axis'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array('class' => array('chart-xaxis')),
  );
  $form['xaxis']['title'] = array(
    '#title' => t('Custom title'),
    '#type' => 'textfield',
    '#default_value' => $options['xaxis_title'],
    '#parents' => array('xaxis_title'),
  );
  $form['xaxis']['labels_rotation'] = array(
    '#title' => t('Labels rotation'),
    '#type' => 'select',
    '#options' => array(
      0 => '0°',
      30 => '30°',
      45 => '45°',
      60 => '60°',
      90 => '90°',
    ),
    '#default_value' => $options['xaxis_labels_rotation'],
    '#parents' => array('xaxis_labels_rotation'),
  );

  $form['yaxis'] = array(
    '#title' => t('Vertical axis'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array('class' => array('chart-yaxis')),
  );
  $form['yaxis']['title'] = array(
    '#title' => t('Custom title'),
    '#type' => 'textfield',
    '#default_value' => $options['yaxis_title'],
    '#parents' => array('yaxis_title'),
  );
  $form['yaxis']['minmax'] = array(
    '#title' => t('Value range'),
    '#theme_wrappers' => array('form_element'),
  );
  $form['yaxis']['minmax']['min'] = array(
    '#type' => 'textfield',
    '#attributes' => array('TYPE' => 'number', 'max' => 999999, 'placeholder' => t('Minimum')),
    '#default_value' => $options['yaxis_min'],
    '#size' => 12,
    '#suffix' => ' ',
    '#theme_wrappers' => array(),
    '#parents' => array('yaxis_min'),
  );
  $form['yaxis']['minmax']['max'] = array(
    '#type' => 'textfield',
    '#attributes' => array('TYPE' => 'number', 'max' => 999999, 'placeholder' => t('Maximum')),
    '#default_value' => $options['yaxis_max'],
    '#size' => 12,
    '#theme_wrappers' => array(),
    '#parents' => array('yaxis_max'),
  );
  $form['yaxis']['labels_rotation'] = array(
    '#title' => t('Labels rotation'),
    '#type' => 'select',
    '#options' => array(
      0 => '0°',
      30 => '30°',
      45 => '45°',
      60 => '60°',
      90 => '90°',
    ),
    '#default_value' => $options['yaxis_labels_rotation'],
    '#parents' => array('yaxis_labels_rotation'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save chart'),
  );
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#access' => isset($component['extra']['chart']),
  );
  return $form;
}

/**
 * Submit handler for webform_charts_edit_chart().
 */
function webform_charts_edit_chart_submit($form, &$form_state) {
  $op = $form_state['triggering_element']['#parents'][0];

  $node = $form['#node'];
  $component = $form['#component'];
  if ($op === 'reset') {
    if (isset($component['extra']['chart'])) {
      unset($component['extra']['chart']);
    }
  }
  else {
    $config = array_diff_key($form_state['values'], array('form_build_id' => NULL, 'form_token' => NULL, 'form_id' => NULL, 'op' => NULL));
    $component['extra']['chart'] = $config;
  }
  $node->webform['components'][$component['cid']] = $component;
  node_save($node);
  drupal_set_message(t('Chart options for the "@name" component saved.', array('@name' => $component['name'])));
  $form_state['redirect'] = 'node/' . $node->nid . '/webform-results/analysis';
}

/**
 * Output the table for the chart colors configuration.
 */
function theme_webform_charts_colors_element($variables) {
  $element = $variables['element'];
  $rows = array();
  foreach (element_children($element) as $key) {
    $title = $element[$key]['#title'];
    $element[$key]['#title'] = NULL;

    $row = array();
    $row[] = drupal_render($element[$key]);
    $row[] = '<label for="' . $element[$key]['#id'] . '">' . webform_filter_xss($title) . '</label>';
    $rows[] = $row;
  }

  return theme('table', array(
    'rows' => $rows,
    'sticky' => FALSE,
    'attributes' => array('class' => array('webform-charts-colors')),
  ));
}

/**
 * Output the entire contents of the Webform chart configuration form.
 */
function theme_webform_charts_edit_chart($variables) {
  $form = $variables['form'];
  $output = '';
  $output .= '<div class="chart-preview-wrapper">';
  $output .= '<div class="chart-preview">' . drupal_render($form['preview']) . '</div>';
  $output .= '<div class="chart-help">' . drupal_render($form['help']) . '</div>';
  $output .= '</div>';
  $output .= '<div class="chart-form-wrapper"><div class="chart-form">';
  $output .= '<h2>' . drupal_render($form['title']) . '</h2>';
  $output .= drupal_render_children($form);
  $output .= '</div></div>';
  return $output;
}