<?php

/**
 * @file
 * Views integration for State Flow
 */

/**
 * Implements hook_views_data().
 */
function state_flow_views_data() {
  $data = array();

  // Make the node_revision_states table available for node and node revision
  // views.
  $data['node_revision_states']['table']['group'] = t('State Flow');
  $data['node_revision_states']['table']['join']['node'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
  );
  $data['node_revision_states']['table']['join']['node_revision'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
  );

  // Make the state and timestamp fields available.
  $data['node_revision_states']['state'] = array(
    'title' => t('State'),
    'help' => t("The workflow state of the selected node's revision."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'state_flow_handler_filter_state',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['node_revision_states']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t("The workflow timestamp of the selected node's revision."),
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

  // Provide fields for a link to the workflow page and to available events.
  $data['node_revision_states']['workflow_link'] = array(
    'title' => t('Workflow link'),
    'help' => t("A link to the workflow page for the selected node's revision."),
    'field' => array(
      'handler' => 'state_flow_handler_field_workflow_link',
    ),
  );
  $data['node_revision_states']['events'] = array(
    'title' => t('Event links'),
    'help' => t("The workflow events available for the selected node's revision."),
    'field' => array(
      'handler' => 'state_flow_handler_field_events',
    ),
  );

// Make the node_revision_states_history table available for node and node revision
  // views.
  $data['node_revision_states_history']['table']['group'] = t('State Flow History');
  $data['node_revision_states_history']['table']['join']['node'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
  );
  $data['node_revision_states_history']['table']['join']['node_revision'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
  );

  // Make the state and timestamp fields available.
  $data['node_revision_states_history']['state'] = array(
    'title' => t('State'),
    'help' => t("The workflow state history of the selected node's revision."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'state_flow_handler_filter_state',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['node_revision_states_history']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t("The workflow history timestamp of the selected node's revision."),
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
