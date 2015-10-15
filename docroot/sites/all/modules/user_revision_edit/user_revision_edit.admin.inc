<?php
/**
 * @file
 * Admin settings for the User Revision Edit module.
 */

/**
 * Generate an overview table of older revisions of a user.
 *
 * This overrides user_revision_overview() of User revision module.
 */
function user_revision_edit_overview($viewed_user) {
  global $user;

  // Sets page title.
  drupal_set_title(t('Revisions for %title',
    array('%title' => $viewed_user->name)), PASS_THROUGH);

  // Sets table view header.
  $header = array(t('Revision'),
    array('data' => t('Operations'), 'colspan' => 3));

  // Checks permissions.
  $revert_permission = user_access('revert user revisions')
    || user_access('revert own user revisions')
    && $user->uid == $viewed_user->uid;
  $edit_permission = user_access('edit user revisions')
    || user_access('edit own user revisions')
    && $user->uid == $viewed_user->uid;
  $delete_permission = user_access('delete user revisions')
    || user_access('delete own user revisions')
    && $user->uid == $viewed_user->uid;

  // Lists revisions.
  $revisions = user_revision_list($viewed_user);
  $rows = array();
  foreach ($revisions as $revision) {
    $row = array();
    $operations = array();
    if ($revision->current_vid > 0) {

      // Prepares top row for latest user revision.
      $row[] = array(
        'data' => t(
          '!date by !username', array(
            '!date' => l(
              format_date($revision->timestamp, 'short'),
              "user/$viewed_user->uid"
            ),
            '!username' => theme('username', array('account' => user_load($revision->authorid))),
          )
        ) . (($revision->log != '') ? '<p class="revision-log">'
          . filter_xss($revision->log) . '</p>' : ''),
        'class' => array('revision-current'));
      $operations[] = array(
        'data' => drupal_placeholder(t('current revision')),
        'class' => array('revision-current'),
        'colspan' => 3,
      );
    }
    else {

      // Prepares rows for older user revisions.
      $row[] = t(
        '!date by !username',
        array(
          '!date' => l(
            format_date($revision->timestamp, 'short'),
            "user/$viewed_user->uid/revisions/$revision->vid/view"
          ),
          '!username' => theme('username', array('account' => user_load($revision->authorid))),
        )
      ) . (($revision->log != '') ? '<p class="revision-log">'
        . filter_xss($revision->log) . '</p>' : '');

      // Adds 'revert' link.
      if ($revert_permission) {
        $operations[] = l(
          t('revert'),
          "user/$viewed_user->uid/revisions/$revision->vid/revert"
        );
      }
      else {
        // Adds empty row for theming purposes.
        $operations[] = "";
      }

      // Adds 'edit' link.
      if ($edit_permission) {
        $operations[] = l(
          t('edit'),
          "user/$viewed_user->uid/revisions/$revision->vid/edit"
        );
      }
      else {
        // Adds empty row for theming purposes.
        $operations[] = "";
      }

      // Adds 'delete' link.
      if ($delete_permission) {
        $operations[] = l(
          t('delete'),
          "user/$viewed_user->uid/revisions/$revision->vid/delete"
        );
      }
      else {
        // Adds empty row for theming purposes.
        $operations[] = "";
      }
    }
    $rows[] = array_merge($row, $operations);
  }

  $build['user_revisions_table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
  );

  return $build;
}

/**
 * Form to edit a revision.
 */
function user_revision_edit_form($form, &$form_state, $user_revision) {

  // Prepare loading the user profile form.
  form_load_include($form_state, 'inc', 'user', 'user.pages');

  // Populate form state.
  $form_state += form_state_defaults();

  // Rebuild the entire user profile form for our needs;
  // we can not use drupal_get_form() here, we need drupal_build_form()
  // without the final processing from drupal_process_form().
  $form = drupal_retrieve_form('user_profile_form', $form_state);
  drupal_prepare_form('user_profile_form', $form, $form_state);

  // Save the revision to the form.
  $form['#user_revision'] = $user_revision;

  $form['revision_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Revision information'),
    '#collapsible' => TRUE,
    '#attributes' => array(
      'class' => array('user-profile-form-revision-information'),
    ),
    '#weight' => 20,
    '#access' => user_access('choose user revisions'),
  );

  // Make log message editable.
  $form['revision_information']['log'] = array(
    '#type' => 'textarea',
    '#title' => t('Revision log message'),
    '#rows' => 4,
    '#description' =>
    t('Provide an explanation of the changes you are making. This will help other authors understand your motivations.'),
    '#default_value' => $user_revision->log,
  );

  $form['vid'] = array(
    '#type' => 'value',
    '#value' => $user_revision->vid,
  );
  $form['ip'] = array(
    '#type' => 'value',
    '#value' => ip_address(),
  );

  // Add a cancel button.
  $form['actions']['cancel'] = array(
    '#type'   => 'submit',
    '#value'  => t('Cancel'),
    '#access' => TRUE,
    '#weight' => 55,
    '#submit' => array('user_revision_edit_form_cancel_submit'),
    '#limit_validation_errors' => array(),
  );

  // Remove global user settings as we only edit the revision.
  unset($form['signature_settings']);
  unset($form['account']['status']);
  unset($form['account']['pass']);
  unset($form['account']['roles']);
  unset($form['timezone']);

  // Clear all callbacks to avoid editing the user and not the revision.
  unset($form['#submit']);
  $form['#submit'] = array('user_revision_edit_form_submit');

  return $form;
}

/**
 * Submit callback for edit revision form.
 */
function user_revision_edit_form_submit($form, &$form_state) {
  $user_revision = $form['#user_revision'];
  form_state_values_clean($form_state);

  // Updates user_revision schema.
  _user_revision_edit_save($user_revision, $form_state['values']);

  $form_state['redirect'] = 'user/' . $user_revision->uid . '/revisions';

  // Clear the cache because pages can contain revision information.
  cache_clear_all();

  watchdog(
    'user',
    "Modified {$user_revision->name} revision {$user_revision->vid}."
  );

  drupal_set_message(
    t('Updated revision %vid of %user.',
      array(
        '%vid' => $user_revision->vid,
        '%user' => $user_revision->name,
      )
    ),
    'status'
  );
}

/**
 * Submit callback for edit revision form cancel button.
 */
function user_revision_edit_form_cancel_submit($form, &$form_state) {
  $user_revision = $form['#user_revision'];
  $form_state['redirect'] = 'user/' . $user_revision->uid . '/revisions';
}
