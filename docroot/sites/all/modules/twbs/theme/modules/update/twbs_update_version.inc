<?php

/**
 * Returns HTML for the version display of a project.
 *
 * @param array $variables
 *   An associative array containing:
 *   - version: An array of data about the latest released version, containing:
 *     - version: The version number.
 *     - release_link: The URL for the release notes.
 *     - date: The date of the release.
 *     - download_link: The URL for the downloadable file.
 *   - tag: The title of the project.
 *   - class: A string containing extra classes for the wrapping table.
 *
 * @ingroup themeable
 */
function twbs_update_version($variables) {
  $version = $variables['version'];
  $tag = $variables['tag'];
  $class = implode(' ', $variables['class']);

  $output = '';
  $output .= '<table class="version ' . $class . '">';
  $output .= '<tr>';
  $output .= '<td class="version-title">' . $tag . "</td>\n";
  $output .= '<td class="version-details">';
  $output .= l($version['version'], $version['release_link']);
  $output .= ' <span class="version-date">(' . format_date($version['date'], 'custom', 'Y-M-d') . ')</span>';
  $output .= "</td>\n";
  $output .= '<td class="version-links">';
  $links = array();
  $links['update-download'] = array(
    'title' => t('Download'),
    'href' => $version['download_link'],
  );
  $links['update-release-notes'] = array(
    'title' => t('Release notes'),
    'href' => $version['release_link'],
  );
  $output .= theme('links__update_version', array('links' => $links));
  $output .= '</td>';
  $output .= '</tr>';
  $output .= "</table>\n";
  return $output;
}
