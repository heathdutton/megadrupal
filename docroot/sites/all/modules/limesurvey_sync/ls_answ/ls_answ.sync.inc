<?php
/**
 * @file
 * functions for the page displaying information on answer synchronization.
 */

/**
 * Display synchronization datas for a specific answer node.
 *
 * This page displays system informations datas about the LimeSurvey
 * references stored into the drupal site and allow to resynchronize the node.
 *
 * @param $node
 *   An object. The answer node.
 *
 * @return
 *   A string. The html page displaying synchronization datas and a form to
 *   resynchronize the node.
 *
 * @see ls_answ_sync_datas_submit()
 * @ingroup forms
 */
function ls_answ_sync_datas($form, $form_state, $node) {
  module_load_include('inc', 'ls_answ', 'ls_answ');
  $form = array();
  $form['#node'] = $node;

  $form['sync_datas'] = array(
    '#type' => 'fieldset',
    '#title' => t('Synchronization datas'),
  );

  $fields_to_show = array(
    'ls_status',
    'ls_sync',
    'ls_last_import',
    'ls_sid',
    'ls_lang',
    'ls_aid',
    'ls_token',
  );
  $header[] = t('fields');
  $header[] = t('values');
  $cell = $cells = array();
  foreach ($fields_to_show as $field) {
    $cell[] = ls_answ_display_sync_headers($field);
    $cell[] = (isset($node->$field) ? ls_answ_display_sync_fields($field, $node->$field) : '');
    $cells[] = $cell;
    $cell = array();
  }
  $form['sync_datas']['datas'] = array(
    '#type' => 'markup',
    '#prefix' => '<div>',
    '#markup' => theme('table', array('header' => $header, 'rows' => $cells)),
    '#suffix' => '</div>',
  );

  $form['sync_datas']['more_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['sync_datas']['more_options']['force_sync'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force the synchronization'),
    '#default_value' => FALSE,
  );

  $allowed_desynchronized_sync = array(
    LS_SYNC_DESYNCHRONIZED_LANG,
    LS_SYNC_DESYNCHRONIZED_LS_SID,
    LS_SYNC_DESYNCHRONIZED_LS_LANG,
    LS_SYNC_DESYNCHRONIZED_NODE,
    LS_SYNC_DESYNCHRONIZED_TYPE,
    //LS_SYNC_DESYNCHRONIZED_REVISION,
  );
  $answer_is_desync = (in_array($node->ls_sync, $allowed_desynchronized_sync));
  $form['sync_datas']['more_options']['force_desync'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force the synchronization of this desynchronized answer'),
    '#description' => t('Use with caution.'),
    '#default_value' => FALSE,
    '#access' => $answer_is_desync,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Resynchronize'),
  );

  return $form;
}

/**
 * Validate sync datas.
 */
function ls_answ_sync_datas_validate($form, &$form_state) {

  if (!empty($form_state['values']['force_sync']) && !user_access('force answers synchronization')) {
    form_set_error('force_sync', t('You are not allowed to force the synchronization'));
  }
  if (!empty($form_state['values']['force_desync']) && !user_access('force answers synchronization')) {
    form_set_error('force_desync', t('You are not allowed to force the synchronization'));
  }
}

/**
 * Submit sync datas.
 */
function ls_answ_sync_datas_submit($form, &$form_state) {
  $node = $form['#node'];
  // Don't use the $form['#node'] value : reload the node instead.
  $node = node_load($node->nid);
  $force = $form_state['values']['force_sync'];
  $force_desync = $form_state['values']['force_desync'];
  $synchonization = FALSE;
  module_load_include('inc', 'ls_answ', 'ls_answ');
  // The user may force the synchronization.
  $sync_node = ls_answ_sync_node($node, $force, $force_desync);

  if ($sync_node != $node) {
    $ls_sync = ls_answ_sync_results($node, $sync_node);
    ls_answ_display_sync_results($ls_sync, FALSE, TRUE);
  }

  // Redirect to the view node page.
  $form_state['redirect'] = 'node/' . $node->nid;
}
