<?php
/**
 * @file
 * Changeset administration pages.
 */

/**
 * List all entities for the given type.
 *
 * @return string
 *   A rendered page.
 */
function cps_changeset_list_page($view_name) {
  $view = views_get_view($view_name);
  if (!$view || !$view->access('default')) {
    return MENU_NOT_FOUND;
  }

  drupal_set_title(t('Site versions'));

  $view->override_path = $_GET['q'];
  return $view->preview('default');
}

/**
 * Page callback to add a new entity.
 *
 * @return array
 *   A drupal renderable array.
 */
function cps_changeset_add_entity_page() {
  $entity = cps_changeset_create();

  drupal_set_title(t('Create new site version'));

  $form_state = array(
    'entity' => $entity,
    'no_redirect' => TRUE,
    'op' => 'add',
    'args' => array(),
  ) + form_state_defaults();

  form_load_include($form_state, 'inc', 'cps', 'includes/forms');
  $output = drupal_build_form('cps_changeset_edit_form', $form_state);

  if (!empty($form_state['executed'])) {
    // Use the one on $form_state because form caching can change the object.
    $form_state['entity']->save();
    watchdog($entity->entityType(), 'Changeset: created %title.', array('%title' => $entity->label()));
    drupal_set_message(t('The site version "%title" has been created.', array('%title' => $entity->label())));
    call_user_func_array('drupal_goto', $entity->uri());
  }

  return $output;
}

/**
 * Page callback to edit a entity.
 *
 * @param CPSChangeset $entity
 *   The entity to edit.
 *
 * @return array
 *   A Drupal renderable array.
 */
function cps_changeset_edit_page(CPSChangeset $entity) {
  $form_state = array(
    'entity' => $entity,
    'no_redirect' => TRUE,
    'op' => 'edit',
  ) + form_state_defaults();

  form_load_include($form_state, 'inc', 'cps', 'includes/forms');
  $output = drupal_build_form('cps_changeset_edit_form', $form_state);

  if (!empty($form_state['executed'])) {
    // Use the one on $form_state because form caching can actually change the object.
    /** @var CPSChangeset $entity */
    $entity = $form_state['entity'];
    $entity->save();
    watchdog($entity->entityType(), 'Changeset: updated %title.', array('%title' => $entity->label()));
    drupal_set_message(t('The site version "%title" has been updated.', array('%title' => $entity->label())));
    call_user_func_array('drupal_goto', $entity->uri());
  }

  return $output;
}

/**
 * Page callback to view a entity.
 *
 * @param CPSChangeset $entity
 *   The entity to view.
 * @param string $view_mode
 *   (optional) The view mode to render. Defaults to 'full'.
 *
 * @return string
 *   A rendered page.
 */
function cps_changeset_view_page(CPSChangeset $entity, $view_mode = 'full') {
  $build = $entity->view($view_mode, NULL, TRUE);
  return drupal_render($build);
}

/**
 * Menu callback via drupal_get_form() -- ask for confirmation of entity deletion
 */
function cps_changeset_delete_form($form, &$form_state, CPSChangeset $entity) {
  $form_state['entity'] = $entity;
  $uri = $entity->uri();
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $entity->label())),
    $uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form submit handler to execute entity deletion
 */
function cps_changeset_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    /** @var CPSChangeset $entity */
    $entity = $form_state['entity'];
    $entity->delete();
    watchdog($entity->entityType(), 'Changeset: deleted %title.', array('%title' => $entity->label()));
    drupal_set_message(t('Site version "%title" has been deleted.', array('%title' => $entity->label())));
  }

  $form_state['redirect'] = 'admin/structure/changesets';
}

/**
 * Page callback to display the publish changeset form.
 *
 * @param CPSChangeset $entity
 *
 * @return array
 *   A Drupal renderable array.
 */
function cps_changeset_publish_page(CPSChangeset $entity) {
  $form_state = array(
    'entity' => $entity,
  ) + form_state_defaults();

  form_load_include($form_state, 'inc', 'cps', 'includes/forms');
  $output = drupal_build_form('cps_changeset_publish_changeset_form', $form_state);
  return $output;
}

/**
 * Page callback to display the unpublish changeset form.
 *
 * @param CPSChangeset $entity
 *
 * @return array
 *   A Drupal renderable array.
 */
function cps_changeset_unpublish_page(CPSChangeset $entity) {
  $form_state = array(
    'entity' => $entity,
  ) + form_state_defaults();

  form_load_include($form_state, 'inc', 'cps', 'includes/forms');
  $output = drupal_build_form('cps_changeset_unpublish_changeset_form', $form_state);
  return $output;
}

/**
 * Page callback to switch to a particular changeset.
 *
 * @param CPSChangeset $entity
 *
 * @return array
 *   A Drupal renderable array.
 */
function cps_changeset_view_site_page(CPSChangeset $entity) {
  $path = $entity->uri();
  drupal_goto($path['path'], array('query' => array('changeset_id' => $entity->changeset_id)));
}

/**
 * Page callback to remove an entity from the given changeset.
 *
 * @param CPSChangeset $changeset
 *   The changeset
 * @param string $entity_type
 *   The type of entity being removed.
 * @param $entity_id
 *   The id of the entity.
 *
 * @return array
 *   A Drupal renderable array.
 */
function cps_changeset_remove_page(CPSChangeset $changeset, $entity_type, $entity_id) {
  cps_override_changeset($changeset->changeset_id);
  $entities = entity_load($entity_type, array($entity_id));
  cps_override_changeset(NULL);
  if (!$entities) {
    return MENU_NOT_FOUND;
  }

  $entity = reset($entities);
  $form_state = array(
    'entity_type' => $entity_type,
    'entity' => $entity,
    'changeset' => $changeset,
  ) + form_state_defaults();

  $output = drupal_build_form('cps_changeset_remove_form', $form_state);
  return $output;
}

/**
 * Form callback to remove an entity from a changeset.
 */
function cps_changeset_remove_form($form, &$form_state) {
  /** @var CPSChangeset $changeset */
  $changeset = $form_state['changeset'];
  $entity_type = $form_state['entity_type'];
  $entity = $form_state['entity'];
  $uri = $changeset->uri();
  return confirm_form($form,
    t('Are you sure you want to remove %entity from site version %title?', array('%entity' => entity_label($entity_type, $entity), '%title' => $changeset->label())),
    $uri['path'],
    t('All changes to %entity in %title will be discarded. This action cannot be undone!', array('%entity' => entity_label($entity_type, $entity), '%title' => $changeset->label())),
    t('Remove'),
    t('Cancel')
  );
}

/**
 * Submit callback to remove an entity from a changeset.
 */
function cps_changeset_remove_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    /** @var CPSChangeset $changeset */
    $changeset = $form_state['changeset'];
    $entity_type = $form_state['entity_type'];
    $entity = $form_state['entity'];

    $success = $changeset->removeEntity($entity_type, $entity);
    if ($success) {
      drupal_set_message(t('Changes to %entity have been removed from the site version %title.', array('%entity' => entity_label($entity_type, $entity), '%title' => $changeset->label())));
    }
    else {
      drupal_set_message(t('Changes to %entity could not be removed from the site version %title.', array('%entity' => entity_label($entity_type, $entity), '%title' => $changeset->label())), 'error');
    }

    $form_state['redirect'] = $changeset->uri();
  }
}

/**
 * Page callback to move an entity from the given changeset to the current changeset.
 *
 * @param CPSChangeset $changeset
 *   The changeset
 * @param string $entity_type
 *   The type of entity being removed.
 * @param $entity_id
 *   The id of the entity.
 *
 * @return array
 *   A Drupal renderable array.
 */
function cps_changeset_move_page(CPSChangeset $changeset, $entity_type, $entity_id) {
  $changeset_to = cps_changeset_load(cps_get_current_changeset());

  $entity = entity_load_single($entity_type, $entity_id);
  if (!$entity) {
    return MENU_NOT_FOUND;
  }

  // If the entity can't be updated in the current changeset...
  if (!cps_can_edit_entities('move to', $entity_type, $entity)) {
    return MENU_ACCESS_DENIED;
  }

  $form_state = array(
      'entity_type' => $entity_type,
      'entity_id' => $entity_id,
      'changeset_from' => $changeset,
      'changeset_to' => $changeset_to,
    ) + form_state_defaults();

  $output = drupal_build_form('cps_changeset_move_form', $form_state);
  return $output;
}

/**
 * Form callback to move an entity to the current changeset
 */
function cps_changeset_move_form($form, &$form_state) {
  /** @var CPSChangeset $changeset */
  $changeset_from = $form_state['changeset_from'];
  $entity_type = $form_state['entity_type'];
  $entity = entity_load_single($form_state['entity_type'], $form_state['entity_id']);
  $uri = $changeset_from->uri();

  $form['type'] = array(
    '#type' => 'radios',
    '#title' => t('Would you like to move or copy changes?'),
    '#description' => t('If you copy, the original site version will be untouched. If you move, all changes for this entity will be moved to the current site version. Note: You cannot move changes if the originating site version is not editable by you or has already been published.'),
    '#options' => array(
      'move' => t('Move'),
      'copy' => t('Copy'),
    ),
  );

  // Disable 'move' if they can't edit the original.
  if (!cps_can_edit_entities('update', $entity_type, $entity, $changeset_from->changeset_id)) {
    $form['type']['move']['#disabled'] = TRUE;
    $form['type']['#default_value'] = 'copy';
  }
  else {
    $form['type']['#default_value'] = 'move';
  }

  return confirm_form($form,
    t('Move or copy changes between site versions'),
    $uri['path'],
    t('Are you sure you want to perform the selected action on "%entity"?', array('%entity' => entity_label($entity_type, $entity), '%title' => $changeset_from->label())),
    t('Confirm'),
    t('Cancel')
  );
}

/**
 * Submit callback to remove an entity from a changeset.
 */
function cps_changeset_move_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $changeset_from = $form_state['changeset_from'];
    $changeset_to = $form_state['changeset_to'];
    $entity_type = $form_state['entity_type'];
    $entity_id = $form_state['entity_id'];

    // Override the changeset to make the correct version ID available.
    cps_override_changeset($changeset_from->changeset_id);
    $entity = entity_load_single($entity_type, $entity_id);

    cps_move_changeset($changeset_from, $changeset_to, $entity_type, $entity, $form_state['values']['type']);
    drupal_set_message(t('Changes to %entity have been @type from the site version %title to the current site version.', array(
      '%entity' => entity_label($entity_type, $entity),
      '%title' => $changeset_from->label(),
      '@type' => $form_state['values']['type'] == 'move' ? t('moved') : t('copied')
    )));
    $form_state['redirect'] = $changeset_from->uri();
  }
}

/**
 * Return a list of revisions with CPS information for the entity.
 *
 * @param object $entity
 *   The entity to find changesets for.
 * @return CPSChangeset[]
 *   An array of changesets, keyed by the revision ID they apply to..
 */
function cps_revision_list($entity, $entity_type) {
  list($entity_id, $revision_id, $bundle) = entity_extract_ids($entity_type, $entity);
  $live_revision_id = isset($entity->published_revision_id) ? $entity->published_revision_id : $revision_id;

  $entity_info = entity_get_info($entity_type);
  $revision_table = $entity_info['revision table'];
  $id_key = $entity_info['entity keys']['id'];
  $revision_key = $entity_info['entity keys']['revision'];

  $query = db_select('cps_entity', 'c')->extend('PagerDefault');
  $query->limit(25);
  $query->join($revision_table, 'r', "c.revision_id = r.$revision_key");
  $query->join('cps_changeset', 'cc', "cc.changeset_id = c.changeset_id");
  $query->fields('c', array('changeset_id', 'revision_id'))
    ->condition('c.entity_type', $entity_type)
    ->condition("r.$id_key", $entity_id)
    // We also only include where c.published - 0, because if it's set
    // that means the entry was added when a changeset was published to
    // preserve the site state at the time it was published and is not relevant
    // here.
    ->condition('c.published', 0)
    // the "initial" revision is a special one created to ensure that content
    // isn't accidentally visible on the site until published, and is not
    // relevant to show.
    ->condition('c.changeset_id', 'initial', '<>')
    // Live revision is always first.
    ->orderBy('CASE WHEN c.revision_id = ' . $live_revision_id . ' THEN 0 ELSE 1 END')
    // Then unpublished revisions.
    ->orderBy('CASE WHEN cc.published = 0 THEN 0 ELSE 1 END')
    // Then order by publish date.
    ->orderBy('cc.published', 'DESC')
    // And finally as a backup, by revision id.
    ->orderBy("r.$revision_key", 'DESC');

  $result = $query->execute();

  $keys = array();
  foreach ($result as $revision) {
    $keys[$revision->changeset_id] = $revision->revision_id;
  }

  $changesets = array();
  foreach (cps_changeset_load_multiple(array_keys($keys)) as $changeset_id => $changeset) {
    $changesets[$keys[$changeset_id]] = $changeset;
  }
  return $changesets;
}

/**
 * Page callback to render a revisions page.
 *
 * This can be added via hook_menu by any module implementing CPS support
 * for an entity to override the revisions page and show a CPS friendly
 * version of it.
 *
 * @param string $entity_type
 *   The entity type of the entity.
 * @param object $entity
 *   The entity to display revisions for.
 *
 * @return array
 *   A Drupal renderable array of the page.
 */
function cps_revisions_page($entity_type, $entity) {
  return drupal_get_form('cps_revisions_page_form', $entity_type, $entity);
}

/**
 * Form callback to render a revisions page.
 *
 * This can be added via hook_menu by any module implementing CPS support
 * for an entity to override the revisions page and show a CPS friendly
 * version of it.
 *
 * @param array $form
 *   The starting form provided by FAPI.
 * @param array $form_state
 *   The form state array reference.
 * @param string $entity_type
 *   The entity type of the entity.
 * @param object $entity
 *   The entity to display revisions for.
 *
 * @return array
 *   A Drupal renderable array of the page.
 */
function cps_revisions_page_form($form, &$form_state, $entity_type, $entity) {
  list($entity_id, $revision_id) = entity_extract_ids($entity_type, $entity);

  $live_revision_id = isset($entity->published_revision_id) ? $entity->published_revision_id : $revision_id;

  $uri = entity_uri($entity_type, $entity);
  $form_state['entity_uri'] = $uri['path'];
  drupal_set_title(t('History for %title', array('%title' => entity_label($entity_type, $entity))), PASS_THROUGH);

  $form['entity_revisions_table'] = array(
    'header' => array(
      'site_version' => array(
        '#markup' => t('Site version'),
      ),
      'author' => array(
        '#markup' => t('Author'),
      ),
      'status' => array(
        '#markup' => t('Status'),
      ),
      'diff_left' => array(),
      'diff_right' => array(),
      'operations' => array(
        '#markup' => t('Operations'),
      ),
    ),
  );

  $status_options = cps_changeset_get_state_labels();
  $changesets = cps_revision_list($entity, $entity_type);

  // Find the changeset that contains the published revision. If it does
  // not exist, find the most recent published changeset that contains
  // the entity.
  if (empty($changesets[$live_revision_id])) {
    $live_changeset_id = db_query("SELECT e.changeset_id FROM {cps_entity} e INNER JOIN {cps_changeset} c ON c.changeset_id = e.changeset_id WHERE e.entity_type = :entity_type AND e.entity_id = :entity_id AND c.published IS NOT NULL ORDER BY c.published DESC LIMIT 1", array(':entity_id' => $entity_id, ':entity_type' => $entity_type))->fetchField();
    if ($live_changeset_id) {
      $changesets = array($live_revision_id => cps_changeset_load($live_changeset_id)) + $changesets;
    }
    else {
      $form['entity_revisions_table']['body']['published'] = array(
        'site_version' => array(
          '#markup' => t('System created')
        ),
        'author' => array(
          '#markup' => t('System created')
        ),
        'status' => array(
          '#markup' => '<b>' . t('Live') . '</b>'
        ),
        'diff_left' => array(
          '#type' => 'radio',
          '#name' => 'diff_left',
          '#return_value' => 'published',
          '#access' => module_exists('diff'),
        ),
        'diff_right' => array(
          '#type' => 'radio',
          '#name' => 'diff_right',
          '#return_value' => 'published',
          '#access' => module_exists('diff'),
        ),
        'operations' => array(),
      );
    }
  }

  foreach ($changesets as $changeset_revision_id => $changeset) {
    $row = array();
    $changeset_uri = $changeset->uri();
    $row['site_version'] = array(
      '#markup' => l($changeset->label(), $changeset_uri['path'])
    );
    $row['author'] = array(
      '#markup' => format_username($changeset)
    );

    if ($changeset_revision_id == $live_revision_id) {
      $status = '<b>' . t('Live') . '</b>';
    }
    else {
      $status = $status_options[$changeset->status];
    }

    if (!empty($changeset->published)) {
      $status .= '<br/>' . t('Published on @date', array('@date' => format_date($changeset->published)));
    }

    $row['status'] = array(
      '#markup' => $status,
    );

    $row['diff_left'] = array(
      '#type' => 'radio',
      '#name' => 'diff_left',
      '#return_value' => $changeset->changeset_id,
      '#access' => module_exists('diff'),
    );

    $row['diff_right'] = array(
      '#type' => 'radio',
      '#name' => 'diff_right',
      '#return_value' => $changeset->changeset_id,
      '#access' => module_exists('diff'),
    );

    $row['operations'] = array(
      '#attributes' => array('class' => 'cps-changeset-operations'),
      'site' => array(
        '#theme' => 'link',
        '#text' => t('view'),
        '#path' => $uri['path'],
        '#options' => array(
          'query' => array('changeset_id' => $changeset->changeset_id),
          'html' => TRUE,
          'attributes' => array('class' => array('cps-changeset-operation'))
        ),
        '#access' => user_access('preview changesets') || user_access('view changesets') || user_access('administer changesets'),
      ),
    );

    $form['entity_revisions_table']['body'][$changeset->changeset_id] = $row;
  }

  // Now go through and add td/th markup to the header and body. This would
  // be way cooler if we had #type => 'table', '#type' => 'table_header',
  // '#type' => 'table_body' and '#type' => 'table_cell' but alas. This fakes
  // that a bit.
  $attributes = isset($form['entity_revisions_table']['#attributes']) ? $form['entity_revisions_table']['#attributes'] : array();
  $form['entity_revisions_table']['#prefix'] = '<table' . drupal_attributes($attributes) . '>';
  $form['entity_revisions_table']['#suffix'] = '</table>';

  foreach ($form['entity_revisions_table']['header'] as $key => &$data) {
    $attributes = isset($data['#attributes']) ? $data['#attributes'] : array();
    $data['#prefix'] = '<th' . drupal_attributes($attributes) . '>';
    $data['#suffix'] = '</th>';
  }

  $attributes = isset($form['entity_revisions_table']['header']['#attributes']) ? $form['entity_revisions_table']['header']['#attributes'] : array();
  $form['entity_revisions_table']['header']['#prefix'] = '<thead' . drupal_attributes($attributes) . '><tr>';
  $form['entity_revisions_table']['header']['#suffix'] = '</tr></thead>';

  $flip = array('even' => 'odd', 'odd' => 'even');
  $class = 'even';
  foreach ($form['entity_revisions_table']['body'] as $row_id => &$row) {
    foreach ($row as $key => &$data) {
      $attributes = isset($data['#attributes']) ? $data['#attributes'] : array();
      $data['#prefix'] = '<td' . drupal_attributes($attributes) . '>';
      $data['#suffix'] = '</td>';
    }
    $attributes = isset($row['#attributes']) ? $row['#attributes'] : array();
    $class = $flip[$class];
    $attributes['class'][] = $class;
    $row['#prefix'] = '<tr' . drupal_attributes($attributes) . '>';
    $row['#suffix'] = '</tr>';
  }

  $attributes = isset($form['entity_revisions_table']['body']['#attributes']) ? $form['entity_revisions_table']['body']['#attributes'] : array();
  $form['entity_revisions_table']['body']['#prefix'] = '<tbody' . drupal_attributes($attributes) . '>';
  $form['entity_revisions_table']['body']['#suffix'] = '</tbody>';

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Compare'),
    '#access' => module_exists('diff'),
  );

  // Attach the pager theme.
  $form['entity_revisions_table_pager'] = array('#theme' => 'pager');

  return $form;
}

/**
 * Submit code for input form to select two revisions.
 */
function cps_revisions_page_form_validate($form, &$form_state) {
  // The ids are ordered so the old revision is always on the left.
  $left = $form_state['input']['diff_left'];
  $right = $form_state['input']['diff_right'];
  if ($left == $right || !$left || !$right) {
    form_set_error('diff', t('Select different revisions to compare.'));
  }
}

/**
 * Form submit callback for the revisions page.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function cps_revisions_page_form_submit($form, &$form_state) {
  $form_state['redirect'] = $form_state['entity_uri'] . '/history/' . $form_state['input']['diff_left'] . '/' . $form_state['input']['diff_right'];
}

/**
 * Page callback to render a diff between two site versions.
 *`
 * @param string $entity_type
 *   The entity type.
 * @param object $entity
 *   The entity.
 * @param CPSChangeset $old_changeset
 *   The left (old) changeset.
 * @param CPSChangeset $new_changeset
 *   The right (new) changeset.
 * @param (optional) $state
 *   The state to view.
 *
 * @return array
 *   A Drupal render array.
 */
function cps_diff_page($entity_type, $entity, $old_changeset, $new_changeset, $state = NULL) {
  $build = array();

  $default_state = variable_get('diff_default_state_node', 'raw');
  if (empty($state)) {
    $state = $default_state;
  }
  $state = str_replace('-', '_', $state);
  if (!array_key_exists($state, diff_available_states())) {
    $state = $default_state;
  }

  // Copied from diff_diffs_show()
  $links = array();
  $uri = entity_uri($entity_type, $entity);
  $history_uri = $uri['path'] . '/history';
  foreach (diff_available_states($entity_type) as $alternative_state => $label) {
    $links[$alternative_state] = array(
      'title' => $label,
      'href' => "$history_uri/{$old_changeset->changeset_id}/{$new_changeset->changeset_id}" . ($alternative_state == $default_state ? '' : '/' . str_replace('_', '-', $alternative_state)),
    );
  }

  if (count($links) > 1) {
    $build['state_links'] = array(
      '#markup' => theme('links', array(
          'links' => $links,
          'attributes' => array('class' => array('links', 'inline')))
      ),
    );
  }

  list($entity_id) = entity_extract_ids($entity_type, $entity);

  // Load the left side.
  cps_override_changeset($old_changeset->changeset_id);
  $old_entity = entity_load_single($entity_type, $entity_id);

  // Load the right side.
  cps_override_changeset($new_changeset->changeset_id);
  $new_entity = entity_load_single($entity_type, $entity_id);

  // Put the changeset back the way we found it.
  cps_override_changeset(NULL);

  ctools_include('diff', 'cps');
  $build['table'] = cps_render_diff($entity_type, $old_entity, $new_entity, $state);

  // Generate table header (date, username, log message).
  $old_header = _cps_diff_header($old_changeset);
  $new_header = _cps_diff_header($new_changeset);

  // Add in a header that cps_diff() doesn't normally.
  $build['table']['#header'] = _diff_default_header($old_header, $new_header);

  return $build;
}

