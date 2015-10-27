<?php
/**
 * @file
 * Form callbacks for the tether_stats_charts module.
 */

/**
 * The filter form for the element report page.
 *
 * @param object $element
 *   A Tether Stats element object.
 *
 * @see tether_stats_charts_admin_element_report_filter_form_validate()
 * @see tether_stats_charts_admin_element_report_filter_form_submit()
 */
function tether_stats_charts_admin_element_report_filter_form($form, $form_state, $element) {

  if (!isset($_SESSION['tether_stats']['element_report_filter'])) {

    $_SESSION['tether_stats']['element_report_filter']['period_starting'] = new DateTime();
    $_SESSION['tether_stats']['element_report_filter']['period_ending'] = new DateTime();
    $_SESSION['tether_stats']['element_report_filter']['activity_type'] = 'hit';

    $_SESSION['tether_stats']['element_report_filter']['period_starting']->setTimestamp(strtotime('-3 month'));
  }

  $session =& $_SESSION['tether_stats']['element_report_filter'];

  $form = array();

  $form['current_element'] = array(
    '#type' => 'fieldset',
    '#title' => t('Stats Element'),
  );

  $descriptors = tether_stats_describe_element($element);

  foreach ($descriptors as $line) {

    $form['current_element'][] = array('#markup' => $line);
  }

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Generate Report'),
  );

  $form['filters']['activity_type'] = array(
    '#type' => 'select',
    '#title' => t('Activity Type'),
    '#options' => array(
      'hit' => t('Views/Hits'),
      'click' => t('Clicks'),
      'impression' => t('Impressions'),
    ),
    '#default_value' => $session['activity_type'],
  );

  $form['filters']['period_starting'] = array(
    '#type' => 'date_popup',
    '#title' => t('Period Starting'),
    '#default_value' => $session['period_starting']->format('Y-m-d 00:00:00'),
    '#date_format' => 'Y-m-d',
    '#date_label_position' => 'within',
    '#datepicker_options' => array('maxDate' => 0),
  );

  $form['filters']['period_ending'] = array(
    '#type' => 'date_popup',
    '#title' => t('Period Ending'),
    '#default_value' => $session['period_ending']->format('Y-m-d 00:00:00'),
    '#date_format' => 'Y-m-d',
    '#date_label_position' => 'within',
    '#datepicker_options' => array('maxDate' => 0),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate Report'),
  );

  return $form;
}

/**
 * Validation handler for the element report filter form.
 *
 * @see tether_stats_charts_admin_element_report_filter_form()
 */
function tether_stats_charts_admin_element_report_filter_form_validate($form, &$form_state) {

  $start_date = new DateTime($form_state['values']['period_starting']);
  $end_date = new DateTime($form_state['values']['period_ending']);

  if ($start_date > $end_date) {

    form_error($form['filters']['period_starting'], t('The <em>period starting</em> must be less than or equal to the <em>period ending</em>.'));
  }

  if ($end_date->getTimestamp() > REQUEST_TIME) {

    form_error($form['filters']['period_ending'], t('The <em>period ending</em> must be less than or equal to the current date.'));
  }
}

/**
 * Submit handler for the element report filter form.
 *
 * @see tether_stats_charts_admin_element_report_filter_form()
 */
function tether_stats_charts_admin_element_report_filter_form_submit($form, &$form_state) {

  $_SESSION['tether_stats']['element_report_filter']['period_starting'] = new DateTime($form_state['values']['period_starting']);
  $_SESSION['tether_stats']['element_report_filter']['period_ending'] = new DateTime($form_state['values']['period_ending']);
  $_SESSION['tether_stats']['element_report_filter']['activity_type'] = $form_state['values']['activity_type'];
}
