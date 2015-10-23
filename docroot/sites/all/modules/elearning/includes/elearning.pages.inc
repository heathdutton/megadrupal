<?php

/**
 * @file
 * Page callbacks for different Elearning pages.
 */

/**
 * Page callback for viewing the elearning entities.
 */
function elearning_entity_page_view($entity_type, $entity) {
  $view = entity_view($entity_type, array($entity), 'default');
  $content = render($view);
  return $content;
}

/**
 * Practice exercise result page callback.
 */
function elearning_exercise_result_page($exercise_result, $js) {

  $view = views_embed_view('elearning_question_results_per_exercise', 'default', $exercise_result->result_id);
  $result_wrapper = entity_metadata_wrapper('elearning_exercise_result', $exercise_result);
  $exercise = $result_wrapper->exercise->value();

  $output = theme('elearning_exercise_result_page', array(
      'question_results' => $view,
      'score_raw' => $result_wrapper->score->value(),
      'score_formatted' => $result_wrapper->score_formatted->value(),
      'status' => $result_wrapper->status->value(),
      'exercise' => $exercise,
      'exercise_result' => $exercise_result,
    ));

  if (!$js) {
    return $output;
  }
  else {
    ctools_include('ajax');
    ctools_include('modal');
    ctools_modal_add_js();
    $commands = array();
    $commands[] = ctools_modal_command_display(t('Results for @exercise' , array('@exercise' => entity_label('elearning_exercise',$exercise))), $output);
    print ajax_render($commands);
    exit;
  }
}

/**
 * Page title callback for exercise result modal.
 */
function elearning_exercise_result_page_title($exercise){
  return t('Results for @exercise' , array('@exercise' => entity_label('elearning_exercise',$exercise)));
}

/**
 * Implements template_preprocess_HOOK().
 *
 * @todo
 *   Template suggestion not working
 */
function template_preprocess_elearning_exercise_result_page(&$variables) {
  $status = $variables['status'];
  if ($status == ELEARNING_STATUS_FAILED) {
    $variables['classes_array'][] = 'failed';
  }
  if ($status == ELEARNING_STATUS_PASSED) {
    $variables['classes_array'][] = 'passed';
  }
  $exercise = $variables['exercise'];
  $variables['theme_hook_suggestions'][] = 'elearning_exercise_result_page__' . $exercise->type;
}

/**
 * Modal for resetting the result.
 */
function elearning_entity_result_reset($js, $entity) {

  $entity_type = $entity->entityType();
  $entity_info = entity_get_info($entity_type);

  if (!$js) {
    return t("This function doesn't work without javascript activated.");
  }
  else {

    ctools_include('modal');
    ctools_include('ajax');
    ctools_add_js('ajax-responder');

    $args = array();
    $args[] = $entity_type;
    $args[] = $entity;
    if ($entity_type == 'elearning_unit') {
      $wrapper = entity_metadata_wrapper('elearning_unit', $entity);
      $course = $wrapper->course->value();
      $args[] = $course;
    }

    $form_state = array(
      'title' => t('Are you sure?'),
      'ajax' => TRUE,
      'build_info' => array('args' => $args),
    );
    form_load_include($form_state, 'inc', 'elearning', 'includes/elearning.result');
    $commands = ctools_modal_form_wrapper('elearning_entity_result_reset_form', $form_state);

    if ($form_state['executed'] == TRUE) {
      $commands[] = ctools_ajax_command_reload();
    }
    elseif ($form_state['executed'] == FALSE && isset($form_state['triggering_element'])) {
      if ($form_state['triggering_element']['#name'] == "cancel") {
        $commands[] = ctools_modal_command_dismiss();
      }
    }
  }
  print ajax_render($commands);
}

/**
 * Callback for resetting a Elearning entity result.
 */
function elearning_entity_result_reset_form($form, &$form_state) {
  $entity_type = $form_state['build_info']['args'][0];
  $entity_info = entity_get_info($entity_type);
  $info = '';
  switch ($entity_type) {
    case 'elearning_course':
      $info = t("You're about to restart this course. This means that you will
      be able to redo all the units and exercises that are part of this coures.
      Previous results will remain saved. Are you sure?");
      break;

    case 'elearning_unit':
      $info = t("You're about to start this unit again. This means that you will
      be able to redo all the exercises that are part of this unit. Previous
      results will remain saved. Are you sure? ");
      break;
  }
  $form['information'] = array(
    '#markup' => $info,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Reset this @entity_type', array('@entity_type' => strtolower($entity_info['label']))),
    '#name' => 'submit',
    '#weight' => 40,
  );
  $form['actions']['cancel'] = array(
    '#type' => 'button',
    '#value' => t("Cancel"),
    '#weight' => 41,
    '#name' => 'cancel',
  );
  return $form;
}

/**
 * Submit callback for the retry form.
 */
function elearning_entity_result_reset_form_submit($form, &$form_state) {

  $entity_type = $form_state['build_info']['args'][0];
  $entity = $form_state['build_info']['args'][1];
  $account = $GLOBALS['user'];
  switch ($entity_type) {
    case 'elearning_course':
      $values = array(
        'course_id' => $entity->course_id,
        'uid' => $account->uid,
        'started' => REQUEST_TIME,
        'status' => ELEARNING_STATUS_STARTED,
        'progress' => 0,
      );
      $course_result = entity_create('elearning_course_result', $values);
      entity_save('elearning_course_result', $course_result);
      drupal_set_message(t('You have succesfully restarted this course.'));
      break;

    case 'elearning_unit':

      $unit_wrapper = entity_metadata_wrapper('elearning_unit', $entity);
      $course_result = $unit_wrapper->course->result->value();

      $values = array(
        'unit_id' => $entity->unit_id,
        'course_result' => $course_result->result_id,
        'uid' => $account->uid,
        'started' => REQUEST_TIME,
        'status' => ELEARNING_STATUS_STARTED,
        'progress' => 0,
      );
      $unit_result = entity_create('elearning_unit_result', $values);
      entity_save('elearning_unit', $unit_result);

      // Open up the course result again.
      $course_result->status = ELEARNING_STATUS_STARTED;
      entity_save('elearning_course_result', $course_result);

      drupal_set_message(t('You have succesfully restarted this unit.'));
      break;
  }
}
