<?php

/**
 * @file
 * Contains form callbacks for Panels Revision module.
 */

/**
 * Display of variant to panels.
 *
 * @param array $id
 *   Panels revision.
 *
 * @return array
 *   Returns rendered display.
 */
function display_panels_review($id) {

  $display = drupal_static(__FUNCTION__);

  $display = $id['configuration'];
  $display->renderer = 'editor';

  unset($display->cache_key);

  ctools_include('plugins', 'panels');
  ctools_include('context');

  $content['content'] = array(
    '#type' => 'markup',
    '#markup' => $display->render(),
  );

  return $content;
}

/**
 * Implementation form look logs in variant.
 */
function panels_revision_form($form, &$form_state) {
  $id = $form_state['handler']->conf['did'];
  $header = array(
    array('data' => t('Time')),
    array('data' => t('User')),
    array('data' => t('View')),
  );

  $query = db_select('panels_revision', 'log')->extend('PagerDefault');
  $query->fields('log', array('time', 'user', 'id'));
  $query->condition('did', $id);

  $result = $query->limit(10)->orderBy('log.time', 'DESC')->execute();

  $rows = array();

  foreach ($result as $row) {
    $row->time = format_date($row->time, 'D');
    $user = user_load($row->user);
    $row->user = theme('username', array('account' => $user));

    $row->view = l(t('Review'), 'admin/structure/panels-revision-content/' . $row->id, array('attributes' => array('target' => '_blank')));
    unset($row->id);
    $rows[] = array('data' => (array) $row);
  }

  // Create a render array ($build) which will be themed as a table with a
  // pager.
  $build['pager_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no date formats found in the db'),
  );

  // Attach the pager theme.
  $build['pager_pager'] = array('#theme' => 'pager');

  return $build;
}

