<?php

/**
 * @file
 * Code required only when comparing available updates to existing data.
 */

module_load_include('inc', 'update', 'update.compare');

/**
 * Calculates the current update status of all projects on the site.
 *
 * The results of this function are expensive to compute, especially on sites
 * with lots of modules or themes, since it involves a lot of comparisons and
 * other operations. Therefore, we cache the results into the {cache_update}
 * table using the 'update_project_data' cache ID. However, since this is not
 * the data about available updates fetched from the network, it is ok to
 * invalidate it somewhat quickly. If we keep this data for very long, site
 * administrators are more likely to see incorrect results if they upgrade to a
 * newer version of a module or theme but do not visit certain pages that
 * automatically clear this cache.
 *
 * @param array $available
 *   Data about available project releases.
 *
 * @return
 *   An array of installed projects with current update status information.
 *
 * @see upgrade_status_get_available()
 * @see update_get_projects()
 * @see update_process_project_info()
 * @see update_project_cache()
 */
function upgrade_status_calculate_project_data($available) {
  // Retrieve the projects from cache, if present.
#  $projects = update_project_cache('upgrade_status_project_data');
  // US: Directly use private cache getter to skip Update's cache invalidation.
  $projects = _update_cache_get('upgrade_status_project_data');
  // If $projects is empty, then the cache must be rebuilt.
  // Otherwise, return the cached data and skip the rest of the function.
  if (!empty($projects)) {
    // US: Return only the data part, not entire $projects array.
    return $projects->data;
  }
  $projects = update_get_projects();
  update_process_project_info($projects);
  foreach ($projects as $project => $project_info) {
    if (isset($available[$project])) {
      upgrade_status_calculate_project_update_status($project, $projects[$project], $available[$project]);
    }
    else {
      $projects[$project]['status'] = UPDATE_UNKNOWN;
      $projects[$project]['reason'] = t('No available releases found');
    }
  }
  // Give other modules a chance to alter the status (for example, to allow a
  // contrib module to provide fine-grained settings to ignore specific
  // projects or releases).
  drupal_alter('update_status', $projects);

  // US: Same for us, afterwards.
  drupal_alter('upgrade_status', $projects);

  // Cache the site's update status for at most 1 hour.
  _update_cache_set('upgrade_status_project_data', $projects, REQUEST_TIME + 3600);
  return $projects;
}

/**
 * Calculates the current update status of a specific project.
 *
 * This function is the heart of the update status feature. For each project it
 * is invoked with, it first checks if the project has been flagged with a
 * special status like "unsupported" or "insecure", or if the project node
 * itself has been unpublished. In any of those cases, the project is marked
 * with an error and the next project is considered.
 *
 * If the project itself is valid, the function decides what major release
 * series to consider. The project defines what the currently supported major
 * versions are for each version of core, so the first step is to make sure the
 * current version is still supported. If so, that's the target version. If the
 * current version is unsupported, the project maintainer's recommended major
 * version is used. There's also a check to make sure that this function never
 * recommends an earlier release than the currently installed major version.
 *
 * Given a target major version, the available releases are scanned looking for
 * the specific release to recommend (avoiding beta releases and development
 * snapshots if possible). For the target major version, the highest patch level
 * is found. If there is a release at that patch level with no extra ("beta",
 * etc.), then the release at that patch level with the most recent release date
 * is recommended. If every release at that patch level has extra (only betas),
 * then the latest release from the previous patch level is recommended. For
 * example:
 *
 * - 1.6-bugfix <-- recommended version because 1.6 already exists.
 * - 1.6
 *
 * or
 *
 * - 1.6-beta
 * - 1.5 <-- recommended version because no 1.6 exists.
 * - 1.4
 *
 * Also, the latest release from the same major version is looked for, even beta
 * releases, to display to the user as the "Latest version" option.
 * Additionally, the latest official release from any higher major versions that
 * have been released is searched for to provide a set of "Also available"
 * options.
 *
 * Finally, and most importantly, the release history continues to be scanned
 * until the currently installed release is reached, searching for anything
 * marked as a security update. If any security updates have been found between
 * the recommended release and the installed version, all of the releases that
 * included a security fix are recorded so that the site administrator can be
 * warned their site is insecure, and links pointing to the release notes for
 * each security update can be included (which, in turn, will link to the
 * official security announcements for each vulnerability).
 *
 * This function relies on the fact that the .xml release history data comes
 * sorted based on major version and patch level, then finally by release date
 * if there are multiple releases such as betas from the same major.patch
 * version (e.g., 5.x-1.5-beta1, 5.x-1.5-beta2, and 5.x-1.5). Development
 * snapshots for a given major version are always listed last.
 *
 * @param $project
 *   An array containing information about a specific project.
 * @param $project_data
 *   An array containing information about a specific project.
 * @param $available
 *   Data about available project releases of a specific project.
 */
function upgrade_status_calculate_project_update_status($project, &$project_data, $available) {
  foreach (array('title', 'link') as $attribute) {
    if (!isset($project_data[$attribute]) && isset($available[$attribute])) {
      $project_data[$attribute] = $available[$attribute];
    }
  }

  // If the project status is marked as something bad, there's nothing else
  // to consider.
  if (isset($available['project_status'])) {
    switch ($available['project_status']) {
      case 'insecure':
        $project_data['status'] = UPDATE_NOT_SECURE;
        if (empty($project_data['extra'])) {
          $project_data['extra'] = array();
        }
        $project_data['extra'][] = array(
          'class' => array('project-not-secure'),
          'label' => t('Project not secure'),
          'data' => t('This project has been labeled insecure by the Drupal security team, and is no longer available for download. Immediately disabling everything included by this project is strongly recommended!'),
        );
        break;
      // US: Maintainers are doing lots of nightmares with in development
      // releases, so we have to take unpublished, revoked, and unsupported
      // into account.
      case 'unpublished':
      case 'revoked':
      case 'unsupported':
        break;
      case 'not-fetched':
        $project_data['status'] = UPDATE_NOT_FETCHED;
        $project_data['reason'] = t('Failed to get available update data.');
        break;

      default:
        // Assume anything else (e.g. 'published') is valid and we should
        // perform the rest of the logic in this function.
        break;
    }
  }

  if (!empty($project_data['status'])) {
    // We already know the status for this project, so there's nothing else to
    // compute. Record the project status into $project_data and we're done.
    $project_data['project_status'] = $available['project_status'];
    return;
  }

  // Figure out the target major version.
  $existing_major = $project_data['existing_major'];
  $supported_majors = array();
  if (isset($available['supported_majors'])) {
    $supported_majors = explode(',', $available['supported_majors']);
  }
  elseif (isset($available['default_major'])) {
    // Older release history XML file without supported or recommended.
    $supported_majors[] = $available['default_major'];
  }

  if (in_array($existing_major, $supported_majors)) {
    // Still supported, stay at the current major version.
    $target_major = $existing_major;
  }
  elseif (isset($available['recommended_major'])) {
    // Since 'recommended_major' is defined, we know this is the new XML
    // format. Therefore, we know the current release is unsupported since
    // its major version was not in the 'supported_majors' list. We should
    // find the best release from the recommended major version.
    $target_major = $available['recommended_major'];
    // US: Projects may port from 6.x-1.x to 7.x-2.x to change their APIs.
#    $project_data['status'] = UPDATE_NOT_SUPPORTED;
  }
  elseif (isset($available['default_major'])) {
    // Older release history XML file without recommended, so recommend
    // the currently defined "default_major" version.
    $target_major = $available['default_major'];
  }
  else {
    // Malformed XML file? Stick with the current version.
    $target_major = $existing_major;
  }

  // US: Some projects are renumbering to 1.x with each new core version.
#  $target_major = max($existing_major, $target_major);

  $release_patch_changed = '';
  $patch = '';

  // If the project is marked as UPDATE_FETCH_PENDING, it means that the
  // data we currently have (if any) is stale, and we've got a task queued
  // up to (re)fetch the data. In that case, we mark it as such, merge in
  // whatever data we have (e.g. project title and link), and move on.
  if (!empty($available['fetch_status']) && $available['fetch_status'] == UPDATE_FETCH_PENDING) {
    $project_data['status'] = UPDATE_FETCH_PENDING;
    $project_data['reason'] = t('No available update data');
    $project_data['fetch_status'] = $available['fetch_status'];
    return;
  }

  // US: Of course, not yet ported don't have any releases. :) Remove handling.

  foreach ($available['releases'] as $version => $release) {

    // US: insecure, unpublished, revoked, unsupported have no meaning.

    // See if this is a higher major version than our target and yet still
    // supported. If so, record it as an "Also available" release.
    // Note: some projects have a HEAD release from CVS days, which could
    // be one of those being compared. They would not have version_major
    // set, so we must call isset first.
    if (isset($release['version_major']) && $release['version_major'] > $target_major) {
      if (in_array($release['version_major'], $supported_majors)) {
        if (!isset($project_data['also'])) {
          $project_data['also'] = array();
        }
        if (!isset($project_data['also'][$release['version_major']])) {
          $project_data['also'][$release['version_major']] = $version;
          $project_data['releases'][$version] = $release;
        }
      }

      // US: Some projects are renumbering to 1.x with each new core version.
#      continue;
    }

    // Look for the 'latest version' if we haven't found it yet. Latest is
    // defined as the most recent version for the target major version.
    if (!isset($project_data['latest_version'])
        && $release['version_major'] == $target_major) {
      $project_data['latest_version'] = $version;
      $project_data['releases'][$version] = $release;
    }

    // Look for the development snapshot release for this branch.
    if (!isset($project_data['dev_version'])
        && $release['version_major'] == $target_major
        && isset($release['version_extra'])
        && $release['version_extra'] == 'dev') {
      $project_data['dev_version'] = $version;
      $project_data['releases'][$version] = $release;
    }

    // Look for the 'recommended' version if we haven't found it yet (see
    // phpdoc at the top of this function for the definition).
    if (!isset($project_data['recommended'])
        && $release['version_major'] == $target_major
        && isset($release['version_patch'])) {
      if ($patch != $release['version_patch']) {
        $patch = $release['version_patch'];
        $release_patch_changed = $release;
      }
      if (empty($release['version_extra']) && $patch == $release['version_patch']) {
        $project_data['recommended'] = $release_patch_changed['version'];
        $project_data['releases'][$release_patch_changed['version']] = $release_patch_changed;
      }
    }

    // US: Don't stop searching, even if we hit the currently installed version.

    // US: Ignore dev snapshot handling.

    // US: Ignore security updates.
  }

  // If we were unable to find a recommended version, then make the latest
  // version the recommended version if possible.
  if (!isset($project_data['recommended']) && isset($project_data['latest_version'])) {
    $project_data['recommended'] = $project_data['latest_version'];
    // US: No recommended version means there's a dev snapshot.
    $project_data['status'] = UPGRADE_STATUS_DEVELOPMENT;
    $project_data['reason'] = t('In development');
  }

  //
  // Check to see if we need an update or not.
  //

  // US: Skip security update status handling.

  // US: Check new Drupal core improvements, regardless of what's figured out
  // below.
  if (upgrade_status_moved_into_core($projects, $project)) {
    $project_data += $projects[$project];
    $project_data['status'] = UPGRADE_STATUS_CORE;
    $project_data['reason'] = t('In core');
  }

  if (isset($project_data['status'])) {
    // If we already know the status, we're done.
    return;
  }

  // If we don't know what to recommend, there's nothing we can report.
  // Bail out early.
  // US: Commenting this out causes core to work again. Hm.
#  if (!isset($project_data['recommended'])) {
#    $project_data['status'] = UPDATE_UNKNOWN;
#    $project_data['reason'] = t('No available releases found');
#    return;
#  }

  // US: Ignore dev snapshot handling.

  // Figure out the status, based on what we've seen and the install type.
  // US: If we were not yet able to assign a status, this project already
  // provides a stable release, so remove handling of official and dev releases.
  switch ($project_data['install_type']) {
    case 'official':
    case 'dev':
      $project_data['status'] = UPGRADE_STATUS_STABLE;
      $project_data['reason'] = t('Available');
      break;

    default:
      // US: A project without releases may be in core.
      if (upgrade_status_moved_into_core($projects, $project)) {
        $project_data['status'] = UPGRADE_STATUS_CORE;
        $project_data['reason'] = t('In core');
      }
      else {
        $project_data['status'] = UPDATE_UNKNOWN;
        $project_data['reason'] = t('Invalid info');
      }
  }
}