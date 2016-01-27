<?php
// $Id$

/**
 * @file
 * User page callbacks for the oauth module.
 */

/**
* Menu callback; Retrieve a JSON object containing autocomplete suggestions for existing identities.
*/
function oauth_autocomplete($oid, $string = '') {
  $matches = array();
  if ($string) {
    $result = db_select('oauthmap')
      ->fields('oauthmap', array('authname'))
      ->condition('oid', $oid)
      ->condition('authname', db_like($string) . '%', 'LIKE')
      ->range(0, 10)
      ->execute();
    foreach ($result as $identity) {
      $matches[$identity->authname] = check_plain($identity->authname);
    }
  }

  drupal_json_output($matches);
}

/**
 * Returns an application submission form.
 */
function oauth_add($type) {
  $types = oauth_type_get_types();
  $application = (object) array('type' => $type, 'language' => LANGUAGE_NONE);
  drupal_set_title(t('Create @name application', array('@name' => $types[$type]->name)), PASS_THROUGH);

  return drupal_get_form($type . '_oauth_form', $application);
}

/**
 * Menu callback; Ask for confirmation of application deletion
 */
function oauth_delete_confirm($form, &$form_state, $application) {
  $form_state['application'] = $application;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['oid'] = array('#type' => 'value', '#value' => $application->oid);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $application->title)),
    'node/' . $application->oid,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute application deletion
 */
function oauth_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $application = oauth_load($form_state['values']['oid']);
    oauth_delete($form_state['values']['oid']);
    watchdog('oauth', '@type: deleted %title.', array('@type' => $application->type, '%title' => $application->title));
    drupal_set_message(t('@type %title has been deleted.', array('@type' => oauth_type_get_name($application), '%title' => $application->title)));
  }

  $form_state['redirect'] = 'admin/config/' . $application->type;
}

/**
* Menu callback; presents the application editing form.
*/
function oauth_page_edit($oauth) {
  drupal_set_title(t('<em>Edit application</em> @title', array('@title' => $oauth->title)), PASS_THROUGH);
  return drupal_get_form($oauth->type . '_oauth_form', $oauth);
}

/**
 * Menu callback; view a single application.
 */
function oauth_page_view($oauth) {
  // If there is a menu link to this application, the link becomes the last part
  // of the active trail, and the link name becomes the page title.
  // Thus, we must explicitly set the page title to be the application title.
  drupal_set_title($oauth->title);
  $uri = entity_uri('oauth', $oauth);
  // Set the node path as the canonical URL to prevent duplicate content.
  drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($uri['path'], $uri['options'])), TRUE);
  // Set the non-aliased path as a default shortlink.
  drupal_add_html_head_link(array('rel' => 'shortlink', 'href' => url($uri['path'], array_merge($uri['options'], array('alias' => TRUE)))), TRUE);
  return oauth_show($oauth);
}

/**
 * Generate the application add/edit form array.
 */
function oauth_form($form, &$form_state, $application) {
  // During initial form build, add the node entity to the form state for use
  // during form building and processing. During a rebuild, use what is in the
  // form state.
  if (!isset($form_state['application'])) {
    if (!isset($application->title)) {
      $application->title = NULL;
    }
    $form_state['application'] = $application;
  }
  else {
    $application = $form_state['application'];
  }

  $form['title'] = array(
    '#title' => t('Application name'),
    '#type' => 'textfield',
    '#default_value' => $application->title,
    '#required' => TRUE,
    '#description' => t('The application title is only displayed for administrative purpose.')
  );

  // Basic application information.
  // These elements are just values so they are not even sent to the client.
  foreach (array('oid', 'type', 'language') as $key) {
    $form[$key] = array(
      '#type' => 'value',
      '#value' => isset($application->$key) ? $application->$key : NULL,
    );
  }

  // Add the buttons.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('oauth_form_submit'),
  );
  if (!empty($application->oid)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('oauth_form_delete_submit'),
    );
  }

  field_attach_form('oauth', $application, $form, $form_state, $application->language);
  return $form;
}

function oauth_form_validate($form, &$form_state) {
  // TODO.
  entity_form_field_validate('oauth', $form, $form_state);
}

function oauth_form_submit($form, &$form_state) {
  $application = $form_state['application'];
  entity_form_submit_build_entity('oauth', $application, $form, $form_state);
  $insert = empty($application->oid);
  oauth_save($application);
  $application_link = l(t('view'), 'oauth/' . $application->oid);
  $watchdog_args = array('@type' => $application->type, '%title' => $application->title);
  $t_args = array('@type' => oauth_type_get_name($application), '%title' => $application->title);

  if ($insert) {
    watchdog('application', '@type: added %title.', $watchdog_args, WATCHDOG_NOTICE, $application_link);
    drupal_set_message(t('@type application <em>%title</em> has been created.', $t_args));
  }
  else {
    watchdog('application', '@type: updated %title.', $watchdog_args, WATCHDOG_NOTICE, $application_link);
    drupal_set_message(t('@type application <em>%title</em> has been updated.', $t_args));
  }
  if ($application->oid) {
    $form_state['values']['oid'] = $application->oid;
    $form_state['oid'] = $application->oid;
    $form_state['redirect'] = 'oauth/' . $application->oid;
  }
  else {
    // In the unlikely case something went wrong on save, the application will be
    // rebuilt and oauth form redisplayed the same way as in preview.
    drupal_set_message(t('The application could not be saved.'), 'error');
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Button submit function: handle the 'Delete' button on the oauth form.
 */
function oauth_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $application = $form_state['application'];
  $form_state['redirect'] = array('oauth/' . $application->oid . '/delete', array('query' => $destination));
}

/**
 * Menu callback; Manage OAuth based identities for the specified user.
 */
function oauth_user_identities($account) {
  $header = array(t('Application'), t('External identity'), t('Operations'));
  $rows = array();

  $result = db_select('oauthmap')
    ->fields('oauthmap')
    ->condition('uid', $account->uid)
    ->execute()
    ->fetchAllAssoc('oid');

  $entities = entity_load('oauth');

  $types = oauth_type_get_types();

  foreach ($entities as $key => $oauth) {
    if (isset($result[$key])) {
      $identity = $result[$key];
      $operations = l(t('Delete'), "user/$account->uid/$key/delete/" . $identity->aid);
    }
    else {
      $identity->authname = $oauth->title;
      $operations = l(t('Add'), "oauth/$key/authenticate");
    }
    // TODO: Logo (Twitter, Facebook...).
    $rows[] = array($oauth->title, check_plain($identity->authname), $operations);
  }

  $build['oauth_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );
  return $build;
}

/**
 * Generate an array which displays an application detail page.
 *
 * @param $oauth
 *   An oauth application object.
 * @return
 *   A $page element suitable for use by drupal_page_render().
 */
function oauth_show($oauth) {
  return oauth_view_multiple(array($oauth->oid => $oauth), 'full');
}

/**
 * Construct a drupal_render() style array from an array of loaded applications.
 *
 * @param $oauths
 *   An array of applications as returned by oauth_load_multiple().
 * @param $view_mode
 *   View mode, e.g. 'full', 'teaser'...
 * @param $weight
 *   An integer representing the weight of the first application in the list.
 * @param $langcode
 *   (optional) A language code to use for rendering. Defaults to the global
 *   content language of the current request.
 *
 * @return
 *   An array in the format expected by drupal_render().
 */
function oauth_view_multiple($applications, $view_mode = 'teaser', $weight = 0, $langcode = NULL) {
  field_attach_prepare_view('oauth', $applications, $view_mode, $langcode);
  entity_prepare_view('oauth', $applications, $langcode);
  $build = array();
  foreach ($applications as $oauth) {
    $build['applications'][$oauth->oid] = oauth_view($oauth, $view_mode, $langcode);
    $build['applications'][$oauth->oid]['#weight'] = $weight;
    $weight++;
  }
  $build['applications']['#sorted'] = TRUE;
  return $build;
}

/**
 * Generate an array for rendering the given application.
 *
 * @param $oauth
 *   An application object.
 * @param $view_mode
 *   View mode, e.g. 'full', 'teaser'...
 * @param $langcode
 *   (optional) A language code to use for rendering. Defaults to the global
 *   content language of the current request.
 *
 * @return
 *   An array as expected by drupal_render().
 */
function oauth_view($oauth, $view_mode = 'full', $langcode = NULL) {
  if (!isset($langcode)) {
    $langcode = $GLOBALS['language_content']->language;
  }

  // Populate $application->content with a render() array.
  oauth_build_content($oauth, $view_mode, $langcode);

  $build = $oauth->content;
  // We don't need duplicate rendering info in application->content.
  unset($oauth->content);

  $build += array(
    '#theme' => 'oauth',
    '#oauth' => $oauth,
    '#view_mode' => $view_mode,
    '#language' => $langcode,
  );

  // Allow modules to modify the structured node.
  $type = 'oauth';
  drupal_alter(array('oauth_view', 'entity_view'), $build, $type);

  return $build;
}

function oauth_build_content($oauth, $view_mode = 'full', $langcode = NULL) {
  if (!isset($langcode)) {
    $langcode = $GLOBALS['language_content']->language;
  }

  // Remove previously built content, if exists.
  $oauth->content = array();

  // Build fields content.
  field_attach_prepare_view('oauth', array($oauth->oid => $oauth), $view_mode, $langcode);
  entity_prepare_view('oauth', array($oauth->oid => $oauth), $langcode);
  $oauth->content += field_attach_view('oauth', $oauth, $view_mode, $langcode);

  module_invoke_all('entity_view', $oauth, 'oauth', $view_mode, $langcode);
}

/**
 * Process variables for oauth.tpl.php
 */
function template_preprocess_oauth(&$variables) {
  $variables['oauth'] = $variables['elements']['#oauth'];
  $oauth = $variables['oauth'];

  $uri = entity_uri('oauth', $oauth);
  $variables['oauth_url']       = url($uri['path'], $uri['options']);
  $variables['title']           = check_plain($oauth->title);

  // Flatten the application object's member fields.
  $variables = array_merge((array) $oauth, $variables);

  // Helpful $content variable for templates.
  $variables += array('content' => array());
  foreach (element_children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }

  // Make the field variables available with the appropriate language.
  field_attach_preprocess('oauth', $oauth, $variables['content'], $variables);

  // Gather application classes.
  $variables['classes_array'][] = drupal_html_class('oauth-' . $oauth->type);

  // Clean up name so there are no underscores.
  $variables['theme_hook_suggestions'][] = 'oauth__' . $oauth->type;
  $variables['theme_hook_suggestions'][] = 'oauth__' . $oauth->oid;
}

/**
 * Menu callback; Delete the specified OAuth based identity from the system.
 */
function oauth_user_delete_form($form, $form_state, $account, $oauth, $aid = 0) {
  $authname = db_query("SELECT authname FROM {oauthmap} WHERE aid = :aid", array(
    ':aid' => $aid,
  ))
  ->fetchField();

  return confirm_form(array(), t('Are you sure you want to delete the %title account for %user?', array('%title' => $oauth->title, '%user' => $account->name)), 'user/' . $account->uid . '/identities');
}

// TODO: maybe use a oauth_user_delete function like oauth_user_add
// unified API to delete user oauth related data.
function oauth_user_delete_form_submit($form, &$form_state) {
  $user = $form_state['build_info']['args'][0];
  $oauth = $form_state['build_info']['args'][1];
  $aid = $form_state['build_info']['args'][2];

  $query = db_delete('oauthmap')
    ->condition('aid', $aid)
    ->execute();

  db_delete(oauth_token_table_get_name($oauth))
    ->condition('aid', $aid)
    ->execute();

  if ($query) {
    drupal_set_message(t('@application account deleted.', array('@application' => $oauth->title)));
  }
  $form_state['redirect'] = 'user/' . $user->uid . '/identities';
}