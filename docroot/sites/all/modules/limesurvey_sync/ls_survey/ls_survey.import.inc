<?php
/**
 * @file
 * functions for importing answers from LimeSurvey to Drupal.
 */

/**
 * Display synchronization datas for a specific survey node.
 *
 * Display data and allow to import (synchronize) answers from LimeSurvey to
 * drupal.
 *
 * @param $form_state
 *   An array. The form statement.
 * @param $node_survey
 *   An object. The survey node.
 *
 * @return
 *   A string. The html page displaying synchronization datas.
 *
 * @see ls_survey_import_validate()
 * @see ls_survey_import_submit()
 * @ingroup forms
 */
function ls_survey_import($form, $form_state, $node_survey) {
  // Load file for the ls_survey_find_answ_type() and the
  // ls_survey_display_sync_fields() functions.
  module_load_include('inc', 'ls_survey', 'ls_survey');
  // Load file for the limesurvey_sync_link_to_ls() function.
  module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');

  // $answ->type has been controled in the ls_survey_import_access() function.
  $answ_type = ls_survey_find_answ_type($node_survey->ls_orig_type);

  // Collect answers from the LimeSurvey database.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
  $all_answers = limesurvey_sync_answ_extract_answers($node_survey->ls_sid);
  if ($all_answers === FALSE) {
    $summary = t('an error has occured, you should check the <a href="@url">LimeSurvey Sync settings</a>', array('@url' => url('admin/config/media/ls_sync')));
  }
  else {
    $n_lang = 0;
    foreach ($all_answers as $answer) {
      if ($answer['ls_lang'] == $node_survey->ls_lang) {
        $n_lang++;
      }
    }
    $ls_answers = count($all_answers);
    $answ_lg = format_plural($n_lang, 'There is <b>1</b> %lang answer stored on your LimeSurvey site', 'There are <b>@count</b> %lang answers stored on your LimeSurvey site', array('%lang' => ls_survey_display_sync_fields('ls_lang', $node_survey->ls_lang)));
    $answ_lg_of = format_plural($ls_answers, '(of 1 answer).', '(of @count answers).');
    $summary = $answ_lg . ' ' . $answ_lg_of;
  }

  if (module_exists('ls_answ')) {
    $d_answ_query = db_select('node', 'n');
    $d_answ_query->fields('n', array('nid'));
    $d_answ_query->join('node_revision', 'r', 'n.vid = r.vid');
    $d_answ_query->join('ls_answ', 'lsa', 'n.vid = lsa.vid');
    $d_answ_query->condition('type', $answ_type->type)
      ->condition('ls_status', LS_STATUS_VIRGIN, '<>')
      ->condition('ls_lang', $node_survey->ls_lang)
      ->addTag('node_access');
    $d_answ = $d_answ_query->countQuery()->execute()->fetchField();
    $summary .=  '<br />' . format_plural($d_answ, 'And there is <b>1</b> %lang answer stored on this site.', 'And there are <b>@count</b> %lang answers stored on this site.', array('%lang' => ls_survey_display_sync_fields('ls_lang', $node_survey->ls_lang)));
  }
  else {
    $summary = t('Answers stored on this site are not reachable.', array('%count' => $ls_answers));
  }

  $link_ls_admin = limesurvey_sync_link_to_ls(t('LimeSurvey admin interface'), $node_survey->ls_sid, 'browse');

  $notice = '<p><b>' . t('Note that in most cases, there is no interest importing answers from your LimeSurvey site to this site.') . '</b>';
  $notice .= '<br />' . t('Up to now, the answers are already availables on the !link. They can be browsed, edited, deleted, exported, printed as graphs, etc...', array('!link' => $link_ls_admin));
  $notice .= '<br />' . t('But if you want to access to more advanced features, for instance :');
  $notice .= '</p>';
  $features = array();
  $features[] = t('making the answers availables for users which have no access to the LimeSurvey admin interface,');
  $features[] = t('or be able to edit or even delete answers <em>from</em> this site,');
  $features[] = t('or comparing 2 or more answers,');
  $features[] = t('or storing revisions of edited answers,');
  $notice .= theme('item_list', array('items' => $features));
  $notice .= '<p>' . t('then you do want to import answers.') . '</p>';
  $output = ls_survey_theme_box(t('Notice'), $notice);

  $disclaimer = '<p><b>' . t('Once you have imported answers, pay attention on :') . '</b></p>';
  $warnings = array();
  $warnings[] = t('if you delete an imported answer <em>from</em> this site, the corresponding answer will be deleted on your LimeSurvey site to !');
  $warnings[] = t('depending on this site <a href="@url">permissions settings</a>, anonymous users may view answers ! (not allowed by default, the risk only exists if you edit the permissions settings).', array('@url' => url('admin/user/permissions')));
  $disclaimer .= theme('item_list', array('items' => $warnings));
  $output .= ls_survey_theme_box(t('Disclaimer'), $disclaimer);


  $form = array();
  $form['#node'] = $node_survey;
  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import answers from The LimeSurvey site'),
  );

  $form['import']['caution'] = array(
    '#type' => 'markup',
    '#prefix' => '<div>',
    '#markup' => $output,
    '#suffix' => '</div>',
  );
  $form['import']['summary'] = array(
    '#type' => 'markup',
    '#markup' => ls_survey_theme_box(t('Summary'), $summary),
  );

  $sync_choice = array(
    'import' => t('Import new answers only'),
    'resync' => t('Synchronize already imported answers'),
  );
  $form['import']['sync_choice'] = array(
    '#type' => 'radios',
    '#title' => t('Choose the synchronization mode'),
    '#options' => $sync_choice,
    '#description' => t('Update answers only if needed.'),
    '#default_value' => 'import',
  );

  $form['import']['more_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['import']['more_options']['verbose'] = array(
    '#type' => 'checkbox',
    '#title' => t('Verbose results'),
    '#description' => t('Print updated or imported answers.'),
    '#default_value' => FALSE,
  );

  $form['import']['more_options']['force_sync'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force the synchronization'),
    '#description' => t('Force the importation of the answer datas, even if it not needed, and import or update uncompleted answers.'),
    '#default_value' => FALSE,
  );

  $form['import']['more_options']['force_desync'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force the synchronization on desynchronized answers'),
    '#description' => t('Use with caution.'),
    '#default_value' => FALSE,
  );

  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $survey_properties = limesurvey_sync_survey_properties($node_survey->ls_sid);
  $more_languages = (isset($survey_properties['#lang']) && count($survey_properties['#lang']) <= 1);
  $form['import']['more_options']['all_lang'] = array(
    '#type' => 'checkbox',
    '#title' => t('One time synchronization for all languages'),
    '#description' => t("This survey node only handle the '%lang' language. If you have create other survey nodes for the survey '%sid', they will be synchronized to.", array('%lang' => ls_survey_display_sync_fields('ls_lang', $node_survey->ls_lang), '%sid' => ls_survey_display_sync_fields('ls_sid', $node_survey->ls_sid))),
    '#default_value' => FALSE,
    '#disabled' => $more_languages,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Synchronize'),
  );

  return $form;
}

/**
 * validate the synchronization process and options.
 */
function ls_survey_import_validate($form, &$form_state) {
  $node_survey = $form['#node'];

  module_load_include('inc', 'ls_survey', 'ls_survey');
  // $answ->type has been controled in the ls_survey_import_access() function.
  $answ = ls_survey_find_answ_type($node_survey->ls_orig_type);

  if (!node_access('view', $node_survey)) {
    form_set_error('force_sync', t('You are not allowed to import answers'));
  }

  if (!empty($form_state['values']['force_sync']) && !user_access('force answers synchronization')) {
    form_set_error('force_sync', t('You are not allowed to force the synchronization'));
  }
  if (!empty($form_state['values']['force_desync']) && !user_access('force answers synchronization')) {
    form_set_error('force_desync', t('You are not allowed to force the synchronization'));
  }

  if (!empty($form_state['values']['all_lang'])) {
    // retrieve all types corresponding to the ls_sid value.
    $import_all_lang_access = TRUE;
    $n_imported_lang = 0;
    $answ_query = db_query('SELECT n.nid AS nid_survey, nt.type AS answ_type FROM {ls_survey} lss
    JOIN {node} n ON n.vid = lss.vid
    JOIN {node_type} nt ON nt.orig_type = lss.ls_orig_type
    WHERE ls_sid = :ls_sid', array(':ls_sid' => $node_survey->ls_sid));
    while ($answ_row = $answ_query->fetchAssoc()) {
      $n_imported_lang++;
      if (!node_access('view', node_load($answ_row['nid_survey']))) {
        $import_all_lang_access = FALSE;
        break;
      }
    }
    if ($n_imported_lang < 2) {
      form_set_error('all_lang', t('This is not a multilingue survey, at least on this drupal site.'));
    }

    if ($import_all_lang_access) {
      form_set_error('all_lang', t('You are not allowed to import answers from some languages'));
    }
  }

  if (empty($form_state['values']['sync_choice']) || !in_array($form_state['values']['sync_choice'], array('import', 'resync'))) {
    form_set_error('sync_choice', t("The '%setting' setting is not valid.", array('%setting' => $form['import']['sync_choice']['#title'])));
  }

  if ($form_state['values']['sync_choice'] == 'import') {
    // Load the survey properties.
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
    $survey_properties = limesurvey_sync_survey_properties($node_survey->ls_sid);
    if ($survey_properties['anonymized']) {
      // Load file for the limesurvey_sync_fixdate_ls_to_drupal() function.
      module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
      $time = REQUEST_TIME;
      $survey_not_finished = (empty($survey_properties['expires']) || $time < $survey_properties['expires']);
      if ($survey_not_finished) {
        form_set_error('sync_choice', t('For privacy raison, you must wait to the survey expiration date (%date) before importing answers.', array('%date' => ((!empty($survey_properties['expires'])) ? format_date($survey_properties['expires'], 'short') : t('undefined')))));
      }
    }
  }
}

/**
 * Proceed to the synchronization.
 */
function ls_survey_import_submit($form, &$form_state) {
  $node_survey = $form['#node'];

  module_load_include('inc', 'ls_survey', 'ls_survey');
  $answ_type = ls_survey_find_answ_type($node_survey->ls_orig_type);

  $verbose = $form_state['values']['verbose'];
  $force = $form_state['values']['force_sync'];
  $force_desync = $form_state['values']['force_desync'];
  $all_lang = $form_state['values']['all_lang'];

  // Batching the synchronization, it may be interesting if there is a
  // lot of answers.
  module_load_include('inc', 'ls_answ', 'ls_answ');
  switch ($form_state['values']['sync_choice']) {
    case 'import':
      $import_type = (!$all_lang) ? $answ_type->type : '';
      $only_new_nodes = TRUE;
      $include_desync_rev = TRUE;
      ls_answ_batch_import($node_survey->ls_sid, $import_type, array(), $only_new_nodes, $include_desync_rev, $force, $force_desync, $verbose);
      break;

    case 'resync':
      if (!$all_lang) {
        $where = array('n.type' => $answ_type->type);
      }
      else {
        // Retrieve all types corresponding to the ls_sid value.
        $types = array();
        $answ_query = db_query('SELECT nt.type AS answ_type FROM {ls_survey} lss
        JOIN {node} n ON n.vid = lss.vid
        JOIN {node_type} nt ON nt.orig_type = lss.ls_orig_type
        WHERE ls_sid = :ls_sid', array(':ls_sid' => $node_survey->ls_sid));
        while ($answ_row = $answ_query->fetchAssoc()) {
          $types[] = $answ_row['answ_type'];
        }
        $where = array('n.type' => $types);
      }
      // Use the access_bypass because all nodes must be updated.
      $access_bypass = TRUE;
      ls_answ_batch_sync($where, $force, $force_desync, $access_bypass, $verbose);
      break;
  }
}
