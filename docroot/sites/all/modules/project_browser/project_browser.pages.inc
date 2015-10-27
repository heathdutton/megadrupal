<?php
/**
 * @file
 * This include contains most of the page callbacks for the project browser.
 */

/**
 * Page to display the projects.
 *
 * @param $type
 *   Type of project to view, ie 'module' or 'theme'.
 *
 * @return
 *   Themed list of the projects.
 */
function project_browser_page($type) {
  module_load_include('inc', 'project_browser', 'project_browser');
  drupal_add_library('project_browser', 'multiselect');
  drupal_add_js(drupal_get_path('module', 'project_browser') . '/js/project_browser_more_link.js');
  drupal_add_js(drupal_get_path('module', 'project_browser') . '/js/project_browser_multiselect.js');

  // Set any filters in the session that are needed.
  if (isset($_GET['repository'])) {
    $_SESSION['project_browser_sever_filter'] = $_GET['repository'];
  }
  if (isset($_GET['order_by'])) {
    $_SESSION['project_browser_order_by_filter_' . $type] = $_GET['order_by'];
  }
  if (isset($_GET['sort'])) {
    $_SESSION['project_browser_sort_filter_' . $type] = $_GET['sort'];
  }

  // Build the filters.
  $drupal_version = explode('.', DRUPAL_CORE_COMPATIBILITY);
  $filters = array(
    'version' => $drupal_version[0],
    'type' => $type,
  );

  // Add filters.
  if (isset($_SESSION['project_browser_category_filter_' . $type])) {
    $categories = array_filter($_SESSION['project_browser_category_filter_' . $type]);
    if (!empty($categories)) {
      $filters['categories'] = project_browser_prepare_categories($categories, $type);
    }
  }
  if (isset($_SESSION['project_browser_text_filter_' . $type])) {
    $filters['text'] = $_SESSION['project_browser_text_filter_' . $type];
  }
  if (isset($_SESSION['project_browser_order_by_filter_' . $type])) {
    $filters['order_by'] = $_SESSION['project_browser_order_by_filter_' . $type];
  }
  if (isset($_SESSION['project_browser_sort_filter_' . $type])) {
    $filters['sort'] = $_SESSION['project_browser_sort_filter_' . $type];
  }
  if (isset($_SESSION['project_browser_server_filter'])) {
    $filters['server'] = $_SESSION['project_browser_server_filter'];
  }
  else {
    $filters['server'] = 0;
  }
  $filters['requested'] = 10;
  $filters['page'] = isset($_GET['page']) ? $_GET['page'] : 0;

  // Get the projects to display here based on the filters.
  $results = project_browser_fetch_results($filters);

  // Save the listed projects in the session so it can be used.
  $_SESSION['project_browser_listed_projects'] = $results['projects'];

  $test = project_browser_get_listed_projects();

  $list = array();
  foreach ($results['projects'] as $project) {
    $list[] = $project;
  }

  // Add the pager.
  $total = $results['total'];
  $num_per_page = 10;
  $page = pager_default_initialize($total, $num_per_page);
  $offset = $num_per_page * $page;
  $start = ($total) ? $offset + 1 : 0;
  $finish = $offset + $num_per_page;
  if ($finish > $total) {
    $finish = $total;
  }

  $sort_options = project_browser_get_sort_options();
  $current_order_by = isset($_SESSION['project_browser_order_by_filter_' . $type]) ? $_SESSION['project_browser_order_by_filter_' . $type] : 'score';
  $current_sort = isset($_SESSION['project_browser_sort_filter_' . $type]) ? $_SESSION['project_browser_sort_filter_' . $type] : 'desc';;

  $build = array();
  $build['content'] = array(
    'project_browser_header' => array(
      '#markup' => t('Showing @start to @finish of @total.', array(
        '@start' => $start, '@finish' => $finish, '@total' => $total)),
      '#weight' => 0,
    ),
    'project_browser_sort_header' => array(
      '#type' => 'item',
      '#weight' => 2,
      '#markup' => project_browser_get_sort_widget($sort_options, $current_order_by, $current_sort),
    ),
    'project_browser_list' => array(
      '#markup' => theme('project_browser_list', array('projects_list' => $list, 'type' => $type)),
      '#weight' => 3,
    ),
    'pager' => array(
      '#theme' => 'pager',
      '#weight' => 99,
    ),
  );

  $servers = project_browser_get_servers();

  if (count($servers) > 1) {
    $build['content']['project_browser_server_header'] = array(
      '#type' => 'item',
      '#weight' => 1,
      '#markup' => project_browser_get_server_widget($servers, $filters['server']),
    );
  }

  return $build;
}

/**
 * Page to install projects.
 *
 * @param $op
 *    Operation to preform.
 */
function project_browser_installation_page($op) {
  drupal_add_css(drupal_get_path('module', 'project_browser') . '/css/project_browser.css', array('preprocess' => FALSE));

  switch ($op) {
    case 'select_versions':
      drupal_set_title(t("Select versions"));
      $content = project_browser_installation_select_versions_page();
      break;
		case 'install_dependencies':
      drupal_set_title(t("Install Dependencies"));
      $content = project_browser_installation_install_dependencies_page();
      break;
    case 'enable':
      drupal_set_title(t("Enable modules"));
      $content = project_browser_installation_enable_page();
      break;
  }
  return theme('project_browser_install', array('current_task' => $op, 'main_content' => drupal_render($content)));
}

/**
 * Task page for the Select versions installation task.
 *
 * Shows a form where the user can select which versions to install for each
 * project.
 */
function project_browser_installation_select_versions_page() {
  module_load_include('inc', 'project_browser', 'project_browser');
  // Show a form that lets the user select which version of the projects to install
  $queued_projects = project_browser_get_queued_projects();
	unset($_SESSION['project_browser_installed_projects']);

  return drupal_get_form('project_browser_installation_select_versions_form', $queued_projects);
}

/**
 * Form builder for the select versions form.
 *
 * @param $projects
 *   An array of projects to get the releases for.
 */
function project_browser_installation_select_versions_form($form, &$form_state, $projects) {
  module_load_include('inc', 'project_browser', 'project_browser');
  drupal_add_js(drupal_get_path('module', 'project_browser') . '/js/select_releases.js');

  $form = array();

  // First unset any old data.
  unset($_SESSION['project_browser_install_releases_list']);

  $form['#tree'] = TRUE;

  $form['releases-header'] = array(
    '#type' => 'item',
    '#markup' => t("You're about to install:"),
  );

  $form['releases'] = array();

  foreach ($projects as $project) {
    // Get the available releases for this project.
    if (!$release_data = project_browser_get_project_release_data($project)) {
    	drupal_set_message(t('Could not fetch releases for project %project.',
	      array('%project' => $project['title'])), 'warning');
    	watchdog('project_browser', 'Could not fetch releases for project %project.',
	      array('%project' => $project['title']), WATCHDOG_ERROR);
			project_browser_install_queue_remove($project['name']);
			continue;
		}

    // We use the update module to calculate the recommended version.
    $project_data = array(
      'existing_major' => 0,
      'existing_version' => 0,
      'install_type' => '',
    );
    module_load_include('inc', 'update', 'update.compare');
    update_calculate_project_update_status($project, $project_data, $release_data);

    $releases_list = array();

    foreach ($release_data['releases'] as $version => $release) {
      $release_title = t("@project @version - @date", array(
        '@project' => $project['title'],
        '@version' => $release['version'],
        '@date' => format_date($release['date'], 'custom', 'M j, Y'),
        ));
      if (isset($release['terms']['Release type']) AND !empty($release['terms']['Release type'])) {
        $release_title .= " (" . implode(', ', $release['terms']['Release type']) . ")";
      }
      if (isset($release['release_link'])) {
        $releases_list[$version] = l($release_title, $release['release_link']);
      }
      else {
        $releases_list[$version] = $release_title;
      }
    }

    $form['releases'][$project['name']]['project'] = array(
      '#type' => 'value',
      '#value' => $project,
    );

    $form['releases'][$project['name']]['release_name'] = array(
      '#type' => 'radios',
      '#title' => t('Select release for @project', array('@project' => $project['title'])),
      '#options' => $releases_list,
      '#default_value' => key($releases_list),
      '#prefix' => '<div class="project-browser-releases-wrapper project-browser-release-' . $project['name'] . '">',
      '#suffix' => '</div>',
      '#attributes' => array(
        'class' => array('project-browser-releases-radios'),
      ),
      '#required' => TRUE,
    );
    $form['releases'][$project['name']]['selected_text'] = array(
      '#type' => 'item',
      '#prefix' => '<div class="project-browser-selected-release project-browser-selected-release-' . $project['name'] . '">',
      '#suffix' => '</div>',
      '#markup' => reset($releases_list),
    );
    if (isset($project_data['recommended'])) {
      // If there is a recommended release set, then only show it and show the
      // jQuery link.
      $recommended_releases = array();
      $recommended_releases[$project_data['recommended']] = $releases_list[$project_data['recommended']];
      $form['releases'][$project['name']]['release_name']['#default_value'] = $project_data['recommended'];
      $form['releases'][$project['name']]['selected_text']['#markup'] = $releases_list[$project_data['recommended']];
    }
    if (count($releases_list) > 1) {
      $form['releases'][$project['name']]['selected_text']['#markup'] .=
        " (<span class='project-browser-show-releases-link' rel='" . $project['name'] . "'>" . t('change release') . "</span>)";
    }
  }

	// If there is nothing to install, go to the enable page.
	if (empty($form['releases'])) {
		drupal_set_message(t('No releases data found for any of the selected projects.'), 'warning');
		drupal_goto('admin/modules/project-browser/install/enable');
	}

  $form['backup_warning'] = array(
    '#type' => 'markup',
    '#markup' => t('Back up your database and site before you continue. !link.',
      array('!link' => l(t('Learn how'), 'http://drupal.org/node/22281'))),
  );
  $form['maintenance_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Perform updates with site in maintenance mode (strongly recommended)'),
    '#default_value' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Install'),
  );

  return $form;
}

/**
 * Submit handler for the select versions form.
 */
function project_browser_installation_select_versions_form_submit($form, &$form_state) {
  module_load_include('inc', 'project_browser', 'project_browser');
  // Store maintenance_mode setting so we can restore it when done.
  $_SESSION['maintenance_mode'] = variable_get('maintenance_mode', FALSE);
  if ($form_state['values']['maintenance_mode'] == TRUE) {
    variable_set('maintenance_mode', TRUE);
  }

  foreach ($form_state['values']['releases'] as $item) {
    // Load the selected release.
    if ($release = project_browser_get_release($item['release_name'], $item['project'])) {
      // Add the release to a session variable.
      $_SESSION['project_browser_install_releases_list'][$item['release_name']] = $item['project'];
    }
  }

  // Install the projects with batch.
  module_load_include('inc', 'update', 'update.manager');

  $queued_releases = project_browser_get_queued_releases();

  $operations = array();
  foreach ($queued_releases as $release_name => $project) {
    $operations[] = array('_project_browser_batch_install_release', array($release_name, $project));
  }
  $batch = array(
    'operations' => $operations,
    'finished' => '_project_browser_batch_install_releases_finished',
    'title' => t('Installing projects'),
    'init_message' => t('Installing modules...'),
    'progress_message' => t('Installed @current out of @total.'),
    'error_message' => t('Installation has encountered an error.'),
    'file' => drupal_get_path('module', 'project_browser') . '/project_browser.inc',
  );
  batch_set($batch);
}

/**
 * Task page for the Install Dependencies installation task.
 *
 * Show a form which lets the user select which version of dependencies to
 * install.
 */
function project_browser_installation_install_dependencies_page() {
  module_load_include('inc', 'project_browser', 'project_browser');
  $projects = project_browser_get_installed_projects();
	$missing = project_browser_get_missing_dependencies($projects);

	if (count($missing) > 0) {
		$missing_projects = array();
	  // Add the project data in the array as best we can.
	  foreach ($missing as $project_shortname => $dependencies) {
	  	foreach ($dependencies as $shortname) {
	  		$missing_projects[$shortname] = array(
					'name' => $shortname,
					'type' => 'module', // FIXME
					'title' => $shortname,
          'includes' => array(),
				);
	  	}
	  }

	  return drupal_get_form('project_browser_installation_select_versions_form', $missing_projects);
  }
	else {
		drupal_goto('admin/modules/project-browser/install/enable');
	}
}

/**
 * Task page for the Enable projects installation task.
 *
 * Show a form which lets the user enable the newly installed projects.
 */
function project_browser_installation_enable_page() {
  module_load_include('inc', 'project_browser', 'project_browser');
  $installed_projects = project_browser_get_installed_projects();

	if (count($installed_projects) > 0) {
  	return drupal_get_form('project_browser_installation_enable_form', $installed_projects);
	}
	else {
		drupal_goto('admin/modules/project-browser');
	}
}

/**
 * Form builder for the select versions form.
 *
 * @param $projects
 *   An array of projects to get the releases for.
 */
function project_browser_installation_enable_form($form, &$form_state, $projects) {
  $modules = system_rebuild_module_data();
  $form['instructions'] = array(
    '#type' => 'item',
    '#markup' => t('The projects you selected have been successfully installed.
      If you installed any new modules, you may enable them using the form below
      or on the main !link page.', array('!link' => l(t('Modules'), 'admin/modules'))),
  );

  $options = array();
  $missing = array();

  foreach ($projects as $project) {
    if ($project['type'] == 'module') {
      $dependency_check = TRUE;
      $dependencies = array();
      if (isset($modules[$project['name']])) {
        foreach ($modules[$project['name']]->info['dependencies'] as $dependency) {
          if (isset($modules[$dependency])) {
            $dependencies[] = $modules[$dependency]->info['name'] . ' (' . t('Installed') . ')';
          }
          else {
            $dependency_check = FALSE;
            $dependencies[] = $dependency . ' (' . t('Missing') . ')';
          }
        }
        if ($dependency_check) {
          $options[$project['name']] = array(
            array('data' => $modules[$project['name']]->info['name']),
            array('data' => $modules[$project['name']]->info['version']),
            array('data' => implode(', ', $dependencies)),
          );
        }
        else {
          $missing[$project['name']] = array(
            array('data' => $modules[$project['name']]->info['name']),
            array('data' => $modules[$project['name']]->info['version']),
            array('data' => implode(', ', $dependencies)),
          );
        }
      }
      else {
        drupal_set_message(t('There was an error getting information for @module',
          array('@module' => $project['name'])), 'error');
      }
    }
  }

  $headers = array(
    array('data' => t('Title')),
    array('data' => t('Version')),
    array('data' => t('Dependencies')),
  );

  if (!empty($options)) {
    $form['modules'] = array(
      '#type' => 'tableselect',
      '#title' => t('Enable modules'),
      '#description' => t('Select which modules you would like to enable.'),
      '#header' => $headers,
      '#options' => $options,
      '#empty' => t('No new modules installed.'),
      '#multiple' => TRUE,
      '#js_select' => TRUE,
      '#weight' => 1,
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#submit' => array('project_browser_installation_enable_form_submit'),
      '#value' => t('Enable modules'),
      '#weight' => 99,
    );
  }

  if (!empty($missing)) {
    $form['missing'] = array(
      '#type' => 'item',
      '#title' => t('Missing Dependencies'),
      '#description' => t('These modules are missing one or more dependencies,
        and so cannot be enabled.'),
      '#markup' => theme('table', array('header' => $headers, 'rows' => $missing)),
      '#weight' => 2,
    );
  }

  return $form;
}

/**
 * Form submit handler for the enable modules form.
 */
function project_browser_installation_enable_form_submit($form, &$form_state) {
  $enable_queue = array_filter($form_state['values']['modules']);
  // Enable these all at once so that dependencies are handled properly.
  module_enable($enable_queue);
	drupal_flush_all_caches();
  drupal_goto('admin/modules');
}

/**
 * Get a task list to the sidebar area when installing projects.
 *
 * This will need to be called from every page of the install process.
 *
 * @param $active
 *   (Optional) Set the active task by key.
 */
function project_browser_installation_task_list($active = NULL) {
  // Default list of tasks.
  $tasks = array(
    'select_versions' => t('Select versions'),
    'install_dependencies' => t('Install Dependencies'),
    'enable' => t('Enable projects'),
  );

  require_once DRUPAL_ROOT . '/includes/theme.maintenance.inc';

  return theme_task_list(array('items' => $tasks, 'active' => $active));
}
