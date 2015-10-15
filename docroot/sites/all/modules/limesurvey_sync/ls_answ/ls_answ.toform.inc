<?php
/**
 * @file
 * Page displaying a submit button to reach the LimeSurvey form.
 */

/**
 * Provide a form button to fill the survey.
 *
 * If the answer node is not provided, create a link for a new answer node.
 *
 * @param $type
 *   A string. The answer content type.
 * @param $node
 *   An answer node object or FALSE if not provided
 *
 * @return
 *   A string. The url to fill the survey.
 */
function ls_answ_to_fill_survey($type, $node = FALSE) {
  // $type has been checked into the ls_answ_to_fill_survey_access() function.
  module_load_include('inc', 'ls_survey', 'ls_survey.type');
  if (empty($node)) {
    module_load_include('inc', 'ls_answ', 'ls_answ');
    $node = ls_answ_new_node($type);
    $node->ls_status = LS_STATUS_EDITING;
    $node->ls_sync = LS_SYNC_UNSYNCHRONIZED;
    $time = REQUEST_TIME;
    $node->ls_last_import = $time; // Optional.
    $node->edit_answer = TRUE; // For validation.
  }
  return drupal_get_form('ls_answ_toform_form', $node);
}

/**
 * Form builder, display a button to fill the survey.
 *
 * @param $node
 *   An answer node object.
 *
 * @see ls_answ_link_submit_form_submit()
 * @ingroup forms
 */
function ls_answ_toform_form($form, &$form_state, $node) {
  $form = array();
  $form['#node'] = $node;
  $type = node_type_get_type($node->type);

  $form['help'] = array(
    '#type' => 'markup',
    '#prefix' => '<div>',
    '#markup' => filter_xss($type->help),
    '#suffix' => '</div>',
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['button'] = array(
    '#type' => 'submit',
    '#value' => t('Go to the survey'),
  );

  return $form;
}

/**
 * Validate the answers button to fill the survey.
 */
function ls_answ_toform_form_validate($form, &$form_state) {
  // The ls_answ_validate() and node_access() functions have been already
  // called by the ls_answ_fill_survey_access() function. No more validations.
}

/**
 * Submit the answers button to fill the survey.
 */
function ls_answ_toform_form_submit($form, &$form_state) {
  $node = $form['#node'];

  module_load_include('inc', 'ls_answ', 'ls_answ.form');
  $display_mode = ls_answ_fix_display_mode($node->type);

  switch ($display_mode) {
    case 'iframe':
    case 'colorbox':
    case 'lightbox2':
    case 'modalframe':
    case 'link':
      $formated_nid = (!empty($node->nid)) ? '/' . $node->nid : '';
      $redirect = 'form/' . $node->type . $formated_nid;
      break;

    case 'redirect':
    case 'form':
    default:
      // Set the invitation.
      module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
      $survey_properties = limesurvey_sync_survey_properties($node->ls_sid);
      if ($survey_properties['tokens_table'] && !$survey_properties['tokenanswerspersistence']) {
        module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
        $node->ls_token = limesurvey_sync_answ_set_invit($node->type, $node->ls_sid, $node->ls_lang, $node->ls_token);
      }

      module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
      $redirect = limesurvey_sync_answ_surveyurl($node);
  }
  $form_state['redirect'] = $redirect;
}
