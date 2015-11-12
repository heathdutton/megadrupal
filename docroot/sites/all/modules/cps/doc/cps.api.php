<?php

/**
 * Alter the access state.
 *
 * By default, CPS denies update operations unless the
 * changeset is in the unpublished state. If additional workflow states are
 * added, access may be granted in other states.
 *
 * @param $access
 *   Either TRUE or FALSE if the user has access to the operation.
 * @param array $context
 *   -- op: The operation, i.e, view, update, delete, create, etc.
 *   -- entity_type: The type of entity being operated on.
 *   -- entity: The entity being operated on. Might be a string for 'create'.
 *   -- changeset: The changeset being operated in.
 */
function hook_cps_can_edit_entities_alter(&$access, $context) {
  // Allow edit access in the 'review' state as well.
  if ($context['changeset']->status == 'review') {
    $access = TRUE;
  }
}

/**
 * Alter the list of workflow state types.
 *
 * All states have a type, so that they can be easily used in Views
 * without having to specify the exact state.
 *
 * By default CPS only has Open and Closed
 *
 * @param string[] $types
 *   An array of state types keyed by state type ID.
 */
function hook_cps_changeset_state_types_alter(&$types) {
  // Add a state type for changesets that aren't closed but aren't
  // going anywhere, either.
  $states['parked'] = t('Parked');
}

/**
 * Alter the list of workflow states.
 *
 * By default CPS only has Unpublished and Archived.
 *
 * @param array[] $states
 *   An array of states keyed by state ID.
 */
function hook_cps_changeset_states_alter(&$states) {
  // Add a state for 'submitted for review.'
  $states['review'] = array(
    'label' => t('Pending review'),
    'weight' => 0,
    'type' => 'open',
  );
}

/**
 * Alter operations to a changeset entity.
 *
 * This can be used to add operations allowing users to create new state
 * transitions on the changeset entity itself.
 *
 * @param array[] $operations
 *   An array of operations as a drupal_render array. Typically they will
 *   use '#theme' => 'link.'
 * @param CPSChangeset $changeset
 *   The changeset entity being operated upon.
 */
function hook_cps_changeset_operations(&$operations, $changeset) {
  $uri = $changeset->uri();
  $operations['submit'] = array(
    '#theme' => 'link',
    '#text' => t('submit for review'),
    '#path' => $uri['path'] . '/submit',
    '#options' => array(
      'query' => drupal_get_destination(),
      'html' => TRUE,
      'attributes' => array('class' => array('cps-changeset-operation'))
    ),
    '#access' => entity_access('submit', 'cps_changeset', $changeset),
  );
  $operations['decline'] = array(
    '#theme' => 'link',
    '#text' => $changeset->uid != $GLOBALS['user']->uid ? t('decline to publish') : t('withdraw'),
    '#path' => $uri['path'] . '/decline',
    '#options' => array(
      'query' => drupal_get_destination(),
      'html' => TRUE,
      'attributes' => array('class' => array('cps-changeset-operation')),
    ),
    '#access' => entity_access('decline', 'cps_changeset', $changeset),
  );
}

/**
 * Alter access to a changeset entity.
 *
 * This alter is for the changeset itself, and is very similar to
 * hook_node_access. However, $access is modified directly.
 *
 * @param $access
 *   A boolean TRUE or FALSE if the op is allowed.
 * @param $op
 *   A string representing the op, such as 'view', 'update', 'delete', or 'publish'.
 *   Often custom ops will be added via hook_cps_changeset_operations.
 * @param $changeset
 *   The changeset to test.
 * @param $account
 *   The account attempting to access this operation.
 */
function hook_cps_changeset_access_alter(&$access, $op, $changeset, $account) {
  // Set up permission for the new state.
  switch ($op) {
    case 'submit':
      $access = $changeset->status == 'unpublished' && $changeset->changeCount && ($changeset->uid == $account->uid || user_access('edit all changesets', $account));
      break;
    case 'decline':
      $access = $changeset->status == 'review' && (user_access('publish changesets', $account) || $changeset->uid == $account->uid);
      break;
    case 'publish':
      if ($access && $changeset->status != 'review') {
        $access = FALSE;
      }
      break;
  }
}

/**
 * Alter the displayed value of a status in the history.
 *
 * This is used because states may sometimes be gotten to via multiple operations,
 * and this allows the user to see the correct way the state was achieved.
 *
 * @param string $status
 *   The status string to display.
 * @param CPSChangeset $changeset
 *   The changeset being displays.
 * @param object $item
 *   The cps_workflow_history entry being displayed.
 */
function hook_cps_changeset_history_status(&$status, $changeset, $item) {
  if ($item->previous_status == 'unpublished' && $item->new_status == 'review') {
    $status = t('Submitted for review');
  }

  if ($item->previous_status == 'review' && $item->new_status == 'unpublished') {
    if ($item->uid == $changeset->uid) {
      $status = t('Withdrawn');
    }
    else {
      $status = t('Declined');
    }
  }
}

/**
 * React when an entity revision is removed from a changeset via the UI.
 *
 * This is called after the entity revision has already been removed.
 *
 * @param CPSChangeset $changeset
 *   The changeset entity.
 * @param string $entity_type
 *   The type of entity being removed from the changeset.
 * @param $entity
 *   The entity being removed.
 */
function hook_cps_remove_changeset($changeset, $entity_type, $entity) {

}

/**
 * React when an entity revision is moved from a changeset via the UI.
 *
 * This is called after the entity revision has already been moved. The
 * destination will be the current changeset.
 *
 * @param CPSChangeset $changeset_from
 *   The changeset the entity is being moved from.
 * @param CPSChangeset $changeset_to
 *   The changeset the entity is being moved to.
 * @param string $entity_type
 *   The type of entity being moved from the changeset.
 * @param $entity
 *   The entity being moved.
 * @param $type
 *   Either 'move' or 'copy'; 'copy' means the original revision should
 *   be left behind.
 */
function hook_cps_move_changeset($changeset_from, $changeset_to, $entity_type, $entity, $type) {

}

/**
 * React when the changeset is published.
 *
 * @param CPSChangeset $changeset
 *   The changeset entity.
 * @param string $type
 *   Either 'published' or 'unpublished'.
 */
function hook_cps_changeset_published($changeset, $type) {

}
