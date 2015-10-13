<?php
// $Id: journal.admin.inc,v 1.2 2010/11/29 20:47:39 sun Exp $

/**
 * @file
 * Journal admin pages.
 */

/**
 * Output a sortable table containing all journal entries.
 */
function journal_view() {
  $header = array(
    array('data' => t('Date'), 'field' => 'j.timestamp', 'sort' => 'desc'),
    array('data' => t('Author'), 'field' => 'u.name'),
    t('Message'),
    t('Location'),
  );

  $query = db_select('journal', 'j')->extend('TableSort')->extend('PagerDefault');
  $query->join('users', 'u', 'j.uid = u.uid');
  $result = $query
    ->fields('j')
    ->fields('u')
    ->orderByHeader($header)
    ->limit(50)
    ->execute()
    ->fetchAllAssoc('jid');

  return journal_output($result, 'table', $header);
}

/**
 * Output a sortable table containing all recorded patches.
 */
function journal_patch_view() {
  $header = array(
    array('data' => t('Date'), 'field' => 'j.timestamp', 'sort' => 'desc'),
    array('data' => t('Module'), 'field' => 'j.module'),
    array('data' => t('Author'), 'field' => 'u.name'),
    t('Description'),
    t('Issue'),
    t('Status'),
    t('Operations'),
  );
  $query = db_select('journal_patch', 'j')->extend('TableSort')->extend('PagerDefault');
  $query->join('users', 'u', 'j.uid = u.uid');
  $result = $query
    ->fields('j')
    ->fields('u', array('name'))
    ->orderByHeader($header)
    ->limit(50)
    ->execute()
    ->fetchAllAssoc('pid');

  $module_list = module_list();
  $rows = array();
  foreach ($result as $pid => $entry) {
    $modules = array();
    foreach (explode(',', $entry->module) as $module) {
      $info = system_get_info('module', $module);
      if (isset($info['project'])) {
        $url = 'http://drupal.org/project/issues/' . $info['project'];
        $modules[] = l($info['name'], $url);
      }
      else {
        $modules[] = $info['name'];
      }
    }
    if ($entry->url != '') {
      if (preg_match('@drupal.org/node/(\d+)@', $entry->url, $issue_title)) {
        $issue_link = l('#'. $issue_title[1], $entry->url);
      }
      else {
        $issue_link = l(t('View'), $entry->url);
      }
    }
    else {
      $issue_link = '';
    }
    $rows[] = array(
      format_date($entry->timestamp, 'small'),
      implode(', ', $modules),
      format_username($entry),
      filter_xss_admin($entry->description),
      $issue_link,
      t($entry->status),
      l(t('Edit'), "admin/reports/journal/patches/edit/$entry->pid"),
    );
  }

  $output['form'] = drupal_get_form('journal_patch_form');
  $output['entries'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No patch entries available.'),
  );
  $output['pager'] = array(
    '#theme' => 'pager',
  );
  return $output;
}

/**
 * Form builder function for patches.
 */
function journal_patch_form($form, &$form_state, $pid = NULL) {
  drupal_add_css(drupal_get_path('module', 'journal') . '/journal_patch.css');
  $patch = array();
  if (isset($pid)) {
    $patch = db_query("SELECT * FROM {journal_patch} WHERE pid = :pid", array(':pid' => $pid), array('fetch' => PDO::FETCH_ASSOC))->fetch();
  }
  $patch += array(
    'pid' => NULL,
    'module' => '',
    'description' => '',
    'url' => '',
    'status' => 'open',
  );

  $form['patch'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add patch record'),
    '#tree' => TRUE,
  );
  $form['patch']['pid'] = array(
    '#type' => 'value',
    '#value' => $patch['pid'],
  );
  $form['patch']['wrapper'] = array(
    '#type' => 'container',
    '#parents' => array('patch'),
    '#attributes' => array(
      'class' => array('clearfix'),
    ),
  );
  $form['patch']['wrapper']['module'] = array(
    '#type' => 'select',
    '#title' => t('Affected modules'),
    '#options' => module_list(FALSE, FALSE, TRUE),
    '#multiple' => TRUE,
    '#default_value' => explode(',', $patch['module']),
    '#size' => 8,
    '#required' => TRUE,
    '#prefix' => '<div class="journal-patch-module-select">',
    '#suffix' => '</div>',
  );
  $form['patch']['wrapper']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $patch['description'],
    '#required' => TRUE,
  );
  $form['patch']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Issue URL'),
    '#default_value' => $patch['url'],
    '#prefix' => '<div class="journal-patch-issue">',
  );
  $form['patch']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => array('open' => t('open'), 'fixed' => t('fixed'), "won't fix" => t("won't fix")),
    '#default_value' => $patch['status'],
    '#suffix' => '</div>',
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($patch['pid']) ? t('Save') : t('Add'),
  );
  if (!empty($patch['pid'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }
  return $form;
}

/**
 * Submit handler for journal patch form.
 */
function journal_patch_form_submit($form, &$form_state) {
  global $user;

  $patch = $form_state['values']['patch'];

  if ($form_state['values']['op'] == t('Delete')) {
    drupal_goto('admin/reports/journal/patches/delete/' . $patch['pid']);
  }

  if (preg_match('@^#\d+@', $patch['url'])) {
    $patch['url'] = 'http://drupal.org/node/'. substr($patch['url'], 1);
  }

  db_merge('journal_patch')
    ->fields(array(
      'uid' => $user->uid,
      'module' => implode(',', $patch['module']),
      'description' => $patch['description'],
      'url' => $patch['url'],
      'status' => $patch['status'],
      'timestamp' => REQUEST_TIME,
    ))
    ->key(array(
      'pid' => $patch['pid'],
    ))
    ->execute();

  $form_state['redirect'] = 'admin/reports/journal/patches';
}

/**
 * Confirmation form to delete a patch record.
 */
function journal_patch_delete_confirm($form, &$form_state, $pid) {
  $form['pid'] = array('#type' => 'value', '#value' => $pid);
  $description = db_query("SELECT description FROM {journal_patch} WHERE pid = :pid", array(':pid' => $pid))->fetchField();
  $form['patch_description'] = array(
    '#type' => 'item',
    '#title' => t('Description'),
    '#markup' => filter_xss_admin($description),
  );
  return confirm_form($form,
    t('Are you sure you want to delete this patch record?'),
    'admin/reports/journal/patches',
    NULL,
    t('Delete')
  );
}

/**
 * Form submit callback for patch record delete confirm form.
 */
function journal_patch_delete_confirm_submit($form, &$form_state) {
  db_delete('journal_patch')
    ->condition('pid', $form_state['values']['pid'])
    ->execute();

  $form_state['redirect'] = 'admin/reports/journal/patches';
}

