<?php

/**
 * Page callback for a node's Workflow page.
 */
function state_flow_events($node) {
  // Gather information on the current published revision
  $state_flow = state_flow_load_state_machine($node);
  $events = $state_flow->get_available_events();
  $current_revision_actions = array();
  foreach ($events as $event_machine_name => $event) {
    if (state_flow_access($node, $event_machine_name)) {
      $current_revision_actions[] = l($event->get_option('label'), 'node/' . $node->nid . '/revisions/' . $node->vid . '/workflow/' . $event_machine_name);
    }
  }
  $current_revision_actions_str = implode(' | ', $current_revision_actions);

  // Build table for the active draft revisions and other revisions of this node
  $drafts_table = array(
    'header' => array(
      t('Revision ID'),
      t('Title'),
      t('Status'),
      t('Last Updated'),
      t('Author'),
      t('Actions'),
    ),
    'rows' => array(),
  );
  $others_table = array(
    'header' => array(
      t('Revision ID'),
      t('Title'),
      t('Status'),
      t('Last Updated'),
      t('Author'),
      t('Actions'),
    ),
    'rows' => array(),
  );
  $states = state_flow_get_revisions($node->nid);
  foreach ($states as $state) {
    if (empty($state->state)) {
      continue;
    }
    $vid = intval($state->vid);
    $path_view = ($vid == $node->vid) ? 'node/' . $node->nid : 'node/' . $node->nid . '/revisions/' . $vid . '/view';
    $state_date_str = format_date($state->timestamp, 'short');
    if (($state->status && !($state->state === 'published' && $vid == $node->vid)) || (!$state->status && $vid != $node->vid)) {
      $state_node = node_load($node->nid, $vid);
      $rev_author = user_load($state_node->revision_uid);
      $state_state_flow = state_flow_load_state_machine($state_node);
      $state_events = $state_state_flow->get_available_events();
      $state_actions = array();
      if (node_access('update', $state_node)) {
        $state_actions[] = l(t('Edit'), 'node/' . $node->nid . '/revisions/' . $vid . '/edit');
      }
      if (_node_revision_access($state_node, 'delete')) {
        $state_actions[] = l(t('Delete'), 'node/' . $node->nid . '/revisions/' . $vid . '/delete');
      }
      foreach ($state_events as $event_machine_name => $event) {
        if (state_flow_access($state_node, $event_machine_name)) {
          $state_actions[] = l($event->get_option('label'), 'node/' . $node->nid . '/revisions/' . $vid . '/workflow/' . $event_machine_name) . ' ';
        }
      }
      $state_actions_str = implode(' | ', $state_actions);
      $item = array(
        l($vid, $path_view),
        l(check_plain($state_node->title), $path_view),
        t($state->state),
        $state_date_str,
        theme('username', array('account' => $rev_author)),
        $state_actions_str,
      );
      if ($state->status) {
        $drafts_table['rows'][] = $item;
      }
      else {
        $others_table['rows'][] = $item;
      }
    }
  }

  // Build a table for the history of this node
  $history_table = array(
    'header' => array(
      t('Date'),
      t('Message'),
    ),
    'rows' => array(),
  );
  $history = state_flow_get_history($node->nid);
  foreach ($history as $state) {
    $vid = intval($state->vid);
    $path_view = ($vid == $node->vid) ? 'node/' . $node->nid : 'node/' . $node->nid . '/revisions/' . $vid . '/view';
    $state_date_str = format_date($state->timestamp, 'short');
    $history_str = '';
    $user_name = format_username((object) array('uid' => (int) $state->uid, 'name' => $state->user_name));
    $history_str = t('!user transitioned revision !vid-link to %state.',
      array(
        '!user' => l(check_plain($user_name), 'user/' . $state->uid),
        '!vid-link' => l($vid,  $path_view),
        '%state' => $state->state
      )
    );
    if (!empty($state->log)) {
      $history_str .= '<br/>' . t('@user said: @log', array('@user' => $user_name, '@log' => $state->log));
    }
    $history_table['rows'][] = array(
      $state_date_str,
      $history_str,
    );
  }

  // Give other modules a chance to modify the table data.
  drupal_alter('state_flow_draft_table', $drafts_table, $node, $states);
  drupal_alter('state_flow_others_table', $others_table, $node, $states);
  drupal_alter('state_flow_history_table', $history_table, $node);

  // Set the title for this page
  drupal_set_title(t('Workflow for @title', array('@title' => $node->title)), PASS_THROUGH);

  // Build the render array
  $output = array(
    'current_revision' => array(
      '#type' => 'fieldset',
      '#title' => t('Current Revision'),
      'current_revision_status' => array(
        '#type' => 'item',
        '#title' => t('Status'),
        '#markup' => check_plain($state_flow->get_current_state()),
      ),
      'current_revision_vid' => array(
        '#type' => 'item',
        '#title' => t('Revision ID'),
        '#markup' => l(check_plain($node->vid), 'node/' . $node->nid),
      ),
    ),
    '#sorted' => TRUE,
    '#attached' => array(
      'js' => array(
        'misc/form.js',
        'misc/collapse.js',
      ),
    ),
  );
  if (!empty($current_revision_actions_str)) {
    $output['content']['current_revision']['current_revision_actions'] = array(
      '#type' => 'item',
      '#title' => t('Actions'),
      '#markup' => $current_revision_actions_str,
    );
  }
  $output['content']['draft_revisions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Working Drafts'),
    'draft_revisions_table' => array(
      '#markup' => theme('table', $drafts_table),
    ),
  );
  $output['content']['other_revisions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Archived Drafts'),
    'other_revisions_table' => array(
      '#markup' => theme('table', $others_table),
    ),
    '#attributes' => array(
      'class' => array('collapsible', 'collapsed'),
    ),
  );
  $output['content']['history'] = array(
    '#type' => 'fieldset',
    '#title' => t('History'),
    'history_table' => array(
      '#markup' => theme('table', $history_table),
    ),
    '#attributes' => array(
      'class' => array('collapsible', 'collapsed'),
    ),
  );

  // Allow other modules to modify the output of the workflow tab
  // before it is rendered.
  drupal_alter('state_flow_events_page', $output, $node, $events);

  return $output;
}

/**
 * Form builder for the node revision workflow transition form.
 */
function state_flow_events_revision($form, &$form_state, $node_revision, $new_event = NULL) {
  $form = array();
  $state_flow = state_flow_load_state_machine($node_revision);
  $all_events = $state_flow->get_available_events();
  $events = array();
  foreach ($all_events as $event_machine_name => $event) {
    if (state_flow_access($node_revision, $event_machine_name)) {
      $events[$event_machine_name] = $event->get_option('label');
    }
  }

  if (empty($events)) {
    drupal_set_message(t('You cannot perform any workflow transitions on this revision.'), 'warning');
    drupal_goto('node/' . $node_revision->nid . '/workflow');
  }
  elseif (empty($new_event) || !array_key_exists($new_event, $events)) {
    drupal_set_title(t('Transition revision @rev-vid to a new state', array('@rev-vid' => $node_revision->vid)));
    $form['event'] = array(
      '#type' => 'radios',
      '#title' => t('Choose the event to fire'),
      '#options' => drupal_map_assoc($events),
    );
  }
  else {
    drupal_set_title(t('Transition @title (revision @rev-vid) to "@state"', array(
      '@title' => $node_revision->title,
      '@rev-vid' => $node_revision->vid,
      '@state' => $events[$new_event])), PASS_THROUGH);

    $form['event'] = array(
      '#type' => 'value',
      '#value' => $new_event,
    );
  }

  $form['event_comment'] = array(
    '#type' => 'textarea',
    '#title' => t('Log message for this state change'),
    '#default_value' => '',
    '#required' => TRUE,
  );

  $form['node_revision'] = array(
    '#type' => 'value',
    '#value' => $node_revision,
  );
  $form['state_flow'] = array(
    '#type' => 'value',
    '#value' => $state_flow,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update state'),
  );
  $form['cancel'] = array(
    '#markup' => l(t('Cancel'), 'node/' . $node_revision->nid . '/workflow'),
  );

  return $form;
}

/**
 * Form submission callback for state_flow_events_revision.
 */
function state_flow_events_revision_submit($form, &$form_state) {
  $node_revision = $form_state['values']['node_revision'];
  $state_flow = $form_state['values']['state_flow'];
  $event = $form_state['values']['event'];
  $event_comment = $form_state['values']['event_comment'];
  if (!empty($event) && !empty($event_comment)) {
    global $user;
    $rv = $state_flow->fire_event($event, $user->uid, $event_comment);
    if ($rv !== FALSE) {
      $state = $state_flow->get_current_state();
      drupal_set_message(t('%title transitioned to the @state state.', array('%title' => $node_revision->title, '@state' => $state)));
      $form_state['redirect'] = 'node/' . $node_revision->nid . '/workflow';
    }
  }
}
