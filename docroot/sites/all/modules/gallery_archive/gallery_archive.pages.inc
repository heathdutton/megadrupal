<?php

/**
 * @file
 * Pages for Gallery Archives.
 */

/**
 * Callback for Gallery Archive page.
 */
function gallery_archive_gallery($node) {
  // Define some variables.
  $directory_name = $node->nid . '-' . time(); // nid + Current UNIX timestamp (must be unique)
  $tmp_directory = file_directory_temp() . '/' . $directory_name;
  $image_base_path = $_SERVER['DOCUMENT_ROOT'] . '/'; // Path to docroot
  $archive_name = $node->nid . '-archive'; // Name of final archive
  $file_directory_path = variable_get('file_' . file_default_scheme() . '_path', conf_path() . '/files'); // Drupal's file directory path
  $archive_path = $file_directory_path . '/gallery-archives/' . $archive_name; // Used for building a link

  // TODO - Before creating a new archive, check if (a) the archive already
  // exists, and (b) if the archive is more than a week old. We'll need to
  // come up with a better solution for stale data here, though... but I want
  // to be efficient!

  if (!file_exists($archive_path . '.tar.gz')) {
    // Create a new temporary directory to store the images.
    if (!mkdir($tmp_directory, 0775)) {
      return '<div>' . t("Error: Temporary directory could not be created. Please check your file system settings.") . '</div>';
    }

    // Get a list of all the images to be added to the archive.
    $image_field = variable_get('gallery_archive_gallery_image_field', 0);
    $images = $node->{$image_field}[LANGUAGE_NONE];

    // Copy all the full-resolution images into $tmp_directory.
    foreach ($images as $image) {
      // Get the path to the full-resolution image.
      $image_to_copy = drupal_realpath($image['uri']);

      // Copy image to $tmp_directory
      file_unmanaged_copy($image_to_copy, $tmp_directory, FILE_EXISTS_REPLACE);
    }

    // @TODO - If PHP is compiled with --enable-zip, allow .zip archive...
    // Opt 1: http://davidwalsh.name/create-zip-php
    // Opt 2: http://www.webandblog.com/hacks/zip-a-folder-on-the-server-with-php/

    // Create the archive using our tgz function.
    gallery_archive_tgz_archive_dir($tmp_directory, $archive_name, TRUE);
  }

  // Check to see if the Archive actually exists and has content.
  $archive_size = filesize($archive_path . '.tar.gz');

  // If it exists, return content with download link.
  if ($archive_size > 0) {
    $output = '<div>' . t("Click on the download link below to download this gallery:") . '</div>';
    $link_text = t("Download this gallery (!size)", array('!size' => _gallery_archive_format_bytes($archive_size)));
    $output .= l($link_text, $archive_path . '.tar.gz');
  }
  // Otherwise, show an error message.
  else {
    $output .= '<div>' . t("Error: No archive has been created. Please check your settings.") . '</div>';
  }

  return $output;
}
