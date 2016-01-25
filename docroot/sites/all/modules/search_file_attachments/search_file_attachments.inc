<?php

/**
 * @file
 * Heler functions, to hold the .module file clean and smart.
 */

/**
 * Check if java is available and executable.
 *
 * @param string $path
 *   The full path to the Java binary. e.g. /usr/bin/java
 */
function search_file_attachments_check_java($path = NULL) {
  $path = $path ? $path : 'java';

  $temp = tempnam(file_directory_temp(), 'asa');
  if (strpos(ini_get('extension_dir'), 'MAMP/')) {
    $path = 'export DYLD_LIBRARY_PATH=""; ' . $path;
  }
  exec($path . ' -version > ' . $temp . ' 2>&1');
  $stderror = file_get_contents($temp);
  $found = preg_match('/Runtime Environment/', $stderror);

  return $found ? TRUE : FALSE;
}

/**
 * Returns the path to the java binary.
 *
 * @return string
 */
function search_file_attachments_java_path() {
  $settings_path = variable_get('search_file_attachments_java_path', '');
  $path = !empty($settings_path) ? $settings_path : 'java';
  return $path;
}

/**
 * Shutdown function to make sure we remember the last element processed.
 */
function search_file_attachments_shutdown() {
  global $_search_file_attachments_last_change, $_search_file_attachments_last_id;
  if ($_search_file_attachments_last_change && $_search_file_attachments_last_id) {
    variable_set('search_file_attachments_cron_last_change', $_search_file_attachments_last_change);
    variable_set('search_file_attachments_cron_last_id', $_search_file_attachments_last_id);
  }
}

/**
 * Load the files to be indexed.
 *
 * @param int $last_change
 *   The timestamp of the last indexed file, or the timestamp of the last time
 *   that files were indexed (if the last indexed file is also the one with the
 *   largest file ID).
 * @param int $last_id
 *   The highest file ID of all files that have been previously indexed.
 *
 * @return array
 *   The array with the file objects.
 */
function search_file_attachments_get_files($last_change = 0, $last_id = 0) {
  $limit = (int) variable_get('search_cron_limit', 10);
  $fids = array();

  $query = db_select('file_managed', 'f');
  $query->fields('f', array('fid'))
        ->condition('f.status', 1)
        ->condition(db_or()
          ->condition('f.fid', $last_id, '>')
          ->condition('f.timestamp', $last_change, '>'))
        ->orderBy('f.fid')
        ->range(0, $limit);
  $results = $query->execute();

  foreach ($results as $record) {
    $fids[] = $record->fid;
  }

  $files = !empty($fids) ? file_load_multiple($fids) : array();
  return $files;
}

/**
 * Extract the content of the given file.
 *
 * @param object $file
 *   The file object where the content should be extracted.
 *
 * @return string
 *   The extracted file content.
 */
function search_file_attachments_get_file_content($file) {
  $image_mimetypes = array('image/jpeg', 'image/jpg', 'image/tiff');

  if ($file->filemime == 'text/plain' || $file->filemime == 'text/x-diff') {
    $content = search_file_attachments_extract_simple($file);
  }
  elseif (in_array($file->filemime, $image_mimetypes)) {
    $content = search_file_attachments_extract_exif($file);
  }
  else {
    $content = search_file_attachments_extract_tika($file);
  }

  return (string) $content;
}

/**
 * Extract simple text.
 *
 * @param object $file
 *   The file object.
 *
 * @return mixed|string
 *   The extracted content.
 */
function search_file_attachments_extract_simple($file) {
  $content = file_get_contents(file_create_url($file->uri));
  $content = iconv("UTF-8", "UTF-8//IGNORE", $content);
  $content = htmlspecialchars(html_entity_decode($content, ENT_NOQUOTES, 'UTF-8'), ENT_NOQUOTES, 'UTF-8');
  $content = trim($content);
  return $content;
}

/**
 * Extract IPTC metadata from image.
 *
 * @param object $file
 *   The file object.
 *
 * @return string
 *   The extracted IPTC content.
 */
function search_file_attachments_extract_exif($file) {
  $content = '';
  $size = getimagesize(file_create_url($file->uri), $info);
  if (isset($info['APP13'])) {
    $iptc_raw = iptcparse($info['APP13']);
    if (empty($iptc_raw)) {
      return $content;
    }

    $tagmarker = search_file_attachments_exif_tagmarker();

    $iptc = array();
    foreach ($iptc_raw as $key => $value) {
      // Add only values from the defined iptc fields.
      if (array_key_exists($key, $tagmarker)) {
        $iptc_field_value = array();
        foreach ($value as $innerkey => $innervalue) {
          $innervalue = trim($innervalue);
          if (!empty($innervalue)) {
            $iptc_field_value[] = $innervalue;
          }
        }

        if (!empty($iptc_field_value)) {
          $iptc[$tagmarker[$key]] = implode(', ', $iptc_field_value);
        }
      }
    }

    foreach ($iptc as $key => $value) {
      $content .= " <strong>{$key}:</strong> {$value}";
    }
    $content = trim($content);
  }
  return $content;
}

/**
 * Extract file content with Apache Tika.
 *
 * @param object $file
 *   The file object.
 *
 * @return string
 *   The extracted content.
 * @throws Exception
 */
function search_file_attachments_extract_tika($file) {
  $filepath = file_create_url($file->uri);
  $tika_path = realpath(variable_get('search_file_attachments_tika_path', ''));
  $tika = realpath($tika_path . '/' . variable_get('search_file_attachments_tika_jar', ''));
  if (!($tika) || !is_file($tika)) {
    throw new Exception(t('Invalid path or filename for tika application jar.'));
  }

  // UTF-8 multibyte characters will be stripped by escapeshellargs().
  // So temporarily set the locale to UTF-8 so that the filepath remain valid.
  $backup_locale = setlocale(LC_CTYPE, '0');
  setlocale(LC_CTYPE, 'en_US.UTF-8');

  $param = '';
  $java_path = search_file_attachments_java_path();

  if ($file->filemime != 'audio/mpeg') {
    $param = ' -Dfile.encoding=UTF8 -cp ' . escapeshellarg($tika_path);
  }

  if (DIRECTORY_SEPARATOR == '\\') {
    // If we on windows, use an other methode to escape the file path strings,
    // to prevent problems with paths that contains spaces. Because the
    // PHP escapeshellarg() function handle these correct.
    $cmd = $java_path . $param . ' -jar "' . str_replace('"', '\\"', $tika) . '" -t "' . str_replace('"', '\\"', $filepath) . '"';
  }
  else {
    $cmd = $java_path . $param . ' -jar ' . escapeshellarg($tika) . ' -t ' . escapeshellarg($filepath);
    if (strpos(ini_get('extension_dir'), 'MAMP/')) {
      $cmd = 'export DYLD_LIBRARY_PATH=""; ' . $cmd;
    }
  }

  // Support utf-8 commands:
  // http://www.php.net/manual/pt_BR/function.shell-exec.php#85095
  $cmd = "LANG=en_US.utf-8; $cmd";
  // Restore the locale.
  setlocale(LC_CTYPE, $backup_locale);

  // Debug print.
  if (variable_get('search_file_attachments_debug', FALSE)) {
    $result = shell_exec($cmd . ' 2>&1');
    watchdog('search_file_attachments', '<p><strong>Tika Command:</strong> <code>%command</code></p><br /> <p><strong>Result:</strong> %result</p>', array(
      '%command' => $cmd,
      '%result' => $result,
    ));

    // Empty the result, if it contains an error message, so that the error
    // is not in the index.
    if (strpos($result, 'Exception in thread') !== FALSE) {
      $result = FALSE;
    }

    return $result;
  }

  return shell_exec($cmd);
}

/**
 * Defines the IPTC fields to be used for the search index.
 */
function search_file_attachments_exif_tagmarker() {
  $tagmarker = array(
    '2#005' => t('Object Name'),
    '2#015' => t('Category'),
    '2#020' => t('Supplementals'),
    '2#025' => t('Keywords'),
    '2#040' => t('Special Instructions'),
    '2#080' => t('By Line'),
    '2#085' => t('By Line Title'),
    '2#090' => t('City'),
    '2#092' => t('Sublocation'),
    '2#095' => t('Province State'),
    '2#100' => t('Country Code'),
    '2#101' => t('Country Name'),
    '2#105' => t('Headline'),
    '2#110' => t('Credits'),
    '2#115' => t('Source'),
    '2#116' => t('Copyright'),
    '2#118' => t('Contact'),
    '2#120' => t('Caption'),
    '2#122' => t('Caption Writer'),
  );

  // Allow other modules to alter defined IPTC fields.
  drupal_alter('search_file_attachments_exif_tagmarker', $tagmarker);

  return $tagmarker;
}

/**
 * Returns a string representing the current list of included file extensions.
 */
function search_file_extensions_included_extensions() {
  return variable_get('search_file_attachments_include_extensions', 'pdf, doc, docx, xls, xlsx, ppt, pptx, txt, rtf, jpg, jpeg, tiff, odt, ods, odp, odg, odc, ofd, odi, odm, epub');
}

/**
 * Get the mimetypes of the excluded file extensions.
 *
 * @param string $extensions
 *  A comma-separated string with file extensions.
 *
 * @return array
 *   The exluded mimetypes.
 */
function search_file_attachments_extensions_to_mimetypes($extensions) {
  $extensions = preg_replace('/[^a-zA-z0-9\/\-,]/', '', $extensions);
  $extensions = explode(',', $extensions);

  $mimetypes = array();
  if (!empty($extensions)) {
    // The default file map, defined in file.mimetypes.inc is quite big.
    // We only load it when necessary.
    include_once DRUPAL_ROOT . '/includes/file.mimetypes.inc';
    $mapping = file_mimetype_mapping();

    foreach ($extensions as $ext) {
      $ext = drupal_strtolower($ext);
      $mimetype = 'application/octet-stream';
      if (isset($mapping['extensions'][$ext])) {
        $mimetype_key = $mapping['extensions'][$ext];
        if (isset($mapping['mimetypes'][$mimetype_key])) {
          $mimetype = $mapping['mimetypes'][$mimetype_key];
        }
      }
      elseif (in_array($ext, $mapping['mimetypes'])) {
        $mimetype = $ext;
      }
      $mimetypes[] = $mimetype;
    }
  }

  return $mimetypes;
}
