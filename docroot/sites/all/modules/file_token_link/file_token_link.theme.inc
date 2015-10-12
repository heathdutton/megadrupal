<?php

/**
 * @file
 * Theme callbacks for the file token link module.
 */

/**
 * Copy of theme_file_file_link() for linking to the file download URL.
 *
 * @see theme_file_file_link()
 */
function theme_file_token_link($variables) {
  $file = $variables['file'];

  // Provide the default link url.
  if (!isset($variables['url'])) {
    $variables['url'] = '[file:download-url]';
  }
  $variables['url'] = token_replace($variables['url'], array('file' => $file), array('clear' => TRUE, 'sanitize' => empty($uri['options']['html'])));

  $icon = '';
  if ($variables['icon']) {
    $icon_directory = $variables['icon_directory'];
    $icon = theme('file_icon', array('file' => $file, 'icon_directory' => $icon_directory));
  }

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples
  $uri['options']['attributes']['type'] = $file->filemime . '; length=' . $file->filesize;

  // Provide the default link text.
  if (!isset($variables['text'])) {
    $variables['text'] = t('[file:name]');
  }

  // Peform unsanitized token replacement if $uri['options']['html'] is empty
  // since then l() will escape the link text.
  $variables['text'] = token_replace($variables['text'], array('file' => $file), array('clear' => TRUE, 'sanitize' => empty($uri['options']['html'])));

  $output = '<span class="file">' . $icon . ' ' . l($variables['text'], $variables['url']);
  if ($variables['size']) {
    $output .= ' ' . '<span class="file-size">(' . format_size($file->filesize) . ')</span>';
  }
  $output .= '</span>';

  return $output;
}
