<?php

/**
 * @file
 * Page callbacks for adding, editing, deleting, and revisions management for galleries.
 */

/**
 * Menu callback; presents the entity gallery editing form.
 */
function entity_gallery_page_edit($entity_gallery) {
  $type_name = entity_gallery_type_get_name($entity_gallery);
  drupal_set_title(t('<em>Edit @type</em> @title', array('@type' => $type_name, '@title' => $entity_gallery->title)), PASS_THROUGH);
  return drupal_get_form($entity_gallery->type . '_entity_gallery_form', $entity_gallery);
}

/**
 * Page callback: Displays add gallery links for available gallery types.
 *
 * Redirects to gallery/add/[type] if only one gallery type is available.
 *
 * @see entity_gallery_menu()
 */
function entity_gallery_add_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);
  // Bypass the gallery/add listing if only one gallery type is available.
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }
  return theme('entity_gallery_add_list', array('content' => $content));
}

/**
 * Returns HTML for a list of available entity gallery types for entity gallery creation.
 *
 * @param $variables
 *   An associative array containing:
 *   - content: An array of gallery types.
 *
 * @ingroup themeable
 */
function theme_entity_gallery_add_list($variables) {
  $content = $variables['content'];
  $output = '';

  if ($content) {
    $output = '<dl class="entity-gallery-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    $output = '<p>' . t('You have not created any gallery types yet. Go to the <a href="@create-gallery">gallery type creation page</a> to add a new gallery type.', array('@create-gallery' => url('admin/structure/gallery-types/add'))) . '</p>';
  }
  return $output;
}


/**
 * Returns an entity gallery submission form.
 *
 * @param $type
 *   The entity gallery type for the submitted entity gallery.
 *
 * @return
 *   The themed form.
 */
function entity_gallery_add($type) {
  global $user;

  $types = entity_gallery_type_get_types();
  $entity_gallery = (object) array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => $type, 'language' => LANGUAGE_NONE);
  drupal_set_title(t('Create @name', array('@name' => $types[$type]->name)), PASS_THROUGH);
  $output = drupal_get_form($type . '_entity_gallery_form', $entity_gallery);

  return $output;
}

/**
 * Form validation handler for entity_gallery_form().
 *
 * @see entity_gallery_form()
 * @see entity_gallery_form_submit()
 */
function entity_gallery_form_validate($form, &$form_state) {
  // $form_state['entity_gallery'] contains the actual entity being edited, but
  // we must not update it with form values that have not yet been validated, so
  // we create a pseudo-entity to use during validation.
  $entity_gallery = (object) $form_state['values'];
  entity_gallery_validate($entity_gallery, $form, $form_state);
  entity_form_field_validate('entity_gallery', $form, $form_state);
}

/**
 * Form constructor for the entity gallery add/edit form.
 *
 * @see entity_gallery_form_validate()
 * @see entity_gallery_form_submit()
 * @see entity_gallery_form_build_preview()
 * @see entity_gallery_form_delete_submit()
 * @ingroup forms
 */
function entity_gallery_form($form, &$form_state, $entity_gallery) {
  global $user;

  // During initial form build, add the entity gallery entity to the form state
  // for use during form building and processing. During a rebuild, use what is
  // in the form state.
  if (!isset($form_state['entity_gallery'])) {
    if (!isset($entity_gallery->title)) {
      $entity_gallery->title = NULL;
    }
    entity_gallery_object_prepare($entity_gallery);
    $form_state['entity_gallery'] = $entity_gallery;
  }
  else {
    $entity_gallery = $form_state['entity_gallery'];
  }

  // Some special stuff when previewing an entity gallery.
  if (isset($form_state['entity_gallery_preview'])) {
    $form['#prefix'] = $form_state['entity_gallery_preview'];
    $entity_gallery->in_preview = TRUE;
  }
  else {
    unset($entity_gallery->in_preview);
  }

  // Identify this as an entity gallery edit form.
  // @todo D8: Remove. Modules can implement hook_form_BASE_FORM_ID_alter() now.
  $form['#entity_gallery_edit_form'] = TRUE;

  $form['#attributes']['class'][] = 'entity-gallery-form';
  if (!empty($entity_gallery->type)) {
    $form['#attributes']['class'][] = 'entity-gallery-' . $entity_gallery->type . '-form';
  }

  // Basic entity gallery information.
  // These elements are just values so they are not even sent to the client.
  foreach (array('egid', 'vid', 'uid', 'created', 'type', 'language') as $key) {
    $form[$key] = array(
      '#type' => 'value',
      '#value' => isset($entity_gallery->$key) ? $entity_gallery->$key : NULL,
    );
  }

  // Changed must be sent to the client, for later overwrite error checking.
  $form['changed'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($entity_gallery->changed) ? $entity_gallery->changed : NULL,
  );
  // Invoke hook_form() to get the entity gallery-specific bits. Can't use
  // entity_gallery_invoke(), because hook_form() needs to be able to receive
  // $form_state by reference. @todo hook_form() implementations are unable to
  // add #validate or #submit handlers to the form buttons below. Remove
  // hook_form() entirely.
  $function = entity_gallery_type_get_base($entity_gallery) . '_form';
  if (function_exists($function) && ($extra = $function($entity_gallery, $form_state))) {
    $form = array_merge_recursive($form, $extra);
  }
  // If the entity gallery type has a title, and the entity gallery type form
  // defined no special weight for it, we default to a weight of -5 for
  // consistency.
  if (isset($form['title']) && !isset($form['title']['#weight'])) {
    $form['title']['#weight'] = -5;
  }
  // @todo D8: Remove. Modules should access the entity gallery using $form_state['entity_gallery'].
  $form['#entity_gallery'] = $entity_gallery;

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  // Add a log field if the "Create new revision" option is checked, or if the
  // current user has the ability to check that option.
  $form['revision_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Revision information'),
    '#collapsible' => TRUE,
    // Collapsed by default when "Create new revision" is unchecked
    '#collapsed' => !$entity_gallery->revision,
    '#group' => 'additional_settings',
    '#attributes' => array(
      'class' => array('entity-gallery-form-revision-information'),
    ),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'entity_gallery') . '/entity_gallery.js'),
    ),
    '#weight' => 20,
    '#access' => $entity_gallery->revision || user_access('administer entity galleries'),
  );
  $form['revision_information']['revision'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create new revision'),
    '#default_value' => $entity_gallery->revision,
    '#access' => user_access('administer entity galleries'),
  );
  // Check the revision log checkbox when the log textarea is filled in.
  // This must not happen if "Create new revision" is enabled by default, since
  // the state would auto-disable the checkbox otherwise.
  if (!$entity_gallery->revision) {
    $form['revision_information']['revision']['#states'] = array(
      'checked' => array(
        'textarea[name="log"]' => array('empty' => FALSE),
      ),
    );
  }
  $form['revision_information']['log'] = array(
    '#type' => 'textarea',
    '#title' => t('Revision log message'),
    '#rows' => 4,
    '#default_value' => !empty($entity_gallery->log) ? $entity_gallery->log : '',
    '#description' => t('Provide an explanation of the changes you are making. This will help other authors understand your motivations.'),
  );

  // Entity gallery author information for administrators
  $form['author'] = array(
    '#type' => 'fieldset',
    '#access' => user_access('administer entity galleries'),
    '#title' => t('Authoring information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
    '#attributes' => array(
      'class' => array('entity-gallery-form-author'),
    ),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'entity_gallery') . '/entity_gallery.js',
        array(
          'type' => 'setting',
          'data' => array('anonymous' => variable_get('anonymous', t('Anonymous'))),
        ),
      ),
    ),
    '#weight' => 90,
  );
  $form['author']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Authored by'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => !empty($entity_gallery->name) ? $entity_gallery->name : '',
    '#weight' => -1,
    '#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))),
  );
  $form['author']['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Authored on'),
    '#maxlength' => 25,
    '#description' => t('Format: %time. The date format is YYYY-MM-DD and %timezone is the time zone offset from UTC. Leave blank to use the time of form submission.', array('%time' => !empty($entity_gallery->date) ? date_format(date_create($entity_gallery->date), 'Y-m-d H:i:s O') : format_date($entity_gallery->created, 'custom', 'Y-m-d H:i:s O'), '%timezone' => !empty($entity_gallery->date) ? date_format(date_create($entity_gallery->date), 'O') : format_date($entity_gallery->created, 'custom', 'O'))),
    '#default_value' => !empty($entity_gallery->date) ? $entity_gallery->date : '',
  );

  // Entity gallery options for administrators
  $form['options'] = array(
    '#type' => 'fieldset',
    '#access' => user_access('administer entity galleries'),
    '#title' => t('Publishing options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
    '#attributes' => array(
      'class' => array('entity-gallery-form-options'),
    ),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'entity_gallery') . '/entity_gallery.js'),
    ),
    '#weight' => 95,
  );
  $form['options']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Published'),
    '#default_value' => $entity_gallery->status,
  );
  $form['options']['sticky'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sticky at top of lists'),
    '#default_value' => $entity_gallery->sticky,
  );

  // Add the buttons.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#access' => variable_get('entity_gallery_preview_' . $entity_gallery->type, DRUPAL_OPTIONAL) != DRUPAL_REQUIRED || (!form_get_errors() && isset($form_state['entity_gallery_preview'])),
    '#value' => t('Save'),
    '#weight' => 5,
    '#submit' => array('entity_gallery_form_submit'),
  );
  $form['actions']['preview'] = array(
    '#access' => variable_get('entity_gallery_preview_' . $entity_gallery->type, DRUPAL_OPTIONAL) != DRUPAL_DISABLED,
    '#type' => 'submit',
    '#value' => t('Preview'),
    '#weight' => 10,
    '#submit' => array('entity_gallery_form_build_preview'),
  );
  if (!empty($entity_gallery->egid) && entity_gallery_access('delete', $entity_gallery)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('entity_gallery_form_delete_submit'),
    );
  }
  // This form uses a button-level #submit handler for the form's main submit
  // action. entity_gallery_form_submit() manually invokes all form-level
  // #submit handlers of the form. Without explicitly setting #submit, Form API
  // would auto-detect entity_gallery_form_submit() as submit handler, but that
  // is the button-level #submit handler for the 'Save' action. To maintain
  // backwards compatibility, a #submit handler is auto-suggested for custom
  // entity gallery type modules.
  $form['#validate'][] = 'entity_gallery_form_validate';
  if (!isset($form['#submit']) && function_exists($entity_gallery->type . '_entity_gallery_form_submit')) {
    $form['#submit'][] = $entity_gallery->type . '_entity_gallery_form_submit';
  }
  $form += array('#submit' => array());

  field_attach_form('entity_gallery', $entity_gallery, $form, $form_state, entity_language('entity_gallery', $entity_gallery));
  return $form;
}

/**
 * Form submission handler for entity_gallery_form().
 *
 * Handles the 'Delete' button on the entity gallery form.
 *
 * @see entity_gallery_form()
 * @see entity_gallery_form_validate()
 */
function entity_gallery_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $entity_gallery = $form['#entity_gallery'];
  $form_state['redirect'] = array('gallery/' . $entity_gallery->egid . '/delete', array('query' => $destination));
}

/**
 * Form submission handler for entity_gallery_form().
 *
 * Handles the 'Preview' button on the entity gallery form.
 *
 * @see entity_gallery_form()
 * @see entity_gallery_form_validate()
 */
function entity_gallery_form_build_preview($form, &$form_state) {
  $entity_gallery = entity_gallery_form_submit_build_entity_gallery($form, $form_state);
  $form_state['entity_gallery_preview'] = entity_gallery_preview($entity_gallery);
  $form_state['rebuild'] = TRUE;
}

/**
 * Generates an entity gallery preview.
 *
 * @param $entity_gallery
 *   The entity gallery to preview.
 *
 * @return
 *   An HTML-formatted string of an entity gallery preview.
 *
 * @see entity_gallery_form_build_preview()
 */
function entity_gallery_preview($entity_gallery) {
  // Clone the entity gallery before previewing it to prevent the entity gallery
  // itself from being modified.
  $cloned_entity_gallery = clone $entity_gallery;
  if (entity_gallery_access('create', $cloned_entity_gallery) || entity_gallery_access('update', $cloned_entity_gallery)) {
    _field_invoke_multiple('load', 'entity_gallery', array($cloned_entity_gallery->egid => $cloned_entity_gallery));
    // Load the user's name when needed.
    if (isset($cloned_entity_gallery->name)) {
      // The use of isset() is mandatory in the context of user IDs, because
      // user ID 0 denotes the anonymous user.
      if ($user = user_load_by_name($cloned_entity_gallery->name)) {
        $cloned_entity_gallery->uid = $user->uid;
        $cloned_entity_gallery->picture = $user->picture;
      }
      else {
        $cloned_entity_gallery->uid = 0; // anonymous user
      }
    }
    elseif ($cloned_entity_gallery->uid) {
      $user = user_load($cloned_entity_gallery->uid);
      $cloned_entity_gallery->name = $user->name;
      $cloned_entity_gallery->picture = $user->picture;
    }

    $cloned_entity_gallery->changed = REQUEST_TIME;
    $entity_galleries = array($cloned_entity_gallery->egid => $cloned_entity_gallery);
    field_attach_prepare_view('entity_gallery', $entity_galleries, 'full');

    // Display a preview of the entity gallery.
    if (!form_get_errors()) {
      $cloned_entity_gallery->in_preview = TRUE;
      $output = theme('entity_gallery_preview', array('entity_gallery' => $cloned_entity_gallery));
      unset($cloned_entity_gallery->in_preview);
    }
    drupal_set_title(t('Preview'), PASS_THROUGH);

    return $output;
  }
}

/**
 * Returns HTML for an entity gallery preview for display during entity gallery creation and editing.
 *
 * @param $variables
 *   An associative array containing:
 *   - entity_gallery: The entity gallery object which is being previewed.
 *
 * @see entity_gallery_preview()
 * @ingroup themeable
 */
function theme_entity_gallery_preview($variables) {
  $entity_gallery = $variables['entity_gallery'];

  $output = '<div class="preview">';

  $preview_trimmed_version = FALSE;

  $elements = entity_gallery_view(clone $entity_gallery, 'teaser');
  $trimmed = drupal_render($elements);
  $elements = entity_gallery_view($entity_gallery, 'full');
  $full = drupal_render($elements);

  // Do we need to preview trimmed version of post as well as full version?
  if ($trimmed != $full) {
    drupal_set_message(t('The trimmed version of your gallery shows what your gallery looks like when displayed in a list or when exported for syndication.<span class="no-js"> You can insert the delimiter "&lt;!--break--&gt;" (without the quotes) to fine-tune where your gallery gets split.</span>'));
    $output .= '<h3>' . t('Preview trimmed version') . '</h3>';
    $output .= $trimmed;
    $output .= '<h3>' . t('Preview full version') . '</h3>';
    $output .= $full;
  }
  else {
    $output .= $full;
  }
  $output .= "</div>\n";

  return $output;
}

/**
 * Form submission handler for entity_gallery_form().
 *
 * @see entity_gallery_form()
 * @see entity_gallery_form_validate()
 */
function entity_gallery_form_submit($form, &$form_state) {
  $entity_gallery = entity_gallery_form_submit_build_entity_gallery($form, $form_state);
  $insert = empty($entity_gallery->egid);
  entity_gallery_save($entity_gallery);
  $entity_gallery_link = l(t('view'), 'gallery/' . $entity_gallery->egid);
  $watchdog_args = array('@type' => $entity_gallery->type, '%title' => $entity_gallery->title);
  $t_args = array('@type' => entity_gallery_type_get_name($entity_gallery), '%title' => $entity_gallery->title);

  if ($insert) {
    watchdog('gallery', '@type: added %title.', $watchdog_args, WATCHDOG_NOTICE, $entity_gallery_link);
    drupal_set_message(t('@type %title has been created.', $t_args));
  }
  else {
    watchdog('gallery', '@type: updated %title.', $watchdog_args, WATCHDOG_NOTICE, $entity_gallery_link);
    drupal_set_message(t('@type %title has been updated.', $t_args));
  }
  if ($entity_gallery->egid) {
    $form_state['values']['egid'] = $entity_gallery->egid;
    $form_state['egid'] = $entity_gallery->egid;
    $form_state['redirect'] = entity_gallery_access('view', $entity_gallery) ? 'gallery/' . $entity_gallery->egid : '<front>';
  }
  else {
    // In the unlikely case something went wrong on save, the entity gallery
    // will be rebuilt and entity gallery form redisplayed the same way as in
    // preview.
    drupal_set_message(t('The gallery could not be saved.'), 'error');
    $form_state['rebuild'] = TRUE;
  }
  // Clear the page and block caches.
  cache_clear_all();
}

/**
 * Updates the form state's entity gallery entity by processing this submission's values.
 *
 * This is the default builder function for the entity gallery form. It is
 * called during the "Save" and "Preview" submit handlers to retrieve the entity
 * to save or preview. This function can also be called by a "Next" button of a
 * wizard to update the form state's entity with the current step's values
 * before proceeding to the next step.
 *
 * @see entity_gallery_form()
 */
function entity_gallery_form_submit_build_entity_gallery($form, &$form_state) {
  // @todo Legacy support for modules that extend the entity gallery form with
  // form-level submit handlers that adjust $form_state['values'] prior to those
  // values being used to update the entity. Module authors are encouraged to
  // instead adjust the entity gallery directly within a
  // hook_entity_gallery_submit() implementation. For Drupal 8, evaluate whether
  // the pattern of triggering form-level submit handlers during button-level
  // submit processing is worth supporting properly, and if so, add a Form API
  // function for doing so.
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);

  $entity_gallery = $form_state['entity_gallery'];
  entity_form_submit_build_entity('entity_gallery', $entity_gallery, $form, $form_state);

  entity_gallery_submit($entity_gallery);
  foreach (module_implements('entity_gallery_submit') as $module) {
    $function = $module . '_entity_gallery_submit';
    $function($entity_gallery, $form, $form_state);
  }
  return $entity_gallery;
}

/**
 * Form constructor for the entity gallery deletion confirmation form.
 *
 * @see entity_gallery_delete_confirm_submit()
 */
function entity_gallery_delete_confirm($form, &$form_state, $entity_gallery) {
  $form['#entity_gallery'] = $entity_gallery;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['egid'] = array('#type' => 'value', '#value' => $entity_gallery->egid);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $entity_gallery->title)),
    'gallery/' . $entity_gallery->egid,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Executes entity gallery deletion.
 *
 * @see entity_gallery_delete_confirm()
 */
function entity_gallery_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $entity_gallery = entity_gallery_load($form_state['values']['egid']);
    entity_gallery_delete($form_state['values']['egid']);
    cache_clear_all();
    watchdog('gallery', '@type: deleted %title.', array('@type' => $entity_gallery->type, '%title' => $entity_gallery->title));
    drupal_set_message(t('@type %title has been deleted.', array('@type' => entity_gallery_type_get_name($entity_gallery), '%title' => $entity_gallery->title)));
  }

  $form_state['redirect'] = '<front>';
}

/**
 * Generates an overview table of older revisions of an entity gallery.
 *
 * @param $entity_gallery
 *   An entity gallery object.
 *
 * @return array
 *   An array as expected by drupal_render().
 *
 * @see entity_gallery_menu()
 */
function entity_gallery_revision_overview($entity_gallery) {
  drupal_set_title(t('Revisions for %title', array('%title' => $entity_gallery->title)), PASS_THROUGH);

  $header = array(t('Revision'), array('data' => t('Operations'), 'colspan' => 2));

  $revisions = entity_gallery_revision_list($entity_gallery);

  $rows = array();
  $revert_permission = FALSE;
  if ((user_access('revert entity gallery revisions') || user_access('administer entity galleries')) && entity_gallery_access('update', $entity_gallery)) {
    $revert_permission = TRUE;
  }
  $delete_permission = FALSE;
  if ((user_access('delete entity gallery revisions') || user_access('administer entity galleries')) && entity_gallery_access('delete', $entity_gallery)) {
    $delete_permission = TRUE;
  }
  foreach ($revisions as $revision) {
    $row = array();
    $operations = array();

    if ($revision->current_vid > 0) {
      $row[] = array('data' => t('!date by !username', array('!date' => l(format_date($revision->timestamp, 'short'), "gallery/$entity_gallery->egid"), '!username' => theme('username', array('account' => $revision))))
                               . (($revision->log != '') ? '<p class="revision-log">' . filter_xss($revision->log) . '</p>' : ''),
                     'class' => array('revision-current'));
      $operations[] = array('data' => drupal_placeholder(t('current revision')), 'class' => array('revision-current'), 'colspan' => 2);
    }
    else {
      $row[] = t('!date by !username', array('!date' => l(format_date($revision->timestamp, 'short'), "gallery/$entity_gallery->egid/revisions/$revision->vid/view"), '!username' => theme('username', array('account' => $revision))))
               . (($revision->log != '') ? '<p class="revision-log">' . filter_xss($revision->log) . '</p>' : '');
      if ($revert_permission) {
        $operations[] = l(t('revert'), "gallery/$entity_gallery->egid/revisions/$revision->vid/revert");
      }
      if ($delete_permission) {
        $operations[] = l(t('delete'), "gallery/$entity_gallery->egid/revisions/$revision->vid/delete");
      }
    }
    $rows[] = array_merge($row, $operations);
  }

  $build['entity_gallery_revisions_table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
  );

  return $build;
}

/**
 * Asks for confirmation of the reversion to prevent against CSRF attacks.
 *
 * @param int $entity_gallery_revision
 *   The entity gallery revision ID.
 *
 * @return array
 *   An array as expected by drupal_render().
 *
 * @see entity_gallery_menu()
 * @see entity_gallery_revision_revert_confirm_submit()
 * @ingroup forms
 */
function entity_gallery_revision_revert_confirm($form, $form_state, $entity_gallery_revision) {
  $form['#entity_gallery_revision'] = $entity_gallery_revision;
  return confirm_form($form, t('Are you sure you want to revert to the revision from %revision-date?', array('%revision-date' => format_date($entity_gallery_revision->revision_timestamp))), 'gallery/' . $entity_gallery_revision->egid . '/revisions', '', t('Revert'), t('Cancel'));
}

/**
 * Form submission handler for entity_gallery_revision_revert_confirm().
 */
function entity_gallery_revision_revert_confirm_submit($form, &$form_state) {
  $entity_gallery_revision = $form['#entity_gallery_revision'];
  $entity_gallery_revision->revision = 1;
  $entity_gallery_revision->log = t('Copy of the revision from %date.', array('%date' => format_date($entity_gallery_revision->revision_timestamp)));

  entity_gallery_save($entity_gallery_revision);

  watchdog('gallery', '@type: reverted %title revision %revision.', array('@type' => $entity_gallery_revision->type, '%title' => $entity_gallery_revision->title, '%revision' => $entity_gallery_revision->vid));
  drupal_set_message(t('@type %title has been reverted back to the revision from %revision-date.', array('@type' => entity_gallery_type_get_name($entity_gallery_revision), '%title' => $entity_gallery_revision->title, '%revision-date' => format_date($entity_gallery_revision->revision_timestamp))));
  $form_state['redirect'] = 'gallery/' . $entity_gallery_revision->egid . '/revisions';
}

/**
 * Form constructor for the revision deletion confirmation form.
 *
 * This form prevents against CSRF attacks.
 *
 * @param $entity_gallery_revision
 *   The entity gallery revision ID.
 *
 * @return
 *   An array as expected by drupal_render().
 *
 * @see entity_gallery_menu()
 * @see entity_gallery_revision_delete_confirm_submit()
 * @ingroup forms
 */
function entity_gallery_revision_delete_confirm($form, $form_state, $entity_gallery_revision) {
  $form['#entity_gallery_revision'] = $entity_gallery_revision;
  return confirm_form($form, t('Are you sure you want to delete the revision from %revision-date?', array('%revision-date' => format_date($entity_gallery_revision->revision_timestamp))), 'gallery/' . $entity_gallery_revision->egid . '/revisions', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Form submission handler for entity_gallery_revision_delete_confirm().
 */
function entity_gallery_revision_delete_confirm_submit($form, &$form_state) {
  $entity_gallery_revision = $form['#entity_gallery_revision'];
  entity_gallery_revision_delete($entity_gallery_revision->vid);

  watchdog('gallery', '@type: deleted %title revision %revision.', array('@type' => $entity_gallery_revision->type, '%title' => $entity_gallery_revision->title, '%revision' => $entity_gallery_revision->vid));
  drupal_set_message(t('Revision from %revision-date of @type %title has been deleted.', array('%revision-date' => format_date($entity_gallery_revision->revision_timestamp), '@type' => entity_gallery_type_get_name($entity_gallery_revision), '%title' => $entity_gallery_revision->title)));
  $form_state['redirect'] = 'gallery/' . $entity_gallery_revision->egid;
  if (db_query('SELECT COUNT(vid) FROM {entity_gallery_revision} WHERE egid = :egid', array(':egid' => $entity_gallery_revision->egid))->fetchField() > 1) {
    $form_state['redirect'] .= '/revisions';
  }
}
