<?php
/**
 * @file
 * Code for the node/N/promote tab on project nodes.
 */

/**
 * Build the form for the node/N/promote tab on project nodes.
 *
 * @param $form_state
 *   The Form API state of the form (set by drupal_get_form()).
 * @param $project
 *   The fully-loaded node object for the project for which to build the
 *   promotion form.
 *
 * @return
 *   A form definition array for use by the Form API.
 *
 * @see project_promote_form_validate()
 * @see project_promote_form_submit()
 * @see drupal_get_form()
 */
function project_promote_project_form($form, $form_state, $project) {
  $form = array();
  $form['help'] = array(
    '#type' => 'markup',
    '#value' => t('You are about to promote the project %project_name to a full project. This action is not reversible.', array('%project_name' => $project->title)),
  );

  $form['confirm'] = array(
    '#type' => 'checkbox',
    '#title' => t('I understand that this action cannot be undone and wish to proceed anyway'),
    '#default_value' => FALSE,
    '#description' => t('Please confirm that you understand the implications of promoting this project.')
  );

  // Only show the shortname field if it's auto-generated for sandbox projects.
  if (variable_get('project_sandbox_numeric_shortname', FALSE)) {
    // Unset this in case the user cancels on the confirm form so we don't leak
    // this into the $_SESSION permanently. Also ensures we start clean.
    // Note that we append the nid to the variable name in case a user is
    // promoting multiple projects simultaneously (yes, it happens).
    unset($_SESSION['project_promote_project_shortname_' . $project->nid]);
    $form['shortname'] = array(
      '#type' => 'textfield',
      '#title' => t('Short project name'),
      '#default_value' => isset($project->field_project_machine_name[LANGUAGE_NONE][0]['value']) ? $project->field_project_machine_name[LANGUAGE_NONE][0]['value'] : NULL,
      '#size' => 40,
      '#maxlength' => 50,
      '#required' => TRUE,
      '#element_validate' => array('project_validate_project_shortname'),
      '#entity' => $project,
    );

    // Update help text if the user is not allowed to update the short name at
    // a later time.
    if (!variable_get('project_allow_machinename_update', TRUE)) {
      $form['shortname']['#description'] = t('You may not edit this value after the project has been promoted.');
    }
  }

  $form['pid'] = array('#type' => 'value', '#value' => $project->nid);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Promote to full project'),
    '#weight' => 45,
  );

  return $form;
}

/**
 * Validation handler for project promote form.
 */
function project_promote_project_form_validate($form, &$form_state) {
  if ($form_state['values']['confirm'] == 0) {
    form_set_error('confirm', t('Please check the confirmation checkbox before promoting this project'));
  }
}

/**
 * Submit handler for project promote form. Redirects to a confirm form.
 */
function project_promote_project_form_submit($form, &$form_state) {
  if (isset($form_state['values']['shortname'])) {
    // Note that we append the nid to the variable name in case a user is
    // promoting multiple projects simultaneously (yes, it happens).
    $session_var = 'project_promote_project_shortname_' . $form_state['values']['pid'];
    $_SESSION[$session_var] = $form_state['values']['shortname'];
  }

  $form_state['redirect'] = 'node/' . $form_state['values']['pid'] . '/edit/promote/confirm';
}

/**
 * Build a confirmation form for promoting sandbox projects to full projects.
 */
function project_promote_project_confirm_form($form, $form_state, $project) {
  if (empty($project)) {
    drupal_set_message(t('Error: you cannot confirm promoting a sandbox to a full project without specifying the project.'), 'error');
    return drupal_goto();
  }
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $project->nid,
  );

  if (isset($_SESSION['project_promote_project_shortname_' . $project->nid])) {
    $shortname = $_SESSION['project_promote_project_shortname_' . $project->nid];
    // Note: we can't unset this value from the $_SESSION here, or this form
    // element won't be included when building the form during submission. So,
    // we'll unset once we actually submit the confirm form.
  }
  else {
    $shortname = $project->field_project_machine_name[LANGUAGE_NONE][0]['value'];
  }
  $form['shortname'] = array(
    '#type' => 'value',
    '#value' => $shortname,
  );

  // If the shortname can't otherwise be changed, make sure they understand
  // this is *really* permanent once they press "Promote".
  if (!variable_get('project_allow_machinename_update', TRUE)) {
    $confirm_text = t('The project will use the shortname %shortname which can not be changed later. Furthermore, once a project is promoted, it can not be turned back into a sandbox.', array('%shortname' => $shortname));
  }
  else {
    $confirm_text = t('Once a project is promoted, it can not be turned back into a sandbox.');
  }

  return confirm_form(
    $form,
    t('Are you sure you want to promote %title from a sandbox to a full project ?', array('%title' => $project->title)),
    'node/' . $project->nid . '/edit/promote',
    $confirm_text, t('Promote'), t('Cancel')
  );
}

/**
 * Submit handler for project promote form.
 */
function project_promote_project_confirm_form_submit($form, &$form_state) {
  $project = node_load($form_state['values']['nid']);
  $project->field_project_type[LANGUAGE_NONE][0]['value'] = 'full' ;

  unset($_SESSION['project_promote_project_shortname_' . $project->nid]);

  // Only update the shortname if it is relevant, and included in the original
  // form.
  if (variable_get('project_sandbox_numeric_shortname', FALSE)) {
    $project->field_project_machine_name[LANGUAGE_NONE][0]['value'] = $form_state['values']['shortname'];
  }

  node_save($project);
  drupal_set_message(t('The project %project_name has been promoted to a full project.', array('%project_name' => $project->title)));
  $form_state['redirect'] = 'node/' . $project->nid;

  // Notify that this promotion has occurred.
  module_invoke_all('project_promote_sandbox', $project);
}
