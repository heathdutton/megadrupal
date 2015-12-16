<?php

/**
 * @file
 * Miscellaneous Web Service functions for GlobalLink.
 */

require_once TPT_ROOT . '/' . ('gl_ws/client/SessionService2.php');
require_once TPT_ROOT . '/' . ('gl_ws/client/ProjectService2.php');
require_once TPT_ROOT . '/' . ('gl_ws/client/DocumentService2.php');
require_once TPT_ROOT . '/' . ('gl_ws/client/SubmissionService2.php');

global $base_url;

$proxy_server = variable_get('globallink_proxy_url');

if (!empty($proxy_server)) {
  define('GL_WSDL_PATH', $proxy_server . '/' . drupal_get_path('module', 'globallink') . '/gl_ws/wsdl/');
}
else {
  define('GL_WSDL_PATH', $base_url . '/' . drupal_get_path('module', 'globallink') . '/gl_ws/wsdl/');
}

function globallink_get_user_pd_projects($pd_obj) {
  $username = $pd_obj->username;
  $password = $pd_obj->password;
  $url = $pd_obj->url;

  $session_service = new SessionService2(GL_WSDL_PATH . 'SessionService2.wsdl', array(
    'location' => $url . '/services/SessionService2',
  ));

  $project_service = new ProjectService2(GL_WSDL_PATH . 'ProjectService2.wsdl', array(
    'location' => $url . '/services/ProjectService2',
  ));

  $token = globallink_login($session_service, $username, $password);
  $projects = globallink_get_user_projects($project_service, $token);

  $proj_arr = array();

  foreach ($projects as $project) {
    $short_code = $project->projectInfo->shortCode;
    $proj_arr[$short_code] = $project->projectInfo->name;
  }

  return $proj_arr;
}

function globallink_test_pd_connectivity($pd_obj) {
  $username = $pd_obj->username;
  $password = $pd_obj->password;
  $url = $pd_obj->url;
  $project_short_code = $pd_obj->projectShortCode;

  $proj_arr = array();

  if ($project_short_code != '') {
    $arr = explode(',', $project_short_code);

    foreach ($arr as $value) {
      $proj_arr[$value] = $value;
    }
  }

  $session_service = new SessionService2(GL_WSDL_PATH . 'SessionService2.wsdl', array(
    'location' => $url . '/services/SessionService2',
  ));

  $project_service = new ProjectService2(GL_WSDL_PATH . 'ProjectService2.wsdl', array(
    'location' => $url . '/services/ProjectService2',
  ));

  $success = FALSE;

  $token = globallink_login($session_service, $username, $password);

  foreach ($proj_arr as $proj_code) {
    try {
      $project = globallink_find_project_by_short_code($project_service, $proj_code, $token);
    }
    catch (SoapFault $se) {
      watchdog('GlobalLink', 'SOAP Exception - %function - Code[%faultcode], Message[%faultstring]', array('%function' => __FUNCTION__, '%faultcode' => $se->faultcode, '%faultstring' => $se->faultstring), WATCHDOG_ERROR);

      return 'Connection Failed - Invalid Project Code: ' . $proj_code;
    }
    catch (Exception $e) {
      watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);

      return 'Connection Failed - Invalid Project Code: ' . $proj_code;
    }

    if (isset($project->ticket)) {
      if (isset($project->fileFormatProfiles) && is_array($project->fileFormatProfiles)) {
        foreach ($project->fileFormatProfiles as $file_format) {
          if ($pd_obj->classifier == $file_format->profileName) {
            $success = TRUE;

            break;
          }
        }
      }
      elseif (isset($project->fileFormatProfiles)) {
        if ($pd_obj->classifier == $project->fileFormatProfiles->profileName) {
          $success = TRUE;
        }
      }
    }

    if (!$success) {
      return 'Connection Failed - Invalid Classifier.';
    }
    else {
      $success = FALSE;
    }
  }

  return TRUE;
}

function globallink_cancel_pd_submission($pd_obj, &$globallink) {
  $username = $pd_obj->username;
  $password = $pd_obj->password;
  $url = $pd_obj->url;

  $session_service = new SessionService2(GL_WSDL_PATH . 'SessionService2.wsdl', array(
    'location' => $url . '/services/SessionService2',
  ));

  $submission_service = new SubmissionService2(GL_WSDL_PATH . 'SubmissionService2.wsdl', array(
    'location' => $url . '/services/SubmissionService2',
  ));

  $token = globallink_login($session_service, $username, $password);

  $cancel_submission_request = new cancelSubmission();

  $cancel_submission_request->userId = $token;
  $cancel_submission_request->submissionId = $globallink->submissionTicket;

  $cancel_submission_response = $submission_service->cancelSubmission($cancel_submission_request);
  $globallink->cancelled = TRUE;

  return $cancel_submission_response->return;
}

function globallink_cancel_select_documents($pd_obj, &$globallink_arr) {
  $username = $pd_obj->username;
  $password = $pd_obj->password;
  $url = $pd_obj->url;

  $session_service = new SessionService2(GL_WSDL_PATH . 'SessionService2.wsdl', array(
    'location' => $url . '/services/SessionService2',
  ));

  $target_service = new TargetService2(GL_WSDL_PATH . 'TargetService2.wsdl', array(
    'location' => $url . '/services/TargetService2',
  ));

  $token = globallink_login($session_service, $username, $password);

  foreach ($globallink_arr as $globallink) {
    $cancel_target_submission = new cancelTargetByDocumentId();

    $document_ticket = new DocumentTicket();
    $document_ticket->submissionTicket = $globallink->submissionTicket;
    $document_ticket->ticketId = $globallink->documentTicket;
    $cancel_target_submission->documentId = $document_ticket;
    $cancel_target_submission->targetLocale = $globallink->targetLocale;
    $cancel_target_submission->userId = $token;

    $target_service->cancelTargetByDocumentId($cancel_target_submission);
    $globallink->cancelled = TRUE;
  }
}

function globallink_get_user_projects($project_service, $token) {
  $get_user_projects = new getUserProjects();

  $get_user_projects->isSubProjectIncluded = FALSE;
  $get_user_projects->userId = $token;

  $get_user_projects_response = $project_service->getUserProjects($get_user_projects);
  $projects = $get_user_projects_response->return;

  if (!is_array($projects)) {
    $arr = array($projects);

    return $arr;
  }

  return $projects;
}

function globallink_find_project_by_short_code($project_service, $project_short_code, $token) {
  $find_project_by_short_code = new findProjectByShortCode();

  $find_project_by_short_code->projectShortCode = $project_short_code;
  $find_project_by_short_code->userId = $token;

  $find_project_by_short_code_response = $project_service->findProjectByShortCode($find_project_by_short_code);

  $project = $find_project_by_short_code_response->return;

  return $project;
}

function globallink_login($session_service, $username, $password) {
  $login = new login();

  $login->username = $username;
  $login->password = $password;

  $login_response = $session_service->login($login);

  $token = $login_response->return;

  return $token;
}

function globallink_get_status($pd_obj, $submission_ticket) {
  $username = $pd_obj->username;
  $password = $pd_obj->password;
  $url = $pd_obj->url;

  $session_service = new SessionService2(GL_WSDL_PATH . 'SessionService2.wsdl', array(
    'location' => $url . '/services/SessionService2',
  ));

  $submission_service = new SubmissionService2(GL_WSDL_PATH . 'SubmissionService2.wsdl', array(
    'location' => $url . '/services/SubmissionService2',
  ));

  $token = globallink_login($session_service, $username, $password);

  $ticket_request = new findByTicket();
  $ticket_request->userId = $token;
  $ticket_request->ticket = $submission_ticket;

  $find_by_ticket_response = $submission_service->findByTicket($ticket_request);

  $result = $find_by_ticket_response->return;
  $status = '';

  if (isset($result->status)) {
    $status = $result->status->name;
  }

  return $status;
}
