<?php

/**
 * @file
 * Hooks provided by the Entity Gallery module.
 */

/**
 * @defgroup entity_gallery_api_hooks Entity Gallery API Hooks
 * @{
 * Functions to define and modify gallery types.
 *
 * Each gallery type is maintained by a primary module, which is either
 * entity_gallery.module (for gallery types created in the user interface) or
 * the module that implements hook_entity_gallery_info() to define the gallery
 * type.
 *
 * During entity gallery operations (create, update, view, delete, etc.), there
 * are several sets of hooks that get invoked to allow modules to modify the
 * base entity gallery operation:
 * - Entity-gallery-type-specific hooks: When defining an entity gallery type,
 *   hook_entity_gallery_info() returns a 'base' component.
 *   Entity-gallery-type-specific hooks are named base_hookname() instead of
 *   mymodule_hookname() (in a module called 'mymodule' for example). Only the
 *   entity gallery type's corresponding implementation is invoked. For example,
 *   poll_entity_gallery_info() in entity_gallery.module would define the base
 *   for the 'poll' entity gallery type as 'poll'. So when a poll entity gallery
 *   is created, hook_insert() is invoked on poll_insert() only.
 *   Hooks that are entity-gallery-type-specific are noted below.
 * - All-module hooks: This set of hooks is invoked on all implementing modules,
 *   to allow other modules to modify what the primary entity gallery module is
 *   doing. For example, hook_entity_gallery_insert() is invoked on all modules
 *   when creating a poll entity gallery.
 * - Field hooks: Hooks related to the fields attached to the entity gallery.
 *   These are invoked from the field operations functions described below, and
 *   can be either field-type-specific or all-module hooks.
 * - Entity hooks: Generic hooks for "entity" operations. These are always
 *   invoked on all modules.
 *
 * Here is a list of the entity gallery and entity hooks that are invoked, field
 * operations, and other steps that take place during entity gallery operations:
 * - Creating a new entity gallery (calling entity_gallery_save() on a new
 *   entity gallery):
 *   - field_attach_presave()
 *   - hook_entity_gallery_presave() (all)
 *   - hook_entity_presave() (all)
 *   - Entity gallery and revision records are written to the database
 *   - hook_insert() (entity-gallery-type-specific)
 *   - field_attach_insert()
 *   - hook_entity_gallery_insert() (all)
 *   - hook_entity_insert() (all)
 * - Updating an existing entity gallery (calling entity_gallery_save() on an
 *   existing entity gallery):
 *   - field_attach_presave()
 *   - hook_entity_gallery_presave() (all)
 *   - hook_entity_presave() (all)
 *   - Entity gallery and revision records are written to the database
 *   - hook_update() (entity-gallery-type-specific)
 *   - field_attach_update()
 *   - hook_entity_gallery_update() (all)
 *   - hook_entity_update() (all)
 * - Loading an entity gallery (calling entity_gallery_load(),
 *   entity_gallery_load_multiple() or entity_load()
 *   with $entity_type of 'entity_gallery'):
 *   - Entity gallery and revision information is read from database.
 *   - hook_load() (entity-gallery-type-specific)
 *   - field_attach_load_revision() and field_attach_load()
 *   - hook_entity_load() (all)
 *   - hook_entity_gallery_load() (all)
 * - Viewing a single entity gallery (calling entity_gallery_view() - note that
 *   the input to entity_gallery_view() is a loaded entity gallery, so the
 *   Loading steps above are already done):
 *   - hook_view() (entity-gallery-type-specific)
 *   - field_attach_prepare_view()
 *   - hook_entity_prepare_view() (all)
 *   - field_attach_view()
 *   - hook_entity_gallery_view() (all)
 *   - hook_entity_view() (all)
 *   - hook_entity_gallery_view_alter() (all)
 *   - hook_entity_view_alter() (all)
 * - Viewing multiple entity galleries (calling entity_gallery_view_multiple() -
 *   note that the input to entity_gallery_view_multiple() is a set of loaded
 *   entity galleries, so the Loading steps above are already done):
 *   - field_attach_prepare_view()
 *   - hook_entity_prepare_view() (all)
 *   - hook_view() (entity-gallery-type-specific)
 *   - field_attach_view()
 *   - hook_entity_gallery_view() (all)
 *   - hook_entity_view() (all)
 *   - hook_entity_gallery_view_alter() (all)
 *   - hook_entity_view_alter() (all)
 * - Deleting an entity gallery (calling entity_gallery_delete() or
 *   entity_gallery_delete_multiple()):
 *   - Entity gallery is loaded (see Loading section above)
 *   - hook_delete() (entity-gallery-type-specific)
 *   - hook_entity_gallery_delete() (all)
 *   - hook_entity_delete() (all)
 *   - field_attach_delete()
 *   - Entity gallery and revision information are deleted from database
 * - Deleting an entity gallery revision (calling
 *   entity_gallery_revision_delete()):
 *   - Entity gallery is loaded (see Loading section above)
 *   - Revision information is deleted from database
 *   - hook_entity_gallery_revision_delete() (all)
 *   - field_attach_delete_revision()
 * - Preparing an entity gallery for editing (calling entity_gallery_form() -
 *   note that if it is an existing entity gallery, it will already be loaded;
 *   see the Loading section above):
 *   - hook_prepare() (entity-gallery-type-specific)
 *   - hook_entity_gallery_prepare() (all)
 *   - hook_form() (entity-gallery-type-specific)
 *   - field_attach_form()
 * - Validating an entity gallery during editing form submit (calling
 *   entity_gallery_form_validate()):
 *   - hook_validate() (entity-gallery-type-specific)
 *   - hook_entity_gallery_validate() (all)
 *   - field_attach_form_validate()
 * - Searching (calling entity_gallery_search_execute()):
 *   - hook_ranking() (all)
 *   - Query is executed to find matching entity galleries
 *   - Resulting entity gallery is loaded (see Loading section above)
 *   - Resulting entity gallery is prepared for viewing (see Viewing a single
 *   entity gallery above)
 *   - hook_entity_gallery_search_result() (all)
 * - Search indexing (calling entity_gallery_update_index()):
 *   - Entity gallery is loaded (see Loading section above)
 *   - Entity gallery is prepared for viewing (see Viewing a single entity
 *   gallery above)
 *   - hook_entity_gallery_update_index() (all)
 * @}
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Add mass entity gallery operations.
 *
 * This hook enables modules to inject custom operations into the mass
 * operations dropdown found at admin/content/gallery, by associating a callback
 * function with the operation, which is called when the form is submitted. The
 * callback function receives one initial argument, which is an array of the
 * checked entity galleries.
 *
 * @return
 *   An array of operations. Each operation is an associative array that may
 *   contain the following key-value pairs:
 *   - label: (required) The label for the operation, displayed in the dropdown
 *     menu.
 *   - callback: (required) The function to call for the operation.
 *   - callback arguments: (optional) An array of additional arguments to pass
 *     to the callback function.
 */
function hook_entity_gallery_operations() {
  $operations = array(
    'publish' => array(
      'label' => t('Publish selected gallery'),
      'callback' => 'entity_gallery_mass_update',
      'callback arguments' => array('updates' => array('status' => ENTITY_GALLERY_PUBLISHED)),
    ),
    'unpublish' => array(
      'label' => t('Unpublish selected gallery'),
      'callback' => 'entity_gallery_mass_update',
      'callback arguments' => array('updates' => array('status' => ENTITY_GALLERY_NOT_PUBLISHED)),
    ),
    'sticky' => array(
      'label' => t('Make selected gallery sticky'),
      'callback' => 'entity_gallery_mass_update',
      'callback arguments' => array('updates' => array('status' => ENTITY_GALLERY_PUBLISHED, 'sticky' => ENTITY_GALLERY_STICKY)),
    ),
    'unsticky' => array(
      'label' => t('Make selected gallery not sticky'),
      'callback' => 'entity_gallery_mass_update',
      'callback arguments' => array('updates' => array('sticky' => ENTITY_GALLERY_NOT_STICKY)),
    ),
    'delete' => array(
      'label' => t('Delete selected gallery'),
      'callback' => NULL,
    ),
  );
  return $operations;
}

/**
 * Respond to entity gallery deletion.
 *
 * This hook is invoked from entity_gallery_delete_multiple() after the
 * type-specific hook_delete() has been invoked, but before hook_entity_delete
 * and field_attach_delete() are called, and before the entity gallery is
 * removed from the entity gallery table in the database.
 *
 * @param $entity_gallery
 *   The entity gallery that is being deleted.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_entity_gallery_delete($entity_gallery) {
  db_delete('mytable')
    ->condition('egid', $entity_gallery->egid)
    ->execute();
}

/**
 * Respond to deletion of an entity gallery revision.
 *
 * This hook is invoked from entity_gallery_revision_delete() after the revision has been
 * removed from the entity_gallery_revision table, and before
 * field_attach_delete_revision() is called.
 *
 * @param $entity_gallery
 *   The entity gallery revision (entity gallery object) that is being deleted.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_entity_gallery_revision_delete($entity_gallery) {
  db_delete('mytable')
    ->condition('vid', $entity_gallery->vid)
    ->execute();
}

/**
 * Respond to creation of a new entity gallery.
 *
 * This hook is invoked from entity_gallery_save() after the database query that
 * will insert the entity gallery into the entity gallery table is scheduled for
 * execution, after the type-specific hook_insert() is invoked, and after
 * field_attach_insert() is called.
 *
 * Note that when this hook is invoked, the changes have not yet been written to
 * the database, because a database transaction is still in progress. The
 * transaction is not finalized until the save operation is entirely completed
 * and entity_gallery_save() goes out of scope. You should not rely on data in
 * the database at this time as it is not updated yet. You should also note that
 * any write/update database queries executed from this hook are also not
 * committed immediately. Check entity_gallery_save() and db_transaction() for
 * more info.
 *
 * @param $entity_gallery
 *   The entity gallery that is being created.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_entity_gallery_insert($entity_gallery) {
  db_insert('mytable')
    ->fields(array(
      'egid' => $entity_gallery->egid,
      'extra' => $entity_gallery->extra,
    ))
    ->execute();
}

/**
 * Act on arbitrary entity galleries being loaded from the database.
 *
 * This hook should be used to add information that is not in the entity galler
 * or entity gallery revisions table, not to replace information that is in
 * these tables (which could interfere with the entity cache). For performance
 * reasons, information for all available entity galleries should be loaded in a
 * single query where possible.
 *
 * This hook is invoked during entity gallery loading, which is handled by
 * entity_load(), via classes EntityGalleryController and
 * DrupalDefaultEntityController. After the entity gallery information is read
 * from the database or the entity cache, hook_load() is invoked on the entity
 * gallery's gallery type module, then field_attach_load_revision() or
 * field_attach_load() is called, then hook_entity_load() is invoked on all
 * implementing modules, and finally hook_entity_gallery_load() is invoked on
 * all implementing modules.
 *
 * @param $entity_galleries
 *   An array of the entity galleries being loaded, keyed by egid.
 * @param $types
 *   An array containing the entity gallery types present in $entity_galleries.
 *   Allows for an early return for modules that only support certain entity
 *   gallery types. However, if your module defines a gallery type, you can use
 *   hook_load() to respond to loading of just that gallery type.
 *
 * For a detailed usage example, see entity_gallery_example.module.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_entity_gallery_load($entity_galleries, $types) {
  // Decide whether any of $types are relevant to our purposes.
  if (count(array_intersect($types_we_want_to_process, $types))) {
    // Gather our extra data for each of these entity galleries.
    $result = db_query('SELECT egid, foo FROM {mytable} WHERE egid IN(:egids)', array(':egids' => array_keys($entity_galleries)));
    // Add our extra data to the entity gallery objects.
    foreach ($result as $record) {
      $entity_galleries[$record->egid]->foo = $record->foo;
    }
  }
}

/**
 * Control access to an entity gallery.
 *
 * Modules may implement this hook if they want to have a say in whether or not
 * a given user has access to perform a given operation on an entity gallery.
 *
 * The administrative account (user ID #1) always passes any access check, so
 * this hook is not called in that case. Users with the
 * "bypass entity gallery access" permission may always view and edit galleries
 * through the administrative interface.
 *
 * Note that not all modules will want to influence access on all entity gallery
 * types. If your module does not want to actively grant or block access, return
 * ENTITY_GALLERY_ACCESS_IGNORE or simply return nothing. Blindly returning
 * FALSE will break other entity gallery access modules.
 *
 * Also note that this function isn't called for entity gallery listings. See
 * @link entity_gallery_access Entity gallery access rights @endlink for a full
 * explanation.
 *
 * @param $entity_gallery
 *   Either an entity gallery object or the machine name of the gallery type on
 *   which to perform the access check.
 * @param $op
 *   The operation to be performed. Possible values:
 *   - "create"
 *   - "delete"
 *   - "update"
 *   - "view"
 * @param $account
 *   The user object to perform the access check operation on.
 *
 * @return
 *   - ENTITY_GALLERY_ACCESS_ALLOW: if the operation is to be allowed.
 *   - ENTITY_GALLERY_ACCESS_DENY: if the operation is to be denied.
 *   - ENTITY_GALLERY_ACCESS_IGNORE: to not affect this operation at all.
 *
 * @ingroup entity_gallery_access
 */
function hook_entity_gallery_access($entity_gallery, $op, $account) {
  $type = is_string($entity_gallery) ? $entity_gallery : $entity_gallery->type;

  if (in_array($type, entity_gallery_permissions_get_configured_types())) {
    if ($op == 'create' && user_access('create ' . $type . ' entity gallery', $account)) {
      return ENTITY_GALLERY_ACCESS_ALLOW;
    }

    if ($op == 'update') {
      if (user_access('edit any ' . $type . ' entity gallery', $account) || (user_access('edit own ' . $type . ' entity gallery', $account) && ($account->uid == $entity_gallery->uid))) {
        return ENTITY_GALLERY_ACCESS_ALLOW;
      }
    }

    if ($op == 'delete') {
      if (user_access('delete any ' . $type . ' entity gallery', $account) || (user_access('delete own ' . $type . ' entity gallery', $account) && ($account->uid == $entity_gallery->uid))) {
        return ENTITY_GALLERY_ACCESS_ALLOW;
      }
    }
  }

  // Returning nothing from this function would have the same effect.
  return ENTITY_GALLERY_ACCESS_IGNORE;
}


/**
 * Act on an entity gallery object about to be shown on the add/edit form.
 *
 * This hook is invoked from entity_gallery_object_prepare() after the
 * type-specific hook_prepare() is invoked.
 *
 * @param $entity_gallery
 *   The entity gallery that is about to be shown on the add/edit form.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_entity_gallery_prepare($entity_gallery) {
  if (!isset($entity_gallery->comment)) {
    $entity_gallery->comment = variable_get("comment_$entity_gallery->type", COMMENT_NODE_OPEN);
  }
}

/**
 * Act on an entity gallery being displayed as a search result.
 *
 * This hook is invoked from entity_gallery_search_execute(), after
 * entity_gallery_load() and entity_gallery_view() have been called.
 *
 * @param $entity_gallery
 *   The entity gallery being displayed in a search result.
 *
 * @return array
 *   Extra information to be displayed with search result. This information
 *   should be presented as an associative array. It will be concatenated with
 *   the post information (last updated, author) in the default search result
 *   theming.
 *
 * @see template_preprocess_search_result()
 * @see search-result.tpl.php
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_entity_gallery_search_result($entity_gallery) {
  $comments = db_query('SELECT comment_count FROM {entity_gallery_comment_statistics} WHERE egid = :egid', array('egid' => $entity_gallery->egid))->fetchField();
  return array('comment' => format_plural($comments, '1 comment', '@count comments'));
}

/**
 * Act on an entity gallery being inserted or updated.
 *
 * This hook is invoked from entity_gallery_save() before the entity gallery is
 * saved to the database.
 *
 * @param $entity_gallery
 *   The entity gallery that is being inserted or updated.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_entity_gallery_presave($entity_gallery) {
  if ($entity_gallery->egid && $entity_gallery->moderate) {
    // Reset votes when entity gallery is updated:
    $entity_gallery->score = 0;
    $entity_gallery->users = '';
    $entity_gallery->votes = 0;
  }
}

/**
 * Respond to updates to an entity gallery.
 *
 * This hook is invoked from entity_gallery_save() after the database query that
 * will update entity gallery in the entity gallery table is scheduled for
 * execution, after the type-specific hook_update() is invoked, and after
 * field_attach_update() is called.
 *
 * Note that when this hook is invoked, the changes have not yet been written to
 * the database, because a database transaction is still in progress. The
 * transaction is not finalized until the save operation is entirely completed
 * and entity_gallery_save() goes out of scope. You should not rely on data in
 * the database at this time as it is not updated yet. You should also note that
 * any write/update database queries executed from this hook are also not
 * committed immediately. Check entity_gallery_save() and db_transaction() for
 * more info.
 *
 * @param $entity_gallery
 *   The entity gallery that is being updated.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_entity_gallery_update($entity_gallery) {
  db_update('mytable')
    ->fields(array('extra' => $entity_gallery->extra))
    ->condition('egid', $entity_gallery->egid)
    ->execute();
}

/**
 * Act on an entity gallery being indexed for searching.
 *
 * This hook is invoked during search indexing, after entity_gallery_load(), and
 * after the result of entity_gallery_view() is added as
 * $entity_gallery->rendered to the entity gallery object.
 *
 * @param $entity_gallery
 *   The entity gallery being indexed.
 *
 * @return string
 *   Additional entity gallery information to be indexed.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_entity_gallery_update_index($entity_gallery) {
  $text = '';
  $comments = db_query('SELECT subject, comment, format FROM {comment} WHERE egid = :egid AND status = :status', array(':egid' => $entity_gallery->egid, ':status' => COMMENT_PUBLISHED));
  foreach ($comments as $comment) {
    $text .= '<h2>' . check_plain($comment->subject) . '</h2>' . check_markup($comment->comment, $comment->format, '', TRUE);
  }
  return $text;
}

/**
 * Perform entity gallery validation before an entity gallery is created or
 * updated.
 *
 * This hook is invoked from entity_gallery_validate(), after a user has has
 * finished editing the entity gallery and is previewing or submitting it. It is
 * invoked at the end of all the standard validation steps, and after the
 * type-specific hook_validate() is invoked.
 *
 * To indicate a validation error, use form_set_error().
 *
 * Note: Changes made to the $entity_gallery object within your hook
 * implementation will have no effect.  The preferred method to change an
 * entity gallery's content is to use hook_entity_gallery_presave() instead. If
 * it is really necessary to change the entity gallery at the validate stage,
 * you can use form_set_value().
 *
 * @param $entity_gallery
 *   The entity gallery being validated.
 * @param $form
 *   The form being used to edit the entity gallery.
 * @param $form_state
 *   The form state array.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_entity_gallery_validate($entity_gallery, $form, &$form_state) {
  if (isset($entity_gallery->end) && isset($entity_gallery->start)) {
    if ($entity_gallery->start > $entity_gallery->end) {
      form_set_error('time', t('An event may not end before it starts.'));
    }
  }
}

/**
 * Act on an entity gallery after validated form values have been copied to it.
 *
 * This hook is invoked when an entity gallery form is submitted with either the
 * "Save" or "Preview" button, after form values have been copied to the form
 * state's entity gallery object, but before the entity gallery is saved or
 * previewed. It is a chance for modules to adjust the entity gallery's
 * properties from what they are simply after a copy from $form_state['values'].
 * This hook is intended for adjusting non-field-related properties. See
 * hook_field_attach_submit() for customizing field-related properties.
 *
 * @param $entity_gallery
 *   The entity gallery object being updated in response to a form submission.
 * @param $form
 *   The form being used to edit the entity gallery.
 * @param $form_state
 *   The form state array.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_entity_gallery_submit($entity_gallery, $form, &$form_state) {
  // Decompose the selected menu parent option into 'menu_name' and 'plid', if
  // the form used the default parent selection widget.
  if (!empty($form_state['values']['menu']['parent'])) {
    list($entity_gallery->menu['menu_name'], $entity_gallery->menu['plid']) = explode(':', $form_state['values']['menu']['parent']);
  }
}

/**
 * Act on an entity gallery that is being assembled before rendering.
 *
 * The module may add elements to $entity_gallery->content prior to rendering.
 * This hook will be called after hook_view(). The structure of
 * $entity_gallery->content is a renderable array as expected by
 * drupal_render().
 *
 * @param $entity_gallery
 *   The entity gallery that is being assembled for rendering.
 * @param $view_mode
 *   The $view_mode parameter from entity_gallery_view().
 * @param $langcode
 *   The language code used for rendering.
 *
 * @see hook_entity_view()
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_entity_gallery_view($entity_gallery, $view_mode, $langcode) {
  $entity_gallery->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_gallery_content_view().
 *
 * This hook is called after the gallery has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * entity gallery content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the entity gallery rather
 * than the structured content array, it may use this hook to add a #post_render
 * callback.  Alternatively, it could also implement
 * hook_preprocess_entity_gallery(). See drupal_render() and theme()
 * documentation respectively for details.
 *
 * @param $build
 *   A renderable array representing the entity gallery content.
 *
 * @see entity_gallery_view()
 * @see hook_entity_view_alter()
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_entity_gallery_view_alter(&$build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;
  }

  // Add a #post_render callback to act on the rendered HTML of the entity
  // gallery.
  $build['#post_render'][] = 'my_module_entity_gallery_post_render';
}

/**
 * Define module-provided entity gallery types.
 *
 * This hook allows a module to define one or more of its own entity gallery
 * types. For example, a commerce site could define a product
 * entity-gallery-type named "Commerce products." The name and attributes of
 * each desired entity gallery type are specified in an array returned by the
 * hook.
 *
 * Only module-provided entity gallery types should be defined through this
 * hook. User-provided (or 'custom') entity gallery types should be defined only
 * in the 'entity_gallery_type' database table, and should be maintained by
 * using the entity_gallery_type_save() and entity_gallery_type_delete()
 * functions.
 *
 * @return
 *   An array of information defining the module's entity gallery types. The
 *   array contains a sub-array for each entity gallery type, with the
 *   machine-readable type name as the key. Each sub-array has up to 10
 *   attributes. Possible attributes:
 *   - name: (required) The human-readable name of the entity gallery type.
 *   - base: (required) The base name for implementations of
 *     entity-gallery-type-specific hooks that respond to this entity gallery
 *     type. Base is usually the name of the module or 'entity_gallery_content',
 *     but not always. See
 *     @link entity_gallery_api_hooks Entity gallery API hooks @endlink for more
 *     information.
 *   - description: (required) A brief description of the entity gallery type.
 *   - help: (optional) Help information shown to the user when creating an
 *     entity gallery of this type.
 *   - has_title: (optional) A Boolean indicating whether or not this entity
 *     gallery type has a title field.
 *   - title_label: (optional) The label for the title field of this gallery
 *     type.
 *   - locked: (optional) A Boolean indicating whether the administrator can
 *     change the machine name of this type. FALSE = changeable (not locked),
 *     TRUE = unchangeable (locked).
 *
 * The machine name of an entity gallery type should contain only letters,
 * numbers, and underscores. Underscores will be converted into hyphens for the
 * purpose of constructing URLs.
 *
 * All attributes of an entity gallery type that are defined through this hook
 * (except for 'locked') can be edited by a site administrator. This includes
 * the machine-readable name of an entity gallery type, if 'locked' is set to
 * FALSE.
 *
 * @ingroup entity_gallery_content_api_hooks
 */
function hook_entity_gallery_info() {
  return array(
    'product' => array(
      'name' => t('Commerce products'),
      'base' => 'product',
      'description' => t('Use for displaying a gallery of selected commerce products.'),
    )
  );
}

/**
 * Provide additional methods of scoring for core search results for entity
 * galleries.
 *
 * An entity gallery's search score is used to rank it among other entity
 * galleries matched by the search, with the highest-ranked entity galleries
 * appearing first in the search listing.
 *
 * For example, a module allowing users to vote on galleries could expose an
 * option to allow search results' rankings to be influenced by the average
 * voting score of an entity gallery.
 *
 * All scoring mechanisms are provided as options to site administrators, and
 * may be tweaked based on individual sites or disabled altogether if they do
 * not make sense. Individual scoring mechanisms, if enabled, are assigned a
 * weight from 1 to 10. The weight represents the factor of magnification of
 * the ranking mechanism, with higher-weighted ranking mechanisms having more
 * influence. In order for the weight system to work, each scoring mechanism
 * must return a value between 0 and 1 for every entity gallery. That value is
 * then multiplied by the administrator-assigned weight for the ranking
 * mechanism, and then the weighted scores from all ranking mechanisms are
 * added, which brings about the same result as a weighted average.
 *
 * @return
 *   An associative array of ranking data. The keys should be strings,
 *   corresponding to the internal name of the ranking mechanism, such as
 *   'recent'. The values should be arrays themselves, with the following keys
 *   available:
 *   - title: (required) The human readable name of the ranking mechanism.
 *   - join: (optional) The part of a query string to join to any additional
 *     necessary table. This is not necessary if the table required is already
 *     joined to by the base query, such as for the {entity_gallery} table.
 *     Other tables should use the full table name as an alias to avoid naming
 *     collisions.
 *   - score: (required) The part of a query string to calculate the score for
 *     the ranking mechanism based on values in the database. This does not need
 *     to be wrapped in parentheses, as it will be done automatically; it also
 *     does not need to take the weighted system into account, as it will be
 *     done automatically. It does, however, need to calculate a decimal between
 *     0 and 1; be careful not to cast the entire score to an integer by
 *     inadvertently introducing a variable argument.
 *   - arguments: (optional) If any arguments are required for the score, they
 *     can be specified in an array here.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_ranking() {
  // If voting is disabled, we can avoid returning the array, no hard feelings.
  if (variable_get('vote_entity_gallery_enabled', TRUE)) {
    return array(
      'vote_average' => array(
        'title' => t('Average vote'),
        // Note that we use i.sid, the search index's search item id, rather than
        // eg.egid.
        'join' => 'LEFT JOIN {vote_entity_gallery_data} vote_entity_gallery_data ON vote_entity_gallery_data.egid = i.sid',
        // The highest possible score should be 1, and the lowest possible score,
        // always 0, should be 0.
        'score' => 'vote_entity_gallery_data.average / CAST(%f AS DECIMAL)',
        // Pass in the highest possible voting score as a decimal argument.
        'arguments' => array(variable_get('vote_score_max', 5)),
      ),
    );
  }
}


/**
 * Respond to entity gallery type creation.
 *
 * This hook is invoked from entity_gallery_type_save() after the entity gallery
 * type is added to the database.
 *
 * @param $info
 *   The entity gallery type object that is being created.
 */
function hook_entity_gallery_type_insert($info) {
  drupal_set_message(t('You have just created a gallery type with a machine name %type.', array('%type' => $info->type)));
}

/**
 * Respond to entity gallery type updates.
 *
 * This hook is invoked from entity_gallery_type_save() after the entity gallery
 * type is updated in the database.
 *
 * @param $info
 *   The entity gallery type object that is being updated.
 */
function hook_entity_gallery_type_update($info) {
  if (!empty($info->old_type) && $info->old_type != $info->type) {
    $setting = variable_get('comment_' . $info->old_type, COMMENT_NODE_OPEN);
    variable_del('comment_' . $info->old_type);
    variable_set('comment_' . $info->type, $setting);
  }
}

/**
 * Respond to entity gallery type deletion.
 *
 * This hook is invoked from entity_gallery_type_delete() after the entity
 * gallery type is removed from the database.
 *
 * @param $info
 *   The entity gallery type object that is being deleted.
 */
function hook_entity_gallery_type_delete($info) {
  variable_del('comment_' . $info->type);
}

/**
 * Respond to entity gallery deletion.
 *
 * This is an entity-gallery-type-specific hook, which is invoked only for the
 * entity gallery type being affected. See
 * @link entity_gallery_api_hooks Entity gallery API hooks @endlink for more
 * information.
 *
 * Use hook_entity_gallery_delete() to respond to entity gallery deletion of all
 * entity gallery types.
 *
 * This hook is invoked from entity_gallery_delete_multiple() before
 * hook_entity_gallery_delete() is invoked and before field_attach_delete() is
 * called.
 *
 * Note that when this hook is invoked, the changes have not yet been written
 * to the database, because a database transaction is still in progress. The
 * transaction is not finalized until the delete operation is entirely
 * completed and entity_gallery_delete_multiple() goes out of scope. You should
 * not rely on data in the database at this time as it is not updated yet. You
 * should also note that any write/update database queries executed from this
 * hook are also not committed immediately. Check
 * entity_gallery_delete_multiple() and db_transaction() for more info.
 *
 * @param $entity_gallery
 *   The entity gallery that is being deleted.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_delete($entity_gallery) {
  db_delete('mytable')
    ->condition('egid', $entity_gallery->egid)
    ->execute();
}

/**
 * Act on an entity gallery object about to be shown on the add/edit form.
 *
 * This is an entity-gallery-type-specific hook, which is invoked only for the
 * entity gallery type being affected. See
 * @link entity_gallery_api_hooks Entity gallery API hooks @endlink for more
 * information.
 *
 * Use hook_entity_gallery_prepare() to respond to entity gallery preparation of
 * all entity gallery types.
 *
 * This hook is invoked from entity_gallery_object_prepare() before the general
 * hook_entity_gallery_prepare() is invoked.
 *
 * @param $entity_gallery
 *   The entity gallery that is about to be shown on the add/edit form.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_prepare($entity_gallery) {
  $file = file_save_upload($field_name, _image_filename($file->filename, NULL, TRUE));
  if ($file) {
    if (!image_get_info($file->uri)) {
      form_set_error($field_name, t('Uploaded file is not a valid image'));
      return;
    }
  }
  else {
    return;
  }
  $entity_gallery->images['_original'] = $file->uri;
  _image_build_derivatives($entity_gallery, TRUE);
  $entity_gallery->new_file = TRUE;
}

/**
 * Display an entity gallery editing form.
 *
 * This is an entity-gallery-type-specific hook, which is invoked only for the
 * entity gallery type being affected. See
 * @link entity_gallery_api_hooks Entity gallery API hooks @endlink for more
 * information.
 *
 * Use hook_form_BASE_FORM_ID_alter(), with base form ID 'entity_gallery_form',
 * to alter entity gallery forms for all entity gallery types.
 *
 * This hook, implemented by entity gallery modules, is called to retrieve the
 * form that is displayed to create or edit an entity gallery. This form is
 * displayed at path gallery/add/[entity gallery type] or
 * gallery/[entity gallery ID]/edit.
 *
 * The submit and preview buttons, administrative and display controls, and
 * sections added by other modules (such as path settings, menu settings and
 * fields managed by the Field UI module) are displayed automatically by the
 * entity gallery module. This hook just needs to return the entity gallery
 * title and form editing fields specific to the entity gallery type.
 *
 * @param $entity_gallery
 *   The entity gallery being added or edited.
 * @param $form_state
 *   The form state array.
 *
 * @return
 *   An array containing the title and any custom form elements to be displayed
 *   in the entity gallery editing form.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_form($entity_gallery, &$form_state) {
  $type = entity_gallery_type_get_type($entity_gallery);

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => check_plain($type->title_label),
    '#default_value' => !empty($entity_gallery->title) ? $entity_gallery->title : '',
    '#required' => TRUE, '#weight' => -5
  );

  $form['field1'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom field'),
    '#default_value' => $entity_gallery->field1,
    '#maxlength' => 127,
  );
  $form['selectbox'] = array(
    '#type' => 'select',
    '#title' => t('Select box'),
    '#default_value' => $entity_gallery->selectbox,
    '#options' => array(
      1 => 'Option A',
      2 => 'Option B',
      3 => 'Option C',
    ),
    '#description' => t('Choose an option.'),
  );

  return $form;
}

/**
 * Respond to creation of a new entity gallery.
 *
 * This is an entity-gallery-type-specific hook, which is invoked only for the
 * entity gallery type being affected. See
 * @link entity_gallery_api_hooks Entity gallery API hooks @endlink for more information.
 *
 * Use hook_entity_gallery_insert() to respond to entity gallery insertion of
 * all entity gallery types.
 *
 * This hook is invoked from entity_gallery_save() after the entity gallery is
 * inserted into the entity gallery table in the database, before
 * field_attach_insert() is called, and before hook_entity_gallery_insert() is
 * invoked.
 *
 * @param $entity_gallery
 *   The entity gallery that is being created.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_insert($entity_gallery) {
  db_insert('mytable')
    ->fields(array(
      'egid' => $entity_gallery->egid,
      'extra' => $entity_gallery->extra,
    ))
    ->execute();
}

/**
 * Act on entity galleries being loaded from the database.
 *
 * This is an entity-gallery-type-specific hook, which is invoked only for the
 * entity gallery type being affected. See
 * @link entity_gallery_api_hooks Entity gallery API hooks @endlink for more
 * information.
 *
 * Use hook_entity_gallery_load() to respond to entity gallery load of all
 * entity gallery types.
 *
 * This hook is invoked during entity gallery loading, which is handled by
 * entity_load(), via classes EntityGalleryController and
 * DrupalDefaultEntityController. After the entity gallery information is read
 * from the database or the entity cache, hook_load() is invoked on the entity
 * gallery's content type module, then field_attach_entity_gallery_revision()
 * or field_attach_load() is called, then hook_entity_load() is invoked on all
 * implementing modules, and finally hook_entity_gallery_load() is invoked on
 * all implementing modules.
 *
 * This hook should only be used to add information that is not in the entity
 * gallery or entity gallery revisions table, not to replace information that is
 * in these tables (which could interfere with the entity cache). For
 * performance reasons, information for all available entity galleries should be
 * loaded in a single query where possible.
 *
 * @param $entity_galleries
 *   An array of the entity galleries being loaded, keyed by egid.
 *
 * For a detailed usage example, see entity_gallery_example.module.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_load($entity_galleries) {
  $result = db_query('SELECT egid, foo FROM {mytable} WHERE egid IN (:egids)', array(':egids' => array_keys($entity_galleries)));
  foreach ($result as $record) {
    $entity_galleries[$record->egid]->foo = $record->foo;
  }
}

/**
 * Respond to updates to an entity gallery.
 *
 * This is an entity-gallery-type-specific hook, which is invoked only for the
 * entity gallery type being affected. See
 * @link entity_gallery_api_hooks Entity gallery API hooks @endlink for more
 * information.
 *
 * Use hook_entity_gallery_update() to respond to entity gallery update of all
 * entity gallery types.
 *
 * This hook is invoked from entity_gallery_save() after the entity gallery is
 * updated in the entity gallery table in the database, before
 * field_attach_update() is called, and before hook_entity_gallery_update() is
 * invoked.
 *
 * @param $entity_gallery
 *   The entity gallery that is being updated.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_update($entity_gallery) {
  db_update('mytable')
    ->fields(array('extra' => $entity_gallery->extra))
    ->condition('egid', $entity_gallery->egid)
    ->execute();
}

/**
 * Perform entity gallery validation before an entity gallery is created or
 * updated.
 *
 * This is an entity-gallery-type-specific hook, which is invoked only for the
 * entity gallery type being affected. See
 * @link entity_gallery_api_hooks Entity gallery API hooks @endlink for more
 * information.
 *
 * Use hook_entity_gallery_validate() to respond to entity gallery validation of
 * all entity gallery types.
 *
 * This hook is invoked from entity_gallery_validate(), after a user has
 * finished editing the entity gallery and is previewing or submitting it. It is
 * invoked at the end of all the standard validation steps, and before
 * hook_entity_gallery_validate() is invoked.
 *
 * To indicate a validation error, use form_set_error().
 *
 * Note: Changes made to the $entity_gallery object within your hook
 * implementation will have no effect.  The preferred method to change an entity
 * gallery's content is to use hook_entity_gallery_presave() instead.
 *
 * @param $entity_gallery
 *   The entity gallery being validated.
 * @param $form
 *   The form being used to edit the entity gallery.
 * @param $form_state
 *   The form state array.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_validate($entity_gallery, $form, &$form_state) {
  if (isset($entity_gallery->end) && isset($entity_gallery->start)) {
    if ($entity_gallery->start > $entity_gallery->end) {
      form_set_error('time', t('An event may not end before it starts.'));
    }
  }
}

/**
 * Display an entity gallery.
 *
 * This is an entity-gallery-type-specific hook, which is invoked only for the
 * entity gallery type being affected. See
 * @link entity_gallery_api_hooks Entity gallery API hooks @endlink for more
 * information.
 *
 * Use hook_entity_gallery_view() to respond to entity gallery view of all
 * entity gallery types.
 *
 * This hook is invoked during entity gallery viewing after the entity gallery
 * is fully loaded, so that the entity gallery type module can define a custom
 * method for display, or add to the default display.
 *
 * @param $entity_gallery
 *   The entity gallery to be displayed, as returned by entity_gallery_load().
 * @param $view_mode
 *   View mode, e.g. 'full', 'teaser', ...
 * @param $langcode
 *   (optional) A language code to use for rendering. Defaults to the global
 *   content language of the current request.
 *
 * @return
 *   The passed $entity_gallery parameter should be modified as necessary and
 *   returned so it can be properly presented. Entity galleries are prepared for
 *   display by assembling a structured array, formatted as in the Form API, in
 *   $entity_gallery->content. As with Form API arrays, the #weight property can
 *   be used to control the relative positions of added elements. After this
 *   hook is invoked, entity_gallery_view() calls field_attach_view() to add
 *   field views to $entity_gallery->content, and then invokes
 *   hook_entity_gallery_view() and hook_entity_gallery_view_alter(), so if you
 *   want to affect the final view of the entity gallery, you might consider
 *   implementing one of these hooks instead.
 *
 * @ingroup entity_gallery_api_hooks
 */
function hook_view($entity_gallery, $view_mode, $langcode = NULL) {
  if ($view_mode == 'full' && entity_gallery_is_page($entity_gallery)) {
    $breadcrumb = array();
    $breadcrumb[] = l(t('Home'), NULL);
    $breadcrumb[] = l(t('Example'), 'example');
    $breadcrumb[] = l($entity_gallery->field1, 'example/' . $entity_gallery->field1);
    drupal_set_breadcrumb($breadcrumb);
  }

  $entity_gallery->content['myfield'] = array(
    '#markup' => theme('mymodule_myfield', $entity_gallery->myfield),
    '#weight' => 1,
  );

  return $entity_gallery;
}

/**
 * @} End of "addtogroup hooks".
 */
