<?php


/**
 * Preprocessor for the main Project XML.
 */
function template_preprocess_project_src_xml(&$vars) {
  // Project-wide terms.
  $vars['terms'] = theme('project_src_terms', $vars);

  // Release-specific variables.
  $vars['releases'] = theme('project_src_releases', $vars);
}


/**
 * Preprocessor for project release xml.
 */
function template_preprocess_project_src_release(&$release) {
  $release['terms'] = theme('project_src_terms', $release['terms']);
}


/**
 * Preprocessor for Project terms.
 */
function template_preprocess_project_src_term(&$term) {
  $term['name'] = $term[0];
  $term['value'] = $term[1];
}


/**
 * Hard function to theme a list of releases.
 */
function project_src_theme_releases($vars) {
  $return = '';

  if (!empty($vars['releases'])) {
    foreach ($vars['releases'] as $release) {
      if (is_array($release)) {
        $return .= theme('project_src_release', $release);
      }
    }
  }

  return $return;
}


/**
 * Hard function to theme a list of project terms.
 */
function project_src_theme_terms($vars) {
  $return = '';

  if (!empty($vars['terms'])) {
    foreach ($vars['terms'] as $term) {
      if (is_array($term)) {
        $return .= theme('project_src_term', $term);
      }
    }
  }

  return $return;
}


/**
 * Hard function to theme an error XML page.
 */
function project_src_theme_error($vars) {
  return '<?xml version="1.0" encoding="utf-8"?>
  <error>' . t('No release history available for @project @version.', array(
    '@project' => $vars['short_name'],
    '@version' => $vars['api_version'],
  )) . '</error>' . "\n";
}
