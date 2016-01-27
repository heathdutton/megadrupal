<?php
/**
 * @file
 * Ctools content type plugin file for bongo module.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Bongo Predictions'),
  'description' => t('Shows a Bongo bus stop with basic configuration options.'),
  'category' => t('Bongo'),
  'edit form' => 'bongo_predictions_edit_form',
  'render callback' => 'bongo_predictions_render',
  'admin info' => 'bongo_predictions_admin_info',
  'defaults' => array(
    'stop' => 0001,
    'predictions_to_show' => 5,
    'prediction_labels' => 'long',
    'show_agency' => 1,
    'show_direction' => 0,
    'show_refresh' => 1,
  ),
);

/**
 * The 'admin info' callback for panel pane.
 */
function bongo_predictions_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass();
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
    $block->content = t('Showing @predictions_to_show predictions from stop <em>@stop</em>.', array(
      '@predictions_to_show' => $conf['predictions_to_show'],
      '@stop' => $conf['stop'],
    ));
    return $block;
  }
}

/**
 * The 'Edit form' callback for the content type.
 */
function bongo_predictions_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $options = bongo_build_stop_list_options();
  $form['stop'] = array(
    '#type' => 'select',
    '#title' => t('Stop'),
    '#options' => $options,
    '#default_value' => $conf['stop'],
    '#description' => t("The Stop ID from !url.", array('!url' => l(t('eBongo.org'), "http://ebongo.org"))),
  );

  $form['predictions_to_show'] = array(
    '#title' => t('Maximum number of predictions to show'),
    '#description' => t('Used to control the maximum number of predictions shown on the page initially. Defaults to 5.'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(range(1, 25)),
    '#default_value' => $conf['predictions_to_show'],
    '#required' => TRUE,
  );

  $form['prediction_labels'] = array(
    '#title' => t('Prediction time labels'),
    '#description' => t('Used to control the labels of preditions. Defaults to <em>minutes/now arriving</em>.'),
    '#type' => 'select',
    '#options' => array(
      'short' => 'min/arriving',
      'long' => 'minutes/now arriving',
    ),
    '#default_value' => $conf['prediction_labels'],
    '#required' => TRUE,
  );

  $form['show_agency'] = array(
    '#title' => t('Show Agency Column'),
    '#description' => t('Sometimes the agency column is not needed. Shown by
      default.'),
    '#type' => 'checkbox',
    '#default_value' => $conf['show_agency'],
  );

  $form['show_direction'] = array(
    '#title' => t('Show Bus Direction'),
    '#description' => t('Show the direction of the bus under the route name.
      Not shown by default.'),
    '#type' => 'checkbox',
    '#default_value' => $conf['show_direction'],
  );

  $form['show_refresh'] = array(
    '#title' => t('Show Refresh link'),
    '#description' => t('Show the "Refresh predictions" link under the table
      of predictions.'),
    '#type' => 'checkbox',
    '#default_value' => $conf['show_refresh'],
  );

  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function bongo_predictions_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Run-time rendering of the body of the block (content type).
 *
 * See ctools_plugin_examples for more advanced info.
 */
function bongo_predictions_render($subtype, $conf, $panel_args, $context = NULL) {
  $block = new stdClass();

  $predictions_table = bongo_build_predictions_table($conf['stop'], $conf['predictions_to_show'], $conf['show_agency'], $conf['prediction_labels'], $conf['show_direction']);

  $block->title = bongo_predictions_build_title($conf['stop']);
  $block->content = array(
    $predictions_table,
    bongo_panes_ajax_render($conf['stop'], $conf['predictions_to_show'], $conf['show_agency'], $conf['show_refresh'], $conf['prediction_labels'], $conf['show_direction']),
  );

  drupal_add_js(
    array(
      'bongoPanes' => array(
        array(
          'stop' => $conf['stop'],
          'predictions_to_show' => $conf['predictions_to_show'],
          'agency' => $conf['show_agency'],
          'prediction_labels' => $conf['prediction_labels'],
          'direction' => $conf['show_direction'],
        ),
      ),
    ),
    'setting'
  );
  drupal_add_js(drupal_get_path('module', 'bongo_panes') . '/js/bongo_panes.js');
  return $block;
}

/**
 * Build an array of options for the configuration form.
 */
function bongo_build_stop_list_options() {
  $stoplist_array = bongo_get_stop_list();
  foreach ($stoplist_array['stops'] as $stop) {
    $stopnumber  = $stop['stop']['stopnumber'];
    $stoptitle = $stop['stop']['stoptitle'];
    $options[$stopnumber] = t('@stopnumber - @stoptitle', array(
      '@stopnumber' => $stopnumber,
      '@stoptitle' => $stoptitle,
    ));
  }
  return $options;
}

/**
 * Build the title.
 */
function bongo_predictions_build_title($stopid) {
  $stop_information = bongo_get_stop_information($stopid);
  $title_formatted = t('Next Arrivals for Stop @stopid', array('@stopid' => $stopid));
  if ($stop_information) {
    $title = $stop_information['stopinfo']['stoptitle'];
    $title_formatted = t('Next Arrivals for Stop @stopid - @stopname', array(
      '@stopid' => $stopid,
      '@stopname' => $title,
    ));
  }
  return $title_formatted;
}
