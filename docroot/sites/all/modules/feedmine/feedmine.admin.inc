<?php

/**
 * @file
 * Administration page callbacks for the Feedmine module.
 */


/**
 * Menu callback with instructions for Feedmine settings overview.
 */
function feedmine_admin_overview(){
  $output = t("Complete the following steps to configure Feedmine:<br/>");
  $output .= t("</br><b>Redmine:</b>");
  $output .= t("<UL>");
  $output .= t("<li>Confirm your version of Redmine is newer than <code>1.1.0</code> by visiting <code>/admin/info</code></li>");
  $output .= t("<li>Check <code>Enable REST API</code> by going to <code>Administration -> Settings -> Authentication</code></li>");
  $output .= t("<li>Copy your API access key located on your Redmine account page at <code>/my/account</code>.  While logged into Redmine, it's on the right-hand pane of the default layout.  Click <code>Show</code> under <code>API access key</code>.</li>");
  $output .= t('</UL>');
  $output .= t('<br><b>Feedmine:</b>');
  $output .= t('<UL>');
  $output .= t('<li>Define !apisettings.</li>', array('!apisettings' => l('Redmine API settings', 'admin/config/feedmine/feedmine_settings/rmapi')));
  $output .= t('<li>Select a !project to post feedback issues to.</li>', array('!project' => l('Redmine project', 'admin/config/feedmine/feedmine_settings/rmproject')));
  $output .= t('<li>Define !permissions.</li>', array('!permissions' => l('Feedmine module permissions', 'admin/people/permissions', array('fragment' => 'module-feedmine'))));
  return $output;
}

/**
 * Form builder to configure Feedmine module.
 *
 * @return
 * Array containing renderable form for Feedmine Redmine connection
 * settings page.
 */
function feedmine_admin_rmapi_config($form, &$form_state) {
  $form['feedmine_rmurl'] = array(
    '#type' => 'textfield',
    '#title' => t('Redmine URL'),
    '#description' => t('The complete URL for Redmine.'),
    '#default_value' => variable_get('feedmine_rmurl', 'http://example.redmine.com'),
    '#required' => TRUE,
  );
  $form['feedmine_rmapikey'] = array(
    '#type' => 'textfield',
    '#title' => t('Redmine API access key'),
    '#description' => t('Redmine API access key for a authorized user.  Located under \'My account\' in your Redmine installation.'),
    '#default_value' => variable_get('feedmine_rmapikey', 'Paste your API access key here.'),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Next',
  );
  return $form;
}

/**
 * Validate user input from feedmine_admin_rmapi_config.
 */
function feedmine_admin_rmapi_config_validate($form, &$form_state) {
  $feedmine_rmurl    = $form_state['values']['feedmine_rmurl'];
  $feedmine_rmapikey = $form_state['values']['feedmine_rmapikey'];

  if (!filter_var($feedmine_rmurl, FILTER_VALIDATE_URL)) {
    form_set_error('feedmine_rmurl', t('Please enter a valid URL. i.e. http://example.redmine.com'));
  };

  if (strlen($feedmine_rmapikey) < 40) {
    form_set_error('feedmine_rmapikey', t('Please enter a valid Redmine API access key. (Min. 40 char.)'));
  };
}

/**
 * Form submit handler for feedmine_admin_rmapi_config.
 */
function feedmine_admin_rmapi_config_submit($form, &$form_state){
  variable_set('feedmine_rmurl', $form_state['values']['feedmine_rmurl']);
  variable_set('feedmine_rmapikey', $form_state['values']['feedmine_rmapikey']);
  $form_state['redirect'] = 'admin/config/feedmine/feedmine_settings/rmproject';
}

/**
 * Form builder to select Redmine project.
 *
 * @return
 * Array containing renderable form for selecting a Redmine project to
 * use with Feedmine.
 */
function feedmine_rmproject_select($form, &$form_state){
  // Verify Redmine API settings have been configured.
  $rmkey_check = variable_get('feedmine_rmapikey', NULL);
  if(!isset($rmkey_check)){
    $form['notice'] = array(
      '#type' => 'item',
      '#title' => 'NOTICE',
      '#markup' => t('Configure your !apisettings first.', array('!apisettings' => l('Redmine API settings', 'admin/config/feedmine/feedmine_settings/rmapi'))),
    );
    return $form;
  }
  else {
    // Retrieve and set the Redmine UID to set as default asignee.
    $rmuid = feedmine_rmuid();
    // Verify a UID exists
    if(isset($rmuid)){
      variable_set('feedmine_rmuid', $rmuid);
    }
    else{
      // Notify user before proceeding.
      drupal_set_message(t('Unable to assign a Redmine UID.'), $type='warning');
    }
    // Retrieve list of available projects from Redmine.
    $projects = feedmine_getrmprojects();
  }

  // Verify there are projects to select from.
  if(!isset($projects)){
    $msg = 'Suggestions:<ul><li>Verify your !apisettings.</li><li>Check the !recentlogs for additional details.</li></ul>';
    $args = array('!apisettings' => l('Redmine API settings', 'admin/config/feedmine/feedmine_settings/rmapi'), '!recentlogs' => l('recent log entries', 'admin/reports/dblog'));
    $form['notice'] = array(
      '#type' => 'item',
      '#title' => t('Unable to retireve a list of projects from Redmine:'),
      '#markup' => t($msg, $args),
    );
    return $form;
  }
  else{
    // Return a project selection form.
    $form['feedmine_rmprojectid'] = array(
      '#type' => 'radios',
      '#title' => t('Select a Redmine project to post feedback issues.'),
      '#options' => $projects,
      '#required' => TRUE,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Finish',
    );
    return $form;
  }
}

/**
 * Form submit handler for feedmine_rmproject_select
 */
function feedmine_rmproject_select_submit($form, &$form_state){
  variable_set('feedmine_rmprojectid', $form_state['values']['feedmine_rmprojectid']);
  $form_state['redirect'] = 'node';
}
