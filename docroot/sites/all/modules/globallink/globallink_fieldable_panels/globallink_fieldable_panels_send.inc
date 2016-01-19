<?php

/**
 * Builds forms on fieldable panels send dashboard.
 *
 * @param string $type
 *   The type of task we are working on.
 *
 * @return array
 *   Array of forms for the fieldable panels send dashboard.
 */
function globallink_fieldable_panels_dashboard($type) {
  $_SESSION['globallink_selected_type'] = $type;

  $array = array();

  $array[] = drupal_get_form('globallink_fieldable_panels_dashboard_filter_form');
  $array[] = drupal_get_form('globallink_fieldable_panels_dashboard_pager_form');
  $array[] = drupal_get_form('globallink_fieldable_panels_dashboard_form');

  return $array;
}

/**
 * Builds form to filter fieldable panels to send for translation on dashboard.
 */
function globallink_fieldable_panels_dashboard_filter_form() {
  module_load_include('inc', 'globallink', 'globallink');

  $form = array();

  $locales = globallink_get_mapped_drupal_locales(FALSE);

  if (!empty($locales)) {
    $filter = FALSE;

    foreach ($locales as $key => $lang) {
      $lang_filter[$key] = $lang;
    }

    $default_language = language_default();
    $default = $default_language->language;

    if (!empty($_SESSION['globallink_selected_fieldable_panels_language'])) {
      $default = $_SESSION['globallink_selected_fieldable_panels_language'];
      $filter = TRUE;
    }

    $form['select_fieldable_panels_type'] = array(
      '#type' => 'fieldset',
      '#title' => t('FILTER FIELDABLE PANELS PANES'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['select_fieldable_panels_type']['filters'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('container-inline')),
    );

    $form['select_fieldable_panels_type']['filters']['language'] = array(
      '#type' => 'select',
      '#title' => 'Language' . ':&nbsp;&nbsp;',
      '#options' => $lang_filter,
      '#default_value' => $default,
      '#prefix' => '<div class="container-inline"><table><tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
      '#suffix' => '</tr></table></div>',
      '#field_suffix' => '</td>',
      '#field_prefix' => '</td><td>',
    );

    $disable_submit = $filter ? TRUE : FALSE;

    $form['select_fieldable_panels_type']['filters']['filter'] = array(
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
        '#markup' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;"><b>' . 'Language' . '</b>:&nbsp;</td><td style="text-align: left;width: 85%;vertical-align: middle;border: none;">' . $lang_filter[$default] . '</td></tr>',
      );

      $form['filter-search']['filter-markup'][] = array('#markup' => '</table>');

      $form['filter-search']['status']['actions'] = array(
        '#type' => 'actions',
        '#attributes' => array('class' => array('container-inline')),
      );

      $form['filter-search']['status']['actions']['reset'] = array(
        '#type' => 'submit',
        '#value' => t('Reset'),
      );
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
function globallink_fieldable_panels_dashboard_filter_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  switch ($op) {
    case t('Filter'):
      $_SESSION['globallink_selected_fieldable_panels_language'] = $form_state['values']['language'];

      break;
    case t('Reset'):
      $_SESSION['globallink_selected_fieldable_panels_language'] = '';

      break;
  }
}

/**
 * Builds form to add pagination to fieldable panels send dashboard.
 */
function globallink_fieldable_panels_dashboard_pager_form() {
  $module_path = drupal_get_path('module', 'globallink');
  drupal_add_css($module_path . '/css/globallink.css');

  $form = array();

  $locales = globallink_get_mapped_drupal_locales(FALSE);

  if (!empty($locales)) {
    $form['page_counter']['markup'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('tpt-center')),
    );

    $form['page_counter']['markup']['p_count'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#default_value' => isset($_SESSION['globallink_fieldable_panels_page_count']) ? $_SESSION['globallink_fieldable_panels_page_count'][0] : TPT_PAGER_LIMIT,
      '#field_prefix' => t('Show') . '&nbsp;&nbsp;',
      '#field_suffix' => '&nbsp;&nbsp;' . t('records') . '&nbsp;&nbsp;&nbsp;&nbsp;',
      '#prefix' => '<div class="container-inline">',
    );

    $form['page_counter']['markup']['action'] = array(
      '#type' => 'submit',
      '#value' => t('Go'),
      '#suffix' => '</div>',
      '#limit_validation_errors' => array(),
      '#submit' => array('globallink_fieldable_panels_dashboard_pager_form_submit'),
      '#access' => TRUE,
    );
  }

  return $form;
}

/**
 * Handles submission of pager form.
 */
function globallink_fieldable_panels_dashboard_pager_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];

  if ($op != t('Go')) {
    return;
  }

  $page_count = $form_state['input']['p_count'];

  if (!is_numeric($page_count) || $page_count < 1) {
    form_set_error('', t('Invalid Page Count.'));
  }
  else {
    $_SESSION['globallink_fieldable_panels_page_count'] = array($page_count);

    return;
  }
}

/**
 * Builds form to create a fieldable panels submission.
 */
function globallink_fieldable_panels_dashboard_form() {
  module_load_include('inc', 'globallink_fieldable_panels', 'globallink_fieldable_panels');

  $form = array();

  $module_path = drupal_get_path('module', 'globallink');
  $languages = globallink_get_mapped_drupal_locales(FALSE);

  if (isset($languages) && count($languages) > 1) {
    $target_lang_arr = globallink_get_mapped_locales_with_drupal_desc(FALSE);
    $source_lang_arr = globallink_get_mapped_locales_with_drupal_desc(FALSE);

    $default_language = language_default();
    $default = $default_language->language;

    if (!empty($_SESSION['globallink_selected_fieldable_panels_language'])) {
      $default = $_SESSION['globallink_selected_fieldable_panels_language'];
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

    $page_count = TPT_PAGER_LIMIT;

    if (isset($_SESSION['globallink_fieldable_panels_page_count'])) {
      $page_count = $_SESSION['globallink_fieldable_panels_page_count'][0];
    }

    $header = array(
      'title' => array(
        'field' => 'fpp.title',
        'data' => t('Fieldable Panels Pane Title'),
      ),
      'category' => array(
        'field' => 'fpp.category',
        'data' => t('Category'),
      ),
      'lang' => t('Language'),
      'active' => t('In Active Submission'),
    );

    $query = db_select('fieldable_panels_panes', 'fpp')
    ->fields('fpp')
    ->condition('language', $default, '=')
    ->extend('PagerDefault') // Pager Extender
    ->limit($page_count) // 10 results per page
    ->extend('TableSort'); // Sorting Extender

    $sort_order = isset($_GET['order']) ? $_GET['order'] : '';

    if ($sort_order == '') {
      $query->orderBy('fpid', 'DESC');
    }
    else {
      $query->orderByHeader($header);
    }

    $results = $query->execute();

    $count = 0;

    $rows = array();

    foreach ($results as $item) {
      $count++;

      $active = '';
      $active_subs = globallink_fieldable_panels_get_active_submission_rows($item->fpid);

      if ($active_subs && array_key_exists($item->fpid, $active_subs)) {
        $sub_arr = $active_subs[$item->fpid];
        $sub_name = '';

        $tgt_arr = array();

        foreach ($sub_arr as $sub_name => $tgt_arr) {
          $l_title = globallink_format_display_string($sub_name);
          $l_href = 'admin/globallink-translations/activeSubmissions/fieldable_panels';

          $l_options = array('query' => array('submission' => urlencode($sub_name)));
          $active .= l($l_title, $l_href, $l_options) . '&nbsp;&nbsp;- &nbsp;&nbsp;';

          $t_count = 0;

          foreach ($tgt_arr as $tgt) {
            $t_count++;

            $tgt_name = $source_lang_arr[$tgt];

            if ($t_count == count($tgt_arr)) {
              $active .= $tgt_name . ' <BR/> ';
            }
            else {
              $active .= $tgt_name . ', &nbsp;&nbsp;';
            }
          }
        }
      }

      $rows[$item->fpid] = array(
        'title' => $item->title,
        'category' => $item->category,
        'lang' => $languages[$item->language],
        'active' => $active,
      );
    }

    $form['table'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $rows,
      '#empty' => t('No items available'),
    );

    if ($count > 0) {
      $form['pager'] = array('#markup' => theme('pager'));
    }

    if (user_access(TPT_ROLE_DO_TRANSLATIONS) || user_access(TPT_ROLE_MANAGE_TRANSLATIONS)) {
      if ($count > 0) {
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

        $form['send_submission']['submission_target_locale'] = array('#type' => 'checkboxes',
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

        $form['send_submission']['submission_instructions'] = array('#type' => 'textarea',
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
 * Validates fieldable panels form input.
 */
function globallink_fieldable_panels_dashboard_form_validate($form, &$form_state) {
  module_load_include('inc', 'globallink', 'globallink_settings');

  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  $pd4 = globallink_get_project_director_details();

  globallink_validate_project_director_details($pd4);

  $source_locale = $form_state['values']['submission_source_locale'];
  $target_locale_arr = $form_state['values']['submission_target_locale'];

  if ($op != t('Send for Translation')) {
    return;
  }

  $fpids = array_filter($form_state['values']['table']);

  if (count($fpids) == 0) {
    $fpids = array_filter($form_state['values']['table']);
    form_set_error('', t('No items selected.'));
  }

  if (!empty($form_state['values']['submission_due_date'])) {
    $due_date = (globallink_convert_date_to_timestamp($form_state['values']['submission_due_date'])) / 1000;

    if ($due_date < REQUEST_TIME) {
      form_set_error('', t('The specified date is in past.'));
    }
  }

  if (count($fpids) == 0) {
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

  foreach ($fpids as $fpid) {
    $rows = globallink_fieldable_panels_get_sent_rows_by_fpid($fpid);

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
 * Handles fieldable panels form submission.
 */
function globallink_fieldable_panels_dashboard_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  $submission_details = array();

  if ($op != t('Send for Translation')) {
    return;
  }

  try {
    $pd4 = globallink_get_project_director_details();

    $fpids = array_filter($form_state['values']['table']);

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
    $updated_globalLink_arr = globallink_fieldable_panels_pane_send_for_translations($fpids, $pd4, $submission_name, $due_date, $project_code, $source_locale, $target_locale_arr, $submission_details);

    if (count($updated_globalLink_arr) > 0) {
      globallink_fieldable_panels_update_ticket_id($updated_globalLink_arr, $project_code);
      drupal_set_message(t('Content has been successfully sent for translations.'));
    }
  }
  catch (Exception $ex) {
    form_set_error('', t('Error: @message', array('@message' => $ex->getMessage())));
  }
}
