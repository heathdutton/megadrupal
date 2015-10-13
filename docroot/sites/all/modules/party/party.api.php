<?php

/**
 * @file
 * Hooks provided by the Party module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Control access to parties and their attachments.
 *
 * Modules may implement this hook if they want to have a say in whether or not
 * a given user has access to perform a given operation on a profile.
 *
 * @param $op
 *   The operation being performed.
 *   Currently one of:
 *    - 'view': Whether the user may view the given party or data set.
 *    - 'edit': Whether the user may edit the given party or data set.
 *    - 'add': Whether the user may create a new entity the given data set to
 *      the party.
 *    - 'attach': Whether the user may attach an existing entity in the given
 *      data set to the party.
 *    - 'detach': Whether the user may detach the given data set from the
 *      party.
 *    - 'create': Whether the user may create a party of the type given by
 *      $party->type.
 * @param $party
 *   A party to check access for.
 * @param $data_set
 *   (optional) A dataset definition to check access for. If nothing is given,
 *   access for just the party itself is determined.
 * @param $account
 *   The user being checked against. Usually the currently logged in user.
 *
 * @return boolean
 *   Return TRUE to grant access, FALSE to explicitly deny access. Return NULL
 *   or nothing to not affect the operation. The return values of each
 *   implentation of this hook are collected and analysed:
 *   - If an implementation returns FALSE access is always denied.
 *   - If no implementations return FALSE and atleast one implementation
 *     returns TRUE access is granted.
 *   - If all implementations return NULL, access is denied.
 *
 * @see party_access()
 */
function hook_party_access($op, $party, $data_set, $account) {

}

/**
 * Defines data sets to be used by parties.
 *
 * A data set is a collection of entities of one type (and optionally also one
 * bundle) from which one or more entities may be attached to a party.
 *
 * @return
 *  An array of sets where each key is the unique identifier of that "set type".
 *  - 'label': The human readable name of the data set.
 *  - 'entity type': The entity type this data set relates to parties.
 *  - 'entity bundle': (optional) The entity bundle that this data set restricts
 *    to. May be omitted to allow any bundle.
 *  - 'singleton': (optional) Whether this set's relationships only have one
 *    entity relating to a party. Default: FALSE.
 *  - 'max cardinality': (optional) The maximum number of entities that may be
 *    attached within this data set. This is set to 1 if 'singleton' is TRUE.
 *  - 'view mode': (optional) The name of one of the entity's view modes, to use
 *    for displaying entities attached to a party. If omitted, defaults to
 *    'party', a view mode which is added to data set-enabled entities in
 *    party_entity_info_alter().
 *  - 'form callback': (optional) This is the name of the form callback function.
 *    Returns the section of the form to do with this data set.
 *    See party_default_attached_entity_form().
 *  - 'module': (optional) The name of the module implementing this data set.
 *    This will be filled in automatically if not supplied.
 *  - 'admin': An array of admin paths for configuring and managing the piece:
 *    - 'edit'
 *    - 'manage fields'
 *    - 'manage display'
 *    - 'delete'
 *    - 'create'
 *    - 'import'
 *    - 'clone'
 *    - 'export'
 *  - 'actions': An array defining actions that allow users to do things with the
 *    attached entities within this data set. The keys of this array are the
 *    action names, which are used as path components below the party and also
 *    correspond to values of $op to party_access().
 *    Each action array may contain the following properties:
 *      - 'controller': The name of an action UI controller class. Party core
 *        provides the following:
 *        - PartyDefaultDataSetUIAdd: creates a new entity that is attached
 *          to the party.
 *        - PartyDefaultDataSetUIAttach: attaches an existing entity to the
 *          party.
 *      - 'action label': The text to use for the menu local action on the data
 *        set piece. This should not be localized, as it is run through t() by
 *        the system, with the following replacements:
 *        - '@data-set': The data set label.
 *    Note that the 'add' action is currently a special case which doesn't use
 *    the controller, though defining the action here determines access to it.
 *  - 'permissions': (optional) An array containing extra properties to pass to
 *    hook_permission() for each of the permissions party module provides for
 *    this data set. Each array may have a key for each of the actions party
 *    module provides a permission for ('view', 'attach', 'edit', 'detach'),
 *    and for any of these, the value should be an array of hook_permission()
 *    properties to set for the permission for this action on this data set.
 *  - piece: (optional) Each set may define one party piece, which will be
 *    returned by party_party_party_pieces(). The contents of this array should
 *    be the same as those returned by hook_party_party_piece_info(), with the
 *    addition of:
 *    - 'maker': Defines the method for generating the piece. Can be one of:
 *      - 'view': The display of this piece is handled by Views, via our
 *        default view in party_views_default_views().
 *      - 'core': The piece is displayed using Party module's attached entity
 *        view page callback, party_view_data_set(). No keys other than this
 *        and the path are needed.
 *      - 'callback': The module provides the callback to display the piece
 *        and defining it here rather than in hook_party_party_piece_info()
 *        is mostly just a convenience (though it does produce local action
 *        links too).
 *    - 'path': The menu path component for the provided piece.
 *    - 'view name': (optional) @todo! write the code for this! ;)
 *      The machine name of the view to define in hook_views_default_views().
 *      This allows multiple default views to exist. Defaults to
 *      'party_attached_entities'.
 */
function hook_party_data_set_info() {
  $sets = array();

  // A user data set.
  $sets['user'] = array(
    'label' => t("User account"),
    'entity type' => 'user',
    'singleton' => TRUE, // There is only one user per party.
    'load callback' => "party_user_load_user",
    'form callback' => "party_user_form_user",
  );
  return $sets;
}

/**
 * Alter data set definitions.
 *
 * @param $data_sets
 *  An array of data sets keyed by set id. Extra items in each definition such
 *  as 'module' will already have been set.
 */
function hook_party_data_set_info_alter(&$data_sets) {
  $data_sets['user']['label'] = t('A different label');
}

/**
 * Defines party pieces, that is, components of the party display.
 *
 * @return
 *  An array of display pieces (similar to hook_menu) where each key is the
 *  path component beneath the 'party/%' path. In addition to normal hook_menu()
 *  items, each array may also have the following properties:
 *   - 'label': The human readable name of the party piece.
 *   - 'page callback': The name of the display function. This should return a
 *     renderable array.
 *   - 'page arguments': Any extra arguments to supply to the page callback.
 *   - 'data set': (optional) The data set this is associated with. (If this is
 *     set, edit and add links will appear around the piece linking to the forms
 *     laid out in data_set_info);
 *   - 'nesting': True if the party piece can have children.
 */
function hook_party_party_piece_info() {
  $pieces = array();
  $pieces['user_username'] = array(
    'label' => "Username only",
    'page callback' => "party_user_display_user",
    'page arguments' => array('username'),
    'data set' => 'user',
  );
  $pieces['user_fullaccount'] = array(
    'label' => "Username only",
    'display callback' => "party_user_display_user",
    'display arguments' => array('full account'),
    'data set' => 'user',
  );
  return $pieces;
}

/**
 * Defines party pieces, that is, components of the party display.
 *
 * @return
 *  An array of items suitable for hook_menu(), where each key is the subpath
 *  below 'party/%party/view'.
 *  A number of defaults will be added by party_menu().
 */
function hook_party_party_pieces() {
  return array(
    'party' => array(
      'title' => 'View',
      'page callback' => 'party_page_view',
      'page arguments' => array(1),
      'file' => 'party.pages.inc',
      'access arguments' => array('view contacts'),
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
    ),
  );
}

/**
 * Add mass party operations for the Community admin page
 *
 * This hook allows modules to inject custom operations into the Community admin
 * page. The callback specified receives one argument: an array of the selected
 * party objects.
 *
 * @return
 *   An array of operations with keys:
 *   - "label": Required. The label for the drop down menu.
 *   - "callback": Required. The function that processes the operation.
 *   - "callback arguments": Optional. Any extra arguments to be sent to the
 *    callback function.
 */
function hook_party_operations() {
  $operations = array(
    'merge' => array(
      'label' => t('Merge parties'),
      'callback' => 'party_party_operations_merge',
    ),
  );
  return $operations;
}

/**
 * Add a column to the table on the Community admin page.
 *
 * This hook allows modules who change the party implementations to send their
 * data to the Community admin page.
 *
 * @return
 *   An array of columns with keys:
 *   - "title": Required. The name of the column.
 *   - "callback": Required. A function that returns the value of the column. This is sent a party object
 *   - "callback arguments": Optional: Any extra arguments to be sent.
 *   - "field": Optional: I don't know what this does.
 *
 */
function hook_party_admin_columns_info() {
  $columns = array(
    'pid' => array(
      'label' => t('Party Id'),
      'field' => 'cp.pid',
      'callback' => 'party_party_admin_columns',
    ),
  );
}

/**
 * Act when an entity is attached to a party.
 *
 * @param PartyDefaultDataSet $controller
 *   The data set controller that the entity has been attached to.
 * @param Object $entity
 *   The entity that has been attached to the party.
 * @param int $delta
 *   The $delta the attached entity has been added at.
 * @param string $method
 *   The $method used. Can be one of:
 *   - 'append': the entity will be added to the end of the list.
 *   - 'prepend': the entity will be added at the front of the list.
 *   - 'insert': the entity will be inserted at $delta.
 * @param bool $reattach
 *   Whether this entity was set to be reattached.
 *
 * @see PartyDefaultDataSet::attachEntity()
 */
function hook_party_data_set_attach($controller, $entity, $delta, $method, $reattach) {
  // No example.
}

/**
 * Act when an entity is attached to a party on a specific data set.
 *
 * @param PartyDefaultDataSet $controller
 *   The data set controller that the entity has been attached to.
 * @param Object $entity
 *   The entity that has been attached to the party.
 * @param int $delta
 *   The $delta the attached entity has been added at.
 * @param string $method
 *   The $method used. Can be one of:
 *   - 'append': the entity will be added to the end of the list.
 *   - 'prepend': the entity will be added at the front of the list.
 *   - 'insert': the entity will be inserted at $delta.
 * @param bool $reattach
 *   Whether this entity was set to be reattached.
 *
 * @see PartyDefaultDataSet::attachEntity()
 * @see hook_party_data_set_attach()
 */
function hook_party_data_set_attach_DATA_SET_NAME($controller, $entity, $delta, $method, $reattach) {
  // No example.
}

/**
 * Act when an entity is detached from a party.
 *
 * @param PartyDefaultDataSet $controller
 *   The data set controller that the entity has been detached from.
 * @param Object $entity
 *   The entity that has been detached from the party.
 * @param int $delta
 *   The $delta the detached entity has been removed from.
 *
 * @see PartyDefaultDataSet::detachEntity()
 * @see PartyDefaultDataSet::detachEntityByDelta()
 */
function hook_party_data_set_detach($controller, $entity, $delta) {
  // No example.
}

/**
 * Act when an entity is detached from a party on a specific data set.
 *
 * @param PartyDefaultDataSet $controller
 *   The data set controller that the entity has been detached from.
 * @param Object $entity
 *   The entity that has been detached from the party.
 * @param int $delta
 *   The $delta the detached entity has been removed from.
 *
 * @see PartyDefaultDataSet::detachEntity()
 * @see PartyDefaultDataSet::detachEntityByDelta()
 * @see hook_party_data_set_detach()
 */
function hook_party_data_set_detach_DATA_SET_NAME($controller, $entity, $delta) {
  // No example.
}

/**
 * Act when a data set controller is saved.
 *
 * @param PartyDefaultDataSet $controller
 *   The data set controller that the entity has been detached from.
 *
 * @see PartyDefaultDataSet::save()
 */
function hook_party_data_set_save($controller) {
  // No example.
}

/**
 * Act when a sepecific data sets controller is saved.
 *
 * @param PartyDefaultDataSet $controller
 *   The data set controller that the entity has been detached from.
 *
 * @see PartyDefaultDataSet::save()
 * @see hook_party_data_set_save()
 */
function hook_party_data_set_save_DATA_SET_NAME($controller) {
  // No example.
}

/**
 * Alter the values being used for party acquisitions.
 *
 * Some callers may specify a name context, in which case you can hook use the
 * name specific hook, hook_party_acquisition_NAME_values_alter().
 *
 * This function should be put in mymodule.party_acquisition.inc and will be
 * automatically include when required.
 *
 * @param array $values
 *   An array of party fields to match on. Keys are the field and values are
 *   the expected values.
 * @param array $context
 *   An array of contextual information from the caller and the acquisition
 *   class. Values should be checked for their existence before being used.
 */
function hook_party_acquisition_values_alter(&$values, array &$context) {
  // Example from party_user_party_acquisition_values_alter().

  if (!empty($context['party_user']['values_reference'])) {
    // Loop over the user values and set them. If they don't exist remove them.
    foreach ($context['party_user']['values'] as $property => &$value) {
      if (isset($values[$value])) {
        $value = $values[$value];
      }
      else {
        unset($context['party_user']['values_reference'][$property]);
      }
    }
  }
}

/**
 * Alter the match query being used for party acquisitions.
 *
 * Some callers may specify a name context, in which case you can hook use the
 * name specific hook, hook_party_acquisition_NAME_query_alter().
 *
 * Implementers MUST NOT add additional fields or cause duplicate rows.
 *
 * This function should be put in mymodule.party_acquisition.inc and will be
 * automatically include when required.
 *
 * @param $query
 *   A SelectQuery or extender with the match conditions already prepared.
 * @param array $context
 *   An array of contextual information from the caller and the acquisition
 *   class. Values should be checked for their existence before being used.
 */
function hook_party_acquisition_query_alter(&$query, array &$context) {
  // Example from party_hat_party_acquisition_query_alter().

  if (!empty($context['party_hat']['filter'])) {
    // Get our operator and set up our join variables.
    $operator = isset($context['party_hat']['filter_operator']) ? $context['party_hat']['filter_operator']: 'OR';
    $type = $operator == 'AND' ? 'INNER': 'LEFT OUTER';
    $table = 'field_data_party_hat';
    $condition = '%alias.entity_type = :entity_type AND %alias.entity_id = party.pid AND %alias.party_hat_hat_name = :hat_name';

    // If no
    if ($operator != 'AND') {
      $or = db_or();
    }

    // Add our joins. To avoid duplicates, we need one per hat. If our
    // operator is 'AND' we'll use inner joins. Otherwise we'll use left joins
    // and set up a condition for each in $or.
    foreach ($context['party_hat']['filter'] as $hat_name) {
      // Unfortunately PartyQuery can't do these non-duplicated joins yet...
      $alias = 'party_hat_' . $hat_name;
      $arguments = array(':entity_type' => 'party', ':hat_name' => $hat_name);
      $alias = $query->addJoin($type, $table, $alias, $condition, $arguments);

      if ($operator != 'AND') {
        $or->isNotNull("{$alias}.entity_id");
      }
    }

    // Add our condition to the query.
    if ($operator != 'AND') {
      $query->condition($or);
    }
  }
}

/**
 * Allow modules to act post acquisition.
 *
 * Modules can set certain values, log message etc based on acquisitions.
 *
 * This function should be put in mymodule.party_acquisition.inc and will be
 * automatically include when required.
 *
 * @param Party|FALSE $party
 *   The party that has been acquired, of FALSE if we failed to acquire.
 * @param string $method
 *   The method of acquisition that has occurred. Can be 'create' or' acquire'.
 * @param array $values
 *   An array of party fields to match on. Keys are the field and values are
 *   the expected values.
 * @param array $context
 *   An array of contextual information from the caller and the acquisition
 *   class. Values should be checked for their existence before being used.
 */
function hook_party_acquisition_post_acquisition(&$party, &$method, array &$values, array &$context) {
  // Example from party_hat_party_acquisition_post_acquisition().

  // Check we have a party.
  if ($party && !empty($context['party_hat']['add'])) {
    // Assign the hats without saving.
    party_hat_hats_assign($party, $context['party_hat']['add'], FALSE);
  }
}

/**
 * Alter the primary field configuration.
 *
 * Adjust source settings or change weights. Note that Label, Email and Primary
 * Field widget based primary fields will already be included and altering here
 * will override any settings in the UI.
 *
 * @param array $primary_fields
 *   The primary fields array to be altered. See PartyPrimaryFields::$fields for
 *   the data structure.
 *
 * @see PartyPrimaryFields
 * @see PartyPrimaryFields::$fields()
 */
function hook_party_primary_fields_fields_alter(&$primary_fields) {
  if (isset($primary_fields['label']['party:pid'])) {
    $primary_fields['label']['pid']['callback'] = 'party_primary_fields_pid_callback';
  }
}

/**
 * Alter the primary field sources.
 *
 * For example you can change the label of a source or add information for
 * conversion callbacks. Callbacks can be used to convert other sources into the
 * required value or type.
 *
 * Sources should not be added here as EntityMetadataWrappers are used to
 * retrieve values. See hook_entity_property_info() for details of how to add or
 * expose properties.
 *
 * @param array $sources
 *   The sources. See the return of PartyPrimaryFields::$sources for the data
 *   structure.
 *
 * @see PartyPrimaryFields
 * @see PartyPrimaryFields::$sources()
 */
function hook_party_primary_fields_sources_alter(&$sources) {
  // Allow uid to be converted to a username.
  if (isset($sources['user']['sources']['user:uid'])) {
    $sources['user']['sources']['user:uid']['callbacks']['uid_to_username'] = array(
      'label' => t('Formatted username'),
      'callback' => 'party_user_primary_fields_callback_uid_to_format_username',
    );
  }
}

/**
 * Alter the primary fields before storing (if applicable).
 *
 * @param Party $party
 *   The Party entity we are dealing with.
 * @param int $needs_store
 *   A bit mask of indicating whether we need to store any changes. Comprised of
 *   the PartyController::UPDATE_* constants.
 */
function hook_party_primary_fields_alter(Party $party, &$needs_store) {
  // No example.
}

/**
 * @} End of "addtogroup hooks"
 */
