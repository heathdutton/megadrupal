<?php

/**
 * Builds forms on GlobalLink receive dashboard.
 *
 * @param string $type
 *   The type of task we are working on.
 *
 * @return array
 *   Array of forms for the GlobalLink receive dashboard.
 */
function globallink_dashboard_receive_page($type) {
  $_SESSION['transpefect_dashboard_receive_type'] = $type;

  $array = array();

  $array[] = drupal_get_form('globallink_node_receive_filter_form');
  $array[] = drupal_get_form('globallink_node_receive_pager_form');
  $array[] = drupal_get_form('globallink_dashboard_receive');

  return $array;
}

/**
 * Builds form to filter GlobalLink nodes to receive on dashboard.
 */
function globallink_node_receive_filter_form() {
  module_load_include('inc', 'globallink', 'globallink');
  module_load_include('inc', 'globallink', 'globallink_node');

  $form = array();

  $submission_options = globallink_get_distinct_active_submission_names();
  unset($submission_options['']);

  $submission_options['[any]'] = 'Any';
  $default_submission = '[any]';

  if (!empty($_SESSION['globallink_node_receive_filter_submission'])) {
    if (!in_array($_SESSION['globallink_node_receive_filter_submission'], $submission_options)) {
      $_SESSION['globallink_node_receive_filter_submission'] = '';
      $_SESSION['globallink_globalLink_arr'] = array();

      unset($_SESSION['globallink_globalLink_arr_last_refreshed']);
    }
  }

  $filter = FALSE;

  if (!empty($_SESSION['globallink_node_receive_filter_submission'])) {
    $default_submission = $_SESSION['globallink_node_receive_filter_submission'];
    $filter = TRUE;
  }

  $n_arr = array('[any]' => t('Any'));
  $t_arr = globallink_get_mapped_locales_with_drupal_desc(FALSE);
  $languages = $n_arr + $t_arr;

  $status_arr = array('[any]' => t('Any'),
    'Translation Completed' => 'Translation Completed',
    'Error' => 'Error',
    'Source Deleted' => 'Source Deleted',
  );

  $default = '[any]';

  if (!empty($_SESSION['globallink_node_receive_filter_language'])) {
    $default = $_SESSION['globallink_node_receive_filter_language'];
    $filter = TRUE;
  }

  $default_status = '[any]';
  if (!empty($_SESSION['globallink_node_receive_filter_status'])) {
    $default_status = $_SESSION['globallink_node_receive_filter_status'];
    $filter = TRUE;
  }

  $form['node_receive_filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('FILTER RECORDS'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['node_receive_filter']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['node_receive_filter']['filters'][] = array('#markup' => '<table>');

  $form['node_receive_filter']['filters']['language'] = array(
    '#type' => 'select', '#title' => t('Target Language'),
    '#options' => $languages,
    '#default_value' => $default,
    '#field_suffix' => '&nbsp;&nbsp;&nbsp;&nbsp;',
    '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
    '#suffix' => '</tr>',
    '#field_suffix' => '</td>',
    '#field_prefix' => '</td><td>',
  );

  $form['node_receive_filter']['filters']['status'] = array(
    '#type' => 'select', '#title' => t('Status'),
    '#options' => $status_arr,
    '#default_value' => $default_status,
    '#field_suffix' => '&nbsp;&nbsp;&nbsp;&nbsp;',
    '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
    '#suffix' => '</tr>',
    '#field_suffix' => '</td>',
    '#field_prefix' => '</td><td>',
  );

  $form['node_receive_filter']['filters']['submission'] = array(
    '#type' => 'select', '#title' => t('Submission'),
    '#options' => $submission_options,
    '#default_value' => $default_submission,
    '#field_suffix' => '&nbsp;&nbsp;&nbsp;&nbsp;',
    '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
    '#suffix' => '</tr>',
    '#field_suffix' => '</td>',
    '#field_prefix' => '</td><td>',
  );

  $form['node_receive_filter']['filters'][] = array('#markup' => '</table>');

  $disable_submit = $filter ? TRUE : FALSE;

  $form['node_receive_filter']['filters']['filter'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#disabled' => $disable_submit,
  );

  if ($filter) {
    $form['filter-search'] = array(
      '#type' => 'fieldset',
      '#title' => t('FILTER CRITERIA'),
    );

    $form['filter-search']['filter-markup'] = array(
      '#type' => 'container',
    );

    $form['filter-search']['filter-markup'][] = array('#markup' => '<table>');

    $form['filter-search']['filter-markup'][] = array(
      '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;"><b>',
      '#markup' => t('Target Language'),
      '#suffix' => '</b>:&nbsp;</td><td style="text-align: left;width: 85%;vertical-align: middle;border: none;">' . $languages[$default] . '</td></tr>',
    );

    $form['filter-search']['filter-markup'][] = array(
      '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;"><b>',
      '#markup' => t('Status'),
      '#suffix' => '</b>:&nbsp;</td><td style="text-align: left;width: 85%;vertical-align: middle;border: none;">' . $status_arr[$default_status] . '</td></tr>',
    );

    $form['filter-search']['filter-markup'][] = array(
      '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;"><b>',
      '#markup' => t('Submission'),
      '#suffix' => '</b>:&nbsp;</td><td style="text-align: left;width: 85%;vertical-align: middle;border: none;">' . $submission_options[$default_submission] . '</td></tr>',
    );

    $form['filter-search']['filter-markup'][] = array('#markup' => '</table>');

    $form['filter-search']['status']['actions'] = array(
      '#type' => 'actions',
      '#attributes' => array(
        'class' => array('container-inline'),
      ),
    );

    $form['filter-search']['status']['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
    );
  }

  return $form;
}

/**
 * Handles submission of filter form.
 */
function globallink_node_receive_filter_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  switch ($op) {
    case t('Filter'):
      $_SESSION['globallink_node_receive_filter_language'] = $form_state['values']['language'];
      $_SESSION['globallink_node_receive_filter_status'] = $form_state['values']['status'];
      $_SESSION['globallink_node_receive_filter_submission'] = $form_state['values']['submission'];
      $_SESSION['globallink_globalLink_arr'] = array();

      unset($_SESSION['globallink_globalLink_arr_last_refreshed']);

      break;
    case t('Reset'):
      $_SESSION['globallink_node_receive_filter_language'] = '';
      $_SESSION['globallink_node_receive_filter_status'] = '';
      $_SESSION['globallink_node_receive_filter_submission'] = '';
      $_SESSION['globallink_globalLink_arr'] = array();

      unset($_SESSION['globallink_globalLink_arr_last_refreshed']);

      break;
  }
}

/**
 * Builds form to add pagination to GlobalLink receive dashboard.
 */
function globallink_node_receive_pager_form() {
  $form = array();

  $form['page_counter']['markup'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('tpt-center')),
  );

  $form['page_counter']['markup']['p_count'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => isset($_SESSION['globallink_receive_page_count']) ? $_SESSION['globallink_receive_page_count'][0] : TPT_PAGER_LIMIT,
    '#field_prefix' => t('Show') . '&nbsp;&nbsp;',
    '#field_suffix' => '&nbsp;&nbsp;' . t('records') . '&nbsp;&nbsp;&nbsp;&nbsp;',
    '#prefix' => '<div class="container-inline">',
  );

  $form['page_counter']['markup']['action'] = array(
    '#type' => 'submit',
    '#value' => t('Go'),
    '#suffix' => '</div>',
    '#limit_validation_errors' => array(),
    '#submit' => array('globallink_node_receive_pager_form_submit'),
    '#access' => TRUE,
  );

  return $form;
}

/**
 * Handles submission of pager form.
 */
function globallink_node_receive_pager_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];

  if ($op != t('Go')) {
    return;
  }

  $page_count = $form_state['input']['p_count'];

  if (!is_numeric($page_count) || $page_count < 1) {
    form_set_error('', t('Invalid Page Count.'));
  }
  else {
    $_SESSION['globallink_receive_page_count'] = array($page_count);

    return;
  }
}

/**
 * Builds form to receive a GlobalLink submission.
 */
function globallink_dashboard_receive() {
  module_load_include('inc', 'globallink', 'globallink_settings');
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_receive_translations');

  $form = array();

  if (!empty($_SESSION['transpefect_dashboard_receive_type'])) {
    $type = $_SESSION['transpefect_dashboard_receive_type'];

    $globallink_arr = array();

    if (empty($_SESSION['globallink_globalLink_arr'])) {
      try {
        $pd4 = globallink_get_project_director_details();

        if (globallink_validate_project_director_details($pd4)) {
          $_SESSION['globallink_globalLink_arr_last_refreshed'] = format_date(REQUEST_TIME, 'custom', 'D M j, Y G:i:s');

          $globallink_arr = globallink_get_ready_translations_details_from_pd($pd4, $type);

          if (sizeof($globallink_arr) > 0) {
            $_SESSION['globallink_globalLink_arr'] = serialize($globallink_arr);
          }
        }
      }
      catch (SoapFault $se) {
        watchdog('GlobalLink', 'SOAP Exception - %function - Code[%faultcode], Message[%faultstring]', array('%function' => __FUNCTION__, '%faultcode' => $se->faultcode, '%faultstring' => $se->faultstring), WATCHDOG_ERROR);
        form_set_error('', t('Web Services Error: @faultcode - @faultstring', array('@faultcode' => $se->faultcode, '@faultstring' => $se->faultstring)));
      }
      catch (Exception $e) {
        watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
        form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));
      }
    }
    else {
      $globallink_arr = unserialize($_SESSION['globallink_globalLink_arr']);
    }

    $header = array(
      'submissionName' => t('Submission'),
      'sourceLocale' => t('Source Language'),
      'targetLocale' => t('Target Language'),
      'title' => t('Title'),
      'status' => t('Status'),
    );

    if (variable_get('globallink_enable_preview', 1)) {
      $header['preview'] = '';
    }

    $rows = array();

    $count = 1;

    $filter_lang = '[any]';

    if (!empty($_SESSION['globallink_node_receive_filter_language'])) {
      $filter_lang = $_SESSION['globallink_node_receive_filter_language'];
    }

    if (isset($globallink_arr) && is_array($globallink_arr)) {
      foreach ($globallink_arr as $key => &$globallink) {
        if ($filter_lang != '[any]' && $globallink->targetLocale != $filter_lang) {
          unset($globallink_arr[$key]);
        }
      }
    }

    if (isset($globallink_arr) && is_array($globallink_arr)) {
      $filter_status = '[any]';

      if (!empty($_SESSION['globallink_node_receive_filter_status'])) {
        $filter_status = $_SESSION['globallink_node_receive_filter_status'];
      }

      $rid_arr = array();

      foreach ($globallink_arr as $key => &$globallink) {
        if ($globallink->type != 'node') {
          continue;
        }

        $source_xml = '';

        $status = 'Translation Completed';

        $title = globallink_get_node_title($globallink);

        if (!$title) {
          $title = $globallink->title;
          $status = 'Source Deleted';
          $source_xml = 'Source Deleted';
          $globallink->status = 'Source Deleted';
          $globallink->sourceDeleted = TRUE;
        }

        if ($globallink->status == 'Error') {
          $status = 'Error';
        }

        if ($filter_status != '[any]' && $status != $filter_status) {
          unset($globallink_arr[$key]);

          continue;
        }

        $filter_submission = '[any]';

        if (!empty($_SESSION['globallink_node_receive_filter_submission'])) {
          $filter_submission = $_SESSION['globallink_node_receive_filter_submission'];
        }

        if ($filter_submission != '[any]' && $globallink->submissionName != $filter_submission) {
          unset($globallink_arr[$key]);

          continue;
        }

        $tpt_row_id = globallink_get_row_id_from_submission($globallink->submissionTicket, $globallink->documentTicket, $globallink->targetLocale);

        if ($tpt_row_id) {
          $rid_arr['tpt_content_div_' . $key] = $tpt_row_id;
        }

        $source_name = globallink_get_drupal_locale_name($globallink->sourceLocale);
        $target_name = globallink_get_drupal_locale_name($globallink->targetLocale);

        $rows[$key] = array(
          'submissionName' => $globallink->submissionName,
          'sourceLocale' => $source_name,
          'targetLocale' => $target_name,
          'title' => $title,
          'status' => $status,
        );

        if (variable_get('globallink_enable_preview', 1)) {
          $rows[$key]['preview'] = $source_xml != 'Source Deleted' ? l(t('Preview Translations'), 'javascript:void(0)', array(
            'fragment' => '',
            'external' => TRUE,
            'attributes' => array(
              'title' => 'Click to Preview',
              'id' => 'tpt_content_link_' . $key,
            ),
          )) : '';
        }

        $count++;
      }

      if (sizeof($globallink_arr) > 0) {
        $_SESSION['globallink_globalLink_arr'] = serialize($globallink_arr);
      }
    }

    $page_count = isset($_SESSION['globallink_receive_page_count']) ? $_SESSION['globallink_receive_page_count'][0] : TPT_PAGER_LIMIT;

    $form['table'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => globallink_pager_array_splice($rows, $page_count),
      '#empty' => t('No items available'),
    );

    $form['pager'] = array('#markup' => theme('pager'));

    if (variable_get('globallink_enable_preview', 1)) {
      $lcount = 1;

      $div_arr = array();

      foreach ($rid_arr as $key => $rid) {
        if (isset($rid)) {
          $lcount = str_replace('tpt_content_div_', '', $key);

          $form['tpt_content_div_' . $lcount] = array(
            '#type' => 'markup',
            '#markup' => '',
            '#prefix' => '<div id="tpt_content_div_' . $lcount . '" style="display: none;">',
            '#suffix' => '</div>',
          );

          $div_arr['tpt_content_link_' . $lcount] = 'tpt_content_div_' . $lcount;
        }
      }

      drupal_add_js(array(
        'globallink' => array(
          'popup' => $div_arr,
          'previewpath' => base_path() . 'admin/globallink-translations/preview',
          'rids' => $rid_arr,
        ),
      ), 'setting');
    }

    if ($count > 1) {
      $form['submit_update_selected'] = array(
        '#type' => 'submit',
        '#value' => t('Update Selected'),
      );

      $form['submit_update_all'] = array(
        '#type' => 'submit',
        '#value' => t('Update All'),
      );

      $form['submit_clear_deleted_hidden'] = array(
        '#type' => 'submit',
        '#value' => t('Clear Deleted Records'),
        '#attributes' => array(
          'style' => 'display:none;',
          'disabled' => 'true',
        ),
      );

      $form['submit_clear_deleted'] = array(
        '#type' => 'submit',
        '#value' => t('Clear Deleted Records'),
      );
    }

    $last_refreshed = '';

    if (isset($_SESSION['globallink_globalLink_arr_last_refreshed'])) {
      $last_refreshed = $_SESSION['globallink_globalLink_arr_last_refreshed'];
    }

    $form['submit_refresh_hidden'] = array(
      '#type' => 'submit',
      '#value' => t('Refresh Page'),
      '#prefix' => '<BR/>',
      '#attributes' => array(
        'style' => 'display:none;',
        'disabled' => 'true',
      ),
    );

    $form['submit_refresh'] = array(
      '#type' => 'submit',
      '#value' => t('Refresh Page'),
    );

    $form['markup'] = array(
      '#type' => 'markup',
      '#markup' => 'Last refreshed : ' . $last_refreshed,
    );
  }

  drupal_add_js('jQuery(document).ready(function(){jQuery("#edit-submit-refresh").click(function(){jQuery("#edit-submit-refresh").hide();jQuery("#edit-submit-refresh-hidden").css("display","inline");});});', 'inline');
  drupal_add_js('jQuery(document).ready(function(){jQuery("#edit-submit-clear-deleted").click(function(){jQuery("#edit-submit-clear-deleted").hide();jQuery("#edit-submit-clear-deleted-hidden").css("display","inline");});});', 'inline');
  drupal_add_library('system', 'ui.dialog');

  $module_path = drupal_get_path('module', 'globallink');
  drupal_add_css($module_path . '/css/globallink.css');
  drupal_add_js($module_path . '/js/globallink_receive.js');

  return $form;
}

/**
 * Validates GlobalLink form input.
 */
function globallink_dashboard_receive_validate($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  $pd4 = globallink_get_project_director_details();
  globallink_validate_project_director_details($pd4);

  if ($op == t('Update Selected')) {
    $rids = array_filter($form_state['values']['table']);

    if (count($rids) == 0) {
      form_set_error('', t('No items selected.'));
    }
  }
}

/**
 * Handles GlobalLink form submission.
 */
function globallink_dashboard_receive_submit($form, &$form_state) {
  $type = $_SESSION['transpefect_dashboard_receive_type'];
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  $pd4 = globallink_get_project_director_details();

  switch ($op) {
    case t('Update All'):
      try {
        $globallink_arr = unserialize($_SESSION['globallink_globalLink_arr']);
        $count = globallink_get_translated_content($pd4, $globallink_arr);

        $error = FALSE;
        $delete_records_selected = FALSE;

        foreach ($globallink_arr as $updated) {
          if ($updated->sourceDeleted == TRUE) {
            $delete_records_selected = TRUE;
          }

          if ($updated->status == 'Error') {
            globallink_update_status($updated);
            $error = TRUE;
          }
        }

        $status = 'status';

        if ($error) {
          drupal_set_message(t('There was some error while updating the records. Please check logs for more details.'), 'warning', NULL);
          $status = 'warning';
        }

        drupal_set_message(check_plain($count . ' record(s) updated successfully.'), $status, NULL);

        if ($delete_records_selected) {
          drupal_set_message(t('Source Deleted record(s) cannot be updated.'), 'warning', NULL);
        }
      }
      catch (Exception $e) {
        watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
        form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));
      }

      $_SESSION['globallink_globalLink_arr'] = array();
      $_SESSION['globallink_selected_submission'] = array();

      unset($_SESSION['globallink_globalLink_arr_last_refreshed']);

      break;
    case t('Clear Deleted Records'):
      try {
        $globallink_arr = unserialize($_SESSION['globallink_globalLink_arr']);

        $success = FALSE;

        $count = 0;

        foreach ($globallink_arr as $globallink) {
          if ($globallink->sourceDeleted == TRUE) {
            $success = globallink_update_deleted_records($pd4, $globallink);

            $count++;
          }
        }

        if ($count == 0) {
          drupal_set_message(t('No source record(s) deleted.'), 'warning', NULL);
        }
        else {
          $_SESSION['globallink_globalLink_arr'] = array();
          $_SESSION['globallink_selected_submission'] = array();

          unset($_SESSION['globallink_globalLink_arr_last_refreshed']);
        }

        if ($success) {
          drupal_set_message(check_plain($count . ' Source Deleted record(s) cleared successfully.'), 'status', NULL);
        }
      }
      catch (Exception $e) {
        watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
        form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));
      }

      break;
    case t('Update Selected'):
      try {
        $delete_records_selected = FALSE;
        $globallink_arr = unserialize($_SESSION['globallink_globalLink_arr']);

        $updated_globallink_arr = array();
        $rids = array_filter($form_state['values']['table']);

        foreach ($rids as $rid) {
          array_push($updated_globallink_arr, $globallink_arr[$rid]);
        }

        $count = globallink_get_translated_content($pd4, $updated_globallink_arr);

        $error = FALSE;

        foreach ($updated_globallink_arr as $updated) {
          if ($updated->sourceDeleted == TRUE) {
            $delete_records_selected = TRUE;
          }

          if ($updated->status == 'Error') {
            $error = TRUE;

            globallink_update_status($updated);
          }
        }

        $status = 'status';

        if ($error) {
          drupal_set_message(t('There was some error while updating the records. Please check logs for more details.'), 'warning', NULL);
          $status = 'warning';
        }

        drupal_set_message(check_plain($count . ' record(s) updated successfully.'), $status, NULL);

        if ($delete_records_selected) {
          drupal_set_message(t('Source Deleted record(s) cannot be updated.'), 'warning', NULL);
        }
      }
      catch (Exception $e) {
        watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
        form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));
      }

      $_SESSION['globallink_globalLink_arr'] = array();
      $_SESSION['globallink_selected_submission'] = array();

      unset($_SESSION['globallink_globalLink_arr_last_refreshed']);

      break;
    case t('Refresh Page'):
      sleep(2);

      $pd4 = globallink_get_project_director_details();

      try {
        $globallink_arr = globallink_get_ready_translations_details_from_pd($pd4, $type);
      }
      catch (SoapFault $se) {
        watchdog('GlobalLink', 'SOAP Exception - %function - Code[%faultcode], Message[%faultstring]', array('%function' => __FUNCTION__, '%faultcode' => $se->faultcode, '%faultstring' => $se->faultstring), WATCHDOG_ERROR);
        form_set_error('', t('Web Services Error: @faultcode - @faultstring', array('@faultcode' => $se->faultcode, '@faultstring' => $se->faultstring)));
      }
      catch (Exception $e) {
        watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
        form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));
      }

      if (sizeof($globallink_arr) > 0) {
        $_SESSION['globallink_globalLink_arr'] = serialize($globallink_arr);
        $_SESSION['globallink_globalLink_arr_last_refreshed'] = format_date(REQUEST_TIME, 'custom', 'D M j, Y G:i:s');
      }
      else {
        $_SESSION['globallink_globalLink_arr'] = array();

        unset($_SESSION['globallink_globalLink_arr_last_refreshed']);
      }

      break;
  }
}

/**
 * Automatically receives translated contents.
 *
 * @param string $cron_type
 *   The type of cron job.
 */
function globallink_auto_receive($cron_type) {
  watchdog('GlobalLink', 'Cron Started by %cron_type', array('%cron_type' => $cron_type), WATCHDOG_INFO);

  try {
    $pd4 = globallink_get_project_director_details();
    $all_target_arr = globallink_get_all_ready_translations_details_from_pd($pd4);

    $count = 0;

    foreach ($all_target_arr as $globallink) {
      $type = $globallink->type;
      $globallink_arr = array($globallink);

      switch ($type) {
        case 'node':
          $count = globallink_get_translated_content($pd4, $globallink_arr);

          watchdog('GlobalLink', 'Imported Node', array(), WATCHDOG_INFO);

          if ($globallink->status == 'Error') {
            globallink_update_status($globallink);
          }

          $_SESSION['globallink_globalLink_arr'] = array();
          $_SESSION['globallink_selected_submission'] = array();

          break;
        case 'entity':
          $count = globallink_entity_get_translated_enties($pd4, $globallink_arr);

          watchdog('GlobalLink', 'Imported Entity', array(), WATCHDOG_INFO);

          if ($globallink->status == 'Error') {
            globallink_entity_update_status($globallink);
          }

          $_SESSION['globallink_entity_globalLink_arr'] = array();
          $_SESSION['globallink_entity_selected_submission'] = array();

          break;
        case 'block':
          $count += globallink_block_get_translated_blocks($pd4, $globallink_arr);

          watchdog('Block', t('Imported Block'), array(), WATCHDOG_INFO);

          $_SESSION['globallink_block_globalLink_arr'] = array();
          $_SESSION['globallink_block_selected_submission'] = array();

          break;
        case 'interface':
          $count += globallink_interface_get_translated($pd4, $globallink_arr);

          watchdog('GlobalLink', 'Imported Interface', array(), WATCHDOG_INFO);

          $_SESSION['globallink_interface_globalLink_arr'] = array();
          $_SESSION['globallink_interface_selected_submission'] = array();

          break;
        case 'menu':
          $count += globallink_menu_get_translated($pd4, $globallink_arr);

          watchdog('GlobalLink', 'Imported Menu', array(), WATCHDOG_INFO);

          $_SESSION['globallink_menu_globalLink_arr'] = array();
          $_SESSION['globallink_menu_selected_submission'] = array();

          break;
        case 'taxonomy':
          $count += globallink_taxonomy_get_translated($pd4, $globallink_arr);

          watchdog('GlobalLink', 'Imported Taxonomy', array(), WATCHDOG_INFO);

          $_SESSION['globallink_taxonomy_globalLink_arr'] = array();
          $_SESSION['globallink_taxonomy_selected_submission'] = array();

          break;
        case 'webform':
          $count += globallink_webform_get_translated($pd4, $globallink_arr);

          watchdog('GlobalLink', 'Imported Webform', array(), WATCHDOG_INFO);

          $_SESSION['globallink_webform_globalLink_arr'] = array();
          $_SESSION['globallink_webform_selected_submission'] = array();

          break;
        case 'fieldable_panels_panes':
          $count += globallink_fieldable_panels_get_translated($pd4, $globallink_arr);

          watchdog('GlobalLink', 'Imported Fieldable Panels Panes', array(), WATCHDOG_INFO);

          $_SESSION['globallink_fieldable_panels_panes_globalLink_arr'] = array();
          $_SESSION['globallink_fieldable_panels_panes_selected_submission'] = array();

          break;
      }
    }
  }
  catch (SoapFault $se) {
    watchdog('GlobalLink', 'SOAP Exception - %function - Code[%faultcode], Message[%faultstring]', array('%function' => __FUNCTION__, '%faultcode' => $se->faultcode, '%faultstring' => $se->faultstring), WATCHDOG_ERROR);
  }
  catch (Exception $e) {
    watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
  }

  watchdog('GlobalLink', 'Cron Completed - %count Records Updated.', array('%count' => $count), WATCHDOG_INFO);
}

/**
 * Previews translated GlobalLink content in the form of JSON.
 */
function globallink_preview_translated_content() {
  $row_id = $_POST['rid'];
  $arr = globallink_get_translations_for_row_id($row_id);

  drupal_json_output($arr);
}

/**
 * Gets GlobalLink translations by row ID.
 *
 * @param string $row_id
 *   The row ID.
 *
 * @return array
 *   Array of GlobalLink translations.
 */
function globallink_get_translations_for_row_id($row_id) {
  module_load_include('inc', 'globallink', 'globallink');
  module_load_include('inc', 'globallink', 'globallink_node');
  module_load_include('inc', 'globallink', 'globallink_settings');
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_receive_translations');

  $row = globallink_get_row($row_id);

  $globallink = new GlobalLink();
  $globallink->tptRowId = $row_id;
  $globallink->nid = $row->nid;
  $globallink->vid = $row->vid;
  $globallink->type = $row->type;
  $globallink->title = $row->title;
  $globallink->sourceLocale = $row->source;
  $globallink->targetLocale = $row->target;
  $globallink->documentTicket = $row->document_ticket;
  $globallink->submissionTicket = $row->submission_ticket;
  $globallink->submission = $row->submission;
  $globallink->status = $row->status;

  $source_arr = array();
  $translation = array();

  $error = FALSE;

  try {
    $pd4 = globallink_get_project_director_details();
    $targets = globallink_get_completed_targets_by_doc($pd4, $globallink->documentTicket);

    if (!is_array($targets)) {
      $targets = array($targets);
    }

    foreach ($targets as $target) {
      if (str_replace('-', '_', $target->targetLanguage->locale) != $globallink->targetLocale) {
        $source_xml = 'Source Deleted';
        $globallink->sourceDeleted = TRUE;

        continue;
      }

      if (!is_null($target->ticket) && $target->ticket != '') {
        $globallink->targetTicket = $target->ticket;
        $globallink->targetXML = globallink_download_target_resource($pd4, $globallink->targetTicket);

        if (!is_null($globallink->targetXML) && $globallink->targetXML != '') {
          $translation = globallink_get_translated_array($globallink->targetXML);

          $nid = $translation['nid'];
          $vid = $translation['vid'];

          $node = node_load($nid, $vid);

          if ($node && is_object($node) && !is_null($node)) {
            $drupal_locale_code = globallink_get_drupal_locale_code($globallink->sourceLocale);

            if ($node->tnid != 0 && $node->language != $drupal_locale_code) {
              $translations = translation_node_get_translations($node->tnid);

              if (isset($translations) && isset($translations[$drupal_locale_code])) {
                $src_tnode = $translations[$drupal_locale_code];
                $node = node_load($src_tnode->nid);
              }
            }

            $name = '';

            $source_xml = globallink_get_xml($node, $globallink->targetLocale, NULL, NULL, $name, TRUE);

            $source_arr = array();

            if ($source_xml != 'Source Deleted') {
              $source_arr = globallink_get_translated_array($source_xml);
            }
          }
          else {
            $source_xml = 'Source Deleted';
            $globallink->sourceDeleted = TRUE;
          }
        }
      }

      $name = '';

      $source_xml = globallink_get_xml($node, $globallink->targetLocale, NULL, NULL, $name, TRUE);

      $source_arr = array();

      if ($source_xml != 'Source Deleted') {
        $source_arr = globallink_get_translated_array($source_xml);
      }
    }
  }
  catch (SoapFault $se) {
    $error = $se->faultstring;
  }
  catch (Exception $ex) {
    $error = $ex->getMessage();
  }

  $arr = array();
  $arr['target_ticket'] = $globallink->targetTicket;

  if ($globallink->sourceDeleted) {
    $arr = array('Source Deleted');
  }
  elseif ($error) {
    $arr = array('error' => $error);
  }
  else {
    $arr['source'] = $source_arr;
    $arr['target'] = $translation;
  }

  return $arr;
}
