<?php
/**
 * @file
 * Theme and helper functions for formatters that enable tracking.
 */

 /**
  * Theme function for the 'default' filefield formatter.
  */
function theme_filefield_track_link($variables) {
  $file = $variables['file'];
  $icon_directory = $variables['icon_directory'];

  $url = filefield_track_path($file);
  $icon = theme('file_icon', array('file' => $file, 'icon_directory' => $icon_directory));

  // Set options as per anchor format.
  // @see http://microformats.org/wiki/file-format-examples
  $options = array(
    'attributes' => array(
      'type' => $file->filemime . '; length=' . $file->filesize,
    ),
    'query' => array(
      'redirect' => $_GET['q'],
    ),
  );

  // Use the description as the link text if available.
  if (empty($file->description)) {
    $link_text = $file->filename;
  }
  else {
    $link_text = $file->description;
    $options['attributes']['title'] = check_plain($file->filename);
  }

  return '<span class="file">' . $icon . ' ' . l($link_text, $url, $options) . '</span>';
}

/**
 * Theme function for the 'Table of files' formatter.
 */
function theme_filefield_track_formatter_table($variables) {
  $header = array(t('Attachment'), t('Size'));
  $rows = array();
  foreach ($variables['items'] as $delta => $item) {
    $rows[] = array(
      theme('filefield_track_link', array('file' => (object) $item)),
      format_size($item['filesize']),
    );
  }

  return empty($rows) ? '' : theme('table', array('header' => $header, 'rows' => $rows));
}
