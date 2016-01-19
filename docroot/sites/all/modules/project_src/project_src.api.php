<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard Drupal
 * manner.
 */

/**
 * @defgroup project_src Project Source integrations.
 *
 * Module integrations with the Project Source module.
 */

/**
 * @defgroup project_src_hooks Project Source's hooks.
 * @{
 * Hooks that can be implemented by other modules in order to extend the Project
 * Source module. Note that you should place your hooks in a file of the form
 * your_module.project_src.inc in your module's root directory.
 */


/**
 * Returns an array of custom project definitions keyed by project short name,
 * nested in an array of supported API versions. The exact makeup of the project
 * definition is up to you, but by default, its contents will be passed directly
 * to the main project XML theme function.
 *
 * It's recommended that project definitions contain the following info:
 * - title: The human-readable name for the specified project,
 * - short_name: The Drupal project short name for the specified project,
 * - creator: A string representing the project's original author,
 * - type: A string representing the type of project. One of: project_core,
     project_distribution, project_module, project_theme, project_theme_engine,
     or project_translation.
 * - api_version: The Drupal API version supported by the project (e.g. 7.x),
 * - link: A fully qualified URL representing the project page,
 * - project_status: The current project status (e.g. "published"),
 * - recommended_major: The recommended major version of the project. For
 *   instance, if you would like to recommend your 7.x-2.x branche you would
 *   want to return "2" here,
 * - supported_majors: A comma seperated list of supported major versions,
 * - default_major: The default major version of the project. If the default
 *   branch for the project was 7.x-2.x, you would return "2" here.
 *
 * @return array
 *   See above for details.
 *
 * @see project_src_get_projects()
 */
function hook_project_src_info() {
  // Define a 7.x API compatible version of a Views module fork.
  $projects['7.x']['my_views_fork'] = array(
    'title' => 'My Views Fork', 
    'short_name' => 'my_views_fork',
    'creator' => 'notmerlinofchaos',
    'type' => 'project_module',
    'api_version' => '7.x',
    'project_status' => 'published',
    'link' => 'http://example.com/project/my_views_fork',
    'recommended_majors' => 3,
    'supported_majors' => 3,
    'default_major' => 3,
  );

  return $projects;
}

/**
 * Allows you to modify project definitions at runtime.
 *
 * @param array &$projects
 *   An array of projects exactly as described in hook_project_src_info(). Note
 *   that this is called for all projects and when projects are loaded on a
 *   module-by-module basis. In both cases, they're passed with API version keys
 *   at the top level.
 */
function hook_project_src_info_alter(&$projects) {
  // Maybe for some reason, we want to translate the title.
  $projects['7.x']['my_views_fork']['title'] = t('My Views Fork');
}


/**
 * Returns an array of release definitions for a given project and API version,
 * keyed by version.
 *
 * The exact makeup of release definitions, like project definitions, is up to
 * you, but by default, its contents will be passed directly to the project
 * release XML theme function.
 *
 * It's recommended that release definitions contain the following info:
 * - name: The human readable name of the release; for reference, Drupal.org
 *   uses a concatenation of project short name and release version (tag),
 * - version: The version of the release (e.g. 7.x-1.0),
 * - tag: The tag associated with this release (often the same as "version"),
 * - date: The UNIX timestamp representing when the release was made available,
 * - version_major: The major version associated with the release. For example,
 *   if the tag for the release is 7.x-2.3, you would use "2" here,
 * - version_patch: The minor version associated with the release. Using the
 *   above 7.x-2.3 example, you would use "3" here,
 * - status: The status of the release (e.g. "published"),
 * - release_link: A fully qualified URL representing the release,
 * - download_link: A fully qualified URL representing a packaged archive of the
 *   project,
 * - filesize: The size, in bytes, of the packaged release archive,
 * - mdhash: The md5 checksum of the packaged release archive.
 *
 * @param string $short_name
 *   The short name of the project.
 *
 * @param string $api_version
 *   The Drupal API version of the project (e.g. 7.x).
 *
 * @param array $info
 *   The $info array for this particular project/API version combination as
 *   provided by your hook_project_src_info() implementation.
 *
 * @return array
 *   See above for details.
 */
function hook_project_src_releases($short_name, $api_version, $info) {
  // The 7.x-3.0 release of My Views Fork.
  $releases['7.x']['my_views_fork']['7.x-3.0'] = array(
    'name' => 'my_views_fork 7.x-3.0',
    'version' => '7.x-3.0',
    'tag' => '7.x-3.0',
    'date' => 1345423902,
    'version_major' => 3,
    'version_patch' => 0,
    'status' => 'published',
    'release_link' => 'http://example.com/project/my_views_fork/releases/7.x-3.0',
    'download_link' => 'http://ftp.example.com/files/projects/my_views_fork-7.x-3.0.tar.gz',
    'filesize' => '10132',
    'mdhash' => '675288f8194d9eb34c28f2f7cffab8ad',
  );

  // Potentially some other releases...
  $releases['7.x']['my_views_fork']['7.x-3.1'] = array(/*...*/);
  $releases['6.x']['my_other_module']['6.x-2.1'] = array(/*...*/);

  $available = isset($releases[$api_version][$short_name]);
  return $available ? $releases[$api_version][$short_name] : array();
}


/**
 * Allows you to modify release definitions at runtime.
 *
 * @param array &$releases
 *   An array of releases exactly as described in hook_project_src_releases().
 *   that this is called for all projects and when projects are loaded on a
 *   module-by-module basis.
 *
 * @param array $info
 *   The $info array for this particular project/API version combination as
 *   provided by your hook_project_src_info() implementation (used for context).
 */
function hook_project_src_releases_alter(&$releases, $info) {
  // Maybe for some reason, we want to translate the name.
  $releases['7.x-3.0']['name'] = t('My Views Fork');
}


/**
 * Returns an array of term definitions for a given project or release.
 *
 * A term definition is a simple enumerated array where the 0th value is the
 * term name and the 1st value is the term value.
 *
 * @param string $type
 *   The type of term to return (one of "project" or "release").
 *
 * @param string $short_name
 *   The short name of the project.
 *
 * @param string $api_version
 *   The Drupal API version of the project (e.g. 7.x).
 *
 * @param array $info
 *   The $info array for this particular project/API version combination as
 *   provided by your hook_project_src_info() implementation.
 *
 * @return array
 *   An array of term definitions; each term definition is a simple enumerated
 *   array whose 0th value is the term name and whose 1st value is the term
 *   value. See code examples below.
 */
function hook_project_src_terms($type, $short_name, $api_version, $info) {
  // My Views Fork is a very buggy module; all releases are bug fixes.
  if ($type == 'release' && $short_name == 'my_views_fork') {
    return array(
      array('Release type', 'Bug fixes'),
    );
  }
}


/**
 * Allows you to modify term definitions at runtime.
 *
 * @param array &$terms
 *   An array of terms as returned by hook_project_src_terms().
 *
 * @param array $info
 *   The $info array for this particular project/API version combination as
 *   provided by your hook_project_src_info() implementation (used for context).
 *
 * @param string $type
 *   The type of term being processed (used for context).
 */
function hook_project_src_terms_alter(&$terms, $info, $type) {
  // Erase all terms for one particular project.
  if ($type == 'project' && $info['short_name'] == 'my_views_fork') {
    $terms = array();
  }
}


/**
 * Returns an array of Project Source settings definitions.
 *
 * A setting definition is a custom configuration that applies globally for all
 * API versions of a given project (e.g. the type of Drupal extension--module,
 * theme, theme engine, distro, etc). You can use this hook to define your own
 * project settings.
 *
 * Use project_src_get_project_settings() to load settings. Users will see and
 * be able to configure these settings at /admin/config/development/project-src.
 *
 * Project Source settings definitions should be keyed by setting ID and must
 * include an "element" and a "scope" attribute. Detailed requirements follow.
 * - scope: A string that represents the scope for which this variable applies.
 *   Possible values:
 *   - project: For settings that apply once for the whole project regardless of
 *     API version, branches, releases, etc.
 *   - api_version: For settings that apply once per Drupal API version (e.g.
 *     6.x or 7.x).
 * - element: An array structured like any render array element, though probably
 *   best structured like a Form API element. Some caveats:
 *   - #title: This will be stripped out when rendering the form element itself,
 *     but will be used as the column header for the table column.
 *   - #description: This will be stripped out when rendered within the table.
 *   - #default_value: If provided, this will be the default value shown on the
 *     form within the table if no value has been saved yet. If a value has been
 *     saved, this will be ignored. If no #default_value is provided, it will be
 *     assumed as NULL. Also note that just because you provide a default value
 *     here, it doesn't mean that the value will show up when you load settings
 *     using the project_src_get_project_settings() function. In fact, the ID
 *     specified may not even appear; sanity check accordingly!
 * - #weight: (Optional) An arbitrary integer used to order the configuration's
 *   place along side all other configurations in the table column. Note that
 *   this element is on the definition itself, not the form element above.
 *
 * @return array
 *   See above for details.
 *
 * @see project_src_get_project_settings()
 * @see project_src_settings_row()
 */
function hook_project_src_settings() {
  // Provide a Project Source project configuration to override the creator.
  $settings['creator_override'] = array(
    'scope' => 'project',
    'element' => array(
      '#title' => t('Creator override'),
      '#type' => 'textfield',
      '#default_value' => 'Not Merlin of Chaos',
    ),
    // We want this to be placed at the very left, next to the project name.
    '#weight' => -100,
  );

  // Provide a custom element at the API version scope.
  $settings['my_custom_element'] = array(
    'scope' => 'api_version',
    'element' => array(
      '#title' => t('Custom element'),
      '#type' => 'select',
      '#options' => array(1, 2, 3),
    ),
  );

  return $settings;
}


/**
 * Allows you to modify Project Source Settings definitions at runtime.
 *
 * @param array $settings
 *   The settings array for the given setting to be modified.
 *
 * @param array $context
 *   An associative array containing useful contextual information for altering
 *   the settings definition. Possible keys include:
 *   - project: An array representing a Project Source project's definition.
 *   - settings: The existing settings for the given Project settings (if any).
 *
 * @param string $setting
 *   A string representing the ID of the given setting.
 *
 * @see project_src_settings_row()
 * @see hook_project_src_settings()
 */
function hook_project_src_settings_alter(&$settings, $context, $setting) {
  // Limit reactions to our custom element.
  if ($setting == 'my_custom_element') {
    $project = $context['project'];
    // For the "my_views_fork" project, 7.x version...
    if ($project['short_name'] == 'my_views_fork' && $project['api_version'] == '7.x') {
      // Remove the 1st option, perhaps because it's not valid.
      unset($settings['element']['#options'][1]);
    }
  }
}


/**
 * @}
 */
