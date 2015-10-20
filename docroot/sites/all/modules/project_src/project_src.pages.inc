<?php

/**
 * @file
 * Page callbacks for the Project Source module.
 */


/**
 * Menu callback for release-history, returned as XML. This matches the
 * output of http://updates.drupal.org/release-history/%/%. If it's not a custom
 * project, we should just get the contents of that URL.
 *
 * @param $short_name
 *   The short name of the project.
 *
 * @param $api_version
 *   The API version for the project (e.g. 7.x).
 */
function project_src_get_project($short_name, $api_version) {
  // Get a list of all declared custom projects to check against.
  $projects = project_src_get_projects($api_version);

  // If the custom project and version is registered, render its XML.
  if (isset($projects[$short_name])) {
    $project = $projects[$short_name];
    $project_copy = $projects[$short_name];

    // Return releases and terms for each project, then return rendered output.
    $project['releases'] = module_invoke_all('project_src_releases', $short_name, $api_version, $project);
    drupal_alter('project_src_releases', $project['releases'], $project_copy);
    $term_type = 'project';
    $project['terms'] = module_invoke_all('project_src_terms', $term_type, $short_name, $api_version, $project);
    drupal_alter('project_src_terms', $project['terms'], $project_copy, $term_type);

    drupal_add_http_header('Content-type', 'text/xml; charset=utf-8');
    return theme('project_src_xml', $project);
  }
  // If it exists but the specified version does not, render the error.
  elseif (isset($projects[$short_name])) {
    drupal_add_http_header('Content-type', 'text/xml; charset=utf-8');
    return theme('project_src_xml_error', array(
      'short_name' => $short_name,
      'api_version' => $api_version,
    ));
  }
  // Otherwise, pass through the request to drupal.org and return it.
  else {
    $response = drupal_http_request(PROJECT_SRC_PASSTHROUGH . '/' . $short_name . '/' . $api_version);

    foreach ($response->headers as $header => $value) {
      drupal_add_http_header($header, $value);
    }

    return $response->data;
  }
}
