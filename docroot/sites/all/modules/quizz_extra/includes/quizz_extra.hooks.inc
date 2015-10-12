<?php

/**
 * Implements hook_views_api().
 */
function quizz_extra_views_api() {
  return array('api' => '3.0', 'path' => drupal_get_path('module', 'quizz_extra') . '/includes/views');
}

/**
 * Implements hook_views_default_views().
 */
function quizz_extra_views_default_views() {
  $files = file_scan_directory(drupal_get_path('module', 'quizz_extra') . '/includes/views/defaults', '/\.view\.inc/');
  $views = array();
  foreach (array_keys($files) as $path) {
    $view = require $path;
    $views[$view->name] = $view;
  }
  return $views;
}

/**
 * Implements hook_schema_alter().
 */
function quizz_extra_schema_alter(&$schema) {
  $schema['quiz_entity']['fields']['comment'] = array(
      'description' => 'Whether comments are allowed on this quiz: 0 = no, 1 = closed (read only), 2 = open (read/write).',
      'type'        => 'int',
      'not null'    => TRUE,
      'default'     => 0,
  );

  $schema['quiz_question_entity']['fields']['comment'] = array(
      'description' => 'Whether comments are allowed on this question: 0 = no, 1 = closed (read only), 2 = open (read/write).',
      'type'        => 'int',
      'not null'    => TRUE,
      'default'     => 0,
  );
}

/**
 * Implements hook_enable().
 */
function quizz_extra_enable() {
  if (!db_field_exists('quiz_entity', 'comment')) {
    $spec = array('type' => 'int', 'not null' => TRUE, 'default' => 0);
    db_add_field('quiz_entity', 'comment', $spec);
  }

  if (!db_field_exists('quiz_question_entity', 'comment')) {
    $spec = array('type' => 'int', 'not null' => TRUE, 'default' => 0);
    db_add_field('quiz_question_entity', 'comment', $spec);
  }
}

/**
 * Implements hook_form_FORM_ID_alter()>
 */
function quizz_extra_form_quiz_question_type_form_alter(&$form, $form_state) {
  $form['vtabs']['configuration']['comment'] = array(
      '#type'          => 'select',
      '#title'         => t('Comment'),
      '#default_value' => $form['#question_type']->getConfig('comment', 1),
      '#options'       => array(
          -1 => t('No comment'),
          0  => t('Close comment by default'),
          1  => t('Open comment by default'),
      ),
  );
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function quizz_extra_form_quiz_type_form_alter(&$form, $form_state) {
  $form['vtabs']['configuration']['comment'] = array(
      '#type'          => 'select',
      '#title'         => t('Comment'),
      '#default_value' => $form['#quiz_type']->getConfig('comment', 1),
      '#options'       => array(
          -1 => t('No comment'),
          0  => t('Close comment by default'),
          1  => t('Open comment by default'),
      ),
  );
}

/**
 * Implements hook_form_FORM_ID_alter()
 */
function quizz_extra_form_quiz_entity_form_alter(&$form, $form_state) {
  /* @var $quiz \Drupal\quizz\Entity\QuizEntity */
  $quiz = $form['#quiz'];

  // Comment feature is disabled on this quiz type.
  if (-1 == ($default_comment = $quiz->getQuizType()->getConfig('comment', 1))) {
    return;
  }

  $form['publishing']['comment_settings'] = array(
      '#type'        => 'fieldset',
      '#access'      => user_access('administer comments'),
      '#title'       => t('Comment settings'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
      '#group'       => 'additional_settings',
      '#attributes'  => array('class' => array('comment-node-settings-form')),
      '#attached'    => array('js' => array(drupal_get_path('module', 'comment') . '/comment-node-form.js')),
      '#weight'      => 30,
      '#group'       => 'publishing_tabs',
  );

  $form['publishing']['comment_settings']['comment'] = array(
      '#type'          => 'radios',
      '#title'         => t('Comments'),
      '#title_display' => 'invisible',
      '#parents'       => array('comment'),
      '#default_value' => isset($quiz->comment) ? $quiz->comment : $default_comment,
      '#options'       => array(0 => t('Closed'), 1 => t('Open')),
  );
}

function quizz_extra_form_alter(&$form, $form_state, $form_id) {
  if (empty($form['#entity_type']) || empty($form['#entity']) || 'quiz_question_entity' !== $form['#entity_type']) {
    return;
  }

  /* @var $question \Drupal\quizz_question\Entity\Question */
  $question = $form['#entity'];

  // Comment feature is disabled on this question type.
  if (-1 == ($default_comment = $question->getQuestionType()->getConfig('comment', 1))) {
    return;
  }

  $form['publishing']['publishing']['comment'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Open comments'),
      '#parents'       => array('comment'),
      '#default_value' => isset($question->comment) ? $question->comment : $default_comment,
  );
}

/**
 * Implements hook_entity_view().
 */
function quizz_extra_entity_view($entity, $type, $view_mode) {
  if (!in_array($type, array('quiz_entity', 'quiz_question_entity'))) {
    return;
  }

  if (!empty($entity->content['quiz_question_body'][0])) {
    $entity->content['quiz_question_body'][0] += array(
        '#prefix' => '<div class="well">',
        '#suffix' => '</div>',
    );
  }

  if ('full' !== $view_mode) {
    return;
  }

  $settings = field_bundle_settings($type, $entity->type);

  if (!empty($settings['extra_fields']['display']['quiz_questions'][$view_mode]['visible'])) {
    $entity->content['quiz_questions'] = array(
        '#weight' => !isset($settings['extra_fields']['display']['quiz_questions'][$view_mode]['weight']) ? 0 : $settings['extra_fields']['display']['quiz_questions'][$view_mode]['weight'],
        '#markup' => views_embed_view('quizz_questions_inside', 'default', $entity->vid),
    );
  }

  if ($domain = variable_get('disqus_domain', '')) {
    $uri = entity_uri($type, $entity);
    $entity->content['comment'] = array(
        '#type'   => 'disqus',
        '#weight' => 75,
        '#disqus' => array(
            'domain'     => $domain,
            'status'     => (bool) $entity->comment,
            'url'        => url($uri['path'], array('absolute' => TRUE)),
            'title'      => $entity->label(),
            'identifier' => $type . '/' . $entity->identifier(),
            'developer'  => variable_get('disqus_developer', FALSE),
        ),
    );
  }
}

/**
 * Implements hook_field_extra_fields_display_alter().
 */
function quizz_extra_field_extra_fields_alter(&$info) {
  foreach ($info['quiz_entity'] as &$b_info) {
    $b_info['display']['quiz_questions'] = array(
        'label'       => t('Questions'),
        'description' => t('Questions in quiz'),
        'weight'      => 8,
    );
  }
}
