<?php

/**
 * @file
 * Views hooks, handlers, defaults etc.
 */

/**
 * Implements hook_views_pre_view().
 */
function node_registration_views_pre_view(&$view, &$display_id, &$args) {
  if ($view->name == 'node_registrations' && $display_id == 'event' && @$args[0]) {
    $view->set_item($display_id, 'field', 'title', NULL);
  }
}

/**
 * Implements hook_views_plugins().
 */
function node_registration_views_plugins() {
  return array(
    'argument default' => array(
      'node_registration' => array(
        'title' => 'Node Registration ID from URL',
        'handler' => 'views_handler_argument_default_node_registration',
        'file' => 'views_handler_argument_default_node_registration.inc',
        'path' => drupal_get_path('module', 'node_registration') . '/includes/views',
      ),
    ),
  );
}

/**
 * Implements hook_views_data().
 */
function node_registration_views_data() {
  $data['node_registration']['table']['group']  = t('Node registration');

  // Table: node_registration.
  $data['node_registration']['table']['base'] = array(
    'field' => 'registration_id',
    'title' => t('Registration'),
    'weight' => -10,
    'access query tag' => 'node_registration_access',
    'defaults' => array(
      'field' => 'email',
    ),
  );
  $data['node_registration']['table']['entity type'] = 'node_registration';

  // Field: registration_id.
  $data['node_registration']['registration_id'] = array(
    'title' => t('Registration ID'),
    'help' => t('The Registration ID.'),
    'field' => array(
      'handler' => 'views_handler_field_node_registration',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Field: email.
  $data['node_registration']['email'] = array(
    'title' => t('E-mail'),
    'help' => t("The registree's e-mail."),
    'field' => array(
      'field' => 'email',
      'group' => t('Node registration'),
      'handler' => 'views_handler_field_node_registration',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Field: slots.
  $data['node_registration']['slots'] = array(
    'title' => t('Slots'),
    'help' => t('The number of slots consumed by this Node registration.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Field: created.
  $data['node_registration']['created'] = array(
    'title' => t('Created'),
    'help' => t('The date the registration was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Field: updated.
  $data['node_registration']['updated'] = array(
    'title' => t('Updated'),
    'help' => t('The date the registration was last changed.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Field: weight.
  $data['node_registration']['weight'] = array(
    'title' => t('Weight'),
    'help' => t('The weight of this registration in this event. Used for waitinglist execution order and display.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  // Virtual field: view.
  $data['node_registration']['view_node_registration']['moved to'] = array('views_entity_node_registration', 'view_node_registration');
  $data['views_entity_node_registration']['view_node_registration'] = array(
    'field' => array(
      'title' => t('View link'),
      'help' => t('Provide a simple link to the Node registration.'),
      'handler' => 'views_handler_field_node_registration_link',
    ),
  );

  // Virtual field: edit.
  $data['node_registration']['edit_node_registration']['moved to'] = array('views_entity_node_registration', 'edit_node_registration');
  $data['views_entity_node_registration']['edit_node_registration'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the Node registration.'),
      'handler' => 'views_handler_field_node_registration_link_edit',
    ),
  );

  // Virtual field: cancel.
  $data['node_registration']['cancel_node_registration']['moved to'] = array('views_entity_node_registration', 'cancel_node_registration');
  $data['views_entity_node_registration']['cancel_node_registration'] = array(
    'field' => array(
      'title' => t('Cancel link'),
      'help' => t('Provide a simple link to cancel the Node registration.'),
      'handler' => 'views_handler_field_node_registration_link_cancel',
    ),
  );

  // Virtual field: delete.
  $data['node_registration']['delete_node_registration']['moved to'] = array('views_entity_node_registration', 'delete_node_registration');
  $data['views_entity_node_registration']['delete_node_registration'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the Node registration.'),
      'handler' => 'views_handler_field_node_registration_link_delete',
    ),
  );

  // Virtual field: path.
  $data['node_registration']['path'] = array(
    'field' => array(
      'title' => t('Path'),
      'help' => t('The aliased path to this Node registration.'),
      'handler' => 'views_handler_field_node_registration_path',
    ),
  );

  // Field: uid.
  $data['node_registration']['uid'] = array(
    'title' => t('User uid'),
    'help' => t('The registree user (optional)'),
    'relationship' => array(
      'title' => t('User'),
      'help' => t('Relate to the registered user.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('User'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_registration_uid',
    ),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
  );

  // Field: author_uid.
  $data['node_registration']['author_uid'] = array(
    'title' => t('Author uid'),
    'help' => t('The user who made this registration (optional)'),
    'relationship' => array(
      'title' => t('Author'),
      'help' => t('Relate to user who made this registration.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'author_uid',
      'label' => t('Author'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
  );

  // Field: nid.
  $data['node_registration']['nid'] = array(
    'title' => t('Node nid'),
    'help' => t('The event node'),
    'relationship' => array(
      'title' => t('Event node'),
      'help' => t('Relate to event node.'),
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'field' => 'nid',
      'label' => t('Event node'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  // Field: cancelled.
  $data['node_registration']['cancelled'] = array(
    'title' => t('Cancelled'),
    'help' => t('Whether this registration was cancelled.'),
    'field' => array(
      'handler' => 'nr_views_handler_field_boolean_togglable',
      'click sortable' => TRUE,
      'output formats' => array(
        'y-n' => array('y', 'n'),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Cancelled'),
      'type' => 'yes-no',
      'use equal' => FALSE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Field: attended.
  $data['node_registration']['attended'] = array(
    'title' => t('Attended'),
    'help' => t('Whether the person has attended the event.'),
    'field' => array(
      'handler' => 'nr_views_handler_field_boolean_togglable',
      'click sortable' => TRUE,
      'output formats' => array(
        'y-n' => array('y', 'n'),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Attended'),
      'type' => 'yes-no',
      'use equal' => FALSE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Field: verified.
  $data['node_registration']['verified'] = array(
    'title' => t('Verified'),
    'help' => t('Whether the person has verified the event.'),
    'field' => array(
      'handler' => 'nr_views_handler_field_boolean_togglable',
      'click sortable' => TRUE,
      'output formats' => array(
        'y-n' => array('y', 'n'),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Verified'),
      'type' => 'yes-no',
      'use equal' => FALSE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Field: waitinglist.
  $data['node_registration']['waitinglist'] = array(
    'title' => t('Waitinglist'),
    'help' => t('Whether the person has waitinglist the event.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'y-n' => array('y', 'n'),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Waitinglist'),
      'type' => 'yes-no',
      'use equal' => FALSE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Field: reminder_sent.
  $data['node_registration']['reminder_sent'] = array(
    'title' => t('Reminder sent'),
    'help' => t('The date a reminder was sent to the registree.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Virtual field: registered.
  $data['users']['registered'] = array(
    'title' => t('Registered'),
    'help' => t("Filters users on whether they are registered to the node passed by the argument. Uses a subquery, don't use this in a front end view."),
    'argument' => array(
      'field' => 'uid',
      'handler' => 'views_handler_argument_node_registration_registered',
    ),
  );


  // NODE FIELDS //

  // Field: register_link.
  $data['node']['register_link']['moved to'] = array('views_entity_node', 'register_link');
  $data['views_entity_node']['register_link'] = array(
    'field' => array(
      'title' => t('Register link'),
      'help' => t('Provide a simple link to register for an event.'),
      'handler' => 'views_handler_field_node_link_register',
    ),
  );

  // Field: no_register_access.
  $data['node']['no_register_access']['moved to'] = array('views_entity_node', 'no_register_access');
  $data['views_entity_node']['no_register_access'] = array(
    'field' => array(
      'title' => t('No register access text'),
      'help' => t("A custom text to show when the current user doesn't have register access for this node."),
      'handler' => 'views_handler_field_node_no_register_access',
    ),
  );

  // Field: registration_capacity.
  $data['node']['registration_capacity']['moved to'] = array('views_entity_node', 'registration_capacity');
  $data['views_entity_node']['registration_capacity'] = array(
    'title' => t('Event capacity'),
    'help' => t("The set capacity for this event."),
    'field' => array(
      'handler' => 'views_handler_field_node_registration_capacity',
    ),
  );

  // Field: registration_used_slots.
  $data['node']['registration_used_slots'] = array(
    'title' => t('Used slots'),
    'help' => t("The used slots for this event."),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_field_node_registration_used_slots',
    ),
  );

  // Field: registration_available_slots.
  $data['node']['registration_available_slots'] = array(
    'title' => t('Available slots'),
    'help' => t("The available slots for this event. Capacity - Used slots."),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_field_node_registration_available_slots',
    ),
  );

  // Field: cancelled date
  $data['node_registration']['cancelled_date'] = array(
    'title' => t('Cancelled date'),
    'help' => t('The date the registration was cancelled.'),
    'real field' => 'cancelled',
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 *
 * Create 'reversed' relationships eg. from node to registration(s)
 */
function node_registration_views_data_alter(&$data) {

  // Node to Registration.
  $data['node']['node_registration_rel'] = array(
    'group' => t('Node registration'),
    'title' => t('Registrations'),
    'help' => t('Relate from node to it\'s registrations.'),
    'relationship' => array(
      'label' => t('Registrations'),
      'base' => 'node_registration',
      'base field' => 'nid',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
    ),
  );
}
