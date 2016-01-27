<?php
/**
 *@file
 *  All the taleo related functions are included in this file.
 *
 */

/**
 *Function that fetches the latest job postings from Taleo.
 *
 */
function get_jobs_from_taleo($country = '', $max_jobs = 5) {
  $error = 0;
  try {
    global $base_path, $base_url;
    $taleo_wsdl_dispatcher = variable_get('taleo_wsdl_dispatcher', 'https://tbe.taleo.net/wsdl/DispatcherAPI.wsdl');
    $taleo_wsdl_web =  variable_get('taleo_wsdl_web', 'https://tbe.taleo.net/wsdl/WebAPI.wsdl');
    $client = new SoapClient($taleo_wsdl_dispatcher, array("trace" => TRUE, "connection_timeout" => 15000));
    $url = $client->__call('getURL', array(variable_get('taleo_company_code', '')));

    $client_taleo = new SoapClient($taleo_wsdl_web, array("trace" => TRUE, "connection_timeout" => 15000));
    $client_taleo->__setLocation($url);

    $params = array(
      'in0' => variable_get('taleo_company_code', ''),
      'in1' => variable_get('taleo_username', ''),
      'in2' => variable_get('taleo_password', ''),
    );
    $session =  $client_taleo->__call('login', $params);

    $params = array(
      'in0' => $session,
    );
    $params = array(
      'in0' => $session,
      'in1' => array('status' => 'Open'),
    );

    $job_ids =  $client_taleo->__call('searchRequisition', $params);

    $jobidarray = $job_ids->array->item;
    shuffle($jobidarray);
    if (empty($jobidarray)) {
      return;
    }

    $job_count = 0;
    $return_jobs = array();
    foreach ($jobidarray as $key => $job_id_object) {
      if ($job_count >= $max_jobs) {
        break;
      }
      $params = array(
        'in0' => $session,
        'in1' => $job_id_object->id,
      );
      $job = $client_taleo->__call('getRequisitionById', $params);
      $return_jobs[] = $job;
      $job_count++;
    }

    return $return_jobs;
  }
  catch (SoapFault $fault) {
    $error = 1;
    $error_message = check_plain("<br/>ERROR: " . $fault->faultcode . "-" . $fault->faultstring);
    drupal_set_message($error_message);
    watchdog('taleo', $error_message, array(), WATCHDOG_ERROR);
    return array();
  }

}
