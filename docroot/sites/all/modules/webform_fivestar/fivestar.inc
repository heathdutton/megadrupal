<?php
/**
 * @file
 * Fivestar component for webform module. Implements webform component hooks.
 *
 */


/**
 * Implements_webform_defaults_component().
 */
function _webform_defaults_fivestar() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '4',
    'mandatory' => 0,
    'email' => 1,
    'extra' => array(
      'admin_display' => 'stars',
      'field_prefix' => '',
      'field_suffix' => '',
      'disabled' => 0,
      'unique' => 0,
      'description' => '',
      'attributes' => array(),
      'default_value' => 0,
      'count_zero_ratings' => 0,
    ),
  );
}


/**
 * Implements _webform_edit_component().
 */
function _webform_edit_fivestar($component) {
  $form = array();

  // Disabling the description if not wanted.
  $form['description'] = array();

  $form['value'] = array(
    '#type' => 'radios',
    '#title' => t('Number of stars'),
    '#default_value' => $component['value'],
    '#description' => t('The number of stars to display'),
    '#weight' => 5,
    '#required' => TRUE,
    '#options' => array(
      '2' => t('Two stars'),
      '3' => t('Three stars'),
      '4' => t('Four stars'),
      '5' => t('Five stars'),
      '6' => t('Six stars'),
      '7' => t('Seven stars'),
      '8' => t('Eight stars'),
      '9' => t('Nine stars'),
      '10' => t('Ten stars'),
    ),
  );

  $form['extra']['count_zero_ratings'] = array(
    '#type' => 'radios',
    '#title' => t('Count zero ratings'),
    '#default_value' => $component['extra']['count_zero_ratings'],
    '#description' => t('Use zero ratings in calculations of averages.'),
    '#weight' => 6,
    '#required' => TRUE,
    '#options' => array(
      '0' =>  t('No'),
      '1' =>  t('Yes'),
    ),
  );

  $form['extra']['default_value'] = array(
    '#type' => 'radios',
    '#title' => t('Default number of stars'),
    '#default_value' => $component['extra']['default_value'],
    '#description' => t('The default number of stars to mark when the user first looks at the form.'),
    '#weight' => 5,
    '#required' => TRUE,
    '#options' => array(
      '0' =>   t('None'),
      '50' =>  t('Half'),
      '100' => t('All'),
    ),
  );

  $form['display']['admin_display'] = array(
    '#type' => 'radios',
    '#title' => t('Administration display settings'),
    '#default_value' => isset($component['extra']['admin_display']) ? $component['extra']['admin_display'] : 'stars',
    '#description' => t('Chooses between displaying results in absolute or starred values.'),
    '#weight' => 5,
    '#options' => array(
      'stars' => t('Use stars in the results display.'),
      'percentage' => t('Use numbers in the results display.'),
    ),
    '#parents' => array('extra', 'admin_display'),
  );

  return $form;
}


/**
 * Implements _webform_render_component().
 */
function _webform_render_fivestar($component, $value = NULL, $filter = TRUE) {

  $element = array(
    '#type'          => 'fivestar',
    '#title'         => $component['name'],
    '#stars'         => $component['value'],
    '#default_value' => $component['extra']['default_value'],
    '#required'      => $component['mandatory'],
    '#weight'        => $component['weight'],
    '#description'   => $component['extra']['description'],
    '#attributes'    => $component['extra']['attributes'],
    '#prefix'        => '<div class="webform-component webform-component-' . $component['type'] . '" id="webform-component-' . $component['form_key'] . '">',
    '#suffix'        => '</div>',
    '#webform_component' => $component,
    '#element_validate' => array(),
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#theme_wrappers' => array('webform_element'),
  );

  // If resuming the form, put in the current value
  if (isset($value)) {
    $element['#default_value'] = $value[0];
  }

  return $element;
}


/**
 * Implements _webform_display_component().
 */
function _webform_display_fivestar($component, $value, $format = 'html') {
 /*
  *  Component holds values for this instance of fivestar.
  *  $component['value'] hold the number of stars to show
  */

  // We are using fivestar's theme function, so add in it's CSS
  drupal_add_css(drupal_get_path('module', 'fivestar') . '/css/fivestar.css');

  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => $component['extra']['admin_display'] == 'stars' ? 'fivestar_static' : 'webform_fivestar_formatter_percentage',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#post_render' => array('webform_element_wrapper'),
    '#component' => $component,
    '#format' => $format,
    '#rating' => $value[0],
    '#stars' => $component['value'],
  );
}


/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_fivestar($component, $sids = array(), $single = FALSE) {
  $submission_count = 0;
  $rating_total = 0;
  $star_weight = (100/$component['value']);
  $ranges = array_fill(0, $component['value']+1, '0');

  // Generate a lookup table of results.
  $query = db_select('webform_submitted_data', 'wsd')
    ->fields('wsd', array('no', 'data'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid'])
    ->condition('data', '', '<>')
    ->groupBy('no')
    ->groupBy('data');
  $query->addExpression('COUNT(sid)', 'datacount');

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }

  $result = $query->execute();
  foreach ($result as $data) {
    $submission_count ++;
    $rating_total += $data->data;
    $stars = ceil($data->data / $star_weight);
    $ranges[$stars] ++;
  }

  if ($component['extra']['count_zero_ratings']) {
    //Calculating of average with zero ratings
    $average_rating = $submission_count ? round(($rating_total/$submission_count), 1) : 0;
    $rows[] = array( t('Average rating (using zero ratings)'), $average_rating . '%');
  }
  else { //Calculating of average without zero ratings
    $average_rating = $submission_count ? round($rating_total/($submission_count- $ranges[0]), 1) : 0;
    $rows[] = array( t('Average rating (without using zero ratings)'), $average_rating . '%');
  }

  // Special case of zero percent
  if (!empty($ranges[0])) {
    $rows[]= array( t('Number of 0 star reviews (0%)'), empty($ranges[0]) ? 0 : $ranges[0] );
  }
  for ($i = 1; $i <= $component['value']; $i++) {
    $ranges[$i] = empty($ranges[$i]) ? 0 : $ranges[$i];
    $lower_bound = round($star_weight*($i-1)) ;
    $upper_bound = round($star_weight*$i);
    $row_name = t('Number of !i star reviews (>!lower_bound% - !upper_bound%) ', array('!i' => $i, '!lower_bound' => $lower_bound, '!upper_bound' => $upper_bound ));
    $rows[] = array( $row_name, $ranges[$i]);
  }

  $row_name = '<strong>' . t('Total number of submissions') . '</strong>';
  $row_value = '<strong>' . $submission_count . '</strong>';
  $rows[] = array($row_name , $row_value);

  return $rows;
}


/**
 * Implements _webform_table_component().
 */
function _webform_table_fivestar($component, $value) {
  return _webform_fivestar_render_score($component, $value[0]);
}


/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_fivestar($component, $export_options) {
  $header = array();
  $header[2] = array($component['name']);
  return $header;
}


/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_fivestar($component, $export_options, $value) {
  return $value;
}


/**
 * Our helper function to return rendered static fivestar output.
 *
 * @param $component
 *   A Webform component array.
 * @param $score
 *   The rating(value) to display as a percentage.
 * @return
 *   Themed HTML output of stars.
 */
function _webform_fivestar_render_score($component, $score) {
  if (isset($component['extra']['admin_display'])) {
    if ($component['extra']['admin_display'] == 'stars') {
      $output = theme('fivestar_static', array(
        'rating' => $score,
        'stars' => $component['value'],
        )
      );
      // We are using fivestar's theme function, so add in it's CSS
      drupal_add_css(drupal_get_path('module', 'fivestar') . '/css/fivestar.css');
    }
    else {
      $output = $score . '%';
    }
  }
  return $output;
}