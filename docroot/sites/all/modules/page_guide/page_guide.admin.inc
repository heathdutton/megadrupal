<?php
/**
 * @file
 * Module admin configuration.
 */

/**
 * Page guide create form.
 */
function page_guide_create($form, &$form_state, $pg_id = NULL) {
  $page_guide = array();
  if ($pg_id) {
    $page_guide = page_guide_get_by_id($pg_id);
    $form['page_guide_url'] = array(
      '#type' => 'value',
      '#value' => $page_guide['url'],
    );
  }
  $form['pg_id'] = array(
    '#type' => 'value',
    '#value' => !empty($page_guide) ? $page_guide['pg_id'] : 0,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Enter page guide name'),
    '#required' => TRUE,
    '#default_value' => !empty($page_guide) ? $page_guide['name'] : '',
  );
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url'),
    '#description' => t('Url of which page need page guide. use <front> to set page guide for front page'),
    '#required' => TRUE,
    '#default_value' => !empty($page_guide) ? $page_guide['url'] : '',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Validation handler for page guide create.
 */
function page_guide_create_validate($form, &$form_state) {
  $url = $form_state['values']['url'];
  $pg_id = $form_state['values']['pg_id'];
  $page_guide_url = '';
  if ($pg_id) {
    $page_guide_url = $form_state['values']['page_guide_url'];
  }
  if (!$pg_id && $page_guide_url != $url) {
    if (page_guide_url_exist($url)) {
      form_set_error('page guide', t('This url already has a page guide'));
    }
    if (!drupal_valid_path($url, TRUE)) {
      form_set_error('page guide', t('Please enter valid url'));
    }
  }
}

/**
 * Submit handler for page guide create.
 */
function page_guide_create_submit($form, &$form_state) {
  $pg_id = $form_state['values']['pg_id'];
  $name = $form_state['values']['name'];
  $url = $form_state['values']['url'];
  if ($pg_id) {
    db_merge('page_guide')
      ->fields(array(
        'name' => $name,
        'url' => $url,
      ))
      ->condition('pg_id', $pg_id, '=')
      ->execute();
    drupal_set_message(t('Page guide has been updated'));
    $form_state['redirect'] = 'admin/config/user-interface/page-guide';
  }
  else {
    $new_id = db_insert('page_guide')
      ->fields(array(
        'name' => $name,
        'url' => $url,
      ))
      ->execute();
    drupal_set_message(t('Page guide has been created'));
    drupal_set_message(t('Add page guide step for your page guide'));
    $form_state['redirect'] = 'admin/config/user-interface/' . $new_id . '/page-guide-step';
  }
}

/**
 * Page guide step create form.
 */
function page_guide_step_create($form, &$form_state, $pg_id, $step_id = NULL) {
  $page_guide = array();
  if ($step_id) {
    $page_guide = page_guide_step_get_by_id($step_id);
  }
  $form['pg_id'] = array(
    '#type' => 'value',
    '#value' => $pg_id,
  );
  $form['step_id'] = array(
    '#type' => 'value',
    '#value' => !empty($page_guide) ? $page_guide->id : 0,
  );
  $form['classname'] = array(
    '#type' => 'textfield',
    '#title' => t('Class / Id Value'),
    '#description' => t('Enter element class / id value example : .node-page or #node-1'),
    '#required' => TRUE,
    '#default_value' => !empty($page_guide) ? $page_guide->class_name : '',
  );
  $form['content'] = array(
    '#type' => 'textarea',
    '#title' => t('Content'),
    '#description' => t('Guide for the user'),
    '#required' => TRUE,
    '#default_value' => !empty($page_guide) ? $page_guide->content : '',
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#description' => t('Weight of the step'),
    '#delta' => 25,
    '#default_value' => !empty($page_guide) ? $page_guide->weight : 0,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit handler for page guide create.
 */
function page_guide_step_create_submit($form, &$form_state) {
  $classname = $form_state['values']['classname'];
  $content = $form_state['values']['content'];
  $weight = $form_state['values']['weight'];
  $id = $form_state['values']['step_id'];
  $pg_id = $form_state['values']['pg_id'];
  db_merge('page_guide_step')
    ->key(array('id' => $id))
    ->fields(array(
      'pg_id' => $pg_id,
      'class_name' => $classname,
      'content' => $content,
      'weight' => $weight,
    ))
    ->execute();
  if ($id) {
    drupal_set_message(t('Page guide step has been updated'));
  }
  else {
    drupal_set_message(t('Page guide step has been created'));
  }
  $form_state['redirect'] = 'admin/config/user-interface/' . $pg_id . '/page-guide-step';
}
