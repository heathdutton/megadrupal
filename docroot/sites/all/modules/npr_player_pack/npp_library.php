<?php
/**
 * @file
 * Provides an MP3 URL scraper to be used by other NPP modules.
 */

/**
 * Retrives the URL of an MP3.
 *
 * @param string $filename
 *   URL of an MP3 or M3U.
 * 
 * @return string
 *   Either a string of and MP3 url or an empty string.
 */
function _npp_get_mp3($filename) {
  // Make sure an argument is provided.
  if (!$filename) {
    watchdog('_npp_get_mp3', 'No filename provided to function.', NULL, WATCHDOG_ERROR);
    return '';
  }

  $pointer = fopen($filename, 'r');
  if (!$pointer) {
    watchdog('_npp_get_mp3', 'Could not open given file.', NULL, WATCHDOG_ERROR);
    return '';
  }

  $headers = stream_get_meta_data($pointer);
  $is_mp3 = FALSE;

  // If "audio/mpeg" is in the header, it must be an MP3.
  foreach ($headers['wrapper_data'] as $header_value) {
    if (strpos(strtolower($header_value), 'audio/mpeg')) {
      return $filename;
    }
  }
  // If it's not an MP3, assume it's an M3U.
  // Make sure the M3U has one line.
  if (count($pointer) == 1) {
    $contents = file_get_contents($filename);
    // Ensure there's actually an MP3 URL in the M3U file.
    if (preg_match('/^http.*\.mp3$/', $contents)) {
      return $contents;
    }
    else {
      watchdog('_npp_get_mp3', 'No mp3 file contained in playlist.', NULL, WATCHDOG_ERROR);
      return '';
    }
  }
  else {
    watchdog('_npp_get_mp3', 'M3U file contains multiple lines.', NULL, WATCHDOG_ERROR);
    return '';
  }
}
