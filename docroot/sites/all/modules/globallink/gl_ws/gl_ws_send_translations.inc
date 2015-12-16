<?php

/**
 * @file
 * File contains Web Service functions for exporting translations.
 */

require_once TPT_ROOT . '/' . ('gl_ws/gl_ws_common.inc');
require_once TPT_ROOT . '/' . ('gl_ws/client/SessionService2.php');
require_once TPT_ROOT . '/' . ('gl_ws/client/ProjectService2.php');
require_once TPT_ROOT . '/' . ('gl_ws/client/DocumentService2.php');
require_once TPT_ROOT . '/' . ('gl_ws/client/SubmissionService2.php');

function globallink_send_documents_for_translation_to_pd(&$globallink_arr, $pd_obj, $project_code, $submitter) {
  $username = $pd_obj->username;
  $password = $pd_obj->password;
  $url = $pd_obj->url;
  $project_short_code = $project_code;

  $session_service = new SessionService2(GL_WSDL_PATH . 'SessionService2.wsdl', array(
    'location' => $url . '/services/SessionService2',
  ));

  $project_service = new ProjectService2(GL_WSDL_PATH . 'ProjectService2.wsdl', array(
    'location' => $url . '/services/ProjectService2',
  ));

  $document_service = new DocumentService2(GL_WSDL_PATH . 'DocumentService2.wsdl', array(
    'location' => $url . '/services/DocumentService2',
  ));

  $submission_service = new SubmissionService2(GL_WSDL_PATH . 'SubmissionService2.wsdl', array(
    'location' => $url . '/services/SubmissionService2',
  ));

  $GLOBALS['g_submissionTicket'] = '';
  $GLOBALS['g_document_count'] = 0;

  $client_identifier = $project_short_code . REQUEST_TIME;

  $token = globallink_login($session_service, $username, $password);
  $project = globallink_find_project_by_short_code($project_service, $project_short_code, $token);
  $submission_info = globallink_create_submission_info($project->ticket, $client_identifier, $submitter);

  $count = 0;

  foreach ($globallink_arr as $globallink) {
    $count++;

    $submission_info->name = $globallink->submissionName;

    $date_requested = new Date();
    $date_requested->date = $globallink->dueDate;
    $date_requested->critical = FALSE;

    $submission_info->dateRequested = $date_requested;
    $resource_info = globallink_create_resource_info($client_identifier, $pd_obj, $globallink);
    $document_info = globallink_create_document_info($project->ticket, $globallink, $client_identifier);

    if (!is_null($GLOBALS['g_submissionTicket']) && $GLOBALS['g_submissionTicket'] != '') {
      $document_info->submissionTicket = $GLOBALS['g_submissionTicket'];
    }

    globallink_submit_document_with_binary_resource($document_service, $resource_info, $document_info, $globallink, $token);

    $GLOBALS['g_submissionTicket'] = $globallink->submissionTicket;
    $GLOBALS['g_document_count'] = $count;
  }

  globallink_start_submission($submission_service, $submission_info, $token);
}

function globallink_submit_document_with_binary_resource($document_service, $resource_info, $document_info, &$globallink, $token) {
  $submit_document_with_binary_resource_request = new submitDocumentWithBinaryResource;

  $submit_document_with_binary_resource_request->documentInfo = $document_info;
  $submit_document_with_binary_resource_request->resourceInfo = $resource_info;
  $submit_document_with_binary_resource_request->data = $globallink->sourceXML;
  $submit_document_with_binary_resource_request->userId = $token;

  $submit_document_with_binary_resource_response = $document_service->submitDocumentWithBinaryResource($submit_document_with_binary_resource_request);

  $globallink->submissionTicket = $submit_document_with_binary_resource_response->return->submissionTicket;
  $globallink->documentTicket = $submit_document_with_binary_resource_response->return->ticketId;
}

function globallink_start_submission($submission_service, $submission_info, $token) {
  $start_submission_request = new startSubmission;

  $start_submission_request->submissionId = $GLOBALS['g_submissionTicket'];
  $start_submission_request->submissionInfo = $submission_info;
  $start_submission_request->userId = $token;

  $start_submission_response = $submission_service->startSubmission($start_submission_request);

  return $start_submission_response->return;
}

function globallink_create_submission_info($project_ticket, $client_identifier, $submitter) {
  $metadata = new Metadata();
  $metadata->key = 'drupalUser';
  $metadata->value = $submitter;

  $submission_info = new SubmissionInfo();
  $submission_info->clientIdentifier = $client_identifier;
  $submission_info->projectTicket = $project_ticket;
  $submission_info->metadata = $metadata;

  return $submission_info;
}

function globallink_create_document_info($project_ticket, &$globallink, $client_identifier) {
  $md_arr = array();

  $metadata = new Metadata();
  $metadata->key = 'type';
  $metadata->value = $globallink->metadata;
  array_push($md_arr, $metadata);

  $metadata = new Metadata();
  $metadata->key = 'type_id';

  if ($globallink->metadata != 'node' && $globallink->metadata != 'entity') {
    $metadata->value = $globallink->otherObjectId;
  }
  else {
    $metadata->value = $globallink->nid;
  }

  array_push($md_arr, $metadata);

  $document_info = new DocumentInfo();
  $document_info->metadata = $md_arr;
  $document_info->projectTicket = $project_ticket;
  $document_info->sourceLocale = $globallink->sourceLocale;
  $document_info->name = $globallink->sourceFileName;
  $document_info->clientIdentifier = $client_identifier;
  $date_requested = new Date();
  $date_requested->date = $globallink->dueDate;
  $date_requested->critical = FALSE;
  $document_info->dateRequested = $date_requested;
  $document_info->instructions = $globallink->submissionInstructions;

  $target_infos = array();

  $count = 0;

  foreach ($globallink->targetLocale as $value) {
    $target_infos[$count] = globallink_create_target_info($value, $globallink);

    $count++;
  }

  $document_info->targetInfos = $target_infos;

  return $document_info;
}

function globallink_create_target_info($locale, &$globallink) {
  $target_info = new TargetInfo();
  $target_info->targetLocale = $locale;

  $date_requested = new Date();
  $date_requested->date = $globallink->dueDate;
  $date_requested->critical = FALSE;

  $target_info->dateRequested = $date_requested;
  $target_info->requestedDueDate = $globallink->dueDate;
  
  $priority = new Priority();
  $priority->value = 1;
  $target_info->priority = $priority;
  $target_info->encoding = 'UTF-8';

  return $target_info;
}

function globallink_create_resource_info($client_identifier, &$pd_obj, $globallink) {
  $resource_type = new ResourceType();
  $resource_type->value = 0;

  $resource_info = new ResourceInfo();
  $resource_info->type = $resource_type;
  $resource_info->mimeType = 'text/xml';
  $resource_info->classifier = $pd_obj->classifier;
  $resource_info->name = $globallink->sourceFileName;
  $resource_info->encoding = 'UTF-8';
  $resource_info->size = strlen($globallink->sourceXML);
  $resource_info->clientIdentifier = $client_identifier;
  $resource_info->resourceInfoId = $GLOBALS['g_document_count'];

  return $resource_info;
}
