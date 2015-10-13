<?php

/**
 * @file
 * Ideal configuration overview.
 */

/**
 * Page callback: display all created ideal configurations.
 *
 * Renders a list or the current ideal configurations and shows
 * the current status. The status icon forwards the user to either
 * the system log or the test page.
 *
 * @return string
 *   Rendered html table with ideal configurations.
 *
 * @see ideal_advanced_menu()
 */
function ideal_advanced_admin_config_list() {
  $header = array(
    t('Title'),
    t('Merchant ID'),
    t('Sub ID'),
    t('Debug mode'),
    t('Status'),
    t('Operations'),
  );

  $rows = array();
  $config_handler = new IdealAdvancedConfigHandler();

  foreach (entity_load('ideal_advanced_config') as $key => $configuration) {
    $links = menu_contextual_links('ideal_advanced', 'admin/config/services/ideal', array($key));
    $status = $config_handler->configValidate($configuration->id);

    if ($configuration->debug_mode) {
      $debug = t('Off');
    }
    else {
      $debug = t('On');
    }

    $rows[] = array(
      check_plain($configuration->title),
      check_plain($configuration->merchant_id),
      check_plain($configuration->sub_id),
      $debug,
      ideal_advanced_status_icon($status),
      theme('links', array('links' => $links, 'attributes' => array('class' => 'links inline operations'))),
    );
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('There are no iDEAL configurations yet. <a href="@link">Add an iDEAL configuration</a>', array('@link' => url('admin/config/services/ideal/add'))),
       'colspan' => 5,),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}
