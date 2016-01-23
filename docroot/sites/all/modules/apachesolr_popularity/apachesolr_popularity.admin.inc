<?php

/**
 * @file
 * Admin page callbacks for the Apache Solr Popularity module.
 *
 * @author Jonathan Gagne (jongagne)
 * @see drupal.org/user/2409764
 */

/**
 * Page callback: Displays popularity of the the top pages.
 *
 * This displays the pages with the highest popularity.
 * Note: only updates ones per cron run.
 */
function apachesolr_popularity_top_pages() {

  // Used for creating and ordering the table.
  $header = array(
    // Note, cannot order by Title.
    array('data' => t('Title')),
    array('data' => t('Ranking Multiplier'), 'field' => 'rankingmultiplier', 'sort' => 'desc'),
    array('data' => t('Popularity'), 'field' => 'popularity'),
    array('data' => t('Recent Count'), 'field' => 'recentcount', 'sort' => 'desc'),
    array('data' => t('Time Tracked'), 'field' => 'trackedtime'),
    array('data' => t('Tracking Flag'), 'field' => 'flag'),
  );

  // Set tracking flags. Aka nodes NOT to track.
  $flag = _apachesolr_popularity_get_tracking_flags();

  // Queries the popularity data from databases.
  $query = db_select('apachesolr_popularity_counter', 'p', array('target' => 'slave'))->extend('PagerDefault')->extend('TableSort');
  $query->fields('p', array('nid', 'popularity', 'rankingmultiplier', 'recentcount', 'trackedtime', 'flag'));
  $query
    ->groupBy('nid')
    ->where('flag & :flag = 0', array(':flag' => $flag))
    ->limit(30)
    ->orderByHeader($header)
    ->orderBy('rankingmultiplier', 'desc')
    ->orderBy('recentcount', 'desc');

  $result = $query->execute();
  $rows = array();
  // Builds table data.
  foreach ($result as $page) {
    $rows[] = array(
      _apachesolr_popularity_format_item($page->nid),
      round($page->rankingmultiplier, 2),
      round($page->popularity, 1),
      $page->recentcount,
      round($page->trackedtime, 4),
      $page->flag,
    );
  }

  $build['popularity_top_pages_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No popularity statistics available.'),
  );
  $build['popularity_top_pages_pager'] = array('#theme' => 'pager');
  return $build;
}


/**
 * Form constructor for the Apache Solr Popularity administration form.
 */
function apachesolr_popularity_settings_form() {

  // Gets Solr server details.
  $default_env_id = variable_get('apachesolr_default_environment', 'solr');
  $default_solr_env = db_query('SELECT * FROM {apachesolr_environment} WHERE env_id = :env_id', array(':env_id' => $default_env_id))->fetchObject();
  if (isset($default_solr_env->name) != '' AND isset($default_solr_env->url)) {
    //$solr_description = t("<i>Default") . " Solr " . t('server name: </i>') . $default_solr_env->name . t('<br /><i>URL: </i>') . $default_solr_env->url . '<br /><br />';
    $solr_description = t("<i>Default Solr server name:</i> @serv_name<br /><i>URL:</i> @url" , 
                          array('@serv_name' => $default_solr_env->name, '@url' => $default_solr_env->url));
  }
  else {
    //$solr_description = t('<strong>* Error locating default Solr server.') . '  ' . l(t('Check Apache Solr Settings'), 'admin/config/search/apachesolr/settings/') . ' *</strong><br /><br />';
    $solr_description = t('<strong>* Error locating default Solr server. @link  *</strong><br /><br />', 
                          array('@link' => l(t('Check Apache Solr Settings'), 'admin/config/search/apachesolr/settings/')));
  }

  // Basic Solr settings.
  $form['solr_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Solr Settings'),
    //'#description' => filter_xss($solr_description, array('strong', 'br', 'i', 'a')),
    '#description' => $solr_description,
  );
  $form['solr_settings']['apachesolr_popularity_enable_tracker'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Apache Solr Node Popularity'),
    '#default_value' => variable_get('apachesolr_popularity_enable_tracker', 0),
    '#description' => t('Track node views and calculate node popularity based on viewing trends.'),
  );
  // Only display if tracking is currently enabled prior to the page load.
  if (variable_get('apachesolr_popularity_enable_tracker', FALSE)) {
    $form['solr_settings']['remove_data_fieldset'] = array(
      '#type' => 'fieldset',
      '#states' => array(
        'visible' => array(
          ':input[name="apachesolr_popularity_enable_tracker"]' => array('checked' => FALSE),
        ),
      ),
    );
    $form['solr_settings']['remove_data_fieldset']['apachesolr_popularity_remove_data'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove tracking data? (Cannot be undone)'),
      '#default_value' => TRUE,
      '#states' => array(
        'visible' => array(
          ':input[name="apachesolr_popularity_enable_tracker"]' => array('checked' => FALSE),
        ),
      ),
    );
  }
  $form['solr_settings']['apachesolr_popularity_solr_data_dir'] = array(
    '#type' => 'textfield',
    '#title' => t("Path of the default Solr server's data directory (must be writtable)"),
    '#default_value' => variable_get('apachesolr_popularity_solr_data_dir', ''),
    '#description' => 'e.g.,  /opt/tomcat/solr/data/',
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  // Basic config settings.
  $form['basic_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic popularity configurations'),
  );
  $popularity_influence_options = array('0.0' => 'None', '0.1' => '5%', '0.2' => '10%', '0.5' => '25%', '1.0' => '50%', '2' => '100%', '4' => '200%', '8' => '400%', '20' => '1000%');
  $form['basic_config']['apachesolr_popularity_influence'] = array(
    '#type' => 'select',
    '#title' => t('Popularity influence'),
    '#default_value' => variable_get('apachesolr_popularity_influence', '2'),
    '#options' => $popularity_influence_options,
    '#description' => t("Amount that node popularity influences Apache Solr's search queries (i.e. ranking scores).<br />Ranking scores will be increase up to this amount."),
    // Removed code because of Drupal core issue.
    /*
    '#states' => array(
      'enabled' => array(
        ':input[name="apachesolr_popularity_enable_tracker"]' => array('checked' => TRUE),
        ':input[name="apachesolr_popularity_solr_data_dir"]' => array('empty' => FALSE)
      ),
    ),
     */
  );
  $forgetting_window_options = array('0.1' => '0.1', '0.5' => '0.5', '2' => '2', '5' => '5', '10' => '10', '20' => '20', '100' => '100', '1000' => '1000');
  $form['basic_config']['apachesolr_popularity_forgetting_window'] = array(
    '#type' => 'select',
    '#title' => t('Forgetting window'),
    '#default_value' => variable_get('apachesolr_popularity_forgetting_window', '5'),
    '#options' => $forgetting_window_options,
    '#description' => t('Older popularity values begin to fade off after about this many days.<br /> The lower the value is, the more that newer popularity measurements are biased positively.'),
  );
  $form['basic_config']['apachesolr_popularity_only_published'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only track published pages'),
    '#description' => t("Pages may receive a number of hits during creation, which may not reflect the popularity of the node at query time.<br />Note: a page is considered published if the page's publish field is checked."),
    '#default_value' => variable_get('apachesolr_popularity_only_published', TRUE),
  );
  $form['basic_config']['apachesolr_popularity_disable_frontpage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not track the front page'),
    '#description' => t('Due to the high number of views of most front pages, disabling popularity tracking of it will generally improve search results.'),
    '#default_value' => variable_get('apachesolr_popularity_disable_frontpage', TRUE),
  );
  // Clear popularity values.
  $form['reset_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clear popularity values'),
  );
  $form['reset_fieldset']['reset_tracker'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reset popularity values for all nodes.'),
    '#default_value' => FALSE,
  );
  $form['reset_fieldset']['confirm_reset_fieldset'] = array(
    '#type' => 'fieldset',
    '#states' => array(
      'visible' => array(
        ':input[name="reset_tracker"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['reset_fieldset']['confirm_reset_fieldset']['confirm_reset'] = array(
    '#type' => 'checkbox',
    '#title' => t('Confirm reset? (Cannot be undone)'),
    '#default_value' => FALSE,
    '#states' => array(
      'unchecked' => array(
        ':input[name="reset_tracker"]' => array('checked' => TRUE),
      ),
      'visible' => array(
        ':input[name="reset_tracker"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['#submit'][] = 'apachesolr_popularity_settings_form_submit';

  return system_settings_form($form);
}


/**
 * Validate setting form - hook_form_validate.
 */
function apachesolr_popularity_settings_form_validate($form, &$form_state) {
  // Verifies that if the reset tracker was clicked, it was also confirmed.
  if ($form_state['values']['reset_tracker'] == 1 AND
      $form_state['values']['confirm_reset'] == 0) {
    form_set_error('confirm_reset', t('Please confirm reset!'));
  }

  // Trim spaces.
  $dir = trim($form_state['values']['apachesolr_popularity_solr_data_dir']);
  // Add slash if not present.
  if ($dir[strlen($dir) - 1] != '\\' AND $dir[strlen($dir) - 1] != '/') {
    $dir .= DIRECTORY_SEPARATOR;
  }
  // Reset cleaned data directory.
  $form_state['values']['apachesolr_popularity_solr_data_dir'] = $dir;
  // Verifies that the Solr data directory exists and is writable.
  if ($dir != '') {
    if (!file_exists($dir)) {
      form_set_error('apachesolr_popularity_solr_data_dir', t('Directory does not exist'));
    }

    if (!is_writable($dir)) {
      form_set_error('apachesolr_popularity_solr_data_dir', t('Directory does is not writable'));
    }
  }
}

/**
 * Process form - hook_form_submit.
 */
function apachesolr_popularity_settings_form_submit($form, &$form_state) {

  // Whether to update solr with popularity data.
  $send_to_solr = FALSE;

  // Sets up popularity tracking table.
  if (($form_state['values']['apachesolr_popularity_enable_tracker'] == 1) AND (variable_get('apachesolr_popularity_enable_tracker') == 0)) {
    variable_set('apachesolr_popularity_enable_tracker', $form_state['values']['apachesolr_popularity_enable_tracker']);
    _apachesolr_popularity_reset_timer();
    _apachesolr_popularity_track_new_nodes();
    $send_to_solr = TRUE;
  }

  // Disables popularity tracking table.
  if (($form_state['values']['apachesolr_popularity_enable_tracker'] == 0) AND (variable_get('apachesolr_popularity_enable_tracker') == 1)) {
    variable_set('apachesolr_popularity_enable_tracker', $form_state['values']['apachesolr_popularity_enable_tracker']);
    $send_to_solr = TRUE;
    if ($form_state['values']['apachesolr_popularity_remove_data'] == 1) {
      _apachesolr_popularity_clear_tracking_data();
    }
  }

  // Checks if Solr data directory has changed.  If yes, than change it now
  // before other data is sent to Solr.
  if ($form_state['values']['apachesolr_popularity_solr_data_dir'] != variable_get('apachesolr_popularity_solr_data_dir')) {
    variable_set('apachesolr_popularity_solr_data_dir', $form_state['values']['apachesolr_popularity_solr_data_dir']);
  }

  // Checks if popularity influence has changed.  If yes, resend popularity
  // to Solr.
  if ($form_state['values']['apachesolr_popularity_influence'] != variable_get('apachesolr_popularity_influence')) {
    variable_set('apachesolr_popularity_influence', $form_state['values']['apachesolr_popularity_influence']);
    $send_to_solr = TRUE;
  }

  // Checks if forgetting window has changed.
  if ($form_state['values']['apachesolr_popularity_forgetting_window'] != variable_get('apachesolr_popularity_forgetting_window')) {
    drupal_set_message(t("Forgetting window changed.  This will effect future popularity values only."));
  }

  // Checks if only track published pages has changed.
  if ($form_state['values']['apachesolr_popularity_only_published'] != variable_get('apachesolr_popularity_only_published')) {
    $send_to_solr = TRUE;
  }

  // Checks if front page tracking has changed.
  if ($form_state['values']['apachesolr_popularity_disable_frontpage'] != variable_get('apachesolr_popularity_disable_frontpage')) {
    $send_to_solr = TRUE;
  }

  // Resets popularity tracking.
  if ($form_state['values']['reset_tracker'] == 1 AND $form_state['values']['confirm_reset'] == 1) {
    apachesolr_popularity_reset_popularity();
    $send_to_solr = TRUE;
  }

  // Check if popularity should be sent to solr.
  if ($send_to_solr == TRUE) {
    $dir = variable_get('apachesolr_popularity_solr_data_dir');
    if ($dir == '' OR !file_exists($dir) OR !is_writable($dir)) {
      drupal_set_message(t('Solr data directory not properly set!'));
    }
    else {
      // If everything is okay, send it to Solr.
      if ($form_state['values']['apachesolr_popularity_enable_tracker']) {
        drupal_set_message(t("Popularity data sent to Apache Solr."));
        _apachesolr_popularity_send_to_solr();
      }
      // Remove data from solr.
      else {
        if (_apachesolr_popularity_remove_data_from_solr()) {
          drupal_set_message(t("Popularity data removed from Apache Solr."));
        }
        else {
          drupal_set_message(t("Popularity is no longer being tracked but the data file (external_eff_popularity) was not able to be removed."));
        }
      }
    }
  }
}


/**
 * Form constructor for the Apache Solr Popularity advanced administration form.
 */
function apachesolr_popularity_advanced_form() {

  // Advanced settings page.
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Popularity Settings'),
  );
  if (variable_get('apachesolr_popularity_enable_tracker') == 1) {
    $form['settings']['apachesolr_popularity_enable_advanced'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable advanced settings'),
      '#default_value' => variable_get('apachesolr_popularity_enable_advanced', FALSE),
      '#description' => t('For advanced users only'),
    );
  }
  else {
    drupal_set_message(l(t('Enable node popularity first'), 'admin/config/search/apachesolr/apachesolr_popularity'), 'error');
    $form['settings']['apachesolr_popularity_enable_advanced'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enables advanced settings'),
      '#default_value' => FALSE,
      '#disabled' => TRUE,
      '#description' => t('For advanced users only'),
    );
  }

  $compression_options = array('N' => 'None', 'S' => 'Moderate', 'L' => 'High');
  $form['settings']['apachesolr_popularity_compression'] = array(
    '#type' => 'select',
    '#title' => t('Popularity compression on high-traffic nodes'),
    '#default_value' => variable_get('apachesolr_popularity_compression', 'N'),
    '#options' => $compression_options,
    '#description' => t("Prevents high-traffic nodes from dominating the search results."),
    '#states' => array(
      'disabled' => array(
        ':input[name="apachesolr_popularity_enable_advanced"]' => array('checked' => FALSE),
      ),
    ),
  );

  $init_popularity_options = array('Lowest' => 'Lowest', 'Low' => 'Low', 'Average' => 'Average', 'High' => 'High', 'Highest' => 'Highest');
  $form['settings']['apachesolr_popularity_init_popularity_quantity'] = array(
    '#type' => 'select',
    '#title' => t('Initial popularity'),
    '#default_value' => variable_get('apachesolr_popularity_init_popularity_quantity', 'Average'),
    '#options' => $init_popularity_options,
    '#description' => t("Sets inital popularity for newly tracked nodes.<br />Lets the initial popularity of new nodes to be boosted."),
    '#states' => array(
      'disabled' => array(
        ':input[name="apachesolr_popularity_enable_advanced"]' => array('checked' => FALSE),
      ),
    ),
  );

  $init_damping_options = array('0' => '0.0', '0.02' => '0.02', '0.1' => '0.1', '0.2' => '0.2', '0.5' => '0.5', '1' => '1', '2' => '2', '5' => '5');
  $form['settings']['apachesolr_popularity_init_damping'] = array(
    '#type' => 'select',
    '#title' => t('Initial popularity decay time'),
    '#default_value' => variable_get('apachesolr_popularity_init_damping', '0.2'),
    '#options' => $init_damping_options,
    '#description' => t("Number of days until the popularity is halfway in between the initial popularity and the actual popularity.<br />I.e., how quickly the initial node popularity disapears.<br /> 0 = Replaced at first popularity estimate, 0.2 = After about five hours, 2 = After two days."),
    '#states' => array(
      'disabled' => array(
        ':input[name="apachesolr_popularity_enable_advanced"]' => array('checked' => FALSE),
      ),
    ),
  );

  $low_pop_influence_options = array('0' => '0', '0.2' => '0.2', '0.5' => '0.5', '1' => '1', '2' => '2');
  $form['settings']['apachesolr_popularity_low_pop_influence'] = array(
    '#type' => 'select',
    '#title' => t('Low-popularity influence'),
    '#default_value' => variable_get('apachesolr_popularity_low_pop_influence', '1'),
    '#options' => $low_pop_influence_options,
    '#description' => t("How easily nodes with low popularity come up in the search results.<br /> 0 = Found difficultly, 1 = Typical, 2 = Found easily"),
    '#states' => array(
      'disabled' => array(
        ':input[name="apachesolr_popularity_enable_advanced"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['#submit'][] = 'apachesolr_popularity_advanced_form_submit';
  $form['submit']['#disabled'] = TRUE;

  return system_settings_form($form);
}


/**
 * Process form - hook_form_submit.
 */
function apachesolr_popularity_advanced_form_submit($form, &$form_state) {

  $send_to_solr = FALSE;

  // Checks if initial damping has changed.
  if ($form_state['values']['apachesolr_popularity_init_damping'] != variable_get('apachesolr_popularity_init_damping')) {
    drupal_set_message(t("Initial popularity decay time changed.  This will effect future popularity values only."));
  }

  // Checks if initial popularity has changed.
  if ($form_state['values']['apachesolr_popularity_init_popularity_quantity'] != variable_get('apachesolr_popularity_init_popularity_quantity')) {
    drupal_set_message(t("Initial popularity of future nodes will be effected."));
  }

  // Checks if popularity compression has changed.  Is yes, send data to Solr.
  if ($form_state['values']['apachesolr_popularity_compression'] != variable_get('apachesolr_popularity_compression')) {
    variable_set('apachesolr_popularity_compression', $form_state['values']['apachesolr_popularity_compression']);
    $send_to_solr = TRUE;
  }

  // Checks if low popularity influence has changed.
  if ($form_state['values']['apachesolr_popularity_low_pop_influence'] != variable_get('apachesolr_popularity_low_pop_influence')) {
    variable_set('apachesolr_popularity_low_pop_influence', $form_state['values']['apachesolr_popularity_low_pop_influence']);
    $send_to_solr = TRUE;
  }

  // Check if popularity should be sent to solr.
  if ($send_to_solr == TRUE) {
    $dir = variable_get('apachesolr_popularity_solr_data_dir');
    if ($dir == '' OR !file_exists($dir) OR !is_writable($dir)) {
      drupal_set_message(l(t('Solr data directory'), 'admin/config/search/apachesolr/apachesolr_popularity') . ' ' . t('not properly set'), 'error');
    }
    else {
      drupal_set_message(t("Popularity data sent to Apache Solr."));
      _apachesolr_popularity_send_to_solr();
    }
  }
}


/**
 * Track all nodes not currently being tracked.
 */
function _apachesolr_popularity_track_new_nodes() {

  // Find nodes that are in Drupal's node table that aren't in the
  // apachesolr_popularity_counter table.
  $node_create_list = db_query(
            'SELECT n.nid, n.status
             FROM {node} AS n
             LEFT JOIN {apachesolr_popularity_counter} AS apc
             ON n.nid=apc.nid
             WHERE apc.nid IS NULL'
            );

  // Get initial popularity value.
  $popularity = _apachesolr_popularity_get_initial_popularity();
  // Get front page nid.
  $frontpage_nid = _apachesolr_popularity_get_front_page_nid();

  // Add nodes.
  $query = db_insert('apachesolr_popularity_counter')
            ->fields(array('nid', 'popularity', 'flag'));
  foreach ($node_create_list as $node) {
    // Set node flags.
    $flag = 0x0;
    // Get the node's published status.
    if (!$node->status) {
      apachesolr_popularity_setflag($flag, UNPUBLISHED);
    }
    // Get whether the page is a front page.
    if ($node->nid == $frontpage_nid) {
      apachesolr_popularity_setflag($flag, FRONTPAGE);
    }
    // Insert new node.
    $query->values(array(
          'nid' => $node->nid,
          'popularity' => $popularity,
          'flag' => $flag,
        ));
  }
  $query->execute();
}


/**
 * Resets node popularities.
 */
function apachesolr_popularity_reset_popularity() {
  // Clear count total.
  variable_set('apachesolr_popularity_count_recent', 0);
  // Get initial popularity.
  $popularity = _apachesolr_popularity_get_initial_popularity(FALSE);
  // Reset all nodes.
  db_update('apachesolr_popularity_counter')
        ->fields(array(
                'popularity' => $popularity,
                'recentcount' => 0,
                'trackedtime' => 0.0,
                )
        )
        ->execute();

  // Update timestamps.
  _apachesolr_popularity_update_time();

}
