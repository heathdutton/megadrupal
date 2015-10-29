<?php

/**
 * @file
 * WHM Overview administrative pages.
 */

/**
 * Definition for the WHM Overview page.
 */
function whm_overview_overview_page() {
  drupal_add_css(drupal_get_path('module', 'whm_overview') . '/css/whm.overview.css');
  $whmovrvw = '<h1>' . t('Web Host Manager Overview') . '</h1>';
  $srvrinfo = '<h2>' . t('Server Information') . '</h2>';
  $statushdng = '<h2>' . t('Status Information') . '</h2>';
  $output = $whmovrvw;
  $output .= '<br />';
  $output .= $srvrinfo;
  $output .= whm_overview_server_info();
  $output .= $statushdng;
  $output .= whm_overview_status_table();

  return $output;
}

/**
 * Build and render WHM server information table.
 */
function whm_overview_server_info() {
  $header = array(
    t('Hostname'),
    t('Main IP'),
    t('WHM Version'),
  );

  $rows = array();

  $hostname = whm_hostname();
  $version = whm_version();
  $ip = whm_main_ip();

  $row = array(
    array(
      $hostname,
      $ip,
      $version,
    ),
  );

  $output = theme_table(array(
    'header' => $header,
    'rows' => $row,
    'attributes' => array(),
    // Table header will be sticky.
    'sticky' => FALSE,
    'caption' => "",
    'colgroups' => array(),
    // The message to be displayed if table is empty.
    'empty' => t('No information found. Check WHM settings.'),
    )
  );
  return $output;
}

/**
 * Build and render WHM Overview server status table.
 */
function whm_overview_status_table() {
  $header = array(
    t('Service Name'),
    t('Installed'),
    t('Enabled'),
    t('Running'),
    t('Monitored'),
    t('Service Machine Name'),
  );

  $rows = array();

  $srvcs = whm_get_services();

  if (!empty($srvcs)) {
    foreach ($srvcs as $srvc) {
      if ((!empty($srvc['installed']))) {
        if ($srvc['installed'] == 1) {
          $installed = '<span class="whm-overview-y">';
          $installed .= t('Y');
          $installed .= '</span>';
        }
      }
      else {
        $installed = '<span class="whm-overview-n">';
        $installed .= t('N');
        $installed .= '</span>';
      }
      if ((!empty($srvc['enabled']))) {
        if ($srvc['enabled'] == 1) {
          $enabled = '<span class="whm-overview-y">';
          $enabled .= t('Y');
          $enabled .= '</span>';
        }
      }
      else {
        $enabled = '<span class="whm-overview-n">';
        $enabled .= t('N');
        $enabled .= '</span>';
      }
      if ((!empty($srvc['running']))) {
        if ($srvc['running'] == 1) {
          $running = '<span class="whm-overview-y">';
          $running .= t('Y');
          $running .= '</span>';
        }
      }
      else {
        $running = '<span class="whm-overview-n">';
        $running .= t('N');
        $running .= '</span>';
      }
      if ((!empty($srvc['monitored']))) {
        if ($srvc['monitored'] == 1) {
          $monitored = '<span class="whm-overview-y">';
          $monitored .= t('Y');
          $monitored .= '</span>';
        }
      }
      else {
        $monitored = '<span class="whm-overview-n">';
        $monitored .= t('N');
        $monitored .= '</span>';
      }
      $row = array(
        $srvc['display_name'],
        $installed,
        $enabled,
        $running,
        $monitored,
        $srvc['name'],
      );
      $rows[] = $row;
    }
  }
  $output = theme_table(array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(),
    // Table header will be sticky.
    'sticky' => TRUE,
    'caption' => "",
    'colgroups' => array(),
    // The message to be displayed if table is empty.
    'empty' => t('No services found. Check WHM settings.'),
    )
  );

  return $output;
}
