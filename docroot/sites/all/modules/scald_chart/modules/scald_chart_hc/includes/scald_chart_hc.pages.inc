<?php
/**
 * @file
 * Scald Highchart is a Scald Atom Player for Scald Charts.
 */

/**
 * The scald_chart_hc/%scald_atom/%/% menu callback.
 */
function scald_chart_hc_chart_iframe($atom, $context, $mode) {
  if ($mode == 'player') {
    // Invoke the Player prerender hook.
    $config = scald_context_config_load($context);
    $player = isset($config->player[$atom->type]) ? $config->player[$atom->type]['*'] : 'default';
    if ('highchart' == $player) {
      // Galleria display settings.
      $players = scald_players();
      $player_settings = $players[$player];
      $settings = isset($config->player[$atom->type]['settings']) ? $config->player[$atom->type]['settings'] : array();

      if (isset($player_settings['settings'])) {
        // Add default settings.
        $settings += $player_settings['settings'];
      }

      $chartid = 'atom-scald-chart-hc-' . $atom->sid;
      $atom_emw = entity_metadata_wrapper('scald_atom', $atom);

      $chart_var['chart'] = array(
        'type' => $atom_emw->scald_chart_type->value(),
        'plotBackgroundColor' => filter_xss($settings['plot_background_color']),
      );

      if (isset($config->data['width']) && !empty($config->data['width'])) {
        if (FALSE == strpos($config->data['width'], '%')) {
          $chart_var['chart']['width'] = $config->data['width'];
        }
      }

      if (isset($config->data['height'])) {
        if (FALSE == strpos($config->data['height'], '%')) {
          $chart_var['chart']['height'] = $config->data['height'];

          if (empty($chart_var['chart']['height'])) {
            $chart_var['chart']['height'] = '400';
          }
        }
      }

      $chart_var['title'] = array(
        'text' => check_plain($atom->title),
        'position' => array(
          'align' => $settings['title_position'],
        ),
      );

      $chart_var['subtitle'] = array(
        'text' => check_plain($atom_emw->scald_chart_subtitle->value()),
        'position' => array(
          'align' => $settings['subtitle_position'],
        ),
      );

      $chart_var['legend'] = array(
        'layout' => 'horizontal',
        'align' => 'center',
        'verticalAlign' => $settings['legend_position'],
      );

      if ('top' == $settings['legend_position']) {
        $chart_var['legend']['y'] = 30;
      }

      if ('true' == $settings['exporting']) {
        $chart_var['exporting']['enabled'] = TRUE;
      }
      else {
        $chart_var['exporting']['enabled'] = FALSE;
      }

      $credit_link = $atom_emw->scald_chart_credit->value();
      if (!empty($credit_link)) {
        $chart_var['credits'] = array(
          'text' => !empty($credit_link['title']) ? check_plain($credit_link['title']) : filter_xss($credit_link['url']),
          'target' => '_blank',
          'position' => array(
            'align' => $settings['credit_position'],
          ),
        );

        if (!empty($credit_link['url'])) {
          $chart_var['credits']['href'] = filter_xss($credit_link['url']);
        }
      }
      else {
        $chart_var['credits']['enabled'] = FALSE;
      }

      $series_color = explode(',', str_replace(' ', '', $settings['series_color_style']));
      $series_label = $atom_emw->scald_chart_series->value();
      $categories = array();
      $chart_data = array();
      foreach ($atom_emw->scald_chart_categories->value() as $category) {
        $category_emw = entity_metadata_wrapper('field_collection_item', $category);
        $categories[] = $category_emw->scald_chart_categories_label->value();
        $data = $category_emw->scald_chart_categories_data->value();
        $data = explode(',', $data);
        foreach ($data as $key => $value) {
          $value = trim($value);

          if ('null' == drupal_strtolower($value)) {
            $chart_data[$key][] = '%null%';
          }
          else {
            $chart_data[$key][] = (int) $value;
          }
        }
      }

      $chart_var['xAxis'] = array(
        'title' => array('text' => ''),
        'categories' => $categories,
      );

      $chart_var['yAxis'] = array(
        'title' => array('text' => ''),
      );

      switch ($atom_emw->scald_chart_type->value()) {
        case 'bar':
          $axis = 'yAxis';
          break;

        case 'line':
          $axis = 'yAxis';
          break;
      }

      if (isset($axis)) {
        if ('' != $atom_emw->scald_chart_min->value()) {
          $chart_var[$axis]['min'] = (int) $atom_emw->scald_chart_min->value();
        }

        if ('' != $atom_emw->scald_chart_max->value()) {
          $chart_var[$axis]['max'] = (int) $atom_emw->scald_chart_max->value();
        }

        if ('' != $atom_emw->scald_chart_tick_interval->value()) {
          $chart_var[$axis]['tickInterval'] = (int) $atom_emw->scald_chart_tick_interval->value();
        }

        $legent_title = $atom_emw->scald_chart_legend_title->value();
        if (!empty($legent_title)) {
          $chart_var[$axis]['title']['text'] = $legent_title;
        }
      }

      $chart_var['series'] = array();
      $count = count($series_label);
      foreach ($series_label as $key => $s_label) {
        --$count;
        $chart_var['series'][] = array(
          'name' => $s_label,
          'color' => $series_color[$key],
          'data' => $chart_data[$key],
          'legendIndex' => $key,
        );
      }

      drupal_alter('scald_chart_hc_chart', $chart_var, $atom, $settings);
      $chart = drupal_json_encode($chart_var);

      drupal_alter('scald_chart_hc_chart_after_build', $chart, $atom, $settings);
      $chart = str_replace('"%null%"', 'null', $chart);

      $chart = '(function($) {
        $("#' . $chartid . '").highcharts(' . $chart . ');
      })(jQuery);';

      $highchart_js_source = '';
      if ('external' == $settings['js_source_type']) {
        $highchart_js_source = $settings['js_source'];
      }
      elseif ('file' == $settings['js_source_type']) {
        $highchart_js_source = libraries_get_path('highcharts') . '/highcharts.js';
      }

      $chart_render = array(
        '#theme' => 'scald_chart_hc',
        '#atom' => $atom,
        '#chartid' => $chartid,
        '#chart' => $chart,
        '#options' => array(
          'chartjs' => $chart,
        ),
        '#attached' => array(
          'js' => array(
            $highchart_js_source => array('type' => $settings['js_source_type']),
            drupal_get_path('module', 'scald_chart_hc') . '/scald_chart_hc.js' => array(
              'type' => 'file',
            ),
          ),
        ),
      );

      return render($chart_render);
    }
  }

  return '';
}

/**
 * Delivery callback for the scald highchart atom.
 */
function scald_chart_hc_chart_iframe_delivery_callback($page_callback_result) {
  print "<html><head>";
  print drupal_get_js();
  print drupal_get_css();
  print "</head><body>";
  print $page_callback_result;
  drupal_page_footer();
  print "</body></html>";
}
