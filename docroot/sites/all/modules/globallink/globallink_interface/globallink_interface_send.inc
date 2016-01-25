<?php

/**
 * Builds forms on interface send dashboard.
 *
 * @param string $type
 *   The type of task we are working on.
 *
 * @return array
 *   Array of forms for the interface send dashboard.
 */
function globallink_interface_dashboard_page($type) {
  $_SESSION['globallink_selected_type'] = $type;

  $array = array();

  $array[] = drupal_get_form('globallink_interface_dashboard_filter_form');
  $array[] = drupal_get_form('globallink_interface_dashboard_pager_form');
  $array[] = drupal_get_form('globallink_interface_dashboard_form');

  return $array;
}

/**
 * Builds form to filter interfaces to send for translation on dashboard.
 */
function globallink_interface_dashboard_filter_form() {
  module_load_include('inc', 'globallink', 'globallink');

  $form = array();

  $locales = globallink_get_mapped_drupal_locales(FALSE);

  if (isset($locales) && count($locales) > 1) {
    $results = globallink_drupal_search_query();

    $form['filter'] = array(
      '#type' => 'fieldset',
      '#title' => t('FILTER STRINGS'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    foreach ($results as $key => $result) {
      $filter = FALSE;

      if ($key == 'string') {
        $form['filter']['string'] = array(
          '#type' => 'textfield',
          '#title' => check_plain($result['title']),
          '#description' => check_plain($result['description']),
        );
      }
      else {
        $form['filter'][$key] = array(
          '#attributes' => array('class' => array('container-inline')),
          '#title' => check_plain($result['title']),
          '#type' => 'select',
          '#empty_value' => 'all',
          '#empty_option' => $result['options']['all'],
          '#options' => $result['options'],
        );

        if (!empty($_SESSION['locale_translation_filter'][$key])) {
          $form['filters']['status'][$key]['#default_value'] = $_SESSION['locale_translation_filter'][$key];
        }
      }

      if (!empty($_SESSION['globallink_interface_filter'][$key])) {
        $form['filter'][$key]['#default_value'] = $_SESSION['globallink_interface_filter'][$key];
        $filter = TRUE;
      }
    }

    $form['filter']['actions'] = array(
      '#type' => 'actions',
    );

    $disable_submit = $filter ? TRUE : FALSE;

    $form['filter']['actions']['submit'] = array(
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

      $form['filter-search']['filter-markup'][] = array(
        '#markup' => '<table>',
      );

      foreach ($_SESSION['globallink_interface_filter'] as $key => $value) {
        if (empty($value)) {
          continue;
        }

        switch ($key) {
          case 'string':
            $form['filter-search']['filter-markup'][] = array(
              '#markup' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;"><b>' . 'String contains' . '</b>:&nbsp;</td><td style="text-align: left;width: 85%;vertical-align: middle;border: none;">' . $value . '</td></tr>',
            );

            break;
          case 'language':
            $form['filter-search']['filter-markup'][] = array(
              '#markup' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;"><b>' . 'Language' . '</b>:&nbsp;</td><td style="text-align: left;width: 85%;vertical-align: middle;border: none;">' . $results['language']['options'][$value] . '</td></tr>',
            );

            break;
          case 'translation':
            $form['filter-search']['filter-markup'][] = array(
              '#markup' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;"><b>' . 'Filter in' . '</b>:&nbsp;</td><td style="text-align: left;width: 85%;vertical-align: middle;border: none;">' . $results['translation']['options'][$value] . '</td></tr>',
            );

            break;
        }
      }

      $form['filter-search']['filter-markup'][] = array(
        '#markup' => '</table>',
      );

      $form['filter-search']['status']['actions'] = array(
        '#type' => 'actions',
        '#attributes' => array('class' => array('container-inline')),
      );

      $form['filter-search']['status']['actions']['reset'] = array(
        '#type' => 'submit',
        '#value' => t('Reset'),
      );

      drupal_add_css(drupal_get_path('module', 'globallink') . '/globallink_interface/css/interface.css');
    }
  }
  else {
    $form['locale_mapping']['no_mapping'] = array(
      '#type' => 'markup',
      '#markup' => t('<br/><b><i>No GlobalLink locale mapping found.</i></b>'),
    );
  }

  return $form;
}

/**
 * Handles submission of filter form.
 */
function globallink_interface_dashboard_filter_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  switch ($op) {
    case t('Filter'):
      $_SESSION['globallink_interface_filter']['string'] = $form_state['values']['string'];
      $_SESSION['globallink_interface_filter']['language'] = $form_state['values']['language'];
      $_SESSION['globallink_interface_filter']['translation'] = $form_state['values']['translation'];

      break;
    case t('Reset'):
      $_SESSION['globallink_interface_filter']['string'] = '';
      $_SESSION['globallink_interface_filter']['language'] = '';
      $_SESSION['globallink_interface_filter']['translation'] = '';

      break;
  }
}

/**
 * Builds form to add pagination to interface send dashboard.
 */
function globallink_interface_dashboard_pager_form() {
  $form = array();

  $locales = globallink_get_mapped_drupal_locales(FALSE);

  if (isset($locales) && count($locales) > 1) {
    $form['page_counter']['markup'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('tpt-center')),
    );

    $form['page_counter']['markup']['p_count'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#default_value' => isset($_SESSION['globallink_interface_page_count']) ? $_SESSION['globallink_interface_page_count'][0] : TPT_PAGER_LIMIT,
      '#field_prefix' => t('Show') . '&nbsp;&nbsp;',
      '#field_suffix' => '&nbsp;&nbsp;' . t('records') . '&nbsp;&nbsp;&nbsp;&nbsp;',
      '#prefix' => '<div class="container-inline">',
    );

    $form['page_counter']['markup']['action'] = array(
      '#type' => 'submit',
      '#value' => t('Go'),
      '#suffix' => '</div>',
      '#limit_validation_errors' => array(),
      '#submit' => array('globallink_interface_pager_form_submit'),
      '#access' => TRUE,
    );
  }

  return $form;
}

/**
 * Handles submission of pager form.
 */
function globallink_interface_pager_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  if ($op != t('Go')) {
    return;
  }

  $page_count = $form_state['input']['p_count'];

  $_SESSION['globallink_interface_page_count'] = array($page_count);
}

/**
 * Builds form to create an interface submission.
 */
function globallink_interface_dashboard_form() {
  module_load_include('inc', 'globallink_interface', 'globallink_interface');

  $form = array();

  $module_path = drupal_get_path('module', 'globallink');

  $locales = globallink_get_mapped_drupal_locales(FALSE);

  if (isset($locales) && count($locales) > 1) {
    $target_lang_arr = globallink_get_mapped_locales_with_drupal_desc(FALSE);
    $source_lang_arr = globallink_get_mapped_locales_with_drupal_desc(FALSE);

    $page_count = TPT_PAGER_LIMIT;

    if (isset($_SESSION['globallink_interface_page_count'])) {
      $page_count = $_SESSION['globallink_interface_page_count'][0];
    }

    $default_language = language_default();
    $default = $default_language->language;

    if (!empty($_SESSION['globallink_selected_interface_language'])) {
      $default = $_SESSION['globallink_selected_interface_language'];
    }

    unset($target_lang_arr[globallink_get_locale_code($default)]);

    $t_count = 0;

    foreach ($target_lang_arr as $key => $value) {
      $t_count++;

      if ($t_count % 2 === 0) {
        $target_lang_arr[$key] = '&nbsp;&nbsp;' . $value . '<BR/>';
      }
      else {
        $target_lang_arr[$key] = '&nbsp;&nbsp;' . $value . '&nbsp;&nbsp;&nbsp;&nbsp;';
      }
    }

    if (!($query = globallink_interface_get_translate_filter_query())) {
      $query = array(
        'translation' => 'all',
        'language' => 'all',
        'string' => '',
      );
    }

    $sql_query = db_select('locales_source', 's')
    ->extend('PagerDefault')
    ->limit($page_count);

    $limit_language = NULL;

    if ($query['language'] != 'en' && $query['language'] != 'all') {
      $sql_query->leftJoin('locales_target', 't', 't.lid = s.lid AND t.language = :langcode', array(
        ':langcode' => $query['language'],
      ));
      $limit_language = $query['language'];
    }
    else {
      $sql_query->leftJoin('locales_target', 't', 't.lid = s.lid');
    }

    $header = array(
      'string' => array(
        'data' => t('String'),
        'field' => 's.location',
      ),
      'active' => t('In Active Submission'),
    );

    $sql_query->fields('s', array('source', 'location', 'context', 'lid', 'textgroup'));
    $sql_query->fields('t', array('translation', 'language'));

    switch ($query['translation']) {
      case 'translated':
        $sql_query->condition('t.translation', '%' . db_like($query['string']) . '%', 'LIKE');

        break;
      case 'untranslated':
        $sql_query->condition(db_and()
        ->condition('s.source', '%' . db_like($query['string']) . '%', 'LIKE')
        ->isNull('t.translation'));

        break;
      case 'all':
      default:
        $condition = db_or()
        ->condition('s.source', '%' . db_like($query['string']) . '%', 'LIKE');

        if ($query['language'] != 'en') {
          $condition->condition('t.translation', '%' . db_like($query['string']) . '%', 'LIKE');
        }

        $sql_query->condition($condition);

        break;
    }

    $sql_query->condition('s.textgroup', 'default');
    $sql_query->extend('TableSort')
    ->groupBy('lid');

    $sort_order = isset($_GET['sort']) ? $_GET['sort'] : '';

    if ($sort_order) {
      $sql_query->orderBy('source', $_GET['sort']);
    }

    $result = $sql_query->execute();

    $lids = array();

    foreach ($result as $item) {
      array_push($lids, $item->lid);
    }

    $active_subs = globallink_interface_get_active_submission_rows_by_lid($lids);
    $locales = globallink_get_mapped_drupal_locales(FALSE);

    $interface_results = $sql_query->execute();

    $rows = array();
    $count = 0;

    foreach ($interface_results as $interface) {
      $active = '';

      if ($active_subs && array_key_exists($interface->lid, $active_subs)) {
        $sub_arr = $active_subs[$interface->lid];
        $sub_name = '';
        $tgt_arr = array();

        foreach ($sub_arr as $sub_name => $tgt_arr) {
          $l_title = globallink_format_display_string($sub_name);
          $l_href = 'admin/globallink-translations/activeSubmissions/interface';
          $l_options = array('query' => array('submission' => urlencode($sub_name)));
          $active .= l($l_title, $l_href, $l_options) . '&nbsp;&nbsp;- &nbsp;&nbsp;';

          $t_count = 0;

          foreach ($tgt_arr as $tgt) {
            $t_count++;

            $tgt_name = $locales[globallink_get_drupal_locale_code($tgt)];

            if ($t_count == count($tgt_arr)) {
              $active .= $tgt_name . ' <BR/> ';
            }
            else {
              $active .= $tgt_name . ', &nbsp;&nbsp;';
            }
          }
        }
      }

      if (!isset($rows[$interface->lid])) {
        $rows[$interface->lid] = array(
          'string' => check_plain(truncate_utf8($interface->source, 150, FALSE, TRUE)) . '<br /><small>' . $interface->location . '</small>',
          'active' => $active,
        );
      }

      if (isset($interface->language)) {
        $rows[$interface->lid]['languages'][$interface->language] = $interface->translation;
      }

      $count++;
    }

    $form['table'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $rows,
      '#empty' => t('No strings available.'),
    );

    if ($count > 0) {
      $form['pager'] = array('#markup' => theme('pager'));
    }

    if (user_access(TPT_ROLE_DO_TRANSLATIONS) || user_access(TPT_ROLE_MANAGE_TRANSLATIONS)) {
      if ($count > 0) {
        drupal_add_css($module_path . '/css/globallink.css');

        $proj_arr = variable_get('globallink_pd_projects', array());

        if (count($proj_arr) > 0) {
          $projects = variable_get('globallink_pd_projectid');

          if (!empty($projects)) {
            $arr = explode(',', $projects);

            foreach ($arr as $value) {
              if (isset($proj_arr[$value])) {
                $proj_arr[$value] = $proj_arr[$value];
              }
              else {
                $proj_arr[$value] = $value;
              }
            }
          }
        }

        $p_key = '';

        if (count($proj_arr) == 1) {
          $p_key = key($proj_arr);
        }

        $proj_arr[''] = ' -- Select a Project -- ';
        $r_proj_arr = array_reverse($proj_arr);

        $form['send_submission'] = array(
          '#type' => 'fieldset',
          '#title' => t('Create Submission'),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );

        $form['send_submission']['submission_name'] = array(
          '#type' => 'textfield',
          '#title' => 'Submission Name',
          '#size' => 40,
          '#required' => TRUE,
          '#default_value' => variable_get('globallink_pd_submissionprefix', 'DRU_') . REQUEST_TIME,
          '#prefix' => '<div class="container-inline"><table><tr><td style="text-align: left;width: 20%;vertical-align: middle;border: none;">',
          '#suffix' => '</tr>',
          '#field_suffix' => '</td>',
          '#field_prefix' => '<BR/><div class="description">' . t('Name for this new Submission.') . '</div></td><td>',
        );

        $default_date = format_date(REQUEST_TIME + (7 * 24 * 60 * 60), 'custom', 'm/d/Y');

        drupal_add_library('system', 'ui.datepicker');
        drupal_add_js('jQuery(document).ready(function(){
          jQuery( ".pickadate1" ).datepicker({
          dateFormat: "mm/dd/yy",
          defaultDate: "' . $default_date . '",
          minDate: 0,
          autoSize: true,
          showOn: "button",
          buttonImage: "' . $GLOBALS['base_url'] . '/' . $module_path . '/css/icon-calendar.png",
          buttonImageOnly: true,
          buttonText: "Click to select date"
          });});', 'inline');

        $form['send_submission']['submission_due_date'] = array(
          '#type' => 'textfield',
          '#title' => 'Submission Due Date',
          '#required' => TRUE,
          '#attributes' => array(
            'class' => array('pickadate1'),
            'style' => array('padding:0px;'),
          ),
          '#default_value' => $default_date,
          '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
          '#suffix' => '</tr>',
          '#field_suffix' => '</td>',
          '#field_prefix' => '<BR/><div class="description">' . t('Expected Completion Date for this Submission.') . '</div></td><td>',
        );

        $form['send_submission']['submission_pd_project'] = array(
          '#type' => 'select',
          '#title' => t('Project Name'),
          '#required' => TRUE,
          '#options' => $r_proj_arr,
          '#default_value' => $p_key,
          '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
          '#suffix' => '</tr>',
          '#field_suffix' => '</td>',
          '#field_prefix' => '<BR/><div class="description">' . t('GlobalLink Project Name for this Submission') . '</div></td><td>',
        );

        $form['send_submission']['submission_source_locale'] = array(
          '#type' => 'select',
          '#title' => t('Source Language'),
          '#required' => TRUE,
          '#options' => $source_lang_arr,
          '#default_value' => globallink_get_locale_code($default),
          '#disabled' => TRUE,
          '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
          '#suffix' => '</tr>',
          '#field_suffix' => '</td>',
          '#field_prefix' => '<BR/><div class="description">' . t('Source language for this Submission.') . '</div></td><td>',
        );

        $form['send_submission']['submission_target_locale'] = array(
          '#type' => 'checkboxes',
          '#options' => $target_lang_arr,
          '#title' => t('Target Language(s)'),
          '#required' => TRUE,
          '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
          '#suffix' => '</tr>',
          '#field_suffix' => '</td>',
          '#field_prefix' => '<BR/><div class="description">' . t('Target language(s) for this Submission.') . '</div></td><td>',
        );

        global $user;

        $form['send_submission']['submission_submitter'] = array(
          '#type' => 'textfield',
          '#title' => 'Submitter',
          '#size' => 40,
          '#required' => TRUE,
          '#default_value' => check_plain($user->name),
          '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
          '#suffix' => '</tr>',
          '#field_suffix' => '</td>',
          '#field_prefix' => '<BR/><div class="description">' . t('Username for this new Submission.') . '</div></td><td>',
        );

        $form['send_submission']['submission_instructions'] = array(
          '#type' => 'textarea',
          '#title' => t('Instructions'),
          '#attributes' => array('style' => 'width: 275px;resize: none;'),
          '#rows' => 3,
          '#resizable' => FALSE,
          '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
          '#suffix' => '</tr></table></div>',
          '#field_suffix' => '</div></td>',
          '#field_prefix' => '<BR/><div class="description">' . t('Other instructions for this Submission.') . '</div></td><td>',
        );

        $form['send_submission']['submit_pd_send'] = array(
          '#type' => 'submit',
          '#value' => t('Send for Translation'),
        );
      }
    }
  }

  return $form;
}

/**
 * Validates interface form input.
 */
function globallink_interface_dashboard_form_validate($form, &$form_state) {
  module_load_include('inc', 'globallink', 'globallink_settings');

  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  $pd4 = globallink_get_project_director_details();

  globallink_validate_project_director_details($pd4);

  $source_locale = $form_state['values']['submission_source_locale'];
  $target_locale_arr = $form_state['values']['submission_target_locale'];

  if ($op != t('Send for Translation')) {
    return;
  }

  $lids = array_filter($form_state['values']['table']);

  if (count($lids) == 0) {
    $lids = array_filter($form_state['values']['table']);
    form_set_error('', t('No items selected.'));
  }

  if (!empty($form_state['values']['submission_due_date'])) {
    $due_date = (globallink_convert_date_to_timestamp($form_state['values']['submission_due_date'])) / 1000;

    if ($due_date < REQUEST_TIME) {
      form_set_error('', t('The specified date is in past.'));
    }
  }

  if (count($lids) == 0) {
    return;
  }

  if (!isset($source_locale) || !isset($target_locale_arr)) {
    return;
  }

  foreach ($target_locale_arr as $key => $target) {
    if ($target == FALSE) {
      unset($target_locale_arr[$key]);
    }
  }

  $message = FALSE;

  if (count($target_locale_arr) == 0) {
    return;
  }

  foreach ($lids as $lid) {
    $rows = globallink_interface_get_sent_rows_by_lid($lid);

    foreach ($rows as $row) {
      if ($row->source == $source_locale && array_search($row->target, $target_locale_arr)) {
        $message = TRUE;
      }
    }
  }

  if ($message) {
    drupal_set_message(t('Ignored record(s) that has already been sent out for translation.'), 'warning');
  }
}

/**
 * Handles interface form submission.
 */
function globallink_interface_dashboard_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  $submission_details = array();

  if ($op != t('Send for Translation')) {
    return;
  }

  try {
    $pd4 = globallink_get_project_director_details();
    $lids = array_filter($form_state['values']['table']);

    $submission_name = $form_state['values']['submission_name'];
    $source_locale = $form_state['values']['submission_source_locale'];
    $target_locale_arr = $form_state['values']['submission_target_locale'];
    $submission_details['instructions'] = $form_state['values']['submission_instructions'];
    $submission_details['submitter'] = $form_state['values']['submission_submitter'];

    foreach ($target_locale_arr as $key => $target) {
      if ($target == FALSE) {
        unset($target_locale_arr[$key]);
      }
    }

    $due_date = globallink_convert_date_to_timestamp($form_state['values']['submission_due_date']);
    $project_code = $form_state['values']['submission_pd_project'];
    $updated_globalLink_arr = globallink_interface_send_for_translations($lids, $pd4, $submission_name, $due_date, $project_code, $source_locale, $target_locale_arr, $submission_details);

    if (count($updated_globalLink_arr) > 0) {
      globallink_interface_update_ticket_id($updated_globalLink_arr, $project_code);
      drupal_set_message(t('Content has been successfully sent for translations.'));
    }
  }
  catch (Exception $ex) {
    form_set_error('', t('Error: @message', array('@message' => $ex->getMessage())));
  }
}
