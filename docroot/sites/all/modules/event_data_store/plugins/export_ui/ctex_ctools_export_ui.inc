<?php
/**
 * @file
 * Event Data Store export UI plugin.
 */

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  // As defined in hook_schema().
  'schema' => 'event_data_store_event',
  // Define a permission users must have to access these pages.
  'access' => 'administer event_data_store',

  // Define the menu item.
  'menu' => array(
    'menu item' => 'event_data_store',
    'menu title' => 'Event Data Store',
    'menu description' => 'Administer Event Data Store events.',
  ),

  // Define user interface texts.
  'title singular' => t('event'),
  'title plural' => t('events'),
  'title singular proper' => t('Event Data Store event'),
  'title plural proper' => t('Event Data Store events'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'event_data_store_ctools_export_ui_form',
    // 'submit' and 'validate' are also valid callbacks.
  ),
);

/**
 * Implements hook_ctools_export_ui_form().
 */
function event_data_store_ctools_export_ui_form(&$form, &$form_state) {
  $event = $form_state['item'];

  $form['event'] = array(
    '#type' => 'select',
    '#title' => t('Event to trigger on'),
    '#options' => event_data_store_get_trigger_events(),
    '#default_value' => $event->event,
    '#required' => TRUE,
  );

  $form['aggregate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Aggregate data'),
    '#default_value' => isset($event->aggregate) ? $event->aggregate : 1,
  );

  $form['aggregate_behaviour'] = array(
    '#type' => 'select',
    '#title' => t('Aggregate behaviour'),
    '#options' => array(
      'tally' => t('Tally / Count'),
      'sum' => t('Sum'),
    ),
    '#default_value' => !empty($event->aggregate_behaviour) ? $event->aggregate_behaviour : 'tally',
    '#required' => TRUE,
    '#description' => t('Tally means to store the number of records created within the interval, sum is to take the tally number and add it to the previous count.'),
  );

  $form['aggregate_hourly'] = array(
    '#type' => 'checkbox',
    '#title' => t('Aggregate hourly data'),
    '#default_value' => isset($event->aggregate_hourly) ? $event->aggregate_hourly : 1,
  );

  $form['aggregate_daily'] = array(
    '#type' => 'checkbox',
    '#title' => t('Aggregate daily data'),
    '#default_value' => isset($event->aggregate_daily) ? $event->aggregate_daily : 1,
  );

  $form['aggregate_weekly'] = array(
    '#type' => 'checkbox',
    '#title' => t('Aggregate weekly data'),
    '#default_value' => isset($event->aggregate_weekly) ? $event->aggregate_weekly : 1,
  );

  $form['aggregate_monthly'] = array(
    '#type' => 'checkbox',
    '#title' => t('Aggregate monthly data'),
    '#default_value' => isset($event->aggregate_monthly) ? $event->aggregate_monthly : 1,
  );

  $form['aggregate_yearly'] = array(
    '#type' => 'checkbox',
    '#title' => t('Aggregate yearly data'),
    '#default_value' => isset($event->aggregate_yearly) ? $event->aggregate_yearly : 0,
  );

  $day = 86400;
  $month = $day * 30;
  $form['prune_interval'] = array(
    '#type' => 'select',
    '#title' => t('Prune old data after'),
    '#options' => array('0' => t('Retain all data')) + drupal_map_assoc(array($day, $day * 7, $month, $month * 3, $month * 6, $day * 365), 'format_interval'),
    '#default_value' => $event->prune_interval,
  );
}
