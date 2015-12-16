<?php
/**
 * @file
 * Functionality and helper functions for Captcha questions administration.
 */

/**
 * Implements hook_form().
 */
function captcha_questions_admin_settings($form, &$form_state) {
  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => t("This is just a very simple mechanism to protect from automated bots. To make it as easy for real humans, consider providing the answer with the question. Example '<em>What is Mickeys last name? It's \"Mouse\"</em>'. You could also do simply 'What is 1+1?', but some bots may figure that out."),
  );

  $form['configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Logging options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Disables checkbox if dblog module is not enabled.
  $form['configuration']['captcha_questions_watchdog'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log to watchdog'),
    '#description' => t('Check this box to log all failed submissions to watchdog'),
    '#default_value' => variable_get('captcha_questions_watchdog', 0),
    '#disabled' => module_exists('dblog') ? FALSE : TRUE,
  );

  // Disables checkbox if captcha_questions_dblog module is not enabled.
  $form['configuration']['captcha_questions_dblog'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable logging internal database table'),
    '#description' => t('Check this box to log all failed submissions in separate database table'),
    '#default_value' => variable_get('captcha_questions_dblog', 0),
    '#disabled' => module_exists('captcha_questions_dblog') ? FALSE : TRUE,
  );

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('CAPTCHA'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $question = variable_get('captcha_questions_question', t("What is Mickeys last name? It's Mouse."));
  $form['settings']['captcha_questions_question'] = array(
    '#type' => 'textfield',
    '#title' => t('Question to ask'),
    '#default_value' => $question,
    '#maxlength' => 256,
  );

  $answers = variable_get('captcha_questions_answers', array('Mouse'));
  $form['settings']['captcha_questions_answers'] = array(
    '#type' => 'textarea',
    '#title' => 'Accepted answer(s)',
    '#default_value' => implode("\n", $answers),
    '#description' => t('Please provide one or more accepted answers, one per line. Answers are case-insensitive'),
    '#format' => NULL,
  );

  $description = variable_get('captcha_questions_description', t('Please answer the question'));
  $form['settings']['captcha_questions_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $description,
    '#maxlength' => 256,
  );

  $form['forms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form protection'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="captcha-questions-form-id-wrapper">',
    '#suffix' => '</div>',
  );

  // Getting form_ids.
  if (!isset($form_state['form_ids'])) {
    $form_state['form_ids'] = captcha_questions_get_form_ids();
  }

  // Adding new custom form_ids.
  if (isset($form_state['values']['add_form_name'])) {
    if ($form_state['values']['add_form_name'] != '') {
      $form_state['form_ids'][] = $form_state['values']['add_form_name'];
    }
  }
  
  // Getting previously selected form_ids.
  $form_state['selected_form_ids'] = variable_get('captcha_questions_form_ids', array());

  // Create select list with previously selected form_ids as selected.
  $form['forms']['captcha_questions_form_ids'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc($form_state['form_ids']),
    '#default_value' => array_intersect($form_state['selected_form_ids'], $form_state['form_ids']),
    '#title' => t('What forms should be protected?'),
  );

  // Adding node title and link to webform_ids options text.
  
  if (module_exists('webform')) {
    $webform_forms = captcha_questions_get_webforms();
    foreach ($webform_forms as $webform_id => $webform) {
      $webform_node_link = l($webform['title'], 'node/' . $webform['nid']);
      $webform_form_id_option_text = $webform_id . ' (' . $webform_node_link . ')';
      $form['forms']['captcha_questions_form_ids']['#options'][$webform_id] = $webform_form_id_option_text;
    }
  }

  $form['forms']['add_form_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom form_id'),
    '#default_value' => '',
    '#maxlength' => 256,
  );

  $form['forms']['add_form'] = array(
    '#type' => 'submit',
    '#value' => t('Add custom form_id'),
    '#submit' => array('captcha_questions_add_form'),
    '#ajax' => array(
      'callback' => 'captcha_questions_add_form_callback',
      'wrapper' => 'captcha-questions-form-id-wrapper',
    ),
  );

  $form['#submit'][] = 'captcha_questions_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Implements hook_validate().
 */
function captcha_questions_admin_settings_validate($form, &$form_state) {

  // If question is specified without answer, ask for answer.
  if (!empty($form_state['values']['captcha_questions_question']) && empty($form_state['values']['captcha_questions_answers'])) {
    form_set_error('captcha_questions_answer', t('Please provide an answer'));
  }

  // If answer given, but no question, ask for question.
  if (empty($form_state['values']['captcha_questions_question']) && !empty($form_state['values']['captcha_questions_answers'])) {
    form_set_error('captcha_questions_question', t('Please add a question'));
  }
}

/**
 * Implements hook_submit().
 */
function captcha_questions_admin_settings_submit($form, &$form_state) {
  // If question, answer and description are all empty, delete the variables.
  // The last variable, captcha_questions_form_ids are removed on uninstall.
  if (empty($form_state['values']['captcha_questions_question']) && empty($form_state['values']['captcha_questions_answer'])) {
    variable_del('captcha_questions_question');
    variable_del('captcha_questions_answer');
    variable_del('captcha_questions_description');
  }
  if (empty($form_state['values']['captcha_questions_question']) && empty($form_state['values']['captcha_questions_answers'])) {
    variable_del('captcha_questions_question');
    variable_del('captcha_questions_answers');
    variable_del('captcha_questions_description');
  }

  // Split answers into arrays and set variable manually.
  $answers = explode("\n", $form_state['values']['captcha_questions_answers']);
  $answers = array_map('trim', $answers);
  asort($answers);
  variable_set('captcha_questions_answers', $answers);
  unset($form_state['values']['captcha_questions_answers']);

  $count_protected_form_ids = 0;

  // Counting number of selected form_ids, removing unselected form_ids.
  foreach ($form_state['values']['captcha_questions_form_ids'] as $form_id => $value) {
    if ($form_id === $value) {
      $count_protected_form_ids++;
    }
    else {
      unset($form_state['values']['captcha_questions_form_ids'][$form_id]);
    }
  }

  if ($count_protected_form_ids == 0) {
    drupal_set_message(t('No forms selected'));
  }
  else {
    $message = format_plural($count_protected_form_ids,
      '1 form protected',
      '@count forms protected'
    );

    drupal_set_message($message);
  }
}

/**
 * Helper function to make a list of candidates of form ids.
 */
function captcha_questions_get_form_ids() {

  // Some form nids that might exist on the site.
  $form_ids = array(
    'contact_site_form', 
    'contact_personal_form',
    'user_register_form',
    'user_pass',
    'user_login',
    'user_login_block',
    'forum_node_form',
  );

  // Fetching form ids for all node types.
  foreach (node_type_get_names() as $type => $name) {
    $form_ids[] = 'comment_node_' . $type . '_form';
  }

  // Fetching webform form_ids
  if (module_exists('webform')) {
    $webform_forms = captcha_questions_get_webforms();
    foreach ($webform_forms as $webform_id => $webform) {
      $form_ids[] = $webform_id;
    }
  }

  // Adding custom form_ids. This will add all selected forms.
  $custom_form_ids = variable_get('captcha_questions_form_ids', array());
  foreach ($custom_form_ids as $custom_form_id) {
    $form_ids[] = $custom_form_id;
  }

  // Removing possible duplicates from last step.
  $form_ids = array_unique($form_ids);
  
  return $form_ids;
}

/**
 * Helper function to find webform form_ids, adopted and adapted
 * from the function webform_mollom_form_list() in webform.module.
 *
 * Returns array keyed by webform form_ids and title, nid of the node.
 */
function captcha_questions_get_webforms() {
  $forms = array();
  $webform_node_types = variable_get('webform_node_types', array('webform'));

  $result = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->condition('n.type', $webform_node_types, 'IN')
    ->execute();

  foreach ($result as $node) {
    $form_id = 'webform_client_form_' . $node->nid;
    $forms[$form_id] = array(
      'title' => t('@name form', array('@name' => $node->title)),
      'nid' => $node->nid,
    );
  }
  return $forms;
}

/**
 * Callback for ajax-enabled button.
 *
 * Selects and returns the fieldset with the names in it.
 */
function captcha_questions_add_form_callback($form, $form_state) {
  return $form['forms'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Causes a rebuild.
 */
function captcha_questions_add_form($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}