<?php

/**
 * @file
 * Defines statistics page functions.
 */

/**
 * Displays accessibility statistical information.
 *
 * This information is derived from nodes on the entire system.
 *
 * @param $statistics_type
 *   One of the following allowed strings:
 *   - 'all' No restrictions on loading of problems.
 *   - 'live' Restrict loading of problems to live content (active revision).
 *   - 'unlive' Restrict loading of problems to non-live content (active
 *     revision).
 *   - 'published' Restrict loading of problems to published content (active
 *     revision).
 *   - 'unpublished' Restrict loading of problems to unpublished content
 *     (active revision).
 *
 */
function node_accessibility_statistics_page($statistics_type = 'all') {
  $conditions = array();
  $title = "All Content";
  $statistics_type_path = '';

  if (is_string($statistics_type)) {
    switch ($statistics_type) {
      case 'live':
        $conditions['live_only'] = TRUE;
        $title = "Live Content";
        $statistics_type_path = '/live';
        break;

      case 'published':
        $conditions['published_only'] = TRUE;
        $conditions['live_only'] = TRUE;
        $title = "Published Content";
        $statistics_type_path = '/published';
        break;

      case 'unpublished':
        $conditions['unpublished_only'] = TRUE;
        $conditions['live_only'] = TRUE;
        $title = "Unpublished Content";
        $statistics_type_path = '/unpublished';
        break;
    }
  }

  drupal_set_title(t("Node Accessibility Statistics - @title", array('@title' => $title)), PASS_THROUGH);

  $static_variables = &drupal_static(__FUNCTION__, array());

  if (!isset($static_variables[$statistics_type]['major_problems'])) {
    $static_variables[$statistics_type]['major_problems'] = NULL;
  }

  if (!isset($static_variables[$statistics_type]['minor_problems'])) {
    $static_variables[$statistics_type]['minor_problems'] = NULL;
  }

  if (!isset($static_variables[$statistics_type]['grand_total'])) {
    $static_variables[$statistics_type]['grand_total'] = 0;
  }

  if (!isset($static_variables[$statistics_type]['statistics'])) {
    $static_variables[$statistics_type]['statistics'] = array(
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );
  }

  $major_problems = &$static_variables[$statistics_type]['major_problems'];
  $minor_problems = &$static_variables[$statistics_type]['minor_problems'];
  $grand_total = &$static_variables[$statistics_type]['grand_total'];
  $statistics = &$static_variables[$statistics_type]['statistics'];

  $form['statistics'] = &$statistics;

  $form['major_problems'] = array(
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $form['minor_problems'] = array(
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $display_levels = quail_api_get_display_levels(NULL);

  $form['major_problems']['#attributes'] = array('id' => 'accessibility_problems-' . $display_levels[1]['machine_name']);
  $form['minor_problems']['#attributes'] = array('id' => 'accessibility_problems-' . $display_levels[2]['machine_name']);

  $form['major_problems']['node_accessibility_statistics-header'] = array(
    '#type' => 'markup',
    '#markup' => '<h3 id="accessibility_problems-quail_test_major">' . t("Major Problems") . '</h3>',
    '#weight' => -100,
  );

  $form['minor_problems']['node_accessibility_statistics-header'] = array(
    '#type' => 'markup',
    '#markup' => '<h3 id="accessibility_problems-quail_test_minor">' . t("Minor Problems") . '</h3>',
    '#weight' => -100,
  );

  $problems = (array) node_accessibility_load_problems($conditions, NULL);
  $tests = quail_api_load_tests(array(), 'id');
  $plot_data = array();
  $data = array();
  $information = array();
  $markup_format = 'full_html';
  $grand_total = 0;

  $major_problems = array();
  $minor_problems = array();

  foreach ($display_levels as $key => &$value) {
    if (empty($value['id'])) continue;

    $information[$value['id']] = array('total' => 0);
  }

  foreach ($problems as $problem_key => &$problem_data) {
    if (!is_object($problem_data)) continue;

    $test = $tests[$problem_data->test_id];

    if ($test->severity == 1) {
      $problems = &$major_problems;
    }
    else if ($test->severity == 2) {
      $problems = &$minor_problems;
    }
    else {
      continue;
    }

    if (!isset($information[$test->severity])) {
      $information[$test->severity] = array('total' => 0);
    }

    if (!isset($problems[$test->machine_name])) {
      $problems[$test->machine_name]['problem_machine_name'] = $test->machine_name;
      $problems[$test->machine_name]['problem_human_name'] = $test->human_name;
      $problems[$test->machine_name]['severity_human_name'] = $display_levels[$test->severity]['human_name'];
      $problems[$test->machine_name]['total'] = 0;
      $problems[$test->machine_name]['explanation'] = $test->description;
      $problems[$test->machine_name]['statistics_type_path'] = $statistics_type_path;
      $problems[$test->machine_name]['title_block'] = 'h4';
    }

    if (!isset($information[$test->severity][$test->machine_name])) {
      $information[$test->severity][$test->machine_name] = array();
      $information[$test->severity][$test->machine_name]['body'] = array();
      $information[$test->severity][$test->machine_name]['body']['title'] = $test->human_name;
      $information[$test->severity][$test->machine_name]['body']['description'] = $test->description;
      $information[$test->severity][$test->machine_name]['total'] = 0;
    }

    $problem = array();
    $problem['line'] = $problem_data->line;
    $problem['element'] = $problem_data->element;
    $information[$test->severity][$test->machine_name]['problems'][] = $problem;

    $problems[$test->machine_name]['total']++;
    $information[$test->severity][$test->machine_name]['total']++;
    $information[$test->severity]['total']++;
    $grand_total++;
  }

  foreach (array('problem_distribution') as $type_name) {
    $plot = new phplot_api_plot('pie');
    $data = array();
    $theme_settings = array();
    $theme_settings['plot_rows'] = array();
    $total = 0;

    switch ($type_name) {
      case 'problem_distribution':
        $theme_settings['plot_human_name'] = t("Problem Distribution");
        break;
    }

    foreach ($information as $display_level => &$value) {
       if ($type_name == 'problem_distribution') {
         if ($display_levels[$display_level]['machine_name'] == 'quail_test_suggestion') {
           continue;
         }
       }

      $data_row = array($display_levels[$display_level]['human_name'], $value['total']);
      $data[] = $data_row;
      $total += $value['total'];
      $plot->set_data('legend', implode(': ', $data_row));

      $theme_setting = array();
      $theme_setting['machine_name'] = $display_levels[$display_level]['machine_name'];
      $theme_setting['human_name'] = $display_levels[$display_level]['human_name'];
      $theme_setting['show_human_name'] = TRUE;
      $theme_setting['link'] = '#accessibility_problems-' . $display_levels[$display_level]['machine_name'];
      $theme_setting['value'] = $data_row[1];
      $theme_settings['plot_rows'][] = $theme_setting;
    }

    if ($total > 0) {
      $no_statistics = FALSE;
      $theme_settings['total_human_name'] = t("Total");
      $theme_settings['total_value'] = '' . $total;

      $plot->set_data('width', 225);
      $plot->set_data('height', 275);

      // Define the data range. PHPlot can do this automatically, but not as well.
      $plot->set_data('plot_area_world', array(0, 0, 225, 275));
      $plot->set_data('margins_pixels', array(NULL, NULL, 10));

      // Select the data array representation and store the data:
      $plot->set_data('data_type', 'text-data-single');
      $plot->set_data('data_values', $data);

      // Select an overall image background color and another color under the plot:
      $plot->set_data('background_color', '#ffffff');
      $plot->set_data('plot_area_background_draw', FALSE);
      $plot->set_data('plot_background_color', '#ffffff');

      // Do not use decimal places
      $plot->set_data('precision_y', FALSE);

      // Make the pie chart flat
      $plot->set_data('shading', 0);
      $plot->set_data('label_scale_position', 0.2);

      $plot->build();
      $plot->create_image();

      $plot_file_uri = $plot->create_file();
      $plot_file_path = preg_replace('`^' . $GLOBALS['base_url'] . '`i', '', file_create_url($plot_file_uri));

      $theme_settings['image_url'] = $plot_file_path;

      foreach ($theme_settings['plot_rows'] as $key => &$row) {
        $row['value'] = $row['value'] . ' (' . round((($row['value']) / $total) * 100) . '%)';
      }

      $statistics[$type_name] = array(
        '#type' => 'markup',
        '#markup' => theme('node_accessibility_statistics_plot', $theme_settings),
      );
    }
  }

  foreach (array('major_problems', 'minor_problems') as $type_name) {
    $plot = new phplot_api_plot('bars');
    $data = array();
    $display_level = 0;
    $theme_settings = array();
    $total = 0;
    $count = 0;
    $available_colors = array('DarkGreen', 'DimGrey', 'SkyBlue', 'SlateBlue', 'YellowGreen', 'brown', 'gold', 'maroon', 'orange', 'tan', 'yellow', 'navy');
    $total_available_colors = count($available_colors);
    $data_colors = array();

    $theme_settings = array();
    $theme_settings['plot_rows'] = array();

    switch ($type_name) {
      case 'major_problems':
        $theme_settings['plot_human_name'] = t("Major Problems");
        $display_level = 1;
        break;

      case 'minor_problems':
        $theme_settings['plot_human_name'] = t("Minor Problems");
        $display_level = 2;
        break;
    }

    if (!empty($information[$display_level])) {
      foreach ($information[$display_level] as $problem => &$value) {
        if ($problem == 'total') continue;

        $total += $value['total'];
        $count++;
        $problem_total = count($value['problems']);
        $data_colors[] = $available_colors[$count % $total_available_colors];
        $data_row = array($count, $problem_total);
        $data[] = $data_row;

        $theme_setting = array();
        $theme_setting['machine_name'] = $problem;
        $theme_setting['human_name'] = isset($value['body']['title']) ? check_plain($value['body']['title']) : $problem;
        $theme_setting['show_human_name'] = FALSE;
        $theme_setting['link'] = '#accessibility_problem-' . $problem;
        $theme_setting['value'] = $data_row[1];
        $theme_settings['list_block'] = 'ol';
        $theme_settings['plot_rows'][] = $theme_setting;
      }
    }

    if ($total > 0) {
      $no_statistics = FALSE;
      $theme_settings['total_human_name'] = t("Total");
      $theme_settings['total_value'] = '' . $total;

      $width = 350;
      $height = 100 + $count * 10;

      if ($height < 275) $height = 275;
      if ($height > 550) $height = 550;

      $plot->set_data('width', $width);
      $plot->set_data('height', $height);

      // Select the data array representation and store the data:
      $plot->set_data('data_type', 'text-data-yx');
      $plot->set_data('data_values', array_reverse($data));

      // Select an overall image background color and another color under the plot:
      $plot->set_data('background_color', '#ffffff');
      $plot->set_data('plot_area_background_draw', FALSE);
      $plot->set_data('plot_background_color', '#ffffff');

      // Do not use decimal places
      $plot->set_data('precision_x', FALSE);

      // Turn off Y tick labels and ticks because they don't apply here:
      $plot->set_data('tick_label_position_y', 'none');
      $plot->set_data('tick_position_y', 'none');

      // Turn on X data labels:
      $plot->set_data('data_label_position_x', 'plotin');

      // With X data labels, we don't need Y ticks or their labels, so turn them off.
      $plot->set_data('tick_label_position_x', 'none');
      $plot->set_data('tick_position_x', 'none');

      // draw x ticks every 20 units for large plots
      if ($total > 100) {
        $plot->set_data('tick_increment_x', 20);
      }

      // Make the pie chart flat
      $plot->set_data('shading', 0);

      // manually set the data colors
      $plot->set_data('data_colors', array_reverse(array($data_colors)));

      $plot->build();

      $plot->create_image();

      $plot_file_uri = $plot->create_file();
      $plot_file_path = preg_replace('`^' . $GLOBALS['base_url'] . '`i', '', file_create_url($plot_file_uri));

      $theme_settings['image_url'] = $plot_file_path;

      $statistics[$type_name] = array(
        '#type' => 'markup',
        '#markup' => theme('node_accessibility_statistics_plot', $theme_settings),
      );
    }
  }

  if ($grand_total == 0) {
    $statistics['#description'] = t("There are no accessibility statistics available.");
  }

  if (count($major_problems) == 0) {
    $form['major_problems']['node_accessibility_statistics-header']['#markup'] .= '<div class="accessibility_problems-major_problems-description">' . t("There are no major problems detected.") . '</div>';
  }
  else {
    foreach ($major_problems as $machine_name => &$problem) {
      $form['major_problems'][$machine_name] = array(
        '#type' => 'markup',
        '#markup' => theme('node_accessibility_statistics_problem_details', $problem),
      );
    }
  }

  if (count($minor_problems) == 0) {
    $form['minor_problems']['node_accessibility_statistics-header']['#markup'] .=  '<div class="accessibility_problems-minor_problems-description">' . t("There are no minor problems detected.") . '</div>';
  }
  else {
    foreach ($minor_problems as $machine_name => &$problem) {
      $form['minor_problems'][$machine_name] = array(
        '#type' => 'markup',
        '#markup' => theme('node_accessibility_statistics_problem_details', $problem),
      );
    }
  }

  return $form;
}

/**
 * Displays accessibility statistical information derived from nodes on the entire system.
 *
 * @param $published_type
 *   One of the following allowed strings:
 *   - 'all' No restrictions on loading of problems.
 *   - 'published' Restrict loading of problems to live published content (active revision).
 *   - 'unpublished' Restrict loading of problems to live unpublished content (active revision).
 *
 * @param $live_type
 *   One of the following allowed strings:
 *   - 'all' No restrictions on loading of problems.
 *   - 'live' Restrict loading of problems to live content (active revision).
 *   - 'unlive' Restrict loading of problems to non-live content (active revision).
 */
function node_accessibility_statistics_page_problem($machine_name, $published_type = 'all', $live_type = 'live') {
  $form = array();

  if (!is_string($machine_name)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_string('machine_name');
    }

    return $form;
  }

  $conditions = array();
  $title = "All";

  if (is_string($published_type)) {
    switch ($published_type) {
      case 'published':
        $conditions['published_only'] = TRUE;
        $conditions['live_only'] = TRUE;
        $title = "Published";
        break;

      case 'unpublished':
        $conditions['unpublished_only'] = TRUE;
        $conditions['live_only'] = TRUE;
        $title = "Unpublished";
        break;
    }
  }

  if (is_string($live_type)) {
    switch ($live_type) {
      case 'live':
        $conditions['live_only'] = TRUE;
        break;

      case 'unlive':
        $conditions['unlive_only'] = TRUE;
        break;
    }
  }

  drupal_set_title(t("Node Accessibility Problem '%problem' - @title", array('%problem' => $machine_name, '@title' => $title)), PASS_THROUGH);

  static $problem_description = NULL;
  static $problem_table = NULL;

  if (is_null($problem_description) || is_null($problem_table)) {
    $test_information = (array) quail_api_load_tests(array('machine_name' => $machine_name), NULL);

    if (count($test_information) == 0) {
      return $form;
    }

    $problem_information = array_pop($test_information);
    unset($test_information);

    $node_type_settings = node_accessibility_load_node_type_settings(array(), 'type');

    $conditions['test_id'] = $problem_information->id;
    $conditions['sort_by'] = 'id';
    $conditions['sort_order'] = 'ASC';
    $conditions['node_columns'] = TRUE;

    $display_level = quail_api_get_display_levels($problem_information->severity);

    $problem_description = array();
    $problem_description['problem_machine_name'] = $problem_information->machine_name;
    $problem_description['problem_human_name'] = $problem_information->human_name;
    $problem_description['severity_human_name'] = $display_level['human_name'];
    $problem_description['total'] = 0;
    $problem_description['explanation'] = $problem_information->description;
    $problem_description['listing_link'] = FALSE;

    if (isset($_GET['order'])) {
      $conditions['sort_by'] = strtolower($_GET['order']);

      switch ($conditions['sort_by']) {
        case 'id':
        case 'nid':
        case 'vid':
        case 'test_id':
        case 'line':
        case 'element':
          break;
        case 'snippet':
          $conditions['sort_by'] = 'element';
          break;

        default:
          $conditions['sort_by'] = 'n.' . $conditions['sort_by'];
      }
    }

    if (isset($_GET['sort'])) {
      $conditions['sort_order'] = strtoupper($_GET['sort']);
    }


    $problems = (array) node_accessibility_load_problems($conditions, NULL);

    $problem_table = array();
    $problem_table['header'] = array();
    $problem_table['header'][] = array(
      'data' => t("ID"),
      'field' => 'id',
      'align' => 'center',
      'sort' => 'asc',
    );
    $problem_table['header'][] = array(
      'data' => t("Title"),
      'field' => 'title',
      'align' => 'left',
    );
    $problem_table['header'][] = array(
      'data' => t("Snippet"),
      'field' => 'element',
      'align' => 'left',
    );
    $problem_table['summary'] = t("All accessibility problems that fall under the category: %problem", array('%problem' => $problem_information->human_name));
    $problem_table['rows'] = array();

    foreach ($problems as $key => $problem) {
      $row = array();

      $markup_format = 'full_html';

      if (isset($node_type_settings[$problem->type]->format)) {
        $markup_format = $node_type_settings[$problem->type]->format;
      }

      $row['data'] = array($problem->id, l($problem->title, 'node/' . $problem->nid . '/accessibility/' . $problem->vid . '/revision'), check_markup($problem->element, $markup_format));
      $problem_table['rows'][] = $row;
      $problem_description['total']++;
    }
  }

  $form['problem_information'] = array(
    '#type' => 'markup',
    '#markup' => theme('node_accessibility_statistics_problem_details', $problem_description),
  );

  $form['problem_table'] = array(
    '#type' => 'markup',
    '#markup' => theme('table', $problem_table),
  );

  return $form;
}
