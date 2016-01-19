<?php

/**
 * @file
 * Defines two new field types for the Elearning module.
 *
 * 1. A custom formatter that puts the questions of each exercise in a form
 *    that can be submitted.
 * 2. A new field type for the question answers.
 */

/**
 * Implements hook_field_info().
 */
function elearning_field_info() {
  return array(
    'elearning_answer_field' => array(
      'label' => t('Elearning answer field'),
      'description' => t('Field for saving the possible and correct answers to a question.'),
      'default_widget' => 'elearning_answer_widget',
      'default_formatter' => 'elearnign_answer_formatter',
      'no_ui' => TRUE,
      'property_type' => 'text',
      'property_callbacks' => array('elearning_question_field_property_info_callback'),
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function elearning_field_is_empty($item, $field) {
  $return = empty($item['key']) && $item['value'] == FALSE ? TRUE : FALSE;
  return $return;
}

/**
 * Implements hook_field_formatter_info().
 *
 * We create our own entity reference formatter, because the original
 * entityreference module doesn't check if the user has access to the entity
 * based on the parent entity to which the entity was linked.
 *
 * Since the field is not passed in anyway
 */
function elearning_field_formatter_info() {
  return array(
    'elearning_question_reference_formatter' => array(
      'label' => t('Elearning Questions'),
      'field types' => array('entityreference'),
    ),
    'elearning_answer_formatter' => array(
      'label' => t('Elearning answer formatter'),
      'field types' => array('elearning_answer_field'),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 *
 * Part of this code was taken from entityreference module.
 */
function elearning_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  if ($display['type'] == 'elearning_entity_reference_formatter') {
    $entity_info = entity_get_info($field['settings']['target_type']);
    $options = array('default' => t('Default'));
    if (!empty($entity_info['view modes'])) {
      foreach ($entity_info['view modes'] as $view_mode => $view_mode_settings) {
        $options[$view_mode] = $view_mode_settings['label'];
      }
    }

    $element['view_mode'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('View mode'),
      '#default_value' => $settings['view_mode'],
      '#access' => count($options) > 1,
    );

    $element['links'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show links'),
      '#default_value' => $settings['links'],
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_prepare_view().
 *
 * Allow submodules to alter the questions before they get added to
 * the exercise form. This can be useful for shuffling the questions
 * or picking a random selection.
 */
function elearning_field_formatter_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items, $displays) {
  if ($entity_type == "elearning_exercise") {
    foreach ($entities as $exercise) {
      $items_altered = $items[$exercise->exercise_id];
      drupal_alter('elearning_question_reference_items', $items_altered, $exercise);
      $items[$exercise->exercise_id] = $items_altered;
    }
  }
}

/**
 * Implements hook_field_formatter_view().
 *
 * @see elearning_exercise_formatter_form()
 * @see elearning_answer_field_process_answer()
 */
function elearning_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $element = array();

  $settings = $display['settings'];
  $exercise = $entity;

  switch ($display['type']) {
    case 'elearning_question_reference_formatter':
      $exercise_from_path = menu_get_object('elearning_exercise', 5);
      if ($field['field_name'] != "elearning_question_reference") {
        drupal_set_message(t('The Question formatter only works on the elearning_question_reference field in the exercise'), 'error');
      }
      elseif (!$exercise_from_path) {
        drupal_set_message(t('The Question formatter can only be displayed in a page with the course, unit and exercise as path arguments.'), 'error');
      }
      else {
        $questions = array();
        foreach ($items as $key => $question) {
          $question = entity_load_single('elearning_question', $question['target_id']);

          // Add a weight key to question object, so we know its position. When
          // generating the form element and when saving the question results.
          $question->weight = $key;
          $questions[] = $question;
        }
        $form = drupal_get_form('elearning_exercise_formatter_form', $exercise, $questions);
        $element = $form;
        return $element;
      }
      break;

    case 'elearning_answer_formatter':
      $question = $entity;

      // Invoke the question_form_element hook.
      $element = module_invoke($question->type, 'elearning_question_form_element', $question, $items);

      if (empty($element)) {
        drupal_set_message(t("The elearning_question_form_element has not
        been defined in the question type submodule."), 'warning');
      }

      // Allow to alter the question form element.
      drupal_alter('elearning_question_form_element', $element, $question->type);

      $element['#element_validate'] = array('elearning_answer_field_process_answer');
      $element['#parents'] = array('answer', $question->question_id);
      $element['#question'] = $question;
      return array(0 => array('element' => $element));
  }
}

/**
 * Implements hook_field_widget_info().
 */
function elearning_field_widget_info() {
  return array(
    'elearning_answer_widget' => array(
      'label' => t('Elearning Question Answer Widget'),
      'field types' => array('elearning_answer_field'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
      'weight' => 2,
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 *
 * @see theme_question_field_form_table
 */
function elearning_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  $count = isset($form_state['count']) ? $form_state['count'] : (!empty($items) ? count($items) : 1);

  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#name'] == "update_rows") {
    $op = $form_state['triggering_element']['#op'];
    $op == "add" ? $count++ : $count--;
  }

  $form_state['count'] = $count;

  $element += array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#prefix' => '<div id="elearning-answer-field-table">',
    '#suffix' => '</div>',
    '#element_validate' => array('elearning_question_answer_widget_validate'),
    '#theme' => 'elearning_question_field_form_table',
  );

  $element['#table_columns'] = array(
    'key' => t('Label'),
    'value' => t('Correct answer'),
  );

  for ($key = 0; $key < $count; $key++) {
    $element['key'][$key] = array(
      '#type' => 'textfield',
      '#default_value' => isset($items[$key]['key']) ? $items[$key]['key'] : '',
      '#parent' => 'embed',
    );
    $element['value'][$key] = array(
      '#type' => 'textfield',
      '#default_value' => isset($items[$key]['value']) ? $items[$key]['value'] : '',
      '#parent' => 'embed',
    );
  }
  // Add and remove buttons.
  $element['add_row'] = array(
    '#type' => 'button',
    '#value' => t('Add another'),
    '#op' => 'add',
    '#name' => 'update_rows',
    '#submit' => array('elearning_question_answer_form_update_row'),
    '#ajax' => array(
      'callback' => 'elearning_question_answer_form_update_row_ajax',
      'wrapper' => 'elearning-answer-field-table',
    ),
  );
  $element['remove_row'] = array(
    '#type' => 'button',
    '#value' => t('Remove last row'),
    '#op' => 'remove',
    '#name' => 'update_rows',
    '#submit' => array('elearning_question_answer_form_update_row'),
    '#ajax' => array(
      'callback' => 'elearning_question_answer_form_update_row_ajax',
      'wrapper' => 'elearning-answer-field-table',
    ),
  );
  // Hide 'remove last row' if there´s only one row.
  if ($count == 1) {
    $element['remove_row']['#type'] = 'hidden';
  }

  $question = $element['#entity'];

  drupal_alter('elearning_question_widget', $element, $form_state, $question, $form);

  return $element;
}

/**
 * Validate callback for the question answer formatter.
 *
 * Validate function to convert the 'array' shaped output into a string which
 * can be saved to the database.
 */
function elearning_question_answer_widget_validate($element, &$form_state, $form) {
  $columns = array('key', 'value');
  $items = array();
  foreach ($columns as $column) {
    foreach (element_children($element[$column]) as $key) {
      if (isset($element[$column][$key]['#value'])) {
        $items[$key][$column] = $element[$column][$key]['#value'];
      }
    }
  }
  form_set_value($element, $items, $form_state);
}

/**
 * Callback for adding and removing questions.
 */
function elearning_question_answer_form_update_row($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for adding or removing rows.
 */
function elearning_question_answer_form_update_row_ajax($form, &$form_state) {
  return $form['answer'][LANGUAGE_NONE];
}

/**
 * Form callback to put the questions in a form().
 *
 * Submodules can change the form by using the hook_exercise_formatter_form hook
 * They are responsible for actually saving an exercise result entity, adding
 * start buttons and save buttons, etc.
 *
 * @todo  move $form_state['#...'] to $form_state['build_info']['args'].
 */
function elearning_exercise_formatter_form($form, &$form_state, $exercise, $questions) {

  $exercise_wrapper = entity_metadata_wrapper('elearning_exercise', $exercise);

  if (empty($form_state['#course'])) {
    $course = $exercise_wrapper->unit->course->value();
    $unit = $exercise_wrapper->unit->value();
    $result = $exercise_wrapper->result->value();

    $form_state['#course'] = $course;
    $form_state['#unit'] = $unit;
    $form_state['#exercise'] = $exercise;
    $form_state['#result'] = $result;
  }

  $entity_info = entity_get_info('elearning_exercise');
  $bundle_name = $entity_info['bundles'][$exercise->type]['label'];

  $questions = isset($form_state['#questions']) ? $form_state['#questions'] : $questions;

  $form_state['#questions'] = $questions;

  $current = 0;
  $total = count($questions);
  $view_mode = $exercise->type;

  $form['questions']['#tree'] = TRUE;
  $questions_renderable = entity_view('elearning_question', $questions, $view_mode);
  $form['questions'] += $questions_renderable['elearning_question'];

  // Add the weight to each question element, which defines the position in the
  // form and will be used to save with the question result.
  $question_children = element_children($form['questions']);
  foreach ($question_children as $weight => $question_id) {
    $form['questions'][$question_id]['answer'][0]['element']['#weight'] = $weight;
  }
  $exercise_type = $exercise->type;

  // We don't add the submit button here but in the exercise submodule.
  // Since the exact behaviour depends on the exercise type.
  drupal_alter('elearning_exercise_formatter_form', $form, $form_state, $exercise_type);

  // We do add a submit callback for saving exercise results.

  $form['#submit'] = array('elearning_exercise_formatter_form_submit');
  return $form;
}

/**
 * Validates each answer and saves an question result.
 */
function elearning_answer_field_process_answer($element, &$form_state, $form) {
  $question = $element['#question'];

  // Invoke the hook for checking whether the answer was correct or not.
  $correct = module_invoke($question->type, 'elearning_question_check_correct', $question, $element, $form_state, $form);

  // Invoke the hook for saving the users ansers.
  $given_answer = module_invoke($question->type, 'elearning_question_given_answer', $question, $element, $form_state, $form);

  $account = $GLOBALS['user'];
  $weight = $element['#weight'];

  $values = array(
    'question_id' => $question->question_id,
    'given_answer' => serialize($given_answer),
    'correct' => $correct,
    'uid' => $account->uid,
    'weight' => $weight,
  );
  $question_result = entity_create('elearning_question_result', $values);
  // We set the question result in the form state for saving upon an ajax call
  // Or on form submit. Saving of questions occurs in the exercise type module.
  $form_state['question_results'][$question->question_id] = $question_result;
}
