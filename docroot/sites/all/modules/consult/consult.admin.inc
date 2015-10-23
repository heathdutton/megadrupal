<?php
/**
 * @file
 * Admin hooks for Consult module.
 */

/**
 * Generates the profile type editing form.
 */
function consult_interview_form($form, &$form_state, ConsultInterview $entity, $op = 'edit', $entity_type = NULL) {
  if ($op == 'clone') {
    // Only label is provided for cloned entities.
    $entity->label .= ' (cloned)';
    $entity->type = $entity_type . '_clone';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($entity->label) ? $entity->label : '',
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine-readable name'),
    '#required' => TRUE,
    '#default_value' => isset($entity->name) ? $entity->name : '',
    '#machine_name' => array(
      'exists' => 'consult_interview_name_exists',
      'source' => array('label'),
    ),
    '#access' => $op == 'add' || $op == 'clone',
    '#maxlength' => 32,
  );

  $form['settings']['#tree'] = TRUE;

  $form['settings'][CONSULT_QUESTION_ENTITY_NAME] = array(
    '#type' => 'fieldset',
    '#title' => t('Questions'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );
  $form['settings'][CONSULT_QUESTION_ENTITY_NAME] += consult_interview_entity_sort_table_form($entity->getQuestions(), array(
    'settings',
    CONSULT_QUESTION_ENTITY_NAME,
  ));

  $form['settings'][CONSULT_RESULT_ENTITY_NAME] = array(
    '#type' => 'fieldset',
    '#title' => t('Results'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );
  $form['settings'][CONSULT_RESULT_ENTITY_NAME] += consult_interview_entity_sort_table_form($entity->getResults(), array(
    'settings',
    CONSULT_RESULT_ENTITY_NAME,
  ));

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save consultation'),
    '#weight' => 40,
  );

  // Remove broken EntityAPI validation handlers.
  $form['#validate'] = array();

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function consult_interview_form_submit(&$form, &$form_state) {
  $consultation = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $consultation->save();

  $form_state['redirect'] = 'admin/structure/consult/manage/' . $consultation->identifier();
}

/**
 * Check to see whether a consultation with the given name exists.
 *
 * @param string $interview_name
 *   The machine name to check.
 *
 * @return bool
 *   Whether or not the machine name exists.
 */
function consult_interview_name_exists($interview_name) {
  $existing_entity = entity_load_single(CONSULT_INTERVIEW_ENTITY_NAME, $interview_name);

  return !empty($existing_entity);
}

/**
 * Form API callback for question form.
 */
function consult_question_form($form, &$form_state, $question, $interview, $op = 'edit') {
  if ($op == 'add') {
    $question = entity_create(CONSULT_QUESTION_ENTITY_NAME, array(
      'interview_name' => $interview->identifier(),
    ));
  }

  if (empty($form_state['entity'])) {
    $form_state['entity'] = $question;
  }
  else {
    $question = $form_state['entity'];
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($question->label) ? $question->label : '',
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine-readable name'),
    '#required' => TRUE,
    '#default_value' => isset($question->name) ? $question->name : '',
    '#machine_name' => array(
      'exists' => 'consult_question_name_exists',
      'source' => array('label'),
    ),
    '#access' => $op == 'add' || $op == 'clone',
    '#maxlength' => 32,
  );

  $form['question'] = array(
    '#type' => 'text_format',
    '#title' => t('Question'),
    '#default_value' => $question->getQuestionText(FALSE),
    '#format' => $question->getQuestionFormat(),
  );

  $form['answers'] = array(
    '#type' => 'textarea',
    '#title' => t('Answers'),
    '#description' => t('The possible answers. Enter one value per line, in the format key|label.'),
    '#default_value' => list_allowed_values_string($question->getAnswers()),
  );

  $form['group_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Group'),
    '#default_value' => $question->getGroupName(),
  );

  $form['dependencies'] = _consult_admin_dependency_form_element($question, $interview);

  $form['interview_name'] = array(
    '#type' => 'value',
    '#value' => $interview->identifier(),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save question'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Submit handler for question entity form.
 */
function consult_question_form_submit($form, &$form_state) {
  $question = $form_state['entity'];

  // Convert pipe separated answer list into array for storage.
  $answers = list_extract_allowed_values($form_state['values']['answers'], 'list_text', FALSE);
  $form_state['values']['answers'] = $answers;

  unset($form_state['values']['dependencies']['add_another']);

  // Remove any dependencies checked with the 'remove' checkbox.
  foreach ($form_state['values']['dependencies'] as $delta => &$dependency) {
    if (!empty($dependency['remove'])) {
      unset($form_state['values']['dependencies'][$delta]);
    }
    unset($dependency['remove']);
  }

  foreach ($form_state['values'] as $key => $value) {
    $question->{$key} = $value;
  }

  entity_save(CONSULT_QUESTION_ENTITY_NAME, $question);
  $form_state['redirect'] = 'admin/structure/consult/manage/' . $form_state['values']['interview_name'];
}

/**
 * Form API callback for result deletion form.
 */
function consult_question_delete_form($form, $form_state, ConsultQuestion $question) {
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $question->identifier(),
  );

  $form['interview_name'] = array(
    '#type' => 'value',
    '#value' => $question->interview_name,
  );

  drupal_set_title(t('Are you sure you want to delete @title?', array(
    '@title' => $question->label(),
  )));

  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Yes'),
  );

  return $form;
}

/**
 * Form API submit handler for result deletion form.
 */
function consult_question_delete_form_submit($form, &$form_state) {
  entity_delete(CONSULT_QUESTION_ENTITY_NAME, $form_state['values']['name']);

  $form_state['redirect'] = 'admin/structure/consult/manage/' . $form_state['values']['interview_name'];
}

/**
 * Form API callback for question form.
 */
function consult_result_form($form, $form_state, $result, $interview, $op = 'edit') {
  if ($op == 'add') {
    $result = entity_create(CONSULT_RESULT_ENTITY_NAME, array(
      'interview_name' => $interview->identifier(),
    ));
  }

  if (empty($form_state['entity'])) {
    $form_state['entity'] = $result;
  }
  else {
    $result = $form_state['entity'];
  }

  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => CONSULT_QUESTION_ENTITY_NAME,
  );

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($result->label) ? $result->label : '',
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine-readable name'),
    '#required' => TRUE,
    '#default_value' => isset($result->name) ? $result->name : '',
    '#machine_name' => array(
      'exists' => 'consult_result_name_exists',
      'source' => array('label'),
    ),
    '#access' => $op == 'add' || $op == 'clone',
    '#maxlength' => 32,
  );

  $form['message'] = array(
    '#type' => 'text_format',
    '#title' => t('Message'),
    '#default_value' => $result->getMessageText(FALSE),
    '#format' => $result->getMessageFormat(),
  );

  $form['group_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Group'),
    '#default_value' => $result->getGroupName(),
  );

  $form['dependencies'] = _consult_admin_dependency_form_element($result, $interview);

  $other_results = $interview->getResults();
  $other_result_options = array();
  foreach ($other_results as $other_result) {
    $other_result_options[$other_result->getGroupName()][$other_result->identifier()] = $other_result->label();
  }

  $form['inherited_dependencies'] = array(
    '#title' => t('Inherited dependencies'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => $other_result_options,
    '#description' => t('If this result requires another result to be displayed, enter it here.'),
    '#default_value' => $result->getInheritedDependencies(),
  );

  $form['interview_name'] = array(
    '#type' => 'value',
    '#value' => $interview->identifier(),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save result'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Submit handler for question entity form.
 */
function consult_result_form_submit($form, &$form_state) {
  $question = $form_state['entity'];

  // Convert pipe separated answer list into array for storage.
  $answers = list_extract_allowed_values($form_state['values']['answers'], 'list_text', FALSE);
  $form_state['values']['answers'] = $answers;

  unset($form_state['values']['dependencies']['add_another']);

  // Remove any dependencies checked with the 'remove' checkbox.
  foreach ($form_state['values']['dependencies'] as $delta => &$dependency) {
    if (!empty($dependency['remove'])) {
      unset($form_state['values']['dependencies'][$delta]);
    }

    unset($dependency['remove']);
  }

  foreach ($form_state['values'] as $key => $value) {
    $question->{$key} = $value;
  }

  entity_save(CONSULT_RESULT_ENTITY_NAME, $question);
  $form_state['redirect'] = 'admin/structure/consult/manage/' . $form_state['values']['interview_name'];
}

/**
 * Form API callback for result deletion form.
 */
function consult_result_delete_form($form, $form_state, ConsultResult $result) {
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $result->identifier(),
  );

  $form['interview_name'] = array(
    '#type' => 'value',
    '#value' => $result->interview_name,
  );

  drupal_set_title(t('Are you sure you want to delete @title?', array(
    '@title' => $result->label(),
  )));

  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Yes'),
  );

  return $form;
}

/**
 * Form API submit handler for result deletion form.
 */
function consult_result_delete_form_submit($form, &$form_state) {
  entity_delete(CONSULT_RESULT_ENTITY_NAME, $form_state['values']['name']);

  $form_state['redirect'] = 'admin/structure/consult/manage/' . $form_state['values']['interview_name'];
}

/**
 * Check to see whether a question with the given name exists.
 *
 * @param string $question_name
 *   The machine name to check.
 *
 * @return bool
 *   Whether or not the machine name exists.
 */
function consult_question_name_exists($question_name) {
  // Prevent reserved words from being used.
  if ($question_name == 'remove') {
    return TRUE;
  }

  $existing_entity = entity_load_single(CONSULT_QUESTION_ENTITY_NAME, $question_name);

  return !empty($existing_entity);
}

/**
 * Check to see whether a result with the given name exists.
 *
 * @param string $result_name
 *   The machine name to check.
 *
 * @return bool
 *   Whether or not the machine name exists.
 */
function consult_result_name_exists($result_name) {
  // Prevent reserved words from being used.
  if ($result_name == 'remove') {
    return TRUE;
  }

  $existing_entity = entity_load_single(CONSULT_RESULT_ENTITY_NAME, $result_name);

  return !empty($existing_entity);
}

/**
 * Linked entity management form for a given interview grouped by group names.
 *
 * @param array $entities
 *   The entities to arrange in the sort table.
 * @param array $parent_trail
 *   The Form API parent trail for this element.
 *
 * @return array
 *   The CRUD form for the question entities associated with the interview.
 */
function consult_interview_entity_sort_table_form($entities, $parent_trail) {
  $form = array();

  $grouped_entities = array();

  foreach ($entities as $entity) {
    $grouped_entities[$entity->getGroupName()][] = $entity;
  }

  foreach ($grouped_entities as $group_name => $entities) {
    $table_id = drupal_html_id('consult_entity_sort_table');
    $rows = array();
    $row_elements = array();

    // Build the rows.
    foreach ($entities as $entity) {
      $id = $entity->identifier();

      $question_trail = $parent_trail;
      $question_trail[] = $id;

      $weight_trail = $question_trail;
      $weight_trail[] = 'weight';

      $shown_trail = $question_trail;
      $shown_trail[] = 'shown';

      $enabled_trail = $question_trail;
      $enabled_trail[] = 'enabled';

      $rows[$id]['data']['cross'] = array(
        'class' => array('entry-cross'),
      );

      $rows[$id]['data']['weight'] = array(
        'data' => array(
          '#type' => 'weight',
          '#title' => t('Weight'),
          '#title_display' => 'invisible',
          '#parents' => $weight_trail,
          '#attributes' => array(
            'class' => array('entry-order-weight'),
          ),
          '#default_value' => $entity->getWeight(),
          '#delta' => 50,
        ),
      );

      $rows[$id]['data']['enabled'] = array(
        'data' => array(
          '#type' => 'checkbox',
          '#title' => t('Enable'),
          '#title_display' => 'invisible',
          '#parents' => $enabled_trail,
          '#default_value' => (int) $entity->isEnabled(),
        ),
      );

      $rows[$id]['data']['method'] = array(
        'data' => array(
          '#type' => 'select',
          '#title' => t('Method'),
          '#title_display' => 'invisible',
          '#parents' => $shown_trail,
          '#options' => array(
            0 => t('Hidden by default'),
            1 => t('Shown by default'),
          ),
          '#default_value' => $entity->getShown(),
        ),
      );

      $rows[$id]['data']['name'] = array(
        'data' => array(
          '#markup' => $entity->label() . ' (' . $id . ')',
        ),
      );
      $rows[$id]['data']['group'] = array(
        'data' => array(
          '#markup' => $entity->getGroupName(),
        ),
      );

      $rows[$id]['data']['op'] = array(
        'data' => array(
          '#theme' => 'link',
          '#text' => t('Edit'),
          '#path' => $entity->getOperationPath('edit'),
          '#options' => array('attributes' => array(), 'html' => FALSE),
        ),
      );

      $rows[$id]['data']['link'] = array(
        'data' => array(
          '#theme' => 'link',
          '#text' => t('Delete'),
          '#path' => $entity->getOperationPath('delete'),
          '#options' => array('attributes' => array(), 'html' => FALSE),
        ),
      );
      $rows[$id]['class'] = array('draggable');

      // Build rows of the form elements in the table.
      $row_elements[$id] = array(
        'weight' => &$rows[$id]['data']['weight']['data'],
        'enabled' => &$rows[$id]['data']['enabled']['data'],
        'method' => &$rows[$id]['data']['method']['data'],
        'name' => &$rows[$id]['data']['name']['data'],
        'group' => &$rows[$id]['data']['group']['data'],
      );
    }

    $header = array(
      array('data' => NULL, 'colspan' => 2),
      t('Enabled'),
      t('Method'),
      t('Name'),
      t('Group'),
      array('data' => t('Operations'), 'colspan' => 2),
    );

    // Add the table to the form.
    $form[$group_name]['table'] = array(
      '#caption' => $group_name,
      '#theme' => 'table',
      'elements' => $row_elements,
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('There are no entries available.'),
      '#attributes' => array(
        'id' => $table_id,
      ),
      '#attached' => array(
        'css' => array(
          drupal_get_path('module', 'consult') . '/css/consult.admin.css',
        ),
      ),
    );
    drupal_add_tabledrag($table_id, 'order', 'sibling', 'entry-order-weight');
  }

  return $form;
}

/**
 * Ajax powered dependency subform for managing for Interview sub entities.
 *
 * @param ConsultInterviewSubEntity $entity
 *   The interview sub entity to get the dependency form for.
 * @param ConsultInterview $interview
 *   The parent interview entity.
 *
 * @return array
 *   The Form API form builder array element.
 */
function _consult_admin_dependency_form_element(ConsultInterviewSubEntity $entity, ConsultInterview $interview) {
  $ajax_id = drupal_html_id('consult-dependency-form');

  $element = array(
    '#type' => 'fieldset',
    '#title' => t('Dependencies'),
    '#interview' => $interview,
    '#tree' => TRUE,
    '#attributes' => array(
      'id' => $ajax_id,
      'class' => array(
        'consult-dependency-form',
      ),
    ),
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'consult') . '/css/consult.admin.css',
      ),
    ),
  );

  $questions = $interview->getQuestions();
  $options = _consult_admin_get_entities_as_options($questions);
  unset($options[$entity->identifier()]);

  $values = _consult_admin_get_entity_answers_as_options($questions);
  unset($values[$entity->identifier()]);

  $submit_handler = '_consult_admin_dependency_form_element_add_question_submit';

  $dependencies = $entity->getDependencies();

  foreach ($dependencies as $delta => $dependency) {
    $element[$delta] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('Dependency @num', array(
        '@num' => $delta,
      )),
    );

    foreach (array_keys($options) as $question_name) {
      $value = $entity->getValueForQuestionInDependency($question_name, $delta);

      $element[$delta][$question_name] = array(
        '#type' => 'select',
        '#title' => $question_name,
        '#default_value' => $value,
        '#options' => $values[$question_name],
      );
    }

    $element[$delta]['remove'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove'),
      '#default_value' => 0,
    );
  }

  $element['add_another'] = array(
    '#type' => 'submit',
    '#value' => t('Add dependency'),
    '#ajax' => array(
      'callback' => '_consult_admin_dependency_form_element_add_callback',
      'wrapper' => $ajax_id,
      'method' => 'replace',
      'effect' => 'none',
    ),
    '#submit' => array($submit_handler),
  );

  // Render in table.
  $element['#pre_render'][] = '_consult_admin_dependency_form_element_pre_render';

  return $element;
}

/**
 * Truth table page callback.
 *
 * @param ConsultInterview $interview
 *   The interview to render the truth table for.
 *
 * @return string
 *   Truth table.
 */
function consult_interview_truth_page(ConsultInterview $interview) {
  $questions = $interview->getQuestions();
  $results = $interview->getResults();

  $sub_entities = array_merge($results, $questions);

  $rows = array();

  foreach ($questions as $question) {
    $header[$question->identifier()] = $question->identifier();
  }
  $header['result'] = t('Result');

  foreach ($sub_entities as $sub_entity) {
    $dependencies = $sub_entity->getDependencies();

    foreach ($dependencies as $dependency) {
      $row = $header;

      foreach ($dependency as $question_name => $question_value) {
        $row[$question_name] = $question_value;
      }

      $row['result'] = $sub_entity->label();
      $rows[] = $row;
    }
  }

  return array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
  );
}

/**
 * Form API pre_render callback for dependency form element.
 */
function _consult_admin_dependency_form_element_pre_render($element) {
  $rows = array();
  $headers = array();

  foreach (element_children($element) as $delta) {
    if ($delta === 'add_another') {
      continue;
    }

    $dependency = $element[$delta];

    foreach (element_children($dependency) as $question_name) {
      $question_element = $dependency[$question_name];
      $headers[$question_name] = $question_element['#title'];

      $question_element['#title_display'] = 'invisible';
      $rows[$delta][$question_name]['data'] = render($question_element);
    }
    unset($element[$delta]);
  }

  // Add column widths to evenly space table.
  foreach ($rows as $row_k => $row) {
    foreach ($row as $column_k => $column) {
      $rows[$row_k][$column_k]['width'] = floor(100 / count($row)) . '%';
    }
  }

  array_unshift($element, array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#empty' => t('No dependencies'),
    '#header' => $headers,
  ));

  return $element;
}

/**
 * Ajax callback for dependency form element.
 */
function _consult_admin_dependency_form_element_add_callback($form, $form_state) {
  $triggering_element_parents = $form_state['triggering_element']['#parents'];
  array_pop($triggering_element_parents);

  $dependency_form_item = array();

  foreach ($triggering_element_parents as $parent_name) {
    if (empty($dependency_form_item)) {
      $dependency_form_item = $form[$parent_name];
    }
    else {
      $dependency_form_item = $dependency_form_item[$parent_name];
    }
  }

  return $dependency_form_item;
}

/**
 * Form API submit handler to force rebuilding of form.
 */
function _consult_admin_dependency_form_element_add_question_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['entity']->addNewDependency();
}

/**
 * Get a form value from the form state of a given form.
 *
 * @param string $key
 *   The form value key.
 * @param array $form_state
 *   The form state to extract the value from.
 *
 * @return mixed
 *   FALSE if not found or the value in the form_state array.
 */
function _consult_interview_get_form_value($key, $form_state) {
  $value = FALSE;
  if (!empty($form_state['values']) || !empty($form_state['values'][$key])) {
    $value = $form_state['values'][$key];
  }

  return $value;
}

/**
 * Create an options array from a set of queried entities.
 *
 * @param array $entities
 *   Entities (of any Entity API type).
 *
 * @return array
 *   The entity options, keyed by id and labelled by label.
 */
function _consult_admin_get_entities_as_options($entities) {
  $options = array();

  foreach ($entities as $entity) {
    $options[$entity->identifier()] = $entity->label();
  }

  return $options;
}

/**
 * Create a multidimensional options array of question and answers.
 *
 * @param array $questions
 *   A list of question entities.
 *
 * @return array
 *   Answers arrays keyed by question names.
 */
function _consult_admin_get_entity_answers_as_options($questions) {
  $options = array();

  foreach ($questions as $entity) {
    $options[$entity->identifier()] = array('' => '-') + $entity->getAnswers();
  }

  return $options;
}
