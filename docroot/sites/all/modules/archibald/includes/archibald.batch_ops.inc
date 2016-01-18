<?php

  /**
   * @file
   * Batch operations content and submission functions
   */
  define('BATCH_OPS_DEBUG', FALSE);
  define('BATCH_OPS_DRYRUN', FALSE);

  /**
   * Checks the lom object for the given action prerequisites.
   *
   * @param ArchibaldLom $lom
   *   The lom object.
   * @param string $action
   *   The action which needs to be checked.
   *
   * @return array
   *   Returns an array including keys: error,msg error is a boolean value which determines if
   *   the check passes (error = FALSE) or not (error = TRUE) and msg holds the error message if there
   *   is an error.
   */
  function archibald_batch_ops_check_prerequisites($action) {
    $msg = array();

    switch ($action) {
      case 'status2draft':
      case 'status2final':
        if (!user_access('change archibald content status')) {
          array_push($msg, t("You are not allowed to change the status of a description"));
        }
        break;
      case 'publish_local':
      case 'unpublish_local':
        if (!user_access('publish archibald content local')) {
          array_push($msg, t("You are not allowed to publish/unpublish locally"));
        }
        break;
      case 'publish_national':
      case 'publish_both':
      case 'unpublish_national':
      case 'unpublish_both':
        if (variable_get('archibald_upload_error', 0) == 1) {
          array_push($msg, t('Publication process has been aborted due to fatal error, please check publication log.'));
        }
        // Checks if at least one content partner is allowed to publish to the national catalogue
        if (!archibald_can_publish_central()) {
          array_push($msg, t('You are not allowed to publish/unpublish nationally.'));
        }

        if ($action == 'publish_both' && !user_access('publish archibald content local')) {
          array_push($msg, t("You are not allowed to publish/unpublish locally"));
        }
        if (!user_access('publish archibald content central')) {
          array_push($msg, t("You are not allowed to publish/unpublish nationally"));
        }
        break;
      case 'delete':
        if (!user_access('delete archibald content')) {
          array_push($msg, t("You are not allowed to delete a description"));
        }
        break;
    }
    return (array(
      'error' => (count($msg)) ? TRUE : FALSE,
      'msg' => (count($msg)) ? $msg : 'OK',
    ));
  }

  /**
   * Checks the lom object for the given action.
   *
   * @param ArchibaldLom $lom
   *   The lom object.
   * @param string $action
   *   The action which needs to be checked.
   *
   * @return array
   *   Returns an array including keys: error,msg error is a boolean value which determines if
   *   the check passes (error = FALSE) or not (error = TRUE) and msg holds the error message if there
   *   is an error.
   */
  function archibald_batch_ops_check_action($lom, $action) {
    $archibald_lom_validate = new ArchibaldLomValidate($lom);
    $status = archibald_get_lom_stats($lom->getLomId());
    $msg = array();

    $publish_type = variable_get('archibald_publish_type', ARCHIBALD_PUBLISH_SEPERATE);

    switch ($action) {
      case 'publish_national':
      case 'publish_both':
      case 'unpublish_national':
      case 'unpublish_both':
        if (variable_get('archibald_upload_error', 0) == 1) {
          array_push($msg, t('Publication process has been aborted due to fatal error, please check publication log.'));
        }
      break;
    }

    switch ($action) {
      case 'publish_local':
      case 'publish_national':
      case 'publish_both':
        if (!$archibald_lom_validate->readyForPublish()) {
          foreach ($archibald_lom_validate->getErrors() as $key => $error_msg) {
            array_push($msg, $error_msg);
          }
          // array_push($msg, t('The status cannot be changed because the description is incomplete.'));
        }

        if ($lom->getLifeCycle()->getStatus() != ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_FINALE) {
          array_push($msg, t('Status of this description must be final before publishing.'));
        }
        break;
    }

    switch ($action) {
      case 'publish_national':
        if ($status->publication_local_status == 'none') {
          array_push($msg, t('This description is not published locally.'));
        }
        break;
      case 'unpublish_local':
        if ($status->publication_central_status != 'none') {
          array_push($msg, t('This description is published nationally.'));
        }
        break;
    }

    switch ($action) {
      case 'publish_national':
      case 'publish_both':
        if (empty($status->content_partner) || !$status->content_partner['key']) {
          array_push($msg, t('The assigned content partner has no rights to publish to the national catalogue.'));
        }
        break;

      case 'status2final':
        if (!$archibald_lom_validate->readyForPublish()) {
          foreach ($archibald_lom_validate->getErrors() as $key => $error_msg) {
            array_push($msg, $error_msg);
          }
          // array_push($msg, t('The status cannot be changed because the description is incomplete.'));
        }
        if ($lom->getLifeCycle()->getStatus() == ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_FINALE) {
          array_push($msg, t('This description already has the status final.'));
        }
        break;
      case 'status2draft':
        if ($lom->getLifeCycle()->getStatus() == ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_DRAFT) {
          array_push($msg, t('This description already has the status draft.'));
        }
        break;
    }

    switch ($action) {
      case 'publish_local':
      case 'publish_national':
      case 'publish_both':
        if ($action != 'publish_national') {
          switch ($status->publication_local_status) {
            case 'none':
              break;
            case 'old':
              break;
            case 'ok':
              if ($publish_type == ARCHIBALD_PUBLISH_SEPERATE) {
                array_push($msg, t('This description is already published locally'));
              }
              break;
            default:
              array_push($msg, t(
                'Error: unknown state of local publication "@status"', array(
                  '@status' => $status->publication_local_status,
                )
              ));
              break;
          }
        }
        if ($action != 'publish_local') {
          switch ($status->publication_central_status) {
            case 'none':
            case 'old':
              break;
            case 'ok':
              if ($publish_type == ARCHIBALD_PUBLISH_SEPERATE) {
                array_push($msg, t('This description is already published nationally'));
              }
              break;
            case 'error':
              array_push($msg, t('Previous publication into the national catalogue failed.'));
              break;
            default:
              if (in_array($status->publication_central_status, array('processing', 'waiting'))) {
                array_push($msg, t('Publication to the national catalogue in progress.'));
              }
              else {
                array_push($msg, t(
                  'Error: unknown state of national publication "@status"', array(
                    '@status' => $status->publication_central_status,
                  )
                ));
              }
              break;
          }
        }
        break;
      case 'unpublish_local':
      case 'unpublish_national':
      case 'unpublish_both':
        if ($action != 'unpublish_national') {
          switch ($status->publication_local_status) {
            case 'none':
              if ($publish_type == ARCHIBALD_PUBLISH_SEPERATE) {
                array_push($msg, t('This description is not published locally'));
              }
              break;
            case 'old':
            case 'ok':
              break;
            default:
              array_push($msg, t(
                'Error: unknown state of local publication "@status"', array(
                  '@status' => $status->publication_local_status,
                )
              ));
              break;
          }
        }
        if ($action != 'unpublish_local') {
          switch ($status->publication_central_status) {
            case 'none':
              if ($publish_type == ARCHIBALD_PUBLISH_SEPERATE) {
                array_push($msg, t('This description is not published nationally'));
              }
              break;
            case 'old':
            case 'ok':
              break;
            case 'error':
              array_push($msg, t('Previous publication into the national catalogue failed.'));
              break;
            default:
              if (in_array($status->publication_central_status, array('processing', 'waiting'))) {
                array_push($msg, t('Publication to the national catalogue in progress.'));
              }
              else {
                array_push($msg, t(
                  'Error: unknown state of national publication "@status"', array(
                    '@status' => $status->publication_central_status,
                  )
                ));
              }
              break;
          }
        }
        break;
    }

    $error = FALSE;
    if (count($msg)) {
      $error = TRUE;
    }
    return array(
      'error' => $error,
      'msg' => $msg,
    );
  }

  /**
   * Ajax handler, will process the given batch_op action from an ajax request.
   */
  function archibald_admin_batch_ops_ajax() {
    $ret = array(
      'ids' => array(),
    );
    $ret['success'] = FALSE;
    $action = $_POST['batch_op'];
    $validate = archibald_batch_ops_check_prerequisites($action);
    if (!$validate['error']) {
      $valid = 0;
      $batch_ids = explode(',', $_POST['batch_op_ids']);
      foreach ($batch_ids as $value) {
        $val = explode('|', $value);
        $lom_id = $val[0];
        $lom = archibald_load($lom_id);
        $validate = archibald_batch_ops_check_action($lom, $action);
        if (!$validate['error']) {
          $valid++;
        }
        $ret['ids'][$value] = $validate;
      }
      if ($valid == count($batch_ids)) {
        $ret['success'] = TRUE;
      }
    }
    else {
      $ret['main_error'] = '<br />' . implode('<br />', $validate['msg']);
    }

    print drupal_json_encode($ret);
    exit();
  }

  /**
   * This is a page callback function displaying a table with all descriptions which need to be republished.
   * The only indicator for the republication is the 'republication required' flag in the archibald_lom_stats table.
   */
  function archibald_admin_batch_ops() {
    $pass = FALSE;

    if (isset($_POST['op'])) {
      switch ($_POST['op']) {
        case 'cancel':
          drupal_goto('admin/archibald');
          exit();
        case 'status':
          archibald_admin_batch_ops_status();
          exit();
        case 'cron':
          archibald_admin_run_batch_cron();
          exit();
        case 'confirm':
          archibald_admin_batch_ops_form_submit();
          exit();
        default:
          $pass = TRUE;
          break;
      }
    }
    else {
      $pass = TRUE;
    }

    if ($pass) {
      $path = drupal_get_path('module', 'archibald');
      drupal_add_css($path . '/css/archibald.css');
      drupal_add_js($path . '/js/archibald.js');
      drupal_add_js(array(
        'archibald_batch_ajax' => url('admin/archibald/batch_ops/ajax'),
      ), 'setting');
      drupal_add_js($path . '/js/archibald_batch_ops.js');
      return theme('archibald_admin_batch_ops', array(
        'form' => 'archibald_admin_batch_ops_form',
      ));
    }
  }

  /**
   * Run the publish cron and return the status.
   *
   * @return boolean
   *   Returns TRUE if the cron could be run, else FALSE.
   */
  function archibald_admin_run_batch_cron() {
    $archibald_lom_publish = new ArchibaldLomPublish();
    return $archibald_lom_publish->cron($_POST['batch_op_key'], $_POST['batch_op_time']);
  }

  /**
   * Finishs the batch op, will reset things.
   */
  function archibald_admin_finish_batch_ops() {
    db_update('archibald_lom_stats')
      ->fields(array(
        'batch_op_key' => '',
        'batch_op_time' => 0,
      ))
      ->condition('batch_op_key', $_POST['batch_op_key'])
      ->execute();
  }
  /**
   * Implement hook_form.
   *
   * Showing two buttons to confirm or cancel the republication.
   *
   * @param array $form
   *   The form array.
   * @param array $form_state
   *   The form state
   * @return array
   *   drupal form API
   */
  function archibald_admin_batch_ops_form($form, &$form_state) {

    $form['batch_op_ids'] = array(
      '#type' => 'hidden',
      '#value' => $_POST['batch_op_ids'],
    );

    $form['batch_op'] = array(
      '#type' => 'hidden',
      '#value' => $_POST['batch_op'],
    );

    $form['save'] = array(
      '#type' => 'submit',
      '#value' => t('Confirm'),
      '#attributes' => array(
        'id' => "archibald_batch_operation_submit",
      ),
    );

    $form['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#attributes' => array(
        'onclick' => "window.location.href = '" . $_POST['redirect'] . "';",
      ),
    );

    $form['#attributes']['onsubmit'] = "return false;";

    $redirect = 'admin/archibald';
    switch ($_POST['batch_op']) {
      case 'unpublish_local':
      case 'status2final':
      case 'unpublish_both':
        if (user_access('publish archibald content local') || user_access('publish archibald content central')){
          $redirect .= '/to_publish';
        }
        break;
      case 'unpublish_national':
        $redirect .= '/to_publish/central';
        break;

      case 'publish_both':
      case 'publish_local':
        $redirect .= '/published';
        break;
      case 'publish_national':
        $redirect .= '/published/central';
        break;
    }

    drupal_add_js(array(
      'archibald_batch_submit_url' => url('admin/archibald/batch_ops'),
      'archibald_batch_success_redirect' => url($redirect),
    ), 'setting');

    return $form;
  }


  /**
   * Handles the republication form submit. In case user clicks 'cancel' go back to the main description list.
   * In case user clicks 'republish all descriptions' call archibald_reset_republication_required() and go to
   * page showing the republication process of all descriptions.
   */
  function archibald_admin_batch_ops_form_submit() {

    $ret = array(
      'valid_ids' => array(),
      'invalid_ids' => array(),
      'batch_op_key' => '',
      'batch_op_time' => 0,
      'cron' => 0,
    );

    $action = $_POST['batch_op'];

    $pre_check = archibald_batch_ops_check_prerequisites($action);
    if (!$pre_check['error']) {

      if ($action == 'status2draft' || $action == 'status2final') {
        $draft_value = FALSE;
        $final_value = FALSE;
        foreach (archibald_get_life_cycle_status_options() as $k => $v) {
          if  ($k == ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_DRAFT) {
            $draft_value = $k;
          }
          elseif ($k == ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_FINALE) {
            $final_value = $k;
          }
          if ($draft_value && $final_value) {
            break;
          }
        }
      }
      else if ($action == 'publish_both' || $action == 'publish_national') {
        $ret['batch_op_key'] = drupal_hash_base64(drupal_random_bytes(55));
        $ret['batch_op_time'] = time();
      }


      foreach (explode(',', $_POST['batch_op_ids']) AS $lom_id_tmp) {
        $val = explode('|', $lom_id_tmp);
        $lom_id = $val[0];
        $lom = archibald_load($lom_id);
        $validate = archibald_batch_ops_check_action($lom, $action);

        if ($validate['error']) {
          $ret['invalid'][] = array(
            'lom_id' => $lom_id,
            'msg' => $validate['msg'],
          );
          continue;
        }

        switch ($action) {
          case 'status2draft':
            $status = $lom->lifeCycle->getStatus();
            if (is_object($status)) {
              $status = $status->value;
            }
            archibald_set_status($lom, $status, $draft_value, variable_get('archibald_send_mails_status2draft', FALSE));
            break;
          case 'status2final':
            $status = $lom->lifeCycle->getStatus();
            if (is_object($status)) {
              $status = $status->value;
            }
            archibald_set_status($lom, $status, $final_value, variable_get('archibald_send_mails_status2final', FALSE));
            break;
          case 'publish_local':
          case 'publish_both':
          case 'publish_national':
            if ($action != 'publish_national') {
              // Publish locally.
              ArchibaldLomSaveHandler::setLocalPublished($lom->getLomId(), $lom->getVersion());
            }

            if ($action != 'publish_local') {
              // Publish nationally.
              $archibald_lom_publish = new ArchibaldLomPublish();
              db_update('archibald_lom_stats')
                ->condition('lom_id', $lom->getLomId())
                ->fields(
                  array(
                    'publication_version' => $lom->getVersion(),
                    'publication_progress' => NULL,
                    'publication_log' => NULL,
                    'batch_op_key' => $ret['batch_op_key'],
                    'batch_op_time' => $ret['batch_op_time'],
                  )
                )->execute();
              $ret['cron'] = TRUE;

              archibald_send_mail($lom->getLomId(), 'proposal2publish');
            }
            break;

          case 'unpublish_local':
          case 'unpublish_both':
          case 'unpublish_national':
            if ($action != 'unpublish_national') {
              // Unpublish locally.
              ArchibaldLomSaveHandler::setLocalPublished($lom->getLomId(), '');
            }

            if ($action != 'unpublish_local') {
              $archibald_lom_publish = new ArchibaldLomPublish();
              // Unpublish nationally.
              if ($archibald_lom_publish->lomApiUnpublish($lom) == TRUE) {
                db_update('archibald_lom_stats')
                  ->condition('lom_id', $lom->getLomId())
                  ->fields(
                    array(
                      'publication_version' => NULL,
                      'republication_required' => 0,
                    )
                  )->execute();
              }
            }
            break;
        }

        // We can safely add the lom id to the valid ones however on (un-)publishing nationally
        // We just ignore this array.
        $ret['valid_ids'][] = $lom_id;
      }
    }

    print drupal_json_encode($ret);
    exit();
  }

  /**
   * Handles the republication form submit. In case user clicks 'cancel' go back to the main description list.
   * In case user clicks 'republish all descriptions' call archibald_reset_republication_required() and go to
   * page showing the republication process of all descriptions.
   */
  function archibald_admin_batch_ops_form_submit_backup() {

    if (BATCH_OPS_DEBUG) {
      $html = '';
    }

    $action = $_POST['batch_op'];

    $cron = FALSE;

    $pre_check = archibald_batch_ops_check_prerequisites($action);
    if (!$pre_check['error']) {

      $batch_op_key = $batch_op_time = FALSE;

      switch ($action) {
        case 'status2draft':
        case 'status2final':

          $options = archibald_get_life_cycle_status_options();

          $draft_value = FALSE;
          $final_value = FALSE;
          foreach ($options as $k => $v) {
            if ($k == ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_DRAFT) {
              $draft_value = $k;
            }
            elseif ($k == ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_FINALE) {
              $final_value = $k;
            }
            if ($draft_value && $final_value) {
              break;
            }
          }
          break;
        case 'publish_both':
        case 'publish_national':
          $batch_op_key = drupal_hash_base64(drupal_random_bytes(55));
          $batch_op_time = time();
          $cron = TRUE;
          break;
      }

      $batch_ids = explode(',', $_POST['batch_op_ids']);
      $valid = $invalid = array();
      foreach ($batch_ids as $value) {
        $val = explode('|', $value);
        $lom_id = $val[0];
        $lom = archibald_load($lom_id);
        $validate = archibald_batch_ops_check_action($lom, $action);

        if (!$validate['error']) {
          $valid[] = $lom_id;

          switch ($action) {
            case 'status2draft':
              $old_status = $lom->lifeCycle->getStatus()->value;
              $new_status = $draft_value;
              $send_info_mail = TRUE;
              if (!BATCH_OPS_DRYRUN) {
                archibald_set_status($lom, $old_status, $new_status, $send_info_mail);
              }
              if (BATCH_OPS_DEBUG) {
                $html .= "archibald_set_status( \$lom, $old_status, $new_status, $send_info_mail )<br />";
              }
              break;
            case 'status2final':
              $old_status = $lom->lifeCycle->getStatus()->value;
              $new_status = $final_value;
              $send_info_mail = TRUE;
              if (!BATCH_OPS_DRYRUN) {
                archibald_set_status($lom, $old_status, $new_status, $send_info_mail);
              }
              if (BATCH_OPS_DEBUG) {
                $html .= "archibald_set_status( \$lom, $old_status, $new_status, $send_info_mail )<br />";
              }
              break;
            case 'publish_local':
            case 'publish_both':
            case 'publish_national':
              if ($action != 'publish_national') {
                // Publish locally.
                if (!BATCH_OPS_DRYRUN) {
                  ArchibaldLomSaveHandler::setLocalPublished($lom->getLomId(), $lom->getVersion());
                }
                if (BATCH_OPS_DEBUG) {
                  $html .= 'Locally published. <br />';
                }
              }

              if ($action != 'publish_local') {
                // Publish nationally.
                if (!BATCH_OPS_DRYRUN) {
                  $archibald_lom_publish = new ArchibaldLomPublish();
                  db_update('archibald_lom_stats')
                    ->condition('lom_id', $lom->getLomId())
                    ->fields(
                      array(
                        'publication_version' => $lom->getVersion(),
                        'publication_progress' => NULL,
                        'publication_log' => NULL,
                        'batch_op_key' => $batch_op_key,
                        'batch_op_time' => $batch_op_time,
                      )
                    )->execute();
                }
              }
              break;

            case 'unpublish_local':
            case 'unpublish_both':
            case 'unpublish_national':
              if ($action != 'unpublish_national') {
                // Unpublish locally.
                if (!BATCH_OPS_DRYRUN) {
                  ArchibaldLomSaveHandler::setLocalPublished($lom->getLomId(), '');
                }
                if (BATCH_OPS_DEBUG) {
                  $html .= 'Locally unpublished. <br />';
                }
              }

              if ($action != 'unpublish_local') {
                if (!BATCH_OPS_DRYRUN) {
                  $archibald_lom_publish = new ArchibaldLomPublish();

                  // Unpublish nationally.
                  if ($archibald_lom_publish->lomApiUnpublish($lom) == TRUE) {
                    db_update('archibald_lom_stats')
                      ->condition('lom_id', $lom->getLomId())
                      ->fields(
                        array(
                          'publication_version' => NULL,
                          'republication_required' => 0,
                        )
                      )->execute();
                  }
                }
                elseif (BATCH_OPS_DEBUG) {
                  $html .= t('Fatal error while unpublishing from the national catalogue, please check the publication log.') . '<br />';
                }
              }
              break;
          }
        }
        else {
          $invalid[] = array(
            'lom_id' => $lom_id,
            'msg' => $validate['msg'],
          );
        }
      }

      if (BATCH_OPS_DEBUG) {
        $html .= print_r($validate, 1);
      }

      $ret = array(
        'valid_ids' => $valid,
        'invalid_ids' => $invalid,
        'batch_op_key' => $batch_op_key,
        'batch_op_time' => $batch_op_time,
        'cron' => $cron,
      );

      if (count($valid) && $cron) {
        if (!BATCH_OPS_DRYRUN) {
          $archibald_lom_publish->cron($batch_op_key, $batch_op_time);
        }
        if (BATCH_OPS_DEBUG) {
          $html .= 'National publishing/unpublishing cron queued. <br />';
        }
      }
    }
    elseif (BATCH_OPS_DEBUG) {
      $html .= print_r($pre_check['msg'], 1);
    }

    $redirect = 'admin/archibald';

    switch ($action) {
      case 'unpublish_local':
      case 'status2final':
      case 'unpublish_both':
        $redirect .= '/to_publish';
        break;
      case 'unpublish_national':
        $redirect .= '/to_publish/central';
        break;

      case 'publish_both':
      case 'publish_local':
        $redirect .= '/published';
        break;
      case 'publish_national':
        $redirect .= '/published/central';
        break;
    }



    switch ($action) {
      case 'publish_both':
      case 'publish_national':
        if (archibald_is_proposal($lom)) {
          archibald_send_mail($lom->getLomId(), 'proposal2publish');
        }
        break;
    }

    if (!BATCH_OPS_DRYRUN) {
      db_update('archibald_lom_stats')
        ->fields(array(
          'batch_op_key' => '',
          'batch_op_time' => 0,
        ))
        ->condition('lom_id', $lom_id)
        ->execute();
    }
    $ret['redirect'] = url($redirect);
    $ret['dry_run'] = BATCH_OPS_DRYRUN;

    if (BATCH_OPS_DEBUG) {
      echo $html;
      die();
    }

    print drupal_json_encode($ret);
    exit();
  }

  /**
   * Theme the table containting all descriptions which need to be republished.
   *
   * @param array $variables
   *   All variables needed to build the theme.
   *
   * @return string
   *   The rendered HTML.
   */
  function theme_archibald_admin_batch_ops($variables) {
    if (!isset($_POST['batch_op'])) {
      drupal_goto('');
    }
    $action = $_POST['batch_op'];
    $batch_ids = explode(',', $_POST['batch_op_ids']);
    $final_batch_ids = array();
    $msgs = $table_rows = $error_table_rows = array();

    if (BATCH_OPS_DEBUG) {
      $debug = array();
    }

    $batch_op_options = array();
    $batch_op_options['status2draft'] = t('Status to draft');
    $batch_op_options['status2final'] = t('Status to final');
    $batch_op_options['publish_local'] =  t('Publish locally');
    $batch_op_options['publish_national'] =  t('Publish nationally');
    $batch_op_options['publish_both'] =  t('Publish locally and nationally');
    $batch_op_options['unpublish_local'] =  t('Unpublish locally');
    $batch_op_options['unpublish_national'] =  t('Unpublish nationally');
    $batch_op_options['unpublish_both'] =  (variable_get('archibald_publish_type', ARCHIBALD_PUBLISH_SEPERATE) == ARCHIBALD_PUBLISH_SEPERATE) ? t('Unpublish locally and nationally') : t('Unpublish');

    if (!isset($batch_op_options[$action])) {
      $batch_op_options[$action] = t('Unknown action');
    }

    $pre_check = archibald_batch_ops_check_prerequisites($action);

    if (!$pre_check['error']) {
      $html = '<div id="batch_op_header">';
      $html .= '<h2>' . t('Please review the pending changes before confirming.') . '</h2>';
      $html .= t("When you click on 'Confirm' at the bottom of this page, the following action will be taken on the items listed below:") . ' <strong>' . $batch_op_options[$action] . '</strong></div>';

      $html .= '<div id="batch_op_notification">';
      $html .= '<h2 class="hide in_progress">' . t('Operation in progress') . '</h2>';
      $html .= '<h2 class="hide done">' . t('Finished') . '</h2>';
      $html .= '<span class="hide wait">' . t("Please wait") . '...</span>';
      $html .= '<span class="hide complete">' . t("All operations completed") . '</span>';
      $html .= '<span class="hide errors error">' . t("Some operations could not be carried out. Please review the reasons below.") . '</span>';
      $html .= '<h2 class="hide fatalerror error">' . t('Publication process has been aborted due to fatal error, please check publication log.') . '</h2>';
      $html .= '<div class="hide batch_op_redirect"><a href="' . archibald_batch_op_get_redirect($action) . '">' . t("Back to the list") . '</a></div>';
      $html .= '<div class="hide bo_working">' . t("Working") . '</div>';
      $html .= '<div class="hide bo_done">' . t("Done") . '</div>';
      $html .= '</div>';

      $html .= '<br />';

      foreach ($batch_ids as $value) {
        $values = explode('|', $value);
        $lom_id = $values[0];
        $version = $values[1];
        $lom = archibald_load($lom_id);

        if (BATCH_OPS_DEBUG) {
          $debug[] = ArchibaldLom::determinTitle($lom) . ' (' . $lom->lifeCycle->getStatus()->value . ') [' . archibald_lom_is_published($lom->getLomId(), $version) . ']';
        }

        $msg = archibald_batch_ops_check_action($lom, $action);
        $msgs[] = $msg;

        if (!$msg['error']) {
          array_push($final_batch_ids, $value);
          $table_rows[] = array(
            ArchibaldLom::determinTitle($lom),
            '<div class="batch_op_status" id="' . $lom_id . '">' . t('Pending') . '</div>',
          );
        }
        else {
          $error_table_rows[] = array(
            ArchibaldLom::determinTitle($lom),
            '<div id="' . $lom_id . '">' . '<ul><li>' . implode('</li><li>', $msg['msg']) . '</li></ul></div>',
          );
        }
      }

      $_POST['batch_op_ids'] = (count($final_batch_ids)) ? implode(',', $final_batch_ids) : '';

      if (BATCH_OPS_DEBUG) {
        $html .= '<pre>';
        $html .= 'ACTION => ' . print_r($_POST['batch_op'], 1) . "\n\n";
        $html .= print_r($debug, 1);
        $html .= print_r($msgs, 1);
        $html .= '</pre>';
      }

      if (count($table_rows)) {
        $table_header = array(
          array('data' => t('Description'), 'field' => 'title'),
          array('data' => t('Progress'), 'field' => 'progress'),
        );

        $html .= theme('table', array(
          'header' => $table_header,
          'rows' => $table_rows,
          'id' => 'table-duplicates',
          'class' => 'duplicates',
        ));

        $vf = $variables['form'];
        $vf = drupal_get_form($vf);
        $html .= drupal_render($vf);
        if (count($error_table_rows)) {
          $html .= '<br /><hr /><br />';
        }
      }

      if (count($error_table_rows)) {
        $html .= '<h2>' . t('INVALID DESCRIPTIONS') . '</h2>';
        $html .= t("The action you have choosen cannot be carried out on the following items") . '</strong><br /><br />';

        $error_table_header = array(
          array('data' => 'Description', 'field' => 'title'),
          array('data' => 'Reason', 'field' => 'error'),
        );

        $html .= theme('table', array(
          'header' => $error_table_header,
          'rows' => $error_table_rows,
          'id' => 'table-duplicates',
          'class' => 'duplicates',
        ));
      }
    }
    else {
      $html = '<h2>' . t('ERROR') . '</h2>';
      $html .= '<ul><li>' . implode('</li><li>', $pre_check['msg']) . '</li></ul>';
    }

    return $html;
  }

  /**
   * Returns the redirect url for the given $action.
   *
   * @param string $action
   *   The action.
   *
   * @return string
   *   The redirect url.
   */
  function archibald_batch_op_get_redirect($action) {
    $redirect = 'admin/archibald';

    switch ($action) {
      case 'unpublish_local':
      case 'status2final':
      case 'unpublish_both':
        if (user_access('publish archibald content local') || user_access('publish archibald content central')){
          $redirect .= '/to_publish';
        }
        break;
      case 'unpublish_national':
        $redirect .= '/to_publish/central';
        break;

      case 'publish_both':
      case 'publish_local':
        $redirect .= '/published';
        break;
      case 'publish_national':
        $redirect .= '/published/central';
        break;
    }

    return url($redirect);
  }
  /**
   * Update the lom status table log.
   *
   * Prints direct json encoded string, only used for ajax requests.
   *
   * @param string $key
   *   The batch op key
   * @param int $time
   *   The batch op time
   */
  function archibald_admin_batch_ops_status() {
    archibald_admin_run_batch_cron();

    $where = array();
    $args = array();
    $c = 0;
    foreach (explode(",", $_POST['batch_op_lom_ids']) AS $tmp_val) {
      list($lom_id, $version) = explode("|", $tmp_val);

      $where[] = 'ls.lom_id = :lom_id_' . $c;
      $args[':lom_id_' . $c] = $lom_id;
      $c++;
    }

    $ret = array(
      'results' => array(),
      'empty' => FALSE,
      'fatal' => FALSE,
    );

    if (!empty($where)) {
      $sql = "
        SELECT ls.lom_id, ls.publication_version, ls.publication_progress, ls.publication_log
        FROM {archibald_lom_stats} AS ls
        WHERE " . implode(" OR " , $where);
      $results = db_query($sql, $args)->fetchAll();

      $all_ok = TRUE;
      $all_error = TRUE;
      foreach ($results as $result) {
        $log = unserialize($result->publication_log);

        if (!empty($log)) {
          if ($log->lom_status == 'success' || $log->lom_status == 'error') {
            db_update('archibald_lom_stats')
              ->fields(array(
                'batch_op_key' => '',
                'batch_op_time' => 0,
              ))
              ->condition('lom_id', $result->lom_id)
              ->execute();
          }

          if (!empty($log) && $log->lom_status == 'success') {
            $ret['results'][$result->lom_id] = 'done';
            $all_error = FALSE;
          }
          else {
            $ret['results'][$result->lom_id] = 'error';
            $all_ok = FALSE;
          }
        }
        elseif ($result->publication_log == NULL) {
          $ret['results'][$result->lom_id] = 'waiting';
        }
      }

      if ($all_ok) {
        archibald_admin_finish_batch_ops();
      }
      else if (variable_get('archibald_upload_error', 0) == 1) {
        $ret['fatal'] = TRUE;
      }

      $ret['finished'] = $all_ok;
      $ret['all_errors'] = $all_error;
    }
    print drupal_json_encode($ret);
  }
