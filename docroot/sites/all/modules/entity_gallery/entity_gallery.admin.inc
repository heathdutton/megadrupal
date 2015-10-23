<?php

/**
 * @file
 * Gallery administration and module settings UI.
 */

/**
 * Implements hook_entity_gallery_operations().
 */
function entity_gallery_entity_gallery_operations() {
  $operations = array(
    'publish' => array(
      'label' => t('Publish selected galleries'),
      'callback' => 'entity_gallery_mass_update',
      'callback arguments' => array('updates' => array('status' => ENTITY_GALLERY_PUBLISHED)),
    ),
    'unpublish' => array(
      'label' => t('Unpublish selected galleries'),
      'callback' => 'entity_gallery_mass_update',
      'callback arguments' => array('updates' => array('status' => ENTITY_GALLERY_NOT_PUBLISHED)),
    ),
    'sticky' => array(
      'label' => t('Make selected galleries sticky'),
      'callback' => 'entity_gallery_mass_update',
      'callback arguments' => array('updates' => array('status' => ENTITY_GALLERY_PUBLISHED, 'sticky' => ENTITY_GALLERY_STICKY)),
    ),
    'unsticky' => array(
      'label' => t('Make selected galleries not sticky'),
      'callback' => 'entity_gallery_mass_update',
      'callback arguments' => array('updates' => array('sticky' => ENTITY_GALLERY_NOT_STICKY)),
    ),
    'delete' => array(
      'label' => t('Delete selected galleries'),
      'callback' => NULL,
    ),
  );
  return $operations;
}

/**
 * List entity gallery administration filters that can be applied.
 *
 * @return
 *   An associative array of filters.
 */
function entity_gallery_filters() {
  // Regular filters
  $filters['status'] = array(
    'title' => t('status'),
    'options' => array(
      '[any]' => t('any'),
      'status-1' => t('published'),
      'status-0' => t('not published'),
      'sticky-1' => t('sticky'),
      'sticky-0' => t('not sticky'),
    ),
  );
  // Include translation states if we have this module enabled
  if (module_exists('translation')) {
    $filters['status']['options'] += array(
      'translate-0' => t('Up to date translation'),
      'translate-1' => t('Outdated translation'),
    );
  }

  $filters['type'] = array(
    'title' => t('type'),
    'options' => array(
      '[any]' => t('any'),
    ) + entity_gallery_type_get_names(),
  );

  // Language filter if there is a list of languages
  if ($languages = module_invoke('locale', 'language_list')) {
    $languages = array(LANGUAGE_NONE => t('Language neutral')) + $languages;
    $filters['language'] = array(
      'title' => t('language'),
      'options' => array(
        '[any]' => t('any'),
      ) + $languages,
    );
  }
  return $filters;
}

/**
 * Applies filters for entity gallery administration filters based on session.
 *
 * @param $query
 *   A SelectQuery to which the filters should be applied.
 */
function entity_gallery_build_filter_query(SelectQueryInterface $query) {
  // Build query
  $filter_data = isset($_SESSION['entity_gallery_overview_filter']) ? $_SESSION['entity_gallery_overview_filter'] : array();
  foreach ($filter_data as $index => $filter) {
    list($key, $value) = $filter;
    switch ($key) {
      case 'status':
        // Note: no exploitable hole as $key/$value have already been checked when submitted
        list($key, $value) = explode('-', $value, 2);
      case 'type':
      case 'language':
        $query->condition('eg.' . $key, $value);
        break;
    }
  }
}

/**
 * Returns the entity gallery administration filters form array to entity_gallery_admin_content().
 *
 * @see entity_gallery_admin_galleries()
 * @see entity_gallery_admin_galleries_submit()
 * @see entity_gallery_admin_galleries_validate()
 * @see entity_gallery_filter_form_submit()
 * @see entity_gallery_multiple_delete_confirm()
 * @see entity_gallery_multiple_delete_confirm_submit()
 *
 * @ingroup forms
 */
function entity_gallery_filter_form() {
  $session = isset($_SESSION['entity_gallery_overview_filter']) ? $_SESSION['entity_gallery_overview_filter'] : array();
  $filters = entity_gallery_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__entity_gallery',
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    if ($type == 'term') {
      // Load term name from DB rather than search and parse options array.
      $value = module_invoke('taxonomy', 'term_load', $value);
      $value = $value->name;
    }
    elseif ($type == 'language') {
      $value = $value == LANGUAGE_NONE ? t('Language neutral') : module_invoke('locale', 'language_name', $value);
    }
    else {
      $value = $filters[$type]['options'][$value];
    }
    $t_args = array('%property' => $filters[$type]['title'], '%value' => $value);
    if ($i++) {
      $form['filters']['current'][] = array('#markup' => t('and where %property is %value', $t_args));
    }
    else {
      $form['filters']['current'][] = array('#markup' => t('where %property is %value', $t_args));
    }
    if (in_array($type, array('type', 'language'))) {
      // Remove the option if it is already being filtered on.
      unset($filters[$type]);
    }
  }

  $form['filters']['status'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('clearfix')),
    '#prefix' => ($i ? '<div class="additional-filters">' . t('and where') . '</div>' : ''),
  );
  $form['filters']['status']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status']['filters'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
      '#title' => $filter['title'],
      '#default_value' => '[any]',
    );
  }

  $form['filters']['status']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['status']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => count($session) ? t('Refine') : t('Filter'),
  );
  if (count($session)) {
    $form['filters']['status']['actions']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['status']['actions']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  }

  drupal_add_js('misc/form.js');

  return $form;
}

/**
 * Form submission handler for entity_gallery_filter_form().
 *
 * @see entity_gallery_admin_gallery()
 * @see entity_gallery_admin_galleries()
 * @see entity_gallery_admin_galleries_submit()
 * @see entity_gallery_admin_galleries_validate()
 * @see entity_gallery_filter_form()
 * @see entity_gallery_multiple_delete_confirm()
 * @see entity_gallery_multiple_delete_confirm_submit()
 */
function entity_gallery_filter_form_submit($form, &$form_state) {
  $filters = entity_gallery_filters();
  switch ($form_state['values']['op']) {
    case t('Filter'):
    case t('Refine'):
      // Apply every filter that has a choice selected other than 'any'.
      foreach ($filters as $filter => $options) {
        if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
          // Flatten the options array to accommodate hierarchical/nested options.
          $flat_options = form_options_flatten($filters[$filter]['options']);
          // Only accept valid selections offered on the dropdown, block bad input.
          if (isset($flat_options[$form_state['values'][$filter]])) {
            $_SESSION['entity_gallery_overview_filter'][] = array($filter, $form_state['values'][$filter]);
          }
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['entity_gallery_overview_filter']);
      break;
    case t('Reset'):
      $_SESSION['entity_gallery_overview_filter'] = array();
      break;
  }
}

/**
 * Make mass update of entity galleries, changing all entity galleries in the
 * $entity_galleries array to update them with the field values in $updates.
 *
 * IMPORTANT NOTE: This function is intended to work when called from a form
 * submission handler. Calling it outside of the form submission process may not
 * work correctly.
 *
 * @param array $entity_galleries
 *   Array of entity gallery egids to update.
 * @param array $updates
 *   Array of key/value pairs with entity gallery field names and the value to
 *   update that field to.
 */
function entity_gallery_mass_update($entity_galleries, $updates) {
  // We use batch processing to prevent timeout when updating a large number
  // of entity galleries.
  if (count($entity_galleries) > 10) {
    $batch = array(
      'operations' => array(
        array('_entity_gallery_mass_update_batch_process', array($entity_galleries, $updates))
      ),
      'finished' => '_entity_gallery_mass_update_batch_finished',
      'title' => t('Processing'),
      // We use a single multi-pass operation, so the default
      // 'Remaining x of y operations' message will be confusing here.
      'progress_message' => '',
      'error_message' => t('The update has encountered an error.'),
      // The operations do not live in the .module file, so we need to
      // tell the batch engine which file to load before calling them.
      'file' => drupal_get_path('module', 'entity_gallery') . '/entity_gallery.admin.inc',
    );
    batch_set($batch);
  }
  else {
    foreach ($entity_galleries as $egid) {
      _entity_gallery_mass_update_helper($egid, $updates);
    }
    drupal_set_message(t('The update has been performed.'));
  }
}

/**
 * Updates individual entity galleries when fewer than 10 are queued.
 *
 * @param $egid
 *   ID of the entity gallery to update.
 * @param $updates
 *   Associative array of updates.
 *
 * @return object
 *   An updated entity gallery object.
 *
 * @see entity_gallery_mass_update()
 */
function _entity_gallery_mass_update_helper($egid, $updates) {
  $entity_gallery = entity_gallery_load($egid, NULL, TRUE);
  // For efficiency manually save the original entity gallery before applying
  // any changes.
  $entity_gallery->original = clone $entity_gallery;
  foreach ($updates as $name => $value) {
    $entity_gallery->$name = $value;
  }
  entity_gallery_save($entity_gallery);
  return $entity_gallery;
}

/**
 * Executes a batch operation for entity_gallery_mass_update().
 *
 * @param array $entity_galleries
 *   An array of entity gallery IDs.
 * @param array $updates
 *   Associative array of updates.
 * @param array $context
 *   An array of contextual key/values.
 */
function _entity_gallery_mass_update_batch_process($entity_galleries, $updates, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($entity_galleries);
    $context['sandbox']['entity_galleries'] = $entity_galleries;
  }

  // Process entity galleries by groups of 5.
  $count = min(5, count($context['sandbox']['entity_galleries']));
  for ($i = 1; $i <= $count; $i++) {
    // For each egid, load the entity gallery, reset the values, and save it.
    $egid = array_shift($context['sandbox']['entity_galleries']);
    $entity_gallery = _entity_gallery_mass_update_helper($egid, $updates);

    // Store result for post-processing in the finished callback.
    $context['results'][] = l($entity_gallery->title, 'gallery/' . $entity_gallery->egid);

    // Update our progress information.
    $context['sandbox']['progress']++;
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Menu callback: Reports the status of batch operation for entity_gallery_mass_update().
 *
 * @param bool $success
 *   A boolean indicating whether the batch mass update operation successfully
 *   concluded.
 * @param int $results
 *   The number of entity galleries updated via the batch mode process.
 * @param array $operations
 *   An array of function calls (not used in this function).
 */
function _entity_gallery_mass_update_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('The update has been performed.'));
  }
  else {
    drupal_set_message(t('An error occurred and processing did not complete.'), 'error');
    $message = format_plural(count($results), '1 item successfully processed:', '@count items successfully processed:');
    $message .= theme('item_list', array('items' => $results));
    drupal_set_message($message);
  }
}

/**
 * Page callback: Form constructor for the gallery administration form.
 *
 * @see entity_gallery_admin_galleries()
 * @see entity_gallery_admin_galleries_submit()
 * @see entity_gallery_admin_galleries_validate()
 * @see entity_gallery_filter_form()
 * @see entity_gallery_filter_form_submit()
 * @see entity_gallery_menu()
 * @see entity_gallery_multiple_delete_confirm()
 * @see entity_gallery_multiple_delete_confirm_submit()
 * @ingroup forms
 */
function entity_gallery_admin_gallery($form, $form_state) {
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    return entity_gallery_multiple_delete_confirm($form, $form_state, array_filter($form_state['values']['entity_galleries']));
  }
  $form['filter'] = entity_gallery_filter_form();
  $form['#submit'][] = 'entity_gallery_filter_form_submit';
  $form['admin'] = entity_gallery_admin_entity_galleries();

  return $form;
}

/**
 * Form builder: Builds the entity gallery administration overview.
 *
 * @see entity_gallery_admin_galleries_submit()
 * @see entity_gallery_admin_galleries_validate()
 * @see entity_gallery_filter_form()
 * @see entity_gallery_filter_form_submit()
 * @see entity_gallery_multiple_delete_confirm()
 * @see entity_gallery_multiple_delete_confirm_submit()
 *
 * @ingroup forms
 */
function entity_gallery_admin_entity_galleries() {
  $admin_access = user_access('administer entity galleries');

  // Build the 'Update options' form.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
    '#access' => $admin_access,
  );
  $options = array();
  foreach (module_invoke_all('entity_gallery_operations') as $operation => $array) {
    $options[$operation] = $array['label'];
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'approve',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array('entity_gallery_admin_galleries_validate'),
    '#submit' => array('entity_gallery_admin_galleries_submit'),
  );

  // Enable language column if translation module is enabled or if we have any
  // entity gallery with language.
  $multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {entity_gallery} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());

  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'eg.title'),
    'type' => array('data' => t('Type'), 'field' => 'eg.type'),
    'author' => t('Author'),
    'status' => array('data' => t('Status'), 'field' => 'eg.status'),
    'changed' => array('data' => t('Updated'), 'field' => 'eg.changed', 'sort' => 'desc')
  );
  if ($multilanguage) {
    $header['language'] = array('data' => t('Language'), 'field' => 'eg.language');
  }
  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('entity_gallery', 'eg')->extend('PagerDefault')->extend('TableSort');
  $query->addTag('entity_gallery_admin_filter');
  entity_gallery_build_filter_query($query);

  if (!user_access('bypass entity gallery access')) {
    // If the user is able to view their own unpublished entity galleries, allow
    // them to see these in addition to published entity galleries. Check that
    // they actually have some unpublished entity galleries to view before
    // adding the condition.
    if (user_access('view own unpublished entity galleries') && $own_unpublished = db_query('SELECT egid FROM {entity_gallery} WHERE uid = :uid AND status = :status', array(':uid' => $GLOBALS['user']->uid, ':status' => 0))->fetchCol()) {
      $query->condition(db_or()
        ->condition('eg.status', 1)
        ->condition('eg.egid', $own_unpublished, 'IN')
      );
    }
    else {
      // If not, restrict the query to published entity galleries.
      $query->condition('eg.status', 1);
    }
  }
  $egids = $query
    ->fields('eg',array('egid'))
    ->limit(50)
    ->orderByHeader($header)
    ->addTag('entity_gallery_access')
    ->execute()
    ->fetchCol();
  $entity_galleries = entity_gallery_load_multiple($egids);

  // Prepare the list of entity galleries.
  $languages = language_list();
  $destination = drupal_get_destination();
  $options = array();
  foreach ($entity_galleries as $entity_gallery) {
    $langcode = entity_language('entity_gallery', $entity_gallery);
    $l_options = $langcode != LANGUAGE_NONE && isset($languages[$langcode]) ? array('language' => $languages[$langcode]) : array();
    $options[$entity_gallery->egid] = array(
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $entity_gallery->title,
          '#href' => 'gallery/' . $entity_gallery->egid,
          '#options' => $l_options,
        ),
      ),
      'type' => check_plain(entity_gallery_type_get_name($entity_gallery)),
      'author' => theme('username', array('account' => $entity_gallery)),
      'status' => $entity_gallery->status ? t('published') : t('not published'),
      'changed' => format_date($entity_gallery->changed, 'short'),
    );
    if ($multilanguage) {
      if ($langcode == LANGUAGE_NONE || isset($languages[$langcode])) {
        $options[$entity_gallery->egid]['language'] = $langcode == LANGUAGE_NONE ? t('Language neutral') : t($languages[$langcode]->name);
      }
      else {
        $options[$entity_gallery->egid]['language'] = t('Undefined language (@langcode)', array('@langcode' => $langcode));
      }
    }
    // Build a list of all the accessible operations for the current entity
    // gallery.
    $operations = array();
    if (entity_gallery_access('update', $entity_gallery)) {
      $operations['edit'] = array(
        'title' => t('edit'),
        'href' => 'gallery/' . $entity_gallery->egid . '/edit',
        'query' => $destination,
      );
    }
    if (entity_gallery_access('delete', $entity_gallery)) {
      $operations['delete'] = array(
        'title' => t('delete'),
        'href' => 'gallery/' . $entity_gallery->egid . '/delete',
        'query' => $destination,
      );
    }
    $options[$entity_gallery->egid]['operations'] = array();
    if (count($operations) > 1) {
      // Render an unordered list of operations links.
      $options[$entity_gallery->egid]['operations'] = array(
        'data' => array(
          '#theme' => 'links__entity_gallery_operations',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
    }
    elseif (!empty($operations)) {
      // Render the first and only operation as a link.
      $link = reset($operations);
      $options[$entity_gallery->egid]['operations'] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => $link['title'],
          '#href' => $link['href'],
          '#options' => array('query' => $link['query']),
        ),
      );
    }
  }

  // Only use a tableselect when the current user is able to perform any
  // operations.
  if ($admin_access) {
    $form['entity_galleries'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#empty' => t('No galleries available.'),
    );
  }
  // Otherwise, use a simple table.
  else {
    $form['entity_galleries'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options,
      '#empty' => t('No galleries available.'),
    );
  }

  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}

/**
 * Validate entity_gallery_admin_galleries form submissions.
 *
 * Checks whether any entity galleries have been selected to perform the chosen
 * 'Update option' on.
 *
 * @see entity_gallery_admin_galleries()
 * @see entity_gallery_admin_galleries_submit()
 * @see entity_gallery_filter_form()
 * @see entity_gallery_filter_form_submit()
 * @see entity_gallery_multiple_delete_confirm()
 * @see entity_gallery_multiple_delete_confirm_submit()
 */
function entity_gallery_admin_galleries_validate($form, &$form_state) {
  // Error if there are no items to select.
  if (!is_array($form_state['values']['entity_galleries']) || !count(array_filter($form_state['values']['entity_galleries']))) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Process entity_gallery_admin_galleries form submissions.
 *
 * Executes the chosen 'Update option' on the selected entity galleries.
 *
 * @see entity_gallery_admin_galleries()
 * @see entity_gallery_admin_galleries_validate()
 * @see entity_gallery_filter_form()
 * @see entity_gallery_filter_form_submit()
 * @see entity_gallery_multiple_delete_confirm()
 * @see entity_gallery_multiple_delete_confirm_submit()
 */
function entity_gallery_admin_galleries_submit($form, &$form_state) {
  $operations = module_invoke_all('entity_gallery_operations');
  $operation = $operations[$form_state['values']['operation']];
  // Filter out unchecked entity galleries
  $entity_galleries = array_filter($form_state['values']['entity_galleries']);
  if ($function = $operation['callback']) {
    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($entity_galleries), $operation['callback arguments']);
    }
    else {
      $args = array($entity_galleries);
    }
    call_user_func_array($function, $args);

    cache_clear_all();
  }
  else {
    // We need to rebuild the form to go to a second step. For example, to
    // show the confirmation form for the deletion of entity galleries.
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Multiple entity gallery deletion confirmation form for entity_gallery_admin_gallery().
 *
 * @see entity_gallery_admin_galleries()
 * @see entity_gallery_admin_galleries_submit()
 * @see entity_gallery_admin_galleries_validate()
 * @see entity_gallery_filter_form()
 * @see entity_gallery_filter_form_submit()
 * @see entity_gallery_multiple_delete_confirm_submit()
 * @ingroup forms
 */
function entity_gallery_multiple_delete_confirm($form, &$form_state, $entity_galleries) {
  $form['entity_galleries'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
  // array_filter returns only elements with TRUE values
  foreach ($entity_galleries as $egid => $value) {
    $title = db_query('SELECT title FROM {entity_gallery} WHERE egid = :egid', array(':egid' => $egid))->fetchField();
    $form['entity_galleries'][$egid] = array(
      '#type' => 'hidden',
      '#value' => $egid,
      '#prefix' => '<li>',
      '#suffix' => check_plain($title) . "</li>\n",
    );
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
  $form['#submit'][] = 'entity_gallery_multiple_delete_confirm_submit';
  $confirm_question = format_plural(count($entity_galleries),
                                  'Are you sure you want to delete this item?',
                                  'Are you sure you want to delete these items?');
  return confirm_form($form,
                    $confirm_question,
                    'admin/content/gallery', t('This action cannot be undone.'),
                    t('Delete'), t('Cancel'));
}

/**
 * Form submission handler for entity_gallery_multiple_delete_confirm().
 *
 * @see entity_gallery_admin_galleries()
 * @see entity_gallery_admin_galleries_submit()
 * @see entity_gallery_admin_galleries_validate()
 * @see entity_gallery_filter_form()
 * @see entity_gallery_filter_form_submit()
 * @see entity_gallery_multiple_delete_confirm()
 */
function entity_gallery_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    entity_gallery_delete_multiple(array_keys($form_state['values']['entity_galleries']));
    cache_clear_all();
    $count = count($form_state['values']['entity_galleries']);
    watchdog('gallery', 'Deleted @count posts.', array('@count' => $count));
    drupal_set_message(format_plural($count, 'Deleted 1 gallery.', 'Deleted @count galleries.'));
  }
  $form_state['redirect'] = 'admin/content/gallery';
}
