<?php
/**
 * @file
 * This file contains functions pertaining to administration pages for module namecards
 */

/**
 * Define administration settings form
 */
function namecards_admin_form($form, &$form_state) {
  $form['#title'] = 'Namecards module settings';
  $form['#description'] = t('General setting for the Namecards module');
  $form['namecards_privacy_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Privacy settings'),
    '#description' => t('This option determines whether a user\'s private contacts are viewable by system administrators.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['namecards_privacy_settings']['namecards_privacy_settings_options'] = array(
    '#type' => 'radios',
    '#title' => t('Users\' private contacts are viewable by namecards administrators (i.e. Users with permissions set to \'administer namecards\')'),
    '#options' => array(
      NAMECARDS_ADMIN_VIEW_PRIV_CONTACTS_DISABLED => t('No, private contacts are <em>not</em> viewable by administrators.'),
      NAMECARDS_ADMIN_VIEW_PRIV_CONTACTS_ENABLED => t('Yes, private contacts are viewable by administrators.'),
    ),
    '#default_value' => variable_get('namecards_privacy_settings_options', 0),
  );
  $form['namecards_user_delete_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Action to take when deleting a user'),
    '#description' => t('This option determines what to do with a user\'s contacts after the user is deleted.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['namecards_user_delete_settings']['namecards_delete_user_contacts_options'] = array(
    '#type' => 'radios',
    '#title' => t('On user delete'),
    '#options' => array(
      t('Delete all contacts belonging to the user.'),
      t('Delete user\'s private contacts, but preserve public contacts. Remaining public contacts can only be edited or deleted by users with permissions set to \'administer namecards\'.'),
    ),
    '#default_value' => variable_get('namecards_delete_user_contacts_options', 0),
  );
  $form['namecards_scan_for_orphaned_nodes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Scan for orphaned nodes'),
    '#description' => t('This function should be run if organizations or events appear which do not contain any contacts (i.e. they have become orphaned).'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['namecards_scan_for_orphaned_nodes']['scan'] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="namecards-scan-orphan-nodes-button">',
    '#value' => t('Scan'),
    '#suffix' => '</div>',
    '#submit' => array('namecards_scan_for_orphan_nodes_submit'),
    '#ajax' => array(
      'callback' => 'namecards_scan_for_orphan_nodes_ajax',
      'wrapper' => 'namecards-scan-orphan-nodes-results',
      'method' => 'replace',
      'progress' => array(
        'type' => 'throbber',
        'message' => t('Scanning...'),
      ),
    ),
  );
  $form['namecards_scan_for_orphaned_nodes']['scan_results'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="namecards-scan-orphan-nodes-results">',
    '#markup' => (!empty($form_state['values']['scan'])) ? _namecards_scan_for_orphan_nodes() : '',
    '#suffix' => '</div>',
  );
  $form['advanced_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced_options']['namecards_caching_settings_options'] = array(
    '#type' => 'radios',
    '#title' => t('Caching'),
    '#description' => t('Increases performance by enabling caching of data.  Caching is disabled by default. Enabling caching will decrease page loading times, but comes at the cost of high resource utilization (e.g. database storage space - as must save copy of each page for each user). Options include: <em>Disabled</em>: caching is turned off (recommended); <em>Moderate</em>: caching applied to data in \'Browse\' tab (moderately resource intensive); <em>Intensive</em>: caching applied to \'Browse\', \'Organizations\', and \'Events\' tabs (very high resource comsumption. Not recommended for use on servers with limited storage capacity or large number of users.'),
    '#options' => array(
      NAMECARDS_CACHING_DISABLED => t('Disabled (default)'),
      NAMECARDS_CACHING_MODERATE => t('Moderate'),
      NAMECARDS_CACHING_INTENSIVE => t('Intensive (Use with caution)'),
//      t('Disabled (default)'),
//      t('Moderate'),
//      t('Intensive (Use with caution)'),
    ),
    '#default_value' => variable_get('namecards_caching_settings_options', 0),
  );
  $form['advanced_options']['namecards_compression_settings_options'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable compression of cached data'),
    '#description' => t('Decreases amount of storage space taken up by cached data on server. Enabling this feature comes with a slight performance penalty as the system must perform compression/decompression of data.'),
    '#default_value' => variable_get('namecards_compression_settings_options', 0),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#submit' => array('namecards_admin_form_submit'),
  );
  return $form;
}

/**
 * Default submit function for administration settings form
 */
function namecards_admin_form_submit($form, &$form_state) {
  variable_set('namecards_delete_user_contacts_options', $form_state['values']['namecards_delete_user_contacts_options']);
  variable_set('namecards_privacy_settings_options', $form_state['values']['namecards_privacy_settings_options']);
  variable_set('namecards_caching_settings_options', $form_state['values']['namecards_caching_settings_options']);
  variable_set('namecards_compression_settings_options', $form_state['values']['namecards_compression_settings_options']);

  // Always clear cache on saving form settings.
  // TODO: Ideally might want to add some complex logic to determine
  // if and what cached data should be cleared based on changes in
  // these settings.  Trick is with the compressed data.  Need an
  // easy way to track which data is compressed and which isn't and
  // then delete from cache based on new compression settings. Could
  // add a new DB table to do this, but this would mean having to
  // add additional database queries when loading cached data, which
  // I want to avoid at all costs.  Retrieving cached data should be
  // as quick as possibe.  For the time being we keep it simple, by
  // clearing the whole cache.
  _namecards_delete_cached_data();
}

/**
 * Fallback submit function for scan for orphaned nodes button if javascript is disabled.
 *
 */
function namecards_scan_for_orphan_nodes_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax function for scan for orphaned nodes button
 *
 * @return
 *   String of HTML formatted text.
 */
function namecards_scan_for_orphan_nodes_ajax($form, $form_state) {
  return $form['namecards_scan_for_orphaned_nodes']['scan_results'];
}

