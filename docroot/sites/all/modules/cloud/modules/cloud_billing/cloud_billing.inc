<?php

/**
 * @file
 * *.inc file: Billing
 * Provides usage estimate table based on user's activities.
 * Works with Cloud module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

define('CLOUD_BILLING_PATH', 'design/billing');

/**
 * Get cloud_billing_report
 */
function _cloud_billing_usage_estimate($cloud_context = '' ) {
  return drupal_get_form('cloud_billing_report', $cloud_context);
}


/**
 * Returns a form with listing of usage estimate
 * Build a form including table header and table body
 * @param $form_submit
 *        This is the form-submit submitted by billing form
 * @param $cloud_context
 *        This is the sub-cloud who's billing details is being displayed
 * @return return a form
 */
function cloud_billing_report($form, $form_submit = '', $cloud_context = '') {

  if (empty($cloud_context)) {
    $cloud_context = '';
    $cloud_name = 'all';
  }
  else {
    $cloud_name = $cloud_context;
  }
  $column = 'name';
  $total  = 0;
  $lumsum = 0;

  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      array('data' => t('Date' )),
      array('data' => t('Total')),
    ),
  );
  $month_range_list = _cloud_billing_get_month_range_array();

  foreach ($month_range_list as $start => $end ) {

    $query = _cloud_billing_generate_report($start, $end, $cloud_context);
    $result = db_query( $query );
    $total  = 0;

    foreach ($result as $billing_obj) {

      $total_instance_type_charge = 0;
      $unit_price = $billing_obj->linux_or_unix_usage;
      $total_hours = _cloud_billing_instance_total_hours($start, $end, $billing_obj->launch_time, $billing_obj->terminated_date);
      $total_instance_type_charge = ($total_hours > 0 )
                                  ? ($total_hours * $unit_price)
                                  : 0;
      $total += $total_instance_type_charge;
    }

    $form['date'][$start] = array(
      array(
        '#markup' => l($start . ' to ' . $end, 
                      'reports/usage_estimate/' . $start . '/' . $end . '/details/' . $cloud_name, 
                      array('query' => array('start' => $start, 'end' => $end))),
      ),
    );

    $form['totals'][$start] = array('#markup' => t( '$' . number_format($total, 2)));
    $lumsum += $total;
  }

  $form['cloud_context'] = array(
    '#type' => 'hidden',
    '#value' => ($cloud_context),
  );
  $form['Total'] = array('#markup' => '$' . number_format($lumsum, 2));

  return $form;
}

/**
 * Theme the billing report 
 */
function theme_cloud_billing_report($form) {

  $form = $form['form'];
  $rows = array();

  foreach (element_children($form['date']) as $key) {

    $rows[] = array(
      drupal_render($form['date'][$key]),
      drupal_render($form['totals'][$key]),
    );
  }
  $rows[] = array(
    '',
    drupal_render($form['Total']),
  );

  $output  = drupal_render( $form['options']);
  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}


/**
 * Returns a form with listing of usage estimate
 * Build a form including table header and table body
 * @param $form_submit
 *        This is the form-submit submitted by billing form
 * @param $start
 *        This is start date of billing report
 * @param $end
 *        This is end date of billing report
 * @param $cloud_context
 *        This is the sub-cloud who's billing details is being displayed
 * @return return a form
 */
function cloud_billing_report_details($form, $form_submit = '', $start = '', $end = '', $cloud_context = '') {

  if ($cloud_context == 'all') {
    $cloud_context = '';
  }

  $column     = 'name';
  $filter     = cloud_get_filter_value( $form_submit, 'filter'    );
  $filter_col = cloud_get_filter_value( $form_submit, 'operation' );

  $query_args     = array();
  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      array(
        'data' => t('Rate (per Hour)' ),
        'field' => 'linux_or_unix_usage',
        'sort' => 'asc',
      ),
      array(
        'data' => t('Instance Type ID'),
        'field' => 'instance_type',
      ),
      array(
        'data' => t('Description'     ),
        'field' => 'description',
      ),
      array('data' => t('Instance(s)'     )),
      array('data' => t('Usage (in Hours)')),
      array('data' => t('Totals'          )),
      array('data' => t('Start Date'      )),
      array('data' => t('End Date'        )),
    ),
  );

  $query = _cloud_billing_get_instance_types($cloud_context);

  $order  = isset($_REQUEST['order']) ? $_REQUEST['order'] : FALSE;
  $sort   = isset($_REQUEST['sort' ]) ? $_REQUEST['sort' ] : FALSE;

  if (empty($order) === FALSE) {

    if (strpos($order, 'Description') === 0) {

      $query .= ' order by description ';
    }
    elseif (strpos($order, 'Rate') === 0) {

      $query .= ' order by linux_or_unix_usage ';
    }
    elseif (strpos($order, 'Instance') === 0) {

      $query .= ' order by instance_type ';
    }
    $query .= $sort . ' ';
  }
  else { // default

    $query .= ' order by linux_or_unix_usage asc ';
  }

  $result = db_query($query);

  $total       = 0;
  $t_usage     = 0;
  $t_instances = 0;

  foreach ($result as $billing_obj) {

    $instance_type = $billing_obj->instance_type;

    $query_types      = _cloud_billing_generate_report($start, $end, $cloud_context, $instance_type);
    $result_types     = db_query($query_types);
    $total_instances  = 0;
    $total_hours_calc = 0;

    foreach ($result_types as $types) {

      $total_hours = _cloud_billing_instance_total_hours($start, $end, $types->launch_time, $types->terminated_date);

      if ($instance_type == $types->instance_type and $total_hours > 0) {

        $total_hours_calc = $total_hours + $total_hours_calc;
        $total_instances++;
      }
    }

    $unit_price = $billing_obj->linux_or_unix_usage;
    $date_created = '';
    $date_updated = '';
    $start_date = ($start) ? date_format(date_create($start), 'm/d/Y')
                                       : '-';
    $end_date   = ($end)   ? date_format(date_create($end > date('Y-m-d')
                                       ? date('Y-m-d')
                                       : $end), 'm/d/Y')
                                       : '-';

    $total_instance_type_charge = ($total_hours_calc > 0           )
                                ? ($total_hours_calc * $unit_price )
                                :  0;
    $st_usage                   =  $total_hours_calc;

    $form['rate'         ][$billing_obj->id] = array('#markup' => t( '$' . number_format($unit_price, 3)));
    $form['instance_type'][$billing_obj->id] = array('#markup' => t( $billing_obj->instance_type ));
    $form['description'  ][$billing_obj->id] = array('#markup' => t( $billing_obj->description . ' instance ' ));
    $form['stinstnaces'  ][$billing_obj->id] = array('#markup' => t(($total_instances  > 0)
                                                                  ?  number_format($total_instances)
                                                                  :  0 ));
    $form['usage'        ][$billing_obj->id] = array('#markup' => t(($total_hours_calc > 0)
                                                                  ?  number_format($total_hours_calc, 0)
                                                                  :  0 ));
    $form['totals'       ][$billing_obj->id] = array('#markup' => t( '$' . number_format($total_instance_type_charge, 2)));
    $form['start'        ][$billing_obj->id] = array('#markup' => $start_date);
    $form['end'          ][$billing_obj->id] = array('#markup' => $end_date  );

    $total       += $total_instance_type_charge;
    $t_usage     += $st_usage                  ;
    $t_instances += $total_instances           ;
  }

  $form['Total'        ] = array('#markup' => '$' . number_format($total,    2));
  $form['tUsage'       ] = array('#markup' =>       number_format($t_usage    ));
  $form['tInstances'   ] = array('#markup' =>       number_format($t_instances));
  $form['cloud_context'] = array(
    '#type' => 'hidden',
    '#value' => $cloud_context,
  );

  return $form;
}


/**
 * Theme billing details page
 */
function theme_cloud_billing_report_details($form) {

  $form = $form['form'];
  $output = drupal_render($form['options']);

  $rows = array();
  $form['rate'] = !empty($form['rate']) ? $form['rate'] : '';
  $form['pager']['#value'] = !empty($form['pager']['#value']) ? $form['pager']['#value'] : '';
  $rows = array();

  foreach (element_children($form['rate']) as $key) {

    $rows[] = array(
      drupal_render($form['rate'         ][$key]),
      drupal_render($form['instance_type'][$key]),
      drupal_render($form['description'  ][$key]),
      drupal_render($form['stinstnaces'  ][$key]),
      drupal_render($form['usage'        ][$key]),
      drupal_render($form['totals'       ][$key]),
      drupal_render($form['start'        ][$key]),
      drupal_render($form['end'          ][$key]),
    );
  }
  // tack on a totals row
  $rows[] = array(
      '',
      '',
      '',
      drupal_render($form['tInstances']),
      drupal_render($form['tUsage'    ]),
      drupal_render($form['Total'     ]),
      '',
      '',
  );

  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows));

  $cloud_context = $form['cloud_context']['#value'];
  $reload_link = l(t('- Back to Report Page -'), 'reports/usage_estimate/' . $cloud_context );
  $ref_link = array(
    '#type' => 'item',
    '#prefix' => '<div id="link_reload" align="right">',
    '#suffix' => '</div>',
    '#markup' => $reload_link,
  );
  $output .= drupal_render($ref_link);
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Helper function to get months range 
 */
function _cloud_billing_get_month_range_array() {

  $month_range_list = array();

  for ($i = 0; $i < 12; $i++ ) { //for last 1 year
    _cloud_billing_get_month_range($start, $end, $i);
    $month_range_list[$start] = $end;
  }

  return $month_range_list;
}

/**
 * Helper function to get month rance
 */
function _cloud_billing_get_month_range(&$start_date, &$end_date, $offset = 0) {

  $start_date = '';
  $end_date   = '';
  $date       = date('Y-m-d');

  list($yr, $mo, $da) = explode('-', $date);
  $start_date = date('Y-m-d',   mktime(0, 0, 0, $mo - $offset, 1, $yr));

  $i = 2;

  list($yr, $mo, $da) = explode('-', $start_date);

  while (date('d',   mktime(0, 0, 0, $mo, $i, $yr)) > 1) {
    $end_date = date('Y-m-d',   mktime(0, 0, 0, $mo, $i, $yr));
    $i++;
  }
}


/**
 * Function to syc the Cloud Instances details with Billing Instances table
 */

function _cloud_billing_syncronize_records() {

  $filter_params = array();

  $all_instances_list      = array();
  $get_all_instances       = array();
  $get_duplicate_instances = array();

  $cloud_list = cloud_get_all_clouds();

  foreach ($cloud_list as $cloud_context) {
    $instances_list = cloud_get_instances($cloud_context, $filter_params );
    $all_instances_list = array_merge($all_instances_list   , $instances_list );
  }

  foreach ($all_instances_list as $instances) {

    if (is_array($instances)) {

      $query_args = array();

      $all_instances[] = $instances['instance_id'  ];
      $instance_cloud  = $instances['cloud_context'];

      $query_args[]    = 'instance_nickname'   ;
      $query_args[]    = $instances['name'    ];
      $query_args[]    = 'dns_name'            ;
      $query_args[]    = $instances['hostname'];
      $query_args[]    = 'instance_state_name' ;
      $query_args[]    = $instances['state'   ];
      $query_args[]    = 'launch_time'         ;
      $query_args[]    = $instances['runtime' ];

      if (strtolower($instances['state']) == 'running') {
        _cloud_billing_update_running($instances['instance_id'], $instance_cloud);
      }

      //Update Billing Instance Detials
      $update_instance = _cloud_billing_syncronize_instances($instances['cloud_context'], $instances['instance_id'], $query_args);
    }
  }


  //Check for duplicate Instance ID for a cloud type and remove the duplicates for orphaned instances
  $cloud_list = cloud_get_all_clouds();

  foreach ($cloud_list as $cloud_context) {
    $get_duplicate_instances = _cloud_billing_check_duplicate($cloud_context);

    if (!empty($get_duplicate_instances)) {
      //Delete the duplicate Instances for a particular Cloud Type
      foreach ($get_duplicate_instances as $key => $cloud_context) {
        _cloud_billing_remove_instances($key, $cloud_context);
      }
    }
  }


  //Update the Instances table if the Instance Id is not found
  $get_all_instances = _cloud_billing_get_all_instances();

  foreach ($get_all_instances as $instances) {
    $query_args = array();
    $instance_id = $instances['instance_id'];
    $launch_date = $instances['launch_time'];

    if (!array_key_exists($instance_id, $all_instances_list)) {
      $query_args[] = 'instance_state_name';
      $query_args[] = 'terminated';
      $query_args[] = 'terminated_date';
      $query_args[] = $launch_date;

      //Update Billing Instance Detials
      $update_instance = _cloud_billing_update_instances($instance_id, $query_args);
    }
  }
}


/**
 * Function get the Total hours between two different dates
 * @param $launch_time
 *        This is Start Date
 * @param $curr_time
 *        This is End Date, which can be also empty
 * @return returns total number of hours
 */
function _cloud_billing_get_runtime($launch_time, $curr_time = '') {

  if (empty($launch_time)) {
    return 0;
  }

  $launch_tmp_time = date_parse($launch_time);

  $gm_launch_time  = gmmktime($launch_tmp_time['hour'  ],
                              $launch_tmp_time['minute'],
                              $launch_tmp_time['second'],
                              $launch_tmp_time['month' ],
                              $launch_tmp_time['day'   ],
                              $launch_tmp_time['year'  ] );

  if (!empty($curr_time)) {
    $curr_tmp_time = date_parse($curr_time);
    $gm_current_time  = gmmktime($curr_tmp_time['hour'  ],
                                 $curr_tmp_time['minute'],
                                 $curr_tmp_time['second'],
                                 $curr_tmp_time['month' ],
                                 $curr_tmp_time['day'   ],
                                 $curr_tmp_time['year'  ] );
  }
  else {
    $gm_current_time = gmmktime();
  }

  $diff =  $gm_current_time - $gm_launch_time;

  $days = $diff / (86400);
  $hr   = $diff / ( 3600);
  $min  = $diff / (   60);
  $sec  = $diff;

  $hours = 0;
  if ( round( $days) > 0 ) {

    $hours = $days * 24;
  }
  elseif ( round( $hr )  > 0 ) {

    $hours = $hr;
  }
  elseif ( round( $min )  > 0 ) {

    $hours = $min / 60;
  }
  elseif ( round( $sec ) > 0 ) {

    $hours = $sec / 3600;
  }

  return round($hours);
}


/**
 * Function to calculate the total number of Hours an Instance is running.
 *
 * @param $start
 *        This is Instance start date
 * @param $end
 *        This is current date
 * @param $runtime
 *        This is Instance launched date
 * @param $terminated
 *        This is Instance terminated date
 * @return returns hour values
 */
function _cloud_billing_instance_total_hours($start, $end, $runtime, $terminated) {

  $return_data = array();

  $start_date = gmdate('c', strtotime($start));
  $end_date   = gmdate('c', strtotime($end . ' 23:59:59'));

  $instance_launch_date = gmdate('c', strtotime($runtime));

  if (!empty($terminated)) {

    $instance_terminated_date = gmdate('c', strtotime($terminated));
  }

  if (strtotime($start_date) < strtotime($instance_launch_date)) {

    $start_date = $instance_launch_date;
  }

  if (strtotime($end_date) > strtotime(gmdate('c'))) {

    $end_date = gmdate('c');
  }

  if (!empty($terminated) && strtotime($end_date) >= strtotime($instance_terminated_date)) {

    $end_date = $instance_terminated_date;
  }

  $total_hours = _cloud_billing_get_runtime($start_date, $end_date);

  return $total_hours;
}

