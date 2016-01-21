<?php

/**
 * @file
 * File contains Web Service functions for retreiving translations.
 */

require_once TPT_ROOT . '/' . ('gl_ws/gl_ws_common.inc');
require_once TPT_ROOT . '/' . ('gl_ws/client/SessionService2.php');
require_once TPT_ROOT . '/' . ('gl_ws/client/ProjectService2.php');
require_once TPT_ROOT . '/' . ('gl_ws/client/TargetService2.php');

function globallink_get_ready_translations_details_from_pd($pd_obj, $type = 'node') {
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

  $target_service = new TargetService2(GL_WSDL_PATH . 'TargetService2.wsdl', array(
    'location' => $url . '/services/TargetService2',
  ));

  $globallink_arr = array();

  $token = globallink_login($session_service, $username, $password);
  $projects = globallink_get_user_projects($project_service, $token);

  $proj_ticket_arr = array();

  foreach ($projects as $project) {
    $short_code = $project->projectInfo->shortCode;

    foreach ($proj_arr as $conf_proj) {
      if ($conf_proj == $short_code) {
        $proj_ticket_arr[] = $project->ticket;
      }
    }
  }

  if (count($proj_ticket_arr) > 0) {
    $targets = globallink_get_completed_targets_by_projects($target_service, $proj_ticket_arr, $token, $pd_obj);

    $count = 1;

    if (is_array($targets)) {
      foreach ($targets as $target) {
        if (!is_null($target->ticket) && $target->ticket != '') {
          $source_file = $target->document->documentInfo->name;
          $m_arr = $target->document->documentInfo->metadata;

          $source_type = '';
          $type_id = '';

          if (is_array($m_arr)) {
            foreach ($m_arr as $m) {
              if ($m->key == 'type') {
                $source_type = $m->value;
              }
              else {
                $type_id = $m->value;
              }
            }
          }
          else {
            $source_type = $target->document->documentInfo->metadata->value;
          }

          switch ($type) {
            case 'entity':
              if ($source_type != 'entity') {
                continue;
              }

              break;
            case 'menu':
              if ($source_type != 'menu') {
                continue;
              }

              break;
            case 'block':
              if ($source_type != 'block') {
                continue;
              }

              break;
            case 'taxonomy':
              if ($source_type != 'taxonomy') {
                continue;
              }

              break;
            case 'interface':
              if ($source_type != 'interface') {
                continue;
              }

              break;
            case 'webform':
              if ($source_type != 'webform') {
                continue;
              }

              break;
            case 'fieldable_panels':
              if ($source_type != 'fieldable_panels_panes') {
                continue;
              }

              break;
            default:
              if ($source_type == 'entity' || $source_type == 'menu' || $source_type == 'block' || $source_type == 'taxonomy' || $source_type == 'interface' || $source_type == 'webform' || $source_type == 'fieldable_panels_panes') {
                continue;
              }
          }

          $globallink = new GlobalLink();
          $globallink->submissionTicket = $target->document->documentGroup->submission->ticket;
          $globallink->submissionName = $target->document->documentGroup->submission->submissionInfo->name;
          $globallink->documentTicket = $target->document->ticket;
          $globallink->sourceFileName = $source_file;
          $globallink->targetTicket = $target->ticket;
          $globallink->sourceLocale = str_replace('-', '_', $target->sourceLanguage->locale);
          $globallink->targetLocale = str_replace('-', '_', $target->targetLanguage->locale);
          $globallink->type = $source_type;

          if ($source_type != 'node' && $source_type != 'entity') {
            $globallink->otherObjectId = $type_id;
          }
          else {
            $globallink->nid = $type_id;
          }

          $globallink_arr[$count] = $globallink;

          $count++;
        }
      }
    }
    elseif (!is_null($targets)) {
      $target = $targets;

      if (!is_null($target->ticket) && $target->ticket != '') {
        $source_file = $target->document->documentInfo->name;
        $m_arr = $target->document->documentInfo->metadata;

        $source_type = '';
        $type_id = '';

        if (is_array($m_arr)) {
          foreach ($m_arr as $m) {
            if ($m->key == 'type') {
              $source_type = $m->value;
            }
            else {
              $type_id = $m->value;
            }
          }
        }
        else {
          $source_type = $target->document->documentInfo->metadata->value;
        }

        switch ($type) {
          case 'entity':
            if ($source_type != 'entity') {
              return $globallink_arr;
            }

            break;
          case 'menu':
            if ($source_type != 'menu') {
              return $globallink_arr;
            }

            break;
          case 'block':
            if ($source_type != 'block') {
              return $globallink_arr;
            }

            break;
          case 'taxonomy':
            if ($source_type != 'taxonomy') {
              return $globallink_arr;
            }

            break;
          case 'interface':
            if ($source_type != 'interface') {
              return $globallink_arr;
            }

            break;
          case 'webform':
            if ($source_type != 'webform') {
              return $globallink_arr;
            }

            break;
          case 'fieldable_panels':
            if ($source_type != 'fieldable_panels_panes') {
              return $globallink_arr;
            }

            break;
          default:
            if ($source_type == 'entity' || $source_type == 'menu' || $source_type == 'block' || $source_type == 'taxonomy' || $source_type == 'interface' || $source_type == 'webform' || $source_type == 'fieldable_panels_panes') {
              return $globallink_arr;
            }
        }

        $globallink = new GlobalLink();
        $globallink->submissionTicket = $target->document->documentGroup->submission->ticket;
        $globallink->submissionName = $target->document->documentGroup->submission->submissionInfo->name;
        $globallink->documentTicket = $target->document->ticket;
        $globallink->sourceFileName = $source_file;
        $globallink->targetTicket = $target->ticket;
        $globallink->sourceLocale = str_replace('-', '_', $target->sourceLanguage->locale);
        $globallink->targetLocale = str_replace('-', '_', $target->targetLanguage->locale);
        $globallink->type = $source_type;

        if ($source_type != 'node' && $source_type != 'entity') {
          $globallink->otherObjectId = $type_id;
        }
        else {
          $globallink->nid = $type_id;
        }

        $globallink_arr[$count] = $globallink;
      }
    }
  }

  return $globallink_arr;
}

function globallink_get_all_ready_translations_details_from_pd($pd_obj) {
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

  $target_service = new TargetService2(GL_WSDL_PATH . 'TargetService2.wsdl', array(
    'location' => $url . '/services/TargetService2',
  ));

  $globallink_arr = array();

  $token = globallink_login($session_service, $username, $password);
  $projects = globallink_get_user_projects($project_service, $token);

  $proj_ticket_arr = array();

  foreach ($projects as $project) {
    $short_code = $project->projectInfo->shortCode;

    foreach ($proj_arr as $conf_proj) {
      if ($conf_proj == $short_code) {
        $proj_ticket_arr[] = $project->ticket;
      }
    }
  }

  if (count($proj_ticket_arr) > 0) {
    $targets = globallink_get_completed_targets_by_projects($target_service, $proj_ticket_arr, $token, $pd_obj);

    $count = 1;

    if (is_array($targets)) {
      foreach ($targets as $target) {
        if (!is_null($target->ticket) && $target->ticket != '') {
          $source_file = $target->document->documentInfo->name;
          $m_arr = $target->document->documentInfo->metadata;

          $source_type = '';
          $type_id = '';

          if (is_array($m_arr)) {
            foreach ($m_arr as $m) {
              if ($m->key == 'type') {
                $source_type = $m->value;
              }
              else {
                $type_id = $m->value;
              }
            }
          }
          else {
            $source_type = $target->document->documentInfo->metadata->value;
          }

          $globallink = new GlobalLink();
          $globallink->submissionTicket = $target->document->documentGroup->submission->ticket;
          $globallink->submissionName = $target->document->documentGroup->submission->submissionInfo->name;
          $globallink->documentTicket = $target->document->ticket;
          $globallink->sourceFileName = $source_file;
          $globallink->targetTicket = $target->ticket;
          $globallink->sourceLocale = str_replace('-', '_', $target->sourceLanguage->locale);
          $globallink->targetLocale = str_replace('-', '_', $target->targetLanguage->locale);
          $globallink->type = $source_type;

          if ($source_type != 'node') {
            $globallink->otherObjectId = $type_id;
          }
          else {
            $globallink->nid = $type_id;
          }

          $globallink_arr[$count] = $globallink;

          $count++;
        }
      }
    }
    elseif (!is_null($targets)) {
      $target = $targets;

      if (!is_null($target->ticket) && $target->ticket != '') {
        $source_file = $target->document->documentInfo->name;
        $m_arr = $target->document->documentInfo->metadata;

        $source_type = '';
        $type_id = '';

        if (is_array($m_arr)) {
          foreach ($m_arr as $m) {
            if ($m->key == 'type') {
              $source_type = $m->value;
            }
            else {
              $type_id = $m->value;
            }
          }
        }
        else {
          $source_type = $target->document->documentInfo->metadata->value;
        }

        $globallink = new GlobalLink();
        $globallink->submissionTicket = $target->document->documentGroup->submission->ticket;
        $globallink->submissionName = $target->document->documentGroup->submission->submissionInfo->name;
        $globallink->documentTicket = $target->document->ticket;
        $globallink->sourceFileName = $source_file;
        $globallink->targetTicket = $target->ticket;
        $globallink->sourceLocale = str_replace('-', '_', $target->sourceLanguage->locale);
        $globallink->targetLocale = str_replace('-', '_', $target->targetLanguage->locale);
        $globallink->type = $source_type;

        if ($source_type != 'node') {
          $globallink->otherObjectId = $type_id;
        }
        else {
          $globallink->nid = $type_id;
        }

        $globallink_arr[$count] = $globallink;
      }
    }
  }

  return $globallink_arr;
}

function globallink_get_completed_targets_by_projects($target_service, $project_tickets, $token, $pd_obj) {
  $get_completed_targets_by_projects_request = new getCompletedTargetsByProjects();

  $get_completed_targets_by_projects_request->projectTickets = $project_tickets;
  $get_completed_targets_by_projects_request->maxResults = $pd_obj->maxTargetCount;
  $get_completed_targets_by_projects_request->userId = $token;

  $get_completed_targets_by_projects_response = $target_service->getCompletedTargetsByProjects($get_completed_targets_by_projects_request);

  return $get_completed_targets_by_projects_response->return;
}

function globallink_get_completed_targets_by_doc($pd_obj, $ticket) {
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

  $get_completed_targets_by_documents_request = new getCompletedTargetsByDocuments();

  $get_completed_targets_by_documents_request->documentTickets = array($ticket);
  $get_completed_targets_by_documents_request->maxResults = 99;
  $get_completed_targets_by_documents_request->userId = $token;

  $get_completed_targets_by_documents_response = $target_service->getCompletedTargetsByDocuments($get_completed_targets_by_documents_request);

  return $get_completed_targets_by_documents_response->return;
}

function globallink_download_target_resource($pd_obj, $target_ticket) {
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
  $globallink_download_target_resource_request = new downloadTargetResource();

  $globallink_download_target_resource_request->targetId = $target_ticket;
  $globallink_download_target_resource_request->userId = $token;

  $globallink_download_target_resource_response = $target_service->downloadTargetResource($globallink_download_target_resource_request);
  $repository_item = $globallink_download_target_resource_response->return;
  $target_content = $repository_item->data->_;

  return $target_content;
}

function globallink_send_download_confirmation($target_ticket, $pd_obj) {
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
  $globallink_send_download_confirmation_request = new sendDownloadConfirmation();

  $globallink_send_download_confirmation_request->targetId = $target_ticket;
  $globallink_send_download_confirmation_request->userId = $token;

  $globallink_send_download_confirmation_response = $target_service->sendDownloadConfirmation($globallink_send_download_confirmation_request);

  return $globallink_send_download_confirmation_response->return;
}
