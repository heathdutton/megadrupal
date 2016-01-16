<?php

function tribune_local_upgrade_page() {
  return drupal_get_form('tribune_local_upgrade_form');
}

function tribune_local_upgrade_form($form, &$form_state) {
  $form = array();

  if (db_table_exists('tribune_posts')) {
    // Data from 6.x-2.x remains
    $count = 0;

    $form['tribune-2'] = array(
      '#type' => 'fieldset',
      '#title' => t('Upgrade from tribune 6.x-2.x'),
      '#tree' => TRUE,
    );

    if (db_table_exists('tribune_nodes')) {
      $tribunes = db_select('tribune_nodes', 'tn')
        ->fields('tn', array('nid', 'tid'))
        ->execute()
        ->fetchAllKeyed();

      if ($count = count($tribunes)) {
        foreach ($tribunes as $nid => $tid) {
          $node = node_load($nid);
          $form['tribune-2'][$tid] = array(
            '#type' => 'checkbox',
            '#title' => check_plain($node->title),
            '#default_value' => 1,
            '#value' => 1,
            '#readonly' => TRUE,
            '#attributes' => array('readonly' => 'readonly'),
          );
        }

        $form['tribune-2']['upgrade'] = array(
          '#type' => 'submit',
          '#value' => t('Upgrade your tribunes'),
          '#submit' => array('_tribune_local_upgrade_tribunes_6x_2x'),
        );
      }
    }

    if (!$count) {
      $form['tribune-2']['cleanup'] = array(
        '#type' => 'submit',
        '#value' => t('Cleanup old tables'),
        '#submit' => array('_tribune_local_cleanup_tables_6x_2x'),
      );

      $form['tribune-2']['#description'] = t('You do not have any old tribunes to upgrade, but the database schema used by the older version of the tribune module was not cleaned up automatically during the module upgrade.');
    }
  } else if (db_table_exists('tribune')) {
    $form['tribune-1']['upgrade'] = array(
      '#type' => 'submit',
      '#value' => t('Upgrade your tribune'),
      '#submit' => array('_tribune_local_upgrade_tribunes_6x_1x'),
    );
  } else {
    $form['nothing'] = array(
      '#type' => 'fieldset',
      '#title' => 'Nothing to upgrade',
      '#description' => t('You do not have any old tribunes or remaining tables.'),
    );
  }

  return $form;
}

function _tribune_local_cleanup_tables_6x_2x($form, &$form_state) {
  db_drop_table('tribune_nodes');
}

function _tribune_local_upgrade_tribunes_6x_2x($form, &$form_state) {
  $batch = array(
    'title' => t('Importing'),
    'progress_message' => t('Importing old posts...'),
    'operations' => array(),
    'file' => drupal_get_path('module', 'tribune_local') . '/tribune_local.update.inc',
    'finished' => '_tribune_local_upgrade_tribunes_6x_2x_finished',
  );

  foreach ($form_state['values']['tribune-2'] as $nid => $upgrade) {
    if ($upgrade and $node = node_load($nid)) {
      $batch['operations'][] = array('_tribune_local_upgrade_tribunes_6x_2x_process', array($node));
    }
  }

  batch_set($batch);
  batch_process();
}

function _tribune_local_upgrade_tribunes_6x_2x_process($node, &$context) {
  $increment = 1000;

  $tid = db_select('tribune_nodes', 'tn')
    ->fields('tn', array('tid'))
    ->condition('nid', $node->nid)
    ->execute()
    ->fetchField();

  if (empty($context['sandbox']['progress'])) {
    $context['finished'] = 0;
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = db_select('tribune_posts', 'tp')->fields('tp')->condition('tid', $tid)->execute()->rowCount();

    $info = db_select('tribune', 't')
      ->fields('t')
      ->condition('tid', $tid)
      ->execute()
      ->fetchAssoc();

    db_insert('tribune_local')
      ->fields(array(
        'nid' => $node->nid,
        'max_length' => $info['max_message_size'],
        'reload_rate' => $info['reload_delay'],
      ))
      ->execute();

    db_insert('tribunes')
      ->fields(array(
        'nid' => $node->nid,
        'type' => 'local',
        'display_posts' => $info['history_size'],
        'favicon_notification' => 0,
        'show_useragent' => 1,
        'mod_access' => 'owner',
        'write_access' => 'all',
        'read_access' => 'all',
        'custom_access' => serialize(array()),
      ))
      ->execute();

    $node->type = 'tribune';
    $node->tribune_type = 'local';
    $node->tribune_display_posts = $info['history_size'];
    $node->tribune_max_length = $info['max_message_size'];
    $node->tribune_reload_rate = $info['reload_delay'];
    $node->tribune_permissions = array(
      'read' => 'all',
      'write' => 'all',
      'mod' => 'owner',
      'custom' => array(),
    );
    node_save($node);
  }

  $posts = db_select('tribune_posts', 'tp')
    ->fields('tp')
    ->condition('tid', $tid)
    ->range($context['sandbox']['progress'], $increment)
    ->execute()
    ->fetchAllAssoc('pid');

  $insert_query = db_insert('tribune_local_post')
    ->fields(array(
      'nid',
      'uid',
      'post_id',
      'text',
      'info',
      'timestamp',
      'clock_index',
      'moderated',
    ));

  foreach ($posts as $post) {
    $context['sandbox']['progress']++;
    $context['message'] = t('Importing post %node of %count', array('%node' => $context['sandbox']['progress'], '%count' => $context['sandbox']['max']));
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];

    $insert_query->values(array(
      'nid' => $node->nid,
      'uid' => $post->uid,
      'post_id' => $post->post_id,
      'text' => $post->original_message,
      'info' => substr($post->info, 0, 128),
      'timestamp' => $post->time,
      'clock_index' => $post->rank,
      'moderated' => $post->moderated,
    ));
  }
  $insert_query->execute();
}

function _tribune_local_upgrade_tribunes_6x_2x_finished($success) {
  if ($success) {
    db_delete('tribune_posts')
      ->execute();

    db_delete('tribune')
      ->execute();

    db_delete('tribune_nodes')
      ->execute();

    db_delete('tribune_post_references')
      ->execute();
  }
}

function _tribune_local_upgrade_tribunes_6x_1x($form, &$form_state) {
  global $user;

  $node = new stdClass();
  $node->uid = $user->uid;
  $node->title = t('Tribune');
  $node->type = 'tribune';
  $node->tribune_type = 'local';
  $node->tribune_display_posts = variable_get('tribune_history_size', 30);
  $node->tribune_favicon_notification = !variable_get('tribune_discreet_notification', FALSE);
  $node->tribune_show_useragent = TRUE;
  $node->tribune_max_length = variable_get('tribune_max_message_size', 256);
  $node->tribune_reload_rate = variable_get('tribune_reload_delay', 30);
  $node->tribune_permissions = array(
    'read' => 'all',
    'write' => 'all',
    'mod' => 'owner',
    'custom' => array(),
  );
  node_save($node);

  $batch = array(
    'title' => t('Importing'),
    'progress_message' => t('Importing old posts...'),
    'operations' => array(
      array('_tribune_local_upgrade_tribunes_6x_1x_process', array($node)),
    ),
    'file' => drupal_get_path('module', 'tribune_local') . '/tribune_local.update.inc',
    'finished' => '_tribune_local_upgrade_tribunes_6x_1x_finished',
  );

  batch_set($batch);
  batch_process();
}

function _tribune_local_upgrade_tribunes_6x_1x_process($node, &$context) {
  $increment = 1000;

  if (empty($context['sandbox']['progress'])) {
    $context['finished'] = 0;
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = db_select('tribune', 't')->fields('t')->execute()->rowCount();
  }

  $posts = db_select('tribune', 't')
    ->fields('t')
    ->range($context['sandbox']['progress'], $increment)
    ->execute()
    ->fetchAllAssoc('post_id');

  $insert_query = db_insert('tribune_local_post')
    ->fields(array(
      'nid',
      'uid',
      'post_id',
      'text',
      'info',
      'timestamp',
      'clock_index',
      'moderated',
    ));

  foreach ($posts as $post) {
    $context['sandbox']['progress']++;
    $context['message'] = t('Importing post %post of %count', array('%post' => $context['sandbox']['progress'], '%count' => $context['sandbox']['max']));
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];

    $insert_query->values(array(
      'nid' => $node->nid,
      'uid' => $post->uid,
      'post_id' => $post->post_id,
      'text' => $post->message,
      'info' => substr($post->info, 0, 128),
      'timestamp' => $post->post_time,
      'clock_index' => $post->post_rank,
      'moderated' => $post->moderated,
    ));
  }
  $insert_query->execute();
}

function _tribune_local_upgrade_tribunes_6x_1x_finished($success) {
  if ($success) {
    db_drop_table('tribune');
  }
}
