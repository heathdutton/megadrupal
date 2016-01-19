<?php

/**
 * Implements hook_drush_command()
 */
function at_cm_drush_command() {
    $cmds = array();

    $cmds['at_cm'] = array(
      'callback' => 'at_cm_drush_callback',
      'description' => 'CLI for CampaignMonitor services',
    );

    $cmds['at_cm.lists'] = array(
      'callback' => 'at_cm_list_drush_callback',
      'description' => 'List all lists',
    );

    return $cmds;
}

/**
 * Cacllback for at_cm drush command.
 *
 * @param string $service
 * @param string $method
 */
function at_cm_drush_callback($service = '', $method = '') {
    $args = func_get_args();

    if (empty($service)) {
        at_cm_drush_callback__list_services();
    }
    elseif (empty($method))  {
        at_cm_drush_callback__list_methods($service);
    }
    else {
        at_cm_drush_callback__list_args($service, $method);
    }
}

function at_cm_drush_callback__list_services() {
    $rows = array();

    $services = at_container('helper.config_fetcher')
        ->getItems('at_base', 'services', 'services', TRUE);
    foreach ($services as $name => $def) {
        if (0 === strpos($name, 'cm.')) {
            $rows[] = array($name, $def['class']);
        }
    }

    drush_print_table(array(
      'header'   => array('Service',   'Class'),
      'seprator' => array('----------', '---------------------'),
    ) + $rows, TRUE);
}

function at_cm_drush_callback__list_methods($service) {
    $service = at_container($service);

    $ref = new ReflectionObject($service);

    $get_params = function($method) {
        $rows = array();
        foreach ($method->getParameters() as $param) {
            $rows[] = '$' . $param->name;
        }
        return implode(', ', $rows);
    };

    $rows = array();
    foreach ($ref->getMethods(ReflectionMethod::IS_PUBLIC) as $method) {
        $rows[] = array($method->class, $method->name, '(' . $get_params($method) . ')');
    }

    drush_print_table(array(
      'header'   => array('Class',      'Method',       'Params',       ),
      'seprator' => array('----------', '------------', '---------------------'),
    ) + $rows, TRUE);
}

function at_cm_drush_callback__list_args($service, $method) {
    $service = at_container($service);
    $ref = new ReflectionObject($service);

    $params = $ref->getMethod($method)->getParameters();

    if (empty($params)) {
        $response = $service->{$method}();
        if ($response->http_status_code) {
            drush_print_r($response->response);
        }
    }
    else {
        $rows = array();

        foreach ($params as $param) {
            $rows[] = array($param->name);
        }

        drush_print_table(array(
          'header'   => array('Param'),
          'seprator' => array('----------'),
        ) + $rows, TRUE);
    }
}

function at_cm_list_drush_callback() {
    $sgeneral = AT_CM::getGeneral();
    $sclient = AT_CM::getClients();
    $rows = array();

    foreach ($sgeneral->get_clients()->response as $client) {
        $sclient->set_client_id($client->ClientID);

        foreach ($sclient->get_lists()->response as $list) {
            $rows[] = array(
              $client->Name,
              $client->ClientID,
              $list->Name,
              $list->ListID
            );
        }
    }

    drush_print_table(array(
      'header'   => array('Client',   'Client ID', 'List Name', 'List ID'),
      'seprator' => array('-------',  '-------',   '-------',   '-------'),
    ) + $rows, TRUE);
}
