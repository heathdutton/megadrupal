<?php
/**
 * @file
 * Various functions that are required by project_browser.
 */

/**
 * Returns the themed install list form.
 */
function project_browser_get_install_list() {
  $build = array(
    '#theme' => 'project_browser_install_queue',
    '#projects' => project_browser_get_queued_projects(),
  );
  return drupal_render($build);
}

/**
 * Shows an install button for the Install Queue block.
 */
function project_browser_install_button_form($form, &$form_state) {
  $form['#attributes']['id'] = 'project-browser-install-button-form';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Install',
  );
  $form['#action'] = url('admin/modules/project-browser/install/select_versions');

  return $form;
}

/**
 * Builds the filters form.
 */
function project_browser_filters_form($form, &$form_state, $type) {
  $form['search_text'] = array(
    '#type' => 'textfield',
    '#size' => '25',
    '#title' => t('Search String'),
    '#default_value' => isset($_SESSION['project_browser_text_filter_' . $type]) ? $_SESSION['project_browser_text_filter_' . $type] : '',
  );

  // Add the categories filter if there are categories.
  if ($categories = project_browser_get_categories($type)) {
    $form['categories'] = array(
      '#type' => 'select',
      '#title' => t('Categories'),
      '#multiple' => TRUE,
      '#options' => $categories,
      '#prefix' => '<div id="project-browser-filters-categories">',
      '#suffix' => '</div>',
      '#default_value' => isset($_SESSION['project_browser_category_filter_' . $type]) ? $_SESSION['project_browser_category_filter_' . $type] : array(),
    );
  }

  $form['project_type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  return $form;
}

/**
 * Returns a themed sort widget for the filters.
 */
function project_browser_get_sort_widget($sort_options, $current_order_by, $current_sort) {
  $sort_list = array();
  $sort_list[] = array('data' => t('Sort by:'), 'class' => array('sort-header'));
  $current_path = drupal_get_path_alias($_GET['q']);

  foreach ($sort_options as $sort_option) {
    $classes = array();
    $query = array(
      'order_by' => $sort_option['method'],
      'sort' => $sort_option['default_sort'],
    );

    // If the sort option is currently active, handle it differently.
    if ($current_order_by == $sort_option['method']) {
      $classes[] = 'sort-active';
      $classes[] = 'sort-' . $current_sort;

      // Set the direction of the sort link to the opposite of what it currently
      // is.
      if ($current_sort == $query['sort']) {
        if ($query['sort'] == 'desc') {
          $query['sort'] = 'asc';
        }
        else {
          $query['sort'] = 'desc';
        }
      }
    }
    else {
      $classes[] = 'sort-inactive';
    }

    $sort_list[] = array(
      'data' => l($sort_option['name'], $current_path, array('query' => $query, 'class' => array())),
      'class' => $classes,
    );
  }

  return theme('item_list', array(
      'items' => $sort_list,
      'type' => 'ul',
      'attributes' => array('class' => array('project-browser-sort-list'))));
}

/**
 * Returns a themed sort widget for the filters.
 */
function project_browser_get_server_widget($servers, $current_server) {
  $list = array();
  $list[] = array('data' => t('Repository:'), 'class' => array('server-header'));
  $current_path = drupal_get_path_alias($_GET['q']);

  $i = 0;

  foreach ($servers as $url => $server) {
    $classes = array();
    $query = array(
      'repository' => $i,
    );

    // If the sort option is currently active, handle it differently.
    if ($current_server == $i) {
      $classes[] = 'server-active';
    }
    else {
      $classes[] = 'server-inactive';
    }

    $list[] = array(
      'data' => l($server['name'], $current_path, array('query' => $query, 'class' => array())),
      'class' => $classes,
    );

    $i += 1;
  }

  return theme('item_list', array(
      'items' => $list,
      'type' => 'ul',
      'attributes' => array('class' => array('project-browser-servers-list'))));
}


/**
 * Returns a list of sort options.
 *
 * @param $full
 *   Set this to TRUE if you want to get all of the supported sort methods.
 */
function project_browser_get_sort_options($full = FALSE) {
  $sort_options = array(
  	'score' => array('method' => 'score', 'name' => t('Relevancy'), 'default_sort' => 'desc'),
    'usage' => array('method' => 'usage', 'name' => t('Most installed'), 'default_sort' => 'desc'),
    'title' => array('method' => 'title', 'name' => t('Title'), 'default_sort' => 'asc'),
    'name' => array('method' => 'name', 'name' => t('Author'), 'default_sort' => 'asc'),
    'latest_release' => array('method' => 'latest_release', 'name' => t('Latest release'), 'default_sort' => 'desc'),
  );

  if ($full) {
  	$sort_options['type'] = array('method' => 'type', 'name' => t('Type'), 'default_sort' => 'asc');
    $sort_options['created'] = array('method' => 'created', 'name' => t('Date created'), 'default_sort' => 'asc');
    $sort_options['latest_activity'] = array('method' => 'latest_activity', 'name' => t('Latest build'), 'default_sort' => 'desc');
  }

  return $sort_options;
}

/**
 * Handles the filters form submit.
 */
function project_browser_filters_form_submit($form, &$form_state) {
  $type = $form_state['values']['project_type'];
  if (isset($form_state['values']['categories'])) {
    $_SESSION['project_browser_category_filter_' . $type] = $form_state['values']['categories'];
  }
  else {
    $_SESSION['project_browser_category_filter_' . $type] = array();
  }
  $_SESSION['project_browser_text_filter_' . $type] = $form_state['values']['search_text'];
}

/**
 * Returns all available categories for a project type.
 *
 * @param string $type
 *   The type of project to get the categories for. Example: 'module' or
 *   'theme'.
 *
 * @return
 *   Array containing all available categories or FALSE if no categories.
 */
function project_browser_get_categories($type) {
  $categories = array();

  // Get the server to use from SESSION.
  if (isset($_SESSION['project_browser_server_filter'])) {
    $use_server = $_SESSION['project_browser_server_filter'];
  }
  else {
    $use_server = 0;
  }

  $categories_raw = project_browser_fetch_categories($type, $use_server);

  if (is_array($categories_raw) AND !empty($categories_raw)) {
    foreach ($categories_raw as $url => $cats) {
	    foreach ($cats as $key => $value) {
	      // Create a new key so that there are no duplicate categories from
        // different sites.
	      $new_key = preg_replace('/[^a-z0-9_]+/', '_', strtolower($value));
	      $categories[$new_key] = $value;
	    }
    }
  }

  if (is_array($categories) AND !empty($categories)) {
    ksort($categories);

    return $categories;
  }
  return FALSE;
}

/**
 * Prepares the categories for sending to the servers as filters.
 *
 * @param $raw_cats
 *   An array of categories from $form_state['values'].
 * @param $type
 *   The type of project to prepare the categories for, eg 'module' or 'theme'.
 */
function project_browser_prepare_categories($raw_cats, $type) {
  $categories = project_browser_fetch_categories($type);

  // Set the value of the categories to true if it is selected.
  foreach ($categories as $url => $cats) {
    foreach ($cats as $key => $value) {
      $new_key = preg_replace('/[^a-z0-9_]+/', '_', strtolower($value));
      if (isset($raw_cats[$new_key]) AND $raw_cats[$new_key]) {
        $categories[$url][$key] = TRUE;
      }
      else {
        unset($categories[$url][$key]);
      }
    }

    // Unset the parent if there are no children.
    if (empty($categories[$url])) {
      unset($categories[$url]);
    }
  }

  return $categories;
}

/**
 * Checks if a project is enabled.
 *
 * @param string $type
 *   The type of project. Could be 'theme' or 'module'.
 * @param string $name
 *   The short name of the project.
 *
 * @return
 *   TRUE if the project is enabled, FALSE otherwise.
 */
function _project_browser_is_project_enabled($type, $name) {
  switch ($type) {
    case 'module':
      return module_exists($name);
      break;
    case 'theme':
      $themes = list_themes();
      return isset($themes[$name]);
      break;
  }
  return FALSE;
}

/**
 * Gets the currently listed projects from the session.
 */
function project_browser_get_listed_projects() {
  if (isset($_SESSION['project_browser_listed_projects'])) {
    return $_SESSION['project_browser_listed_projects'];
  }

  return array();
}

/**
 * Gets the currently queued projects from the session.
 */
function project_browser_get_queued_projects($type = NULL) {
  $projects = array();

  if (isset($_SESSION['project_browser_install_list'])) {
    foreach ($_SESSION['project_browser_install_list'] as $project) {
      if (is_array($project) AND !empty($project)) {
        if (isset($type) AND $type != $project['type']) {
          continue;
        }
        else {
          $projects[$project['name']] = $project;
        }
      }
    }
  }

  return $projects;
}

/**
 * Gets a release from a project and a release_name.
 */
function project_browser_get_release($release_name, $project) {
  $release_data = project_browser_get_project_release_data($project);

  return isset($release_data['releases'][$release_name]) ? $release_data['releases'][$release_name] : FALSE;
}

/**
 * Gets the newly installed projects from the session.
 */
function project_browser_get_installed_projects() {
  $projects = array();

  if (isset($_SESSION['project_browser_installed_projects'])) {
    foreach ($_SESSION['project_browser_installed_projects'] as $project) {
      if (is_array($project) AND !empty($project)) {
        $projects[$project['name']] = $project;
      }
    }
  }

  return $projects;
}

/**
 * Adds a project to the install queue.
 */
function project_browser_install_queue_add($project) {
  $_SESSION['project_browser_install_list'][$project['name']] = $project;
}

/**
 * Removes a project from the install queue.
 */
function project_browser_install_queue_remove($project_name) {
	if (isset($_SESSION['project_browser_install_list'][$project_name])) {
  	unset($_SESSION['project_browser_install_list'][$project_name]);
	}
}

/**
 * Gets the currently queued releases from the session.
 */
function project_browser_get_queued_releases() {
  $releases = array();

  if (isset($_SESSION['project_browser_install_releases_list'])) {
    foreach ($_SESSION['project_browser_install_releases_list'] as $release_name => $project) {
      if (is_array($project) AND !empty($project)) {
        $releases[$release_name] = $project;
      }
    }
  }

  return $releases;
}

/**
 * Fetches results from the servers based on the parameters passed in.
 *
 * $filters should be an associative array with the following keys:
 * array(
 *   'version' => '7', // The Major Version of Drupal that is running on the Client
 *   'text' => 'views', // The text that was entered as the search query, or '' if none
 *   'categories' => array() // The categories that were selected, if any
 *   'type' => 'module', // The type of project being searched
 *   'page' => 3, // The zero-based page number
 *   'requested' => 12, // How many results are requested per page
 * )
 *
 * The project_browser_fetch_results($filters) call returns an array like this:
 *
 * array(
 *   'total' = 5, // The total number of results found for the filters
 *   'projects' => array( // An array of projects returned for this page request
 *     'views' => array( // A project array keyed by the machine name
 *       'type' => 'module', // The type of project this is. Can be 'module' or 'theme'
 *       'title' => 'Views', // The title of the project
 *       'name' => 'views', // The machine name of the project
 *       'author' => 'merlinofchaos', // The author's name
 *       'description' => "Long project description ...",
 *       'image' => 'http://www.example.com/image.jpg', // Absolute url to the image, if any
 *       'usage' => '542312', // How many Downloads the module has had
 *       'project url' => 'http://www.drupal.org/projects/views', // Absolute url to the project page, if any
 *       'project status url' => 'http://updates.drupal.org/release-history/views/7.x', // The absolute url of the update checker, formatted like how Drupal.org Update Status does it
 *       'last updated' => '12342523', // UNIX Timestamp of when the project was last updated
 *       'maintenance status' => 'Actively maintained', // Maintenance status
 *       'development status' => 'Under active development', // Development status
 *       'rating' => '9.6', // A rating on a scale of 1 to 10 of the project, if available
 *       'dependencies' => array( // An array of the dependencies of this module
 *         'ctools',
 *       ),
 *     ),
 *   'name_2 => array( ... ),
 *   ),
 * );
 *
 * @param array $filters
 *   An associative array of queries to use to filter results.
 *
 * @return
 *   Returns an array of results.
 */
function project_browser_fetch_results($filters) {
  $servers = project_browser_get_servers($filters['server']);
  // Attempt to retrieve the cached version of this page.
  $cid = md5(serialize(array_merge($filters, $servers)));

  if ($cache = cache_get($cid)) {
    return $cache->data;
  }

  $results = array(
    'projects' => array(),
    'total' => 0,
  );

  unset($filters['server']);

  foreach ($servers as $url => $server) {
    $local_filters = $filters;

    // We are not using this right now because we only expect to handle 1 server
    // at a time currently.
    // $local_filters['requested'] = floor($filters['requested'] / count($servers));

    // Send only the relevant categories to the server.
    if (isset($filters['categories'])) {
      if (!isset($filters['categories'][$url])) {
        // Don't call a server for results if categories are being used, and
        // none of them belong to the server.
        continue;
      }
      $local_filters['categories'] = $filters['categories'][$url];
    }

    // Use XMLRPC if it is set.
    if ($server['method'] == 'xmlrpc') {
      $results_raw = xmlrpc($url, array(
        'project_browser_server.fetch_results' => array($local_filters),
      ));

      // Check for errors.
      if ($error = xmlrpc_error() AND $error->is_error) {
        drupal_set_message(t("Encountered an error when trying to fetch results from @name. Error @code : @message",
          array('@name' => $server['name'], '@code' => $error->code, '@message' => $error->message)));
        continue;
      }
    }

    // Use json if it is set.
    if ($server['method'] == 'json') {
      $local_filters['method'] = 'query';
      if (isset($local_filters['categories'])) {
        $local_filters['categories'] = serialize($local_filters['categories']);
      }

			$query_url = $url . '/query/' . $local_filters['type'] . '/7?' . http_build_query($local_filters, FALSE, '&');
      $response = drupal_http_request($query_url);
      if ($response->code == '200') {
        $results_raw = drupal_json_decode($response->data);
      }
      else {
        drupal_set_message(t("Encountered an error when trying to fetch results from @name. Error @code : @message",
          array('@name' => $server['name'], '@code' => $response->code, '@message' => $response->error)));
        continue;
      }
    }

    if (isset($results_raw['total'])) {
      $results['total'] += $results_raw['total'];
    }

    if (isset($results_raw['projects']) AND !empty($results_raw['projects'])) {
      // Merge the results.
      $results['projects'] = array_merge($results['projects'], $results_raw['projects']);
    }
  }

  // Set the cached version of the results.
  cache_set($cid, $results, 'cache', strtotime("+24 hours"));

  return $results;
}

/**
 * Fetches categories from the servers based on the type of project.
 *
 * @param $type
 *   The type of project we are getting categories for.
 * @param $use_server
 *   (Optional) The server to use. Defaults to 'all'.
 *
 * @return
 *   Returns an array of the categories.
 */
function project_browser_fetch_categories($type, $use_server = 'all') {
  $servers = project_browser_get_servers($use_server);

  // Attempt to retrieve the cached version of this page.
  $cid = md5('categories_' . $type . serialize($servers));

  if ($cache = cache_get($cid)) {
    return $cache->data;
  }
  else {
    $categories = array();

    foreach ($servers as $url => $server) {
      // Use xmlrpc if it is set.
      if ($server['method'] == 'xmlrpc') {
        $categories_raw = xmlrpc($url, array(
          'project_browser_server.fetch_categories' => array($type),
        ));

        // Check for errors.
        if ($error = xmlrpc_error() AND $error->is_error) {
          drupal_set_message(t("Encountered an error when trying to fetch categories from @name. Error @code : @message",
            array('@name' => $server['name'], '@code' => $error->code, '@message' => $error->message)));
          continue;
        }
      }

      // Use json if it is set.
      if ($server['method'] == 'json') {
        $params = array(
          'method' => 'categories',
          'type' => $type,
        );
        $response = drupal_http_request($url . '/categories/' . $type . '?' . http_build_query($params, FALSE, '&'));
        if ($response->code == '200') {
          $categories_raw = drupal_json_decode($response->data);
        }
        else {
          drupal_set_message(t("Encountered an error when trying to fetch categories from @name. Error @code : @message",
            array('@name' => $server['name'], '@code' => $response->code, '@message' => $response->error)));
          continue;
        }
      }

      if (is_array($categories_raw) AND !empty($categories_raw)) {
        $categories[$url] = $categories_raw;
      }
    }

    // Cache this for 24 hours.
    cache_set($cid, $categories, 'cache', strtotime("+24 hours"));
  }

  return $categories;
}

/**
 * Gets the servers to use for fetching results.
 *
 * @return
 *   Returns an associative array of servers, populated from the
 *   project_browser_servers variable, in 'url => name' format.
 */
function project_browser_get_servers($use_server = 'all') {
  // FIXME - Change the link once drupal.org is ready
  $servers = variable_get('project_browser_default_server', array(
    'http://www.moneyscripts.net/project_browser/server' => array(
      'name' => 'Drupal',
      'method' => 'json',
    ),
  ));

  if ($servers_raw = variable_get('project_browser_servers', '')) {
    // Process the variable and add the servers to the list.
    $custom_servers = array();

    $list = explode("\n", $servers_raw);
    $list = array_map('trim', $list);
    $list = array_filter($list, 'strlen');

    foreach ($list as $position => $text) {
      $method = $name = $url = FALSE;

      $matches = array();
      if (preg_match('/(.*)\|(.*)\|(.*)/', $text, $matches)) {
        $url = $matches[1];
        $method = $matches[2];
        $name = $matches[3];
        $custom_servers[$url] = array('name' => $name, 'method' => $method);
      }
    }

    $servers = array_merge($servers, $custom_servers);
  }

  // Filter out servers if necessary.
  if ($use_server !== 'all') {
    $i = 0;
    foreach ($servers as $url => $server) {
      if ($use_server != $i) {
        unset($servers[$url]);
      }
      $i += 1;
    }
  }

  return $servers;
}

/**
 * Uses the project status url to get the available releases for a project.
 *
 * @param $project
 *   The project to get the releases for.
 *
 * @return
 *   An array of releases for this project.
 */
function project_browser_get_project_release_data($project) {
  $project['project_type'] = $project['type'];

  // Build the releases cache for this project
  module_load_include('inc', 'update', 'update.fetch');
  if (_update_process_fetch_task($project)) {
    $data = _update_cache_get('available_releases::' . $project['name']);
    if (isset($data->data) AND isset($data->data['releases']) AND is_array($data->data['releases'])) {
      return $data->data;
    }
  }

  return FALSE;
}

/**
 * Helper function to download a project. This code is mostly copied and pasted
 * from modules/update/update.manager.inc
 *
 * There were no suitable functions that could be used besides
 * drupal_form_submit('update_manager_install_form', $form_state, $project['type']);
 * and it wouldn't work because this is being run from a Batch function.
 *
 * @todo  Ideally, this should be in the update module as a standalone function,
 * to reduce coupling and duplication.
 */
function project_browser_download_project($url) {
  module_load_include('inc', 'update', 'update.manager');
  // Download the file.
  $local_cache = update_manager_file_get($url);
  if (!$local_cache) {
    return array(
      'success' => FALSE,
      'message' => t('Unable to retrieve Drupal project from %url.', array('%url' => $url)),
    );
  }

  // Try to extract it.
  $directory = _update_manager_extract_directory();
  try {
    $archive = update_manager_archive_extract($local_cache, $directory);
  }
  catch (Exception $e) {
    return array(
      'success' => FALSE,
      'message' => $e->getMessage(),
    );
  }
  $files = $archive->listContents();
  if (!$files) {
    return array(
      'success' => FALSE,
      'message' => t('Provided archive contains no files.'),
    );
  }

  $project = strtok($files[0], '/\\');

  $archive_errors = update_manager_archive_verify($project, $local_cache, $directory);
  if (!empty($archive_errors)) {
    if (!empty($archive_errors)) {
      foreach ($archive_errors as $error) {
        drupal_set_message(check_plain($error), 'error');
      }
    }
    return array(
      'success' => FALSE,
      'message' => array_shift($archive_errors),
    );
  }

  // Make sure the Updater registry is loaded.
  drupal_get_updaters();

  $project_location = $directory . '/' . $project;
  try {
    $updater = Updater::factory($project_location);
  }
  catch (Exception $e) {
    return array(
      'success' => FALSE,
      'message' => $e->getMessage(),
    );
  }

  try {
    $project_title = Updater::getProjectTitle($project_location);
  }
  catch (Exception $e) {
    return array(
      'success' => FALSE,
      'message' => $e->getMessage(),
    );
  }

  if ($updater->isInstalled()) {
    return array(
      'success' => FALSE,
      'message' => t('%project is already installed.', array('%project' => $project_title)),
    );
  }

  $project_real_location = drupal_realpath($project_location);
  $updater_name = get_class($updater);

  if (fileowner($project_real_location) == fileowner(conf_path())) {
    module_load_include('inc', 'update', 'update.authorize');
    $filetransfer = new FileTransferLocal(DRUPAL_ROOT);

    // Initialize some variables in the Batch API $context array.
    $updater = new $updater_name($project_real_location);

    try {
      if ($updater->isInstalled()) {
        // This is an update.
        $updater->update($filetransfer);
      }
      else {
        $updater->install($filetransfer);
      }
    }
    catch (UpdaterException $e) {
      return array(
        'success' => FALSE,
        'message' => t('Error installing / updating. Error: @error', array('@error' => $e->getMessage())),
      );
    }
  }
  else {
    return array(
      'success' => FALSE,
      'message' => t('Permissions are not set up properly.'),
    );
  }

  return array(
    'success' => TRUE,
  );
}

/**
 * Installs a single release of a project during batch.
 */
function _project_browser_batch_install_release($release_name, $project, &$context) {
  module_load_include('inc', 'project_browser', 'project_browser.pages');
  $release = project_browser_get_release($release_name, $project);

  $result = project_browser_download_project($release['download_link']);

  if ($result['success']) {
    $context['results']['successes'][] = t('Successfully installed %project.', array('%project' => $project['title']));
    $context['message'] = t('Installed %project...', array('%project' => $project['title']));

    // Add this to the session variable and remove it from the install_queue
    // variable.
    $_SESSION['project_browser_installed_projects'][$project['name']] = $project;
    unset($_SESSION['project_browser_install_list'][$project['name']]);
  }
  else {
    watchdog('project_browser', 'There was an error while installing %project.
      !message',
      array('%project' => $project['title'], '!message' => $result['message']), WATCHDOG_ERROR);
    $context['results']['failures'][] = t('Error installing %project. Errors have been logged.',
      array('%project' => $project['title']));
    $context['message'] = t('Error installing %project. !message',
      array('%project' => $project['title'], '!message' => $result['message']));
  }
}

/**
 * Shows a message and finish up the batch.
 */
function _project_browser_batch_install_releases_finished($success, $results, $operations) {
  drupal_get_messages();

  // Restore the maintenance mode to what it was at the start.
  variable_set('maintenance_mode', $_SESSION['maintenance_mode']);
  unset($_SESSION['maintenance_mode']);

  unset($_SESSION['project_browser_install_releases_list']);
  if ($success) {
    if (!empty($results)) {
      if (!empty($results['failures'])) {
        drupal_set_message(format_plural(count($results['failures']), 'Failed to install one project.', 'Failed to install @count projects.'), 'error');
      }
    }
  }
  else {
    drupal_set_message(t('Error installing projects.'), 'error');
    drupal_goto('admin/modules/project-browser/install/select_versions');
  }

	$projects = project_browser_get_installed_projects();
	$missing = project_browser_get_missing_dependencies($projects);
	// If there are missing dependencies, go to install dependencies.
	if (count($missing) > 0) {
		drupal_goto('admin/modules/project-browser/install/install_dependencies');
	}
	else {
  	drupal_goto('admin/modules/project-browser/install/enable');
  }
}

/**
 * Gets the dependencies for installed projects.
 */
function project_browser_get_missing_dependencies($projects) {
	$modules = system_rebuild_module_data();

	$missing = array();

  foreach ($projects as $project) {
    if ($project['type'] == 'module') {
      $dependencies = array();
      if (isset($modules[$project['name']])) {
        foreach ($modules[$project['name']]->info['dependencies'] as $dependency) {
          if (!isset($modules[$dependency])) {
            $dependencies[] = $dependency;
          }
        }
        if (count($dependencies) > 0) {
          $missing[$project['name']] = $dependencies;
        }
      }
      else {
        drupal_set_message(t('There was an error getting information for @module',
          array('@module' => $project['name'])), 'error');
      }
    }
  }

	return $missing;
}
