<?php

/**
 * @file
 * Page callbacks for adding, editing, deleting, and revisions management for
 * files.
 */

define('MARK_STATE_IS_DELETED', '*');

/**
 * Generates an overview table of older revisions of a file.
 * This is the callback for the Revisions Workflow tab.
 * file/%file/revisions
 *
 * @param object $file
 *   A file object.
 *
 * @return array
 *   An array as expected by drupal_render().
 *
 * @see file_menu()
 */
function file_entity_revisions_overview($file) {
  drupal_set_title(t('Revisions for %filename', array('%filename' => $file->filename)), PASS_THROUGH);

  // Generate history for each revision.
  $historyrows = _file_entity_revisions_history('file', $file);
  $widgitoutput = $historyrows['widgetoutput'];
  
  // Set header.
  $header = array(
    t('Revision ID'),
    t('File Info'),
    t('Moderated Transitions'),
    array('data' => t('Revision Operations'), 'colspan' => 2),

  );

  $revisions = file_entity_revisions_list($file);

  $rows = array();
  $revert_permission = FALSE;
  if ((user_access('revert revisions') || user_access('administer files')) && file_entity_access('update', $file)) {
    $revert_permission = TRUE;
  }
  $delete_permission = FALSE;
  if ((user_access('delete revisions') || user_access('administer files')) && file_entity_access('delete', $file)) {
    $delete_permission = TRUE;
  }

  // Get domain keys
  $domains = db_select('domain', 'd')
  ->fields('d', array('domain_id', 'sitename'))
  ->execute()->fetchAllAssoc('domain_id');
  $domainkeys = array();
  $domainkeys[0] = 'All Sites';
  foreach ($domains as $dkey => $darr) {
    if (isset($darr->sitename)) {
      $domainkeys[$dkey] = $darr->sitename;
    }
  }
  // Loop through each revision.
  foreach ($revisions as $revision) {
    $row = array();
    $operations = array();

    // TODO: implement revision log messages.
    $revision->log = isset($revision->log) ? $revision->log : '';
    $published = $revision->published ? t('Accessible') : t('Unpublished');

    $file_url = l('View', file_create_url($revision->uri), array('attributes' => array('target' => '_blank')));
    // Append domains to file_url.
    if (isset($revision->domains) && count($revision->domains) > 0) {
      $file_url .= '<br>(';
      $domains = $revision->domains;
      $file_url .= $domainkeys[array_shift($domains)];
      foreach ($domains as $d) {
        $file_url .= ', ' . $domainkeys[$d];
      }
      $file_url .= ')';
    }
    // Determine Workflow history for this revision.
    $historyheader = $historyactions = '';
    $premarkup = '<div class="fer_history_header_name">';
    $postmarkup = '</div>';
    $historyheaders = array('Time', 'Previous State', 'Next State', 'User', 'Comment');
    $columns = 0;
    foreach ($historyheaders as $h) {
      $historyheader .= $premarkup . $h . $postmarkup;
      $columns++;      
    }
    if (isset($historyrows[$revision->vid])) {
      foreach ($historyrows[$revision->vid] as $history) {
        if (isset($history['data'])) {
          $premarkup = '<div class="fer_history">';
          $postmarkup = '</div>';
          $historyactions .= '<div class="fer_history_row">';
          $nowcolumn = 0;
          foreach ($history['data'] as $val) {
            if (isset($val['data']) && strlen($val['data']) > 0) {
              $historyactions .= $premarkup . $val['data'] . $postmarkup;
              $nowcolumn++;
            }
          }
          for (;$nowcolumn < $columns; $nowcolumn++) {
            $historyactions .= $premarkup . $postmarkup;
          }
          $historyactions .= '</div>';
        }
      }
    }
    $historyblock = '<div class="fer_history_top"><div class="fer_history_header">' 
        . $historyheader . '</div>' . $historyactions . '</div>';
    
    // Create row for current revision.
    if ($revision->current_vid > 0) {
      $revisionName = $revision->vid . '<div class="fer-current-revision">Current Revision</div>';
      if ($revision->published == 1) {
        $revisionName .= '<div class="fer-live-revision">' . $published . '</div>';
      }
      
      $row[] = array(
        'data' => $revisionName,
        'class' => array('revision-current'),
      );
      $row[] = array(
        'data' => $file_url,
        'class' => array('revision-current'),
      );

      $row[] = array(
        'data' => (strlen($historyactions) > 0) ? $historyblock : $published,
        'class' => array('revision-current'),
      );
      $operations[] = array(
        'data' => $widgitoutput,
        'colspan' => 2,
      );
    }
    else {
      // Create row for any non-current revision.
      $revisionName = $revision->vid;
      if ($revision->published == 1) {
        $revisionName .= '<div class="fer-live-revision">' . $published . '</div>';
      }
      $row[] = $revisionName;
      $row[] = $file_url;
      $row[] = (strlen($historyactions) > 0) ? $historyblock : $published;
      if ($revert_permission) {
        $operations[] = l(t('Make Current Revision'), "file/$file->fid/revisions/$revision->vid/revert");
      }
      if ($delete_permission) {
        $operations[] = l(t('Delete this Revision'), "file/$file->fid/revisions/$revision->vid/delete");
      }
    }
    $rows[] = array_merge($row, $operations);
  }
  $rows[0]['class'] = array('test');
  // Done looping through revisions, create build array and return it.
  $build['file_entity_revisions_table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
  );
  return $build;
}
/**
 * Helper function that pulls file revision history data.
 * This code was taken from Workflow and modified so that we could
 * include their templates in the Revisions tab.
 * 
 * @param unknown $entity_type
 * @param string $entity
 * @return string
 */
function _file_entity_revisions_history($entity_type, $entity = NULL) {
  $form = array();

  // Figure out the $entity's bundle and id.
  list($entity_id, , $entity_bundle) = entity_extract_ids($entity_type, $entity);
  $entity_id = entity_id($entity_type, $entity);

  // When having multiple workflows per bundle, use Views display
  // 'Workflow history per entity' instead!
  $workflow = workflow_get_workflows_by_type($entity_bundle, $entity_type);

  // Show the current state and the Workflow form to allow state changing.
  // N.B. This part is replicated in hook_node_view, workflow_tab_page, workflow_vbo.
  // @todo: support multiple workflows per entity.
  // For workflow_tab_page with multiple workflows, use a separate view. See [#2217291].
  if ($workflow) {
    // $field = _workflow_info_field($field_name, $workflow);
    $field_info = _workflow_info_fields($entity, $entity_type, $entity_bundle);
    $field = reset($field_info);
    $field_name = $field['field_name'];
    $field_id = $field['id'];
    $instance = field_info_instance($entity_type, $field_name, $entity_bundle);
    if (!$field['id']) {
      // This is a Workflow Node workflow. Set widget options as in v7.x-1.2
      $field['settings']['widget']['comment'] = isset($workflow->options['comment_log_tab']) ? $workflow->options['comment_log_tab'] : 1; // vs. ['comment_log_node'];
      $field['settings']['widget']['current_status'] = TRUE;
    }
  }

  $current_sid = workflow_node_current_state($entity, $entity_type, $field_name);

  // Add the form/widget to the formatter, and include the nid and field_id in the form id,
  // to allow multiple forms per page (in listings, with hook_forms() ).
  // Ultimately, this is a wrapper for WorkflowDefaultWidget.
  $form['workflow_current_state'] = workflow_state_formatter($entity_type, $entity, $field, $instance, $current_sid);
  // Set a proper weight, which works for Workflow Options in select list AND action buttons.
  $form['workflow_current_state']['#weight'] = -0.005;
  $form += drupal_get_form(implode('_', array('workflow_transition_form', $entity_type, $entity_id, $field_id)), $field, $instance, $entity_type, $entity);
  // Update the widget values to look better.
  $form['workflow_current_state']['#title'] = '';
  if (isset($form['workflow_current_state'][0])) {
    $form['workflow_current_state'][0]['#markup'] = '<div class="workflow-current-state-label">Current Workflow State:</div>'
        . '<div class="workflow-current-state">'
            . $form['workflow_current_state'][0]['#markup'] . '</div>';
  }
  $form['workflow']['workflow_sid']['#title'] = 'Change Workflow State:';
  $widgetoutput = drupal_render($form);

  // Show the history table.
  $rows = array();
  $current_themed = FALSE;
  $limit = 2000; // = variable_get('workflow_states_per_page', 2000);
  foreach (workflow_transition_load_multiple($entity_type, array($entity_id), $field_name, $limit) as $history) {
    $old_state_name = $new_state_name = '';

    $label = $name = '';
    $new_state = $history->getNewState();
    if ($new_state) {
      $name = $new_state->getName();
      $label = check_plain(t($new_state->label()));
    }

    if (!$new_state) {
      // This is an invalid/deleted state.
      $old_state_name = $label;
    }
    elseif ($history->new_sid == $current_sid && $new_state->isActive() && !$current_themed) {
      // Theme the current state differently so it stands out.
      $new_state_name = theme('file_er_history_current_state', array(
          'state_name' => $label,
          'state_system_name' => $name,
          'sid' => $history->new_sid,
      ));
      // Make a note that we have themed the current state; other times in the history
      // of this node where the node was in this state do not need to be specially themed.
      $current_themed = TRUE;
    }
    elseif (!$new_state->isActive()) {
      // The state has been deleted, but we include it in the history.
      $new_state_name = theme('file_er_deleted_state', array(
          'state_name' => $label,
          'state_system_name' => $name,
          'sid' => $history->new_sid,
      ));
      $footer_needed = TRUE;
    }
    else {
      // Regular state.
      $new_state_name = $label;
    }
    unset($new_state); // Not needed anymore.

    $label = $name = MARK_STATE_IS_DELETED;
    $old_state = $history->getOldState();
    if ($old_state) {
      $name = $old_state->getName();
      $label = check_plain(t($old_state->label()));
    }

    if (!$old_state) {
      // This is an invalid/deleted state.
      $old_state_name = $label;
    }
    elseif (!$old_state->isActive()) {
      $old_state_name = theme('file_er_deleted_state', array(
          'state_name' => $label,
          'state_system_name' => $name,
          'sid' => $history->old_sid,
      ));
      $footer_needed = TRUE;
    }
    else {
      // Regular state.
      $old_state_name = $label;
    }
    unset($old_state); // Not needed anymore.

    $variables = array(
        'transition' => $history, // @todo D8: pass this WorkflowTransition as only variable. It contains everything.
        'extra' => '',

        'history' => $history,  // @todo D8: remove, as this is the same as 'transition'.
        'old_sid' => $history->old_sid, // @todo D8: remove this redundant property.
        'sid' => $history->new_sid, // @todo D8: remove this redundant property.
        'uid' => $history->uid, // @todo D8: remove this redundant property.
        'old_state_name' => $old_state_name,
        'state_name' => $new_state_name,
    );
    // Now store themed output in associative array so history is split up by revision.
    $rows[$variables['history']->revision_id][] = theme('file_er_table_row', $variables);
  }
  // Store the operations widget as well.
  $rows['widgetoutput'] = $widgetoutput;
  return $rows;
}

/**
 * Theme one WorkflowTansition in a workflow history table row.
 *
 * $old_state_name and $state_name must be run through check_plain(t()) prior
 * to calling this theme function.
 */
function theme_file_er_table_row($variables) {
  $row = array();
  $old_state_name = $variables['old_state_name'];
  $state_name = $variables['state_name'];
  $transition = $variables['transition'];
  $account = user_load($transition->uid);
  $row = array(
      'data' => array(
          array('data' => format_date($transition->stamp), 'class' => array('timestamp')),
          array('data' => $old_state_name, 'class' => array('previous-state-name')),
          array('data' => $state_name, 'class' => array('state-name')),
          array('data' => theme('username', array('account' => $account)), 'class' => array('user-name')),
          array('data' => filter_xss($transition->comment), 'class' => array('log-comment')),
          $variables['extra'],
      ),
      'class' => array('workflow_history_row'),
  );

  return $row;
}

/**
 * Theme the current state in the workflow history table.
 *
 * $state_name must be run through check_plain(t()) prior
 * to calling this theme function.
 */
function theme_file_er_history_current_state($variables) {
  return $variables['state_name'];
}

/**
 * Theme a deleted state in the workflow history table.
 *
 * $state_name must be run through check_plain(t()) prior
 * to calling this theme function.
 */
function theme_file_er_deleted_state($variables) {
  return $variables['state_name'] . MARK_STATE_IS_DELETED;
}

/**
 * Asks for confirmation of the reversion to prevent against CSRF attacks.
 *
 * @param int $file_revision
 *   The file revision ID.
 *
 * @return array
 *   An array as expected by drupal_render().
 *
 * @see file_menu()
 * @see file_entity_revision_revert_confirm_submit()
 * @ingroup forms
 */
function file_entity_revision_revert_confirm($form, $form_state, $file_revision) {
  $form['#file_revision'] = $file_revision;
  return confirm_form($form, t('Are you sure you want to make the revision from %revision-date the current revision?', array('%revision-date' => format_date($file_revision->timestamp))), 'file/' . $file_revision->fid . '/revisions', '', t('Make Current'), t('Cancel'));
}

/**
 * Form submission handler for file_revision_revert_confirm().
 */
function file_entity_revision_revert_confirm_submit($form, &$form_state) {
  $file_revision = $form['#file_revision'];
  $file_revision->revision = 1;
  $file_revision->log = t('Copy of the revision from %date.', array('%date' => format_date($file_revision->timestamp)));

  file_save($file_revision);

  watchdog('content', '@type: reverted %filename revision %revision.', array(
    '@type' => $file_revision->type,
    '%filename' => $file_revision->filename,
    '%revision' => $file_revision->vid,
  ));
  drupal_set_message(t('@type %filename has been reverted back to the revision from %revision-date.', array(
    '@type' => $file_revision->type,
    '%filename' => $file_revision->filename,
    '%revision-date' => format_date($file_revision->timestamp),
  )));
  $form_state['redirect'] = 'file/' . $file_revision->fid . '/revisions';
}


/**
 * Form constructor for the revision deletion confirmation form.
 *
 * This form prevents against CSRF attacks.
 *
 * @param int $file_revision
 *   The file revision ID.
 *
 * @return array
 *   An array as expected by drupal_render().
 *
 * @see file_entity_revisions_menu()
 * @see file_entity_revision_delete_confirm_submit()
 * @ingroup forms
 */
function file_entity_revision_delete_confirm($form, $form_state, $file_revision) {
  $form['#file_revision'] = $file_revision;
  return confirm_form($form, t('Are you sure you want to delete the revision from %revision-date?', array('%revision-date' => format_date($file_revision->timestamp))), 'file/' . $file_revision->fid . '/revisions', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Form submission handler for file_entity_revision_delete_confirm().
 */
function file_entity_revision_delete_confirm_submit($form, &$form_state) {
  $file_revision = $form['#file_revision'];
  file_entity_revision_delete($file_revision->vid, 'file');

  watchdog('content', '@type: deleted %filename revision %revision.', array(
    '@type' => $file_revision->type,
    '%filename' => $file_revision->filename,
    '%revision' => $file_revision->vid,
  ));
  drupal_set_message(t('Revision from %revision-date of @type %filename has been deleted.', array(
    '%revision-date' => format_date($file_revision->timestamp),
    '@type' => $file_revision->type,
    '%filename' => $file_revision->filename,
  )));
  $form_state['redirect'] = 'file/' . $file_revision->fid;
  if (db_query('SELECT COUNT(vid) FROM {file_managed_revisions} WHERE fid = :fid', array(':fid' => $file_revision->fid))->fetchField() > 1) {
    $form_state['redirect'] .= '/revisions';
  }
}
 