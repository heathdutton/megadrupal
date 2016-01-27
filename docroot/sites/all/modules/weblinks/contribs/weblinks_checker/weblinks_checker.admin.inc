<?php

/**
 * @file
 * Settings form.
 */

/**
 * Validity check settings.
 */
function weblinks_checker_settings() {
  drupal_add_js(drupal_get_path('module', 'weblinks_checker') . '/weblinks_checker.js');
  drupal_add_css(drupal_get_path('module', 'weblinks') . '/weblinks.admin.css');

  $form = array('#attributes' => array('class' => array('weblinks-settings')));
  $yesno = array(1 => t('Yes'), 0 => t('No'));

  include(drupal_get_path('module', 'weblinks') . '/weblinks_errors.inc');

  $form['weblinks_allow_dupes'] = array(
    '#type' => 'radios',
    '#title' => t('Allow duplicate URLs'),
    '#default_value' => variable_get('weblinks_allow_dupes', 0),
    '#options' => array(1 => t('Yes'), 0 => t('No'), 2 => t('Warn')),
    '#description' => t('This check is performed at link creation time. Most users will not want to allow duplicate URLs.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['weblinks_validate_check'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Check if link is valid when entered'),
    '#default_value' => (int) variable_get('weblinks_validate_check', FALSE),
    '#description' => t('If enabled, the module will attempt to use the URL to contact the site when the link is created or updated. If the link returns any error, the add/update will fail.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['weblinks_checker_enabled'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Enable link validity checker via Cron'),
    '#description' => t('Do you want to check the links regularly during cron runs?'),
    '#default_value' => (int) variable_get('weblinks_checker_enabled', FALSE),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div class="weblinks_checker_hide">',
    );

  $form['basic']['weblinks_checker_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Links to include'),
    '#description' => t('This sets the number of links to be checked on each Cron run. The maximum value is 150 but Cron may time out if the workload is high. You may wish to experiment with different numbers as this will vary between sites and hosts.'),
    '#size' => 5,
    '#maxlength' => 3,
    '#default_value' => variable_get('weblinks_checker_limit', 5),
    );

    $count = db_query("SELECT COUNT(nid) FROM {node} WHERE type='weblinks'")->fetchField();
    $form['basic']['weblinks_checker_limit']['#description'] .= ' ' . t('There are currently !count links defined.', array('!count' => $count));

  $form['basic']['weblinks_checker_show_status'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t('Show error status'),
    '#description' => t('If this is selected and the link is returning an error, the status will be displayed with the link.'),
    '#default_value' => (int) variable_get('weblinks_checker_show_status', TRUE),
    );

  $form['basic']['weblinks_checker_detail_log'] = array(
    '#type' => 'radios',
    '#options' => array(
      1 => t('Every URL that was checked'),
      2 => t('Only if the http status has changed'),
      0 => t('None'),
      ),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t('Show URLs checked'),
    '#description' => t('This option controls what messages are written to the dblog for each URL that is tested. The log will always show links which get published or unpublished during the run.'),
    '#default_value' => variable_get('weblinks_checker_detail_log', 0),
    );

  $form['basic']['weblinks_checker_summary_msg'] = array(
    '#type' => 'radios',
    '#title' => t('Summary log message'),
    '#options' => array(
      1 => t('Short'),
      2 => t('Long'),
      0 => t('None'),
      ),
    '#description' => t('The short message just says how many links were checked. The long message also gives the link titles.'),
    '#default_value' => variable_get('weblinks_checker_summary_msg', 0),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div><div class="clear-block"></div>',
    );

  $mins = 60;
  $hour = 3600;
  $day = 86400;
  $interval = array(0, 15 * $mins, 30 * $mins, 60 * $mins, 2 * $hour, 3 * $hour, 4 * $hour, 6 * $hour, 12 * $hour, 1 * $day, 2 * $day, 3 * $day, 7 * $day, 14 * $day, 28 * $day);
  $form['basic']['weblinks_checker_interval'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc($interval, 'format_interval'),
    '#title' => t('Cron interval'),
    '#description' => t("This sets the frequency of checking. If you run Cron frequently, you may wish to set this higher than your Cron frequency so that you don't overload the linked site. The check will be run at the next scheduled Cron run after this interval expires. An interval of 0 means every cron run."),
    '#size' => count($interval) / 2,
    '#default_value' => variable_get('weblinks_checker_interval', 24 * $hour),
    );

  $form['weblinks_error_handling'] = array(
    '#type' => 'fieldset',
    '#title' => t('Handling links that have errors'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    );

  $form['weblinks_error_handling']['weblinks_checker_redirect'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Update link on permanent redirect?'),
    '#description' => t('If a link is marked as a permanent redirect ("301"), do you want the link url updated?'),
    '#default_value' => (int) variable_get('weblinks_checker_redirect', FALSE),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['weblinks_error_handling']['weblinks_checker_order'] = array(
    '#type' => 'radios',
    '#title' => t('Checking sequence'),
    '#options' => array(
      0 => t('Check links in strict rotation'),
      1 => t('Give priority to links in error'),
      ),
    '#description' => t('Using strict rotation will make sure each link is checked in turn before any are re-checked. Giving priority to links which are in error ensures that their status is monitored closely so that they can be re-published as soon as they return a good status.'),
    '#default_value' => variable_get('weblinks_checker_order', 0),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['weblinks_error_handling']['weblinks_checker_unpublish'] = array(
    '#type' => 'radios',
    '#title' => t('Unpublish bad links'),
    '#options' => array(
      1 => t('Unpublish after N cron runs'),
      2 => t('Unpublish after N days'),
      0 => t('Do not unpublish'),
      ),
    '#description' => t('Weblinks can be unpublished after being in the same error state for a number of consecutive cron runs or days.'),
    '#default_value' => variable_get('weblinks_checker_unpublish', 0),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['weblinks_error_handling']['weblinks_checker_unpublish_after'] = array(
    '#type' => 'textfield',
    '#title' => t('Unpublish after N'),
    '#description' => t('This is the number of the days or cron runs on which a link in error will be unpublished. For example, enter 2 to unpublish a bad link on the second cron run or the day after it is discovered. Use 1 to unpublish a bad link immediately, i.e. on the first run or first day that it is found to be in error. The maximum value is 99.'),
    '#size' => 5,
    '#maxlength' => 2,
    '#default_value' => variable_get('weblinks_checker_unpublish_after', 2),
    '#prefix' => '<div class="weblinks_checker_unpublish_hide">',
    );

  $form['weblinks_error_handling']['weblinks_checker_action_on_unpublished'] = array(
    '#type' => 'radios',
    '#title' => t('Action on unpublished links'),
    '#options' => array(
      1 => t('Include in checking'),
      2 => t('Include and re-publish if OK'),
      0 => t('Exclude from checking'),
      ),
    '#description' => t('Do you want to re-check and get an up-to-date status for links which have been unpublished because they were in error? These links can also be re-published if they are now reporting a good status code.'),
    '#default_value' => variable_get('weblinks_checker_action_on_unpublished', 0),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => "</div>",
    );

  $form['weblinks_error_handling']['weblinks_checker_new_revision'] = array(
    '#type' => 'radios',
    '#title' => t('Create new revision on change of status'),
    '#options' => $yesno,
    '#description' => t('When the link status changes for good to bad, or vice-versa, create a new revision so that admin users can track the change.'),
    '#default_value' => variable_get('weblinks_checker_new_revision', 0),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => "</div></div><!-- /weblinks_checker_unpublish_hide -->",
    );

  // Fieldset for defining which status values are considered to be OK.

  $ignore_deflt = variable_get('weblinks_checker_ignore', array());
  // If no previously saved value then set defaults.
  if (!$ignore_deflt) {
    $ignore_deflt = weblinks_error_codes();
    foreach ($ignore_deflt as $code => $text) {
      // All codes in the 100s and 200s are OK, as is 302.
      if (substr($code, 0, 1) == '1' || substr($code, 0, 1) == '2' || $code == '302') {
        $ignore_deflt[$code] = $code;
      }
      else {
        $ignore_deflt[$code] = FALSE;
      }
    }
  }

  $form['ignore'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ignore error codes'),
    '#description' => t('The validity checker will treat all the selected codes as OK. For more information, see <a href="!url" target="_blank">this page</a>.',
      array('!url' => url('http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html', array('external' => TRUE)))),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    );

  // Add a separate checkbox for code 0, which is (mainly) when you are not
  // connected to the internet. It has to be done separately from the rest of
  // the codes because the key must be something other than 0. If it were added
  // to the normal array then testing $ignore['0'] would not give any useful
  // result because it would always be false.
  $form['ignore']['weblinks_checker_ignore_zero'] = array(
    '#type' => 'checkboxes',
    '#options' => array('1' => t('0 - Not Connected')),
    '#default_value' => variable_get('weblinks_checker_ignore_zero', array()),
    '#prefix' => '<div class="weblinks-checkboxes">',
    '#suffix' => '</div>',
    );

  $form['ignore']['weblinks_checker_ignore'] = array(
    '#type' => 'checkboxes',
    '#options' => weblinks_error_codes(),
    '#default_value' => $ignore_deflt,
    '#prefix' => '<div class="weblinks-checkboxes">',
    '#suffix' => '</div></div><!-- /weblinks_checker_hide --><br />',
    );

  // The following are for user deletion.
  $form['rescue'] = array(
    '#type' => 'fieldset',
    '#title' => t("User Deletion Action"),
    '#description' => t('This section determines what you want to do with links submitted by a user who is subsequently deleted.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    );

  $result = db_query("SELECT uid, name FROM {users}");
  $rescue_users = array();
  foreach ($result as $row) {
    $rescue_users[$row->uid] = $row->name;
  }
  $rescue_users[0] = variable_get('anonymous', t('Anonymous'));

  $move_to = variable_get('weblinks_rescue_move', 1);
  if (!user_load($move_to)) {
    // The specified user no longer exists, so set to user 1.
    $move_to = 1;
  }
  $form['rescue']['weblinks_rescue_move'] = array(
    '#type' => 'select',
    '#options' => $rescue_users,
    '#title' => t('Move to user'),
    '#description' => t('Select the user to whom the links will be moved.') . ' '
       . t('This choice overrides the "re-assign to %anonymous" system option.', array('%anonymous' => $rescue_users[0])),
    '#default_value' => $move_to,
    '#prefix' => '<div class="weblinks-move-user-list">',
    '#suffix' => '</div>',
    );

  $form['rescue']['weblinks_rescue_unpub'] = array(
    '#type' => 'radios',
    '#options' => array(0 => t('Unpublished'), 1 => t('Published')),
    '#title' => t('Status after the move'),
    '#description' => t('When moving the link to the other user, should the link be published or unpublished?'),
    '#default_value' => (int) variable_get('weblinks_rescue_unpub', 0),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['#submit'][] = 'weblinks_checker_settings_submit';

  return system_settings_form($form);
}

/**
 * Validate the settings.
 */
function weblinks_checker_settings_validate($form, &$form_state) {
  // Check that the number of links to include is in the range 1 to 50.
  $limit = $form_state['values']['weblinks_checker_limit'];
  if (($limit < 1 || $limit > 150) && $form_state['values']['weblinks_checker_enabled'] <> 0) {
    form_set_error('weblinks_checker_limit', t("You entered an invalid value for 'links to include'. Please use a number in the range 1 to 150."));
  }

  // If unpublishing links in error, check that the value of N days or N crons
  // is in the range 1 to 99.
  $unpublish_after = $form_state['values']['weblinks_checker_unpublish_after'];
  if ($form_state['values']['weblinks_checker_unpublish'] <> 0 && ($unpublish_after < 1 || $unpublish_after > 99)) {
    form_set_error('weblinks_checker_unpublish_after', t("You entered an invalid value for 'unpublish after'. Please use a number in the range 1 to 99."));
  }

  if (form_get_errors()) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Submit handler for Weblinks Checker Settings form.
 */
function weblinks_checker_settings_submit($form, &$form_state) {
  // If the choice of 'N days' or 'N crons' has been changed, reset the existing
  // last_status_info values to zero where a value has been set, as they would
  // now give wrong results.
  $unpublish = $form['weblinks_error_handling']['weblinks_checker_unpublish'];
  if ($unpublish['#value'] != $unpublish['#default_value']) {
    // Create a subquery of the current version ids for all weblink nodes.
    $vids = db_select('node')
      ->fields('node', array('vid'))
      ->condition('type', 'weblinks');
    // Update the 'weblinks' table
    $num_updated = db_update('weblinks')
      ->fields(array('last_status_info' => '0'))
      ->condition ('last_status_info', 0, '>')
      ->condition ('vid', $vids, 'IN')->execute();
    drupal_set_message(t('The stored information on when links in error changed status has been cleared because you altered the criteria for unpublishing.'));
    drupal_set_message(format_plural($num_updated, '1 row updated in the Weblinks table.', '@count rows updated in the Weblinks table.'));
  }
}