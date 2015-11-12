<?php

use \StructuredDynamics\osf\framework\QuerierExtension;

class DrupalQuerierExtension extends QuerierExtension {
  private $startTime;
  private $stopTime;
  function stopQuery(\StructuredDynamics\osf\framework\WebServiceQuerier $wsq, $data) {
    if (@$_GET['wsf_debug'] || variable_get('osf_enable_osf_web_services_debug_for_admins', 0)) {
      $stimer = explode(' ', microtime());
      $this->stopTime = $stimer[1] + $stimer[0];
    }
  }

  function debugQueryReturn(\StructuredDynamics\osf\framework\WebServiceQuerier $wsq, $data) {         
    if (@$_GET['wsf_debug'] || variable_get('osf_enable_osf_web_services_debug_for_admins', 0)) {
      $non_dpm_debug_format = 'Web service query: [[url: <b>@url</b>] [method: @method] [mime: @mime] [parameters: @params] [execution time: <b>@exectime</b>]] (status: @status) @status_message - @status_message_description.';
      if (@$_GET['wsf_debug'] == 1 || variable_get('osf_enable_osf_web_services_debug_for_admins', 0)) {
        $non_dpm_debug_format .= ' [[[[<span style="white-space: pre;">@data</span>]]]';
      }

      if(!function_exists('dpm')) {
        drupal_set_message(
          t($non_dpm_debug_format,
            array(
              '@url' => $wsq->getURL(),
              '@method' => $wsq->getMethod(),
              '@mime' => $wsq->getMIME(),
              '@params' => $wsq->getParameters(),
              '@exectime' => $this->stopTime - $this->startTime,
              '@status' => $wsq->getStatus(),
              '@status_message' => $wsq->getStatusMessage(),
              '@status_message_description' => $wsq->getStatusMessageDescription(),
              '@data' => substr($data, strpos($data, "\r\n\r\n") + 4),
            )), 'warning');
      }
      else {
        dpm(array(
             'url' => $wsq->getURL(),
             'method' => $wsq->getMethod(),
             'mime' => $wsq->getMIME(),
             'parameters' => $wsq->getParameters(),
             'execution time' => $this->stopTime - $this->startTime,
             'status' => $wsq->getStatus(),
             'status message' => $wsq->getStatusMessage(),
             'status description' => $wsq->getStatusMessageDescription(),
             'data' => substr($data, strpos($data, "\r\n\r\n") + 4),
             'wsq' => var_export($wsq, TRUE)
            ), 'WSQ Query Debug: ' . $wsq->getURL());
      }
    }
  }

  function startQuery(\StructuredDynamics\osf\framework\WebServiceQuerier $wsq) {
    if (@$_GET['wsf_debug'] || variable_get('osf_enable_osf_web_services_debug_for_admins', 0)) {
      $stimer = explode(' ', microtime());
      $this->startTime = $stimer[1] + $stimer[0];
    }
  }

  function alterQuery(\StructuredDynamics\osf\framework\WebServiceQuerier $wsq, $curl_handle) {
    drupal_alter('osf_query', $curl_handle, $wsq);
  }

  function displayError($qe) {
    if (user_access('administer osf')) {
      // In case that it is a 500 error returned from the web server and not the endpoint.
      if(!$qe->level) {
        if($wsq->queryStatus == 500) {
          $qe->level = 'fatal';
          $qe->id = '500-Internal-Server-Error';
          $qe->name = 'Internal Server Error';
          $qe->debugInfo = $wsq->getStatusMessageDescription;
        }
      }

      $error_info = array(
                     '@id' => strip_tags($qe->id),
                     '@ws' => strip_tags($qe->webservice),
                     '@name' => strip_tags(trim($qe->name, '.') . '.'),
                     '@description' => strip_tags(trim($qe->description, '.') . '.'),
                     '@debugInfo' => strip_tags($qe->debugInfo));

      switch ($qe->level) {
        case 'fatal':
          $error_level = 'error';
          $error_info['!type'] = 'Fatal error';
          break;

        case 'error':
          $error_level = 'error';
          $error_info['!type'] = 'Error';
          break;

        case 'warning':
          $error_level = 'warning';
          $error_info['!type'] = 'Warning';
          break;

        case 'notice':
          $error_level = 'status';
          $error_info['!type'] = 'Notice';
          break;

        default:
          $error_level = 'warning';
          $error_info['!type'] = 'Error of unknown level';
      }
      drupal_set_message(t("!type (@id) on web service endpoint '@ws': \n\n@name \n\n @description \n\n @debugInfo", $error_info), $error_level, TRUE); 
    }
    else {
      switch ($qe->level) {
        case 'fatal':
          drupal_set_message(
            t('A fatal error occured in the system, please contact the site administrator if the problem persists. Error number to report: @id', array('@id' => strip_tags($qe->id))), 'error', TRUE);
          break;

        case 'warning':
          switch ($this->error->id) {
            case 'WS-AUTH-VALIDATOR-304':
            case 'WS-AUTH-VALIDATOR-305':
            case 'WS-AUTH-VALIDATOR-306':
            case 'WS-AUTH-VALIDATOR-307':
              drupal_set_message(t("You don't have the permissions to perform this request"), 'warning', TRUE);
              break;

            case 'WS-BROWSE-300':
              drupal_set_message(t("You don't have access to browse any dataset from this service"), 'warning', TRUE);
              break;

            case 'WS-SEARCH-300':
              drupal_set_message(t("You don't have access to search any dataset from this service"), 'warning', TRUE);
              break;

            case 'WS-CRUD-READ-300':
              drupal_set_message(t('This resource does not exist'), 'warning', TRUE);
              break;

            case "WS-DATASET-READ-304":
            case "WS-DATASET-UPDATE-202":
              drupal_set_message(t("This dataset does not exist in the system"), 'warning', TRUE);
              break;

            default:
              drupal_set_message(
                t("An error occured in the system, please contact the site administrator if the system hasn't been able to fullfill your request. Error number to report: @id", array('@id' => strip_tags($qe->id))), 'warning', TRUE);
              break;
          }
          break;
      }
    }
  }

  function alterParams(\StructuredDynamics\osf\php\api\framework\WebServiceQuery $query, &$params) {
    drupal_alter('osf_params', $params, $query);
  }
}
