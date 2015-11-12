<?php
/**
 * @file
 *  Git repository admin pages.
 */

/**
 * List repositories page.
 */
function git_repositories_list() {
  $table = array(
    '#header' => array(
      array('data' => t('ID'), 'sort' => TRUE, 'field' => 'rid'),
      array('data' => t('Title'), 'sort' => TRUE, 'field' => 'title'),
      array('data' => t('Name'), 'sort' => TRUE, 'field' => 'name'),
      array('data' => t('Server'), 'sort' => TRUE, 'field' => 'server_name'),
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
    'rid' => array(
      'data' => t('ID'),
      'type' => 'property',
      'specifier' => 'rid',
    ),
    'title' => array(
      'data' => t('Title'),
      'type' => 'property',
      'specifier' => 'title',
    ),
    'name' => array(
      'data' => t('Name'),
      'type' => 'property',
      'specifier' => 'name',
    ),
  );

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'git_repository');
  $query->pager(10);
  $query->tableSort($sort_headers);

  $result = $query->execute();
  $repos = !empty($result['git_repository']) ? entity_load('git_repository', array_keys($result['git_repository'])) : array();

  foreach ($repos as $rid => $repo) {
    $server = git_server_name_load($repo->server_name);
    $server_uri = entity_uri('git_server', $server);
    $uri = entity_uri('git_repository', $repo);
    $table['#rows'][] = array(
      entity_id('git_repository', $repo),
      l(entity_label('git_repository', $repo), $uri['path'], $uri['options']),
      check_plain($repo->name),
      l(entity_label('git_server', $server), $server_uri['path'], $server_uri['options']),
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
 * Git repository edit page.
 */
function git_repository_edit($repository = NULL) {
  if (is_null($repository)) {
    $values = array(
      'title' => 'Git',
      'name' => '',
      'bundle' => 'repository',
      'server_name' => NULL,
    );
    $repository = entity_get_controller('git_repository')->create($values);
  }

  return drupal_get_form('git_repository_edit_form', $repository);
}

/**
 * Git repository edit form.
 */
function git_repository_edit_form($form, $form_state, $repository) {
  $form = array();

  $page_title = t('Add Git Repository');
  if (!empty($repository->is_new)) {
    $page_title = entity_label('git_repository', $repository);
  }
  drupal_set_title($page_title);

  $form['is_new'] = array(
    '#type' => 'value',
    '#value' => !empty($repository->is_new),
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Repository name'),
    '#description' => t('A descriptive title for the git repository.'),
    '#default_value' => $repository->title,
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $repository->name,
    '#machine_name' => array(
      'exists' => 'git_repository_exists',
      'source' => array('title'),
    ),
    '#disabled' => !empty($repository->name),
  );

  $server_names = git_server_list_names();

  $form['server_name'] = array(
    '#type' => 'select',
    '#title' => t('Git server'),
    '#description' => t('The server this repository is in.'),
    '#default_value' => $repository->server_name,
    '#options' => $server_names,
    '#required' => TRUE,
  );

  $form['actions'] = array(
    '#weight' => 99,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
  );

  $form['bundle'] = array(
    '#type' => 'value',
    '#value' => $repository->bundle,
  );

  if (!empty($repository->rid)) {
    $form['rid'] = array(
      '#type' => 'value',
      '#value' => (int) $repository->rid,
    );

    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('git_repository_edit_form_delete_submit'),
    );
  }

  return $form;
}

/**
 * Git repository edit form submit callback.
 */
function git_repository_edit_form_submit($form, &$form_state) {
  $repository = entity_get_controller('git_repository')->create($form_state['values']);
  $repository->save();
  $form_state['redirect'] = 'admin/config/development/git/repositories';
}

/**
 * Git repository delete button submit callback.
 */
function git_repository_edit_form_delete_submit($form, &$form_state) {
  $repository = entity_get_controller('git_repository')->create($form_state['values']);
  $uri = entity_uri('git_repository', $repository);
  drupal_goto($uri['path'] . "/delete", $uri['options']);
}

function git_repository_delete($repository) {
  return drupal_get_form('git_repository_delete_form', $repository);
}

/**
 * Git repository delete form.
 */
function git_repository_delete_form($form, $form_state, $repository) {
  return confirm_form($form, 'Are you sure you want to delete this git repository?', "admin/config/development/git/repositories", NULL, "Delete", "Cancel");
}

/**
 * Git repository delete form submit handler.
 */
function git_repository_delete_form_submit($form, $form_state) {
  if (!empty($form_state['values']['confirm'])) {
    $repository = reset($form_state['build_info']['args']);
    $repository->delete();
    drupal_goto('admin/config/development/git/repositories');
  }
}

/**
 * View a git repository.
 */
function git_repository_view($git_repository, $view_mode = 'full') {
  drupal_set_title(entity_label('git_repository', $git_repository));
  return entity_view('git_repository', array(entity_id('git_repository', $git_repository) => $git_repository), $view_mode);
}
