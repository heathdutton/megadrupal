<?php
/**
 * @file
 *  Git admin pages.
 */

/**
 * List servers page.
 */
function git_server_list() {
  $table = array(
    '#header' => array(
      array('data' => t('ID'), 'sort' => TRUE, 'field' => 'sid'),
      array('data' => t('Title'), 'sort' => TRUE, 'field' => 'title'),
      array('data' => t('URL'), 'sort' => TRUE, 'field' => 'user'),
      array('data' => t('Type'), 'sort' => TRUE, 'field' => 'bundle'),
      t('Operations'),
    ),
    '#theme' => 'table',
    '#attributes' => array(),
    '#caption' => '',
    '#colgroups' => array(),
    '#sticky' => FALSE,
    '#empty' => 'None',
    '#rows' => array(),
  );

  $sort_headers = array(
    'sid' => array(
      'data' => t('ID'),
      'type' => 'property',
      'specifier' => 'sid',
    ),
    'title' => array(
      'data' => t('Title'),
      'type' => 'property',
      'specifier' => 'title',
    ),
    'user' => array(
      'data' => t('URL'),
      'type' => 'property',
      'specifier' => 'user',
    ),
  );

  $entity_info = entity_get_info('git_server');
  $bundles = $entity_info['bundles'];

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'git_server');
  $query->pager(10);
  $query->tableSort($sort_headers);

  $result = $query->execute();
  $servers = !empty($result['git_server']) ? entity_load('git_server', array_keys($result['git_server'])) : array();

  foreach ($servers as $id => $server) {
    $uri = entity_uri('git_server', $server);
    $table['#rows'][] = array(
      entity_id('git_server', $server),
      l(entity_label('git_server', $server), $uri['path'], $uri['options']),
      check_plain("{$server->user}@{$server->host}"),
      $bundles[$server->bundle]['label'],
      theme(
        'links',
        array(
          'links' => array(
            array('title' => 'edit', 'href' => $uri['path'] . "/edit"),
            array('title' => 'delete', 'href' => $uri['path'] . "/delete"),
          ),
        )
      ),
    );
  }

  $output = array(
    'table' => $table,
    'pager' => array(
      '#theme' => 'pager',
      array('tags' => array()),
    ),
  );

  return $output;
}

/**
 * Page callback for adding git server.
 */
function git_server_add() {
  $info = entity_get_info('git_server');
  $bundles = $info['bundles'];
  $base_add_url = 'git/server/add';

  if (count($bundles) == 1) {
    drupal_goto("$base_add_url/" . current(array_keys($bundles)));
  }

  $links = array(
    '#theme' => 'links',
    '#links' => array(),
  );

  foreach ($bundles as $name => $bundle) {
    $links['#links'][] = array(
      'title' => $bundle['label'],
      'href' => "$base_add_url/$name",
    );
  }

  return array(
    'heading' => array('#markup' => '<h2>Select the type of server to create.</h2>'),
    'links' => $links
  );
}

/**
 * Git server edit page.
 */
function git_server_edit($server = NULL, $bundle = NULL) {
  if (NULL === $server) {
    $values = array(
      'bundle' => $bundle,
      'title' => 'Git',
      'user' => 'git',
      'host' => 'localhost',
      'name' => '',
    );
    $server = entity_get_controller('git_server')->create($values);
  }

  return drupal_get_form('git_server_edit_form', $server);
}

/**
 * Git server edit form.
 */
function git_server_edit_form($form, $form_state, $server) {

  // Set some sane default values.
  $entity_info = entity_get_info('git_server');
  $bundle_label = $entity_info['bundles'][$server->bundle]['label'];
  if ($server->is_new) {
    drupal_set_title(t('Add new @bundle server', array('@bundle' => $bundle_label)));
  }
  else {
    drupal_set_title(entity_label('git_server', $server));
  }

  $form = array();

  $form['bundle'] = array(
    '#type' => 'value',
    '#value' => $server->bundle,
  );

  $form['is_new'] = array(
    '#type' => 'value',
    '#value' => !empty($server->is_new),
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Server name'),
    '#description' => t('A descriptive title for the git server.'),
    '#default_value' => $server->title,
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $server->name,
    '#machine_name' => array(
      'exists' => 'git_server_exists',
      'source' => array('title'),
    ),
    '#disabled' => !empty($server->name),
  );

  $form['user'] = array(
    '#type' => 'textfield',
    '#title' => t('Server user'),
    '#description' => t('The user git runs as on the server.'),
    '#default_value' => $server->user,
    '#maxlength' => 32,
    '#required' => TRUE,
  );

  $form['host'] = array(
    '#type' => 'textfield',
    '#title' => t('Server host'),
    '#description' => t('The host the git runs on.'),
    '#default_value' => $server->host,
    '#maxlength' => 223,
    '#required' => TRUE,
  );

  field_attach_form('git_server', $server, $form, $form_state);

  $form['actions'] = array(
    '#weight' => 99,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
  );

  if (!empty($server->sid)) {
    $form['sid'] = array(
      '#type' => 'value',
      '#value' => (int) $server->sid,
    );

    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('git_server_edit_form_delete_submit'),
    );
  }

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/development/git',
    '#weight' => 25,
  );

  return $form;
}

/**
 * Validate handler for git_server_edit_form
 */
function git_server_edit_form_validate($form, &$form_state) {
  $server = (object) $form_state['values'];
  field_attach_form_validate('git_server', $server, $form, $form_state);
}

/**
 * Git server edit form submit callback.
 */
function git_server_edit_form_submit($form, &$form_state) {
  $server = entity_get_controller('git_server')->create($form_state['values']);
  field_attach_submit('git_server', $server, $form, $form_state);
  $server->save();
  $form_state['redirect'] = 'admin/config/development/git/servers';
}

/**
 * Git server delete button submit callback.
 */
function git_server_edit_form_delete_submit($form, &$form_state) {
  $server = entity_get_controller('git_server')->create($form_state['values']);
  drupal_goto("git/server/{$server->sid}/delete");
}

function git_server_delete($server) {
  return drupal_get_form('git_server_delete_form', $server);
}

/**
 * Git server delete form.
 */
function git_server_delete_form($form, $form_state, $server) {
  return confirm_form($form, 'Are you sure you want to delete this git server?', "admin/config/development/git/servers", NULL, "Delete", "Cancel");
}

/**
 * Git server delete form submit handler.
 */
function git_server_delete_form_submit($form, $form_state) {
  if (!empty($form_state['values']['confirm'])) {
    $server = reset($form_state['build_info']['args']);
    $server->delete();
    drupal_goto('admin/config/development/git/servers');
  }
}

/**
 * View a git server.
 */
function git_server_view($git_server, $view_mode = 'full') {
  drupal_set_title(entity_label('git_server', $git_server));
  return entity_view('git_server', array(entity_id('git_server', $git_server) => $git_server), $view_mode);
}
