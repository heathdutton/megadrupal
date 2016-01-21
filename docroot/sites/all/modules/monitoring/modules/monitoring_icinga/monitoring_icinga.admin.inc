<?php

/**
 * @file
 * Monitoring Icinga admin pages.
 */

/**
 * Icinga config passive page callback.
 */
function monitoring_icinga_config_passive_page() {

  $output[] = array(
    '#type' => 'markup',
    '#markup' => '<div class="messages warning">' . t('Passive result checing is not recommended as in case of the monitored system failure the Icinga server will not get notified of this fact. Due to this the <a href="@url">passive checks with freshness checking</a> is recommended.',
      array('@url' => url('admin/config/system/monitoring/icinga/passive-with-freshness'))) . '</div>',
  );

  $output[] = array(
    '#theme' => 'monitoring_config_box',
    '#description' => t('Passive services check means the host is responsible for contacting submitting the status data. Info on this page will help you to configure this setup.'),
  );

  $output[] = array(
    '#theme' => 'monitoring_config_box',
    '#title' => t('Prerequisites'),
    '#description' => t('Before continuing with following setup you need to have Icinga server up and running with "nsca" daemon and the "send_nsca" command at the monitored system. Refer to the README.txt for a brief setup procedure.'),
  );

  $output[] = monitoring_icinga_objects_definitions_output(FALSE);

  $conf_path = explode('/', conf_path());

  $output[] = array(
    '#theme' => 'monitoring_config_box',
    '#title' => t('Setting up send_nsca command'),
    '#description' => t('Following script is a working example of retrieving the check results and submitting them to the Icinga server. Create such script at your monitored system and hook it into cron tab to run every five minutes. Make sure all paths are correct.',
      array('@name' => str_replace('.', '_', monitoring_host()))),
    '#code' => monitoring_config_code('monitoring_icinga', 'submit_check_result', array(
      '@central_server' => 'REPLACE WITH YOUR ICINGA SERVER ADDRESS',
      '@host_name' => monitoring_host(),
      '@root' => DRUPAL_ROOT,
      '@uri' => array_pop($conf_path),
    )),
    '#code_height' => '300'
  );

  return $output;
}

/**
 * Icinga config active page callback.
 */
function monitoring_icinga_config_active_page() {
  $output = array();

  $output[] = array(
    '#theme' => 'monitoring_config_box',
    '#description' => t('Active services check means Icinga server itself is responsible for contacting host to retrieve status data. Info on this page will help you to configure this setup.'),
  );

  $output[] = array(
    '#theme' => 'monitoring_config_box',
    '#title' => t('Prerequisites'),
    '#description' => t('Before continuing with following setup you need to have Icinga server up and running and the NRPE connector at the monitored system. Refer to the README.txt for a brief setup procedure.'),
  );

  $output[] = array(
    '#theme' => 'monitoring_config_box',
    '#title' => t('Command definition to issue the remote NRPE call'),
    '#description' => t('In the /etc/icinga/commands.cfg file add following command definition.'),
    '#code' => monitoring_config_code('monitoring_icinga', 'command'),
    '#code_height' => '70',
  );

  $output[] = monitoring_icinga_objects_definitions_output();

  $conf_path = explode('/', conf_path());

  $output[] = array(
    '#theme' => 'monitoring_config_box',
    '#title' => t('NRPE configuration'),
    '#description' => t('After installing NRPE on the monitored machine in the /etc/nagios/nrpe_local.cfg add following code.'),
    '#code' => monitoring_config_code('monitoring_icinga', 'nrpe_local', array(
      '@root' => DRUPAL_ROOT,
      '@uri' => array_pop($conf_path),
      '@site_key' => monitoring_host_key(),
      '@ip' => $_SERVER['SERVER_ADDR'],
    )),
    '#code_height' => '300'
  );

  return $output;
}

/**
 * Icinga config active page callback.
 */
function monitoring_icinga_config_passive_with_freshness_page() {
  $output = array();

  $output[] = array(
    '#theme' => 'monitoring_config_box',
    '#description' => t('This approach combines passive and active result checks. In case Icinga freshness check of a sensor will not pass it will conduct a separate active check of the sensor.'),
  );

  $output[] = array(
    '#theme' => 'monitoring_config_box',
    '#title' => t('Prerequisites'),
    '#description' => t('Before continuing with following setup you need to have Icinga server up and running and the NRPE connector at the monitored system. Refer to the README.txt for a brief setup procedure.'),
  );

  $output[] = array(
    '#theme' => 'monitoring_config_box',
    '#title' => t('Command definition to issue the remote NRPE call'),
    '#description' => t('In the /etc/icinga/commands.cfg file add following command definition.'),
    '#code' => monitoring_config_code('monitoring_icinga', 'command'),
    '#code_height' => '70',
  );

  $output[] = monitoring_icinga_objects_definitions_output(FALSE, TRUE);

  $conf_path = explode('/', conf_path());

  $output[] = array(
    '#theme' => 'monitoring_config_box',
    '#title' => t('NRPE configuration'),
    '#description' => t('After installing NRPE on the monitored machine in the /etc/nagios/nrpe_local.cfg add following code.'),
    '#code' => monitoring_config_code('monitoring_icinga', 'nrpe_local', array(
      '@root' => DRUPAL_ROOT,
      '@uri' => array_pop($conf_path),
      '@site_key' => monitoring_host_key(),
      '@ip' => $_SERVER['SERVER_ADDR'],
    )),
    '#code_height' => '300'
  );

  $output[] = array(
    '#theme' => 'monitoring_config_box',
    '#title' => t('Setting up send_nsca command'),
    '#description' => t('Following script is a working example of retrieving the check results and submitting them to the Icinga server. Create such script at your monitored system and hook it into cron tab to run every five minutes. Make sure all paths are correct.',
      array('@name' => str_replace('.', '_', monitoring_host()))),
    '#code' => monitoring_config_code('monitoring_icinga', 'submit_check_result', array(
      '@central_server' => 'REPLACE WITH YOUR ICINGA SERVER ADDRESS',
      '@host_name' => monitoring_host(),
      '@root' => DRUPAL_ROOT,
      '@uri' => array_pop($conf_path),
    )),
    '#code_height' => '300'
  );

  return $output;
}

/**
 * Creates the object definitions output part.
 *
 * @param bool $active_checks
 * @param bool $check_freshness
 *
 * @return array
 *   Renderable array.
 */
function monitoring_icinga_objects_definitions_output($active_checks = TRUE, $check_freshness = FALSE) {
  $host = monitoring_host();
  $host_def = monitoring_config_code('monitoring_icinga', 'host', array(
    '@host' => $host,
    '@ip' => $_SERVER['SERVER_ADDR'],
  ));

  $services_def = array();
  $servicegroups = array();
  foreach (monitoring_sensor_manager()->getEnabledSensorInfo() as $sensor_name => $sensor_info) {
    $services_def[] = monitoring_config_code('monitoring_icinga', 'service', array(
      '@host' => $host,
      '@service_description' => monitoring_icinga_service_description($sensor_info),
      '@sensor_name' => $sensor_name,
      '@site_key' => monitoring_host_key(),
      '@description' => $sensor_info->getDescription(),
      '@active_checks' => (int) $active_checks,
      '@passive_checks' => (int) (!$active_checks),
      '@check_freshness' => (int) $check_freshness,
      '@check_command' => ($check_freshness ? 'service_is_stale' : 'check_drupal'),
    ));

    $category = strtolower(str_replace(' ', '_', $sensor_info->getCategory()));
    $servicegroups[$category]['alias'] = $category;
    $servicegroups[$category]['members'][] = $host . ',' . monitoring_icinga_service_description($sensor_info);
  }
  $services_def = implode("\n\n", $services_def);

  $servicegroups_def = array();
  foreach ($servicegroups as $name => $servicegroup) {
    $servicegroups_def[] = monitoring_config_code('monitoring_icinga', 'servicegroup', array(
      '@name' => monitoring_host_key() . '_' . $name,
      '@alias' => $servicegroup['alias'],
      '@members' => implode(', ', $servicegroup['members']),
    ));
  }
  $servicegroups_def = implode("\n", $servicegroups_def);

  // $system_services = monitoring_config_code('monitoring_icinga', 'system', array('@host' => $host));

  return array(
    '#theme' => 'monitoring_config_box',
    '#title' => t('Host and services configuration'),
    '#description' => t('At the Icinga server create /etc/icnga/objects/@name_icinga.cfg file with following code. Note that the code changes based on which sensor are enabled/disabled.',
      array('@name' => monitoring_host_key())),
    '#code' => "; === HOST DEFINITION ===\n" . $host_def . "\n; === SERVICEGROUPS DEFINITIONS ===\n" . $servicegroups_def . "\n; === DRUPAL SERVICES DEFINITIONS ===\n" . $services_def,
    '#code_height' => '300'
  );
}
