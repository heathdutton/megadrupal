<?php

/**
 * Implements hook_rules_action_info().
 */
function rules_download_rules_action_info() {
  // We have two actions that share very similar action definition
  $action = array(
    'parameter' => array(
      'filename' => array(
        'type' => 'text',
        'label' => t('Filename'),
        'description' => t('Optional filename, if left empty, the filename from the response, if available, will be used. Otherwise it will fallback to a default one.'),
        'restriction' => 'input',
        'optional' => TRUE,
      ),
      'uri_scheme' => array(
        'type' => 'text',
        'label' => t('Save destination'),
        'description' => t('Select where the final files should be stored. Private file storage has significantly more overhead than public files, but allows restricted access to files within this field.'),
        'restriction' => 'input',
        'options list' => 'rules_download_scheme_options'
      ),
      'file_directory' => array(
        'type' => 'text',
        'label' => t('File directory'),
        'description' => t('Optional subdirectory within the save destination where files will be stored. Do not include preceding or trailing slashes.'),
        'restriction' => 'input',
        'optional' => TRUE,
      ),
      'temporary' => array(
        'type' => 'boolean',
        'label' => t('Temporary?'),
        'description' => t('Saves the file as temporary, useful if you just want it stored for the duration of the rules evaluation. It will be cleared on cron after an internally set period of time (aprox. 6h).'),
        'restriction' => 'input',
        'default value' => 0,
      ),
      'replace' => array(
        'type' => 'integer',
        'label' => t('Replace behavior'),
        'description' => t('Replace behavior when the destination file already exists.'),
        'restriction' => 'input',
        'options list' => 'rules_download_file_options_list',
        'default value' => FILE_EXISTS_RENAME,
      ),
    ),
    'group' => t('Rules download'),
    'provides' => array(
      'rules_download_file' => array(
        'type' => 'file',
        'label' => t('Downloaded file'),
      ),
    ),
    'callbacks' => array(
      'validate' => 'rules_download_action_validate',
    ),
  );

  // Merge common action definition with the specifics
  $items['rules_download_action_generate_path'] = array_merge_recursive(array(
    'label' => t('Download a file from the internet'),
    'parameter' => array(
      'path' => array(
        'type' => 'text',
        'label' => t('Drupal path'),
        'description' => t('This can be an internal Drupal path such as %example or an external URL such as %drupal. Enter %front to link to the front page.', array('%front' => '<front>', '%example' => 'node/1', '%drupal' => 'http://drupal.org')),
        'restriction' => 'input',
      ),
      'cookie' => array(
        'type' => 'boolean',
        'label' => t('Send cookie?'),
        'description' => t('Sends the cookie information as sent by the browser to the request. This is useful if you want the request to use the session information, such as the logged in user.'),
        'restriction' => 'input',
      ),
    )
  ), $action);

  return $items;
}

/**
 * Rules action: Fetches a content through HTTP and save it as a file
 */
function rules_download_action_generate_path($path, $cookie, $filename, $uri_scheme, $file_directory, $temporary, $replace, $settings, $state, $element) {
  // Flag to prevent infinite loops
  if (isset($_GET['drupal_rules_download'])) {
    throw new RulesEvaluationException('Action triggerred when responding to a request from this action, preventing loops. Request: @uri', array('@uri' => request_uri()), NULL, RulesLog::WARN);
  }

  $url_options = array(
    'absolute' => TRUE,
  );
  if ($path == '<front>') {
    $url = url('', $url_options);
  }
  elseif (!url_is_external($path)) {
    $url_parsed = drupal_parse_url($path);
    $options = $url_options;

    if (!empty($url_parsed['fragment'])) {
      $options['fragment'] = $url_parsed['fragment'];
    }
    if (!empty($url_parsed['query'])) {
      $options['query'] = $url_parsed['query'];
    }

    $url = url($url_parsed['path'], $options);
  }
  else {
    // URL should be external now
    $url = $path;
  }

  // Check if URL belongs to this site and adds an URL parameter that will
  // make ignore this rule if executed when fetching the file.
  // This is not 100% fail proof, as if the url you are accesing the site
  // is different from the url you are using, but it is still the same site
  // it will eventually fail, but this should be very very rarely
  $site = url('', $url_options);
  $pattern = preg_replace('/^https?/', '^https?', preg_quote($site, '/'));
  if (preg_match('/' . $pattern . '/', $url)) {
    // Adds a special URL argument just in case we are accessing this site
    // to prevent infinite loops
    $url_parsed = parse_url($url);
    $url_parsed['query'] = (isset($url_parsed['query']) && $url_parsed['query'] ? $url_parsed['query'] . '&' : '') . 'drupal_rules_download=1';
    $url = (isset($url_parsed['scheme']) ? $url_parsed['scheme'] . '://' : '') .
           (isset($url_parsed['user']) ? $url_parsed['user'] . ':' : '') .
           (isset($url_parsed['pass']) ? $url_parsed['pass'] . '@' : '') .
           (isset($url_parsed['host']) ? $url_parsed['host'] : '') .
           (isset($url_parsed['port']) ? ':' . $url_parsed['port'] : '') .
           (isset($url_parsed['path']) ? $url_parsed['path'] : '') .
           (isset($url_parsed['query']) ? '?' . $url_parsed['query'] : '') .
           (isset($url_parsed['fragment']) ? '#' . $url_parsed['fragment'] : '');
  }

  $headers = array();
  // Add cookie so that we can use the same access as the logged in user, if present.
  if ($cookie) {
    $headers = array(
      'Cookie' => $_SERVER['HTTP_COOKIE'],
    );
  }
  // Fetch the content
  $response = drupal_http_request($url, array('headers' => $headers));

  // Check response
  if ($response->code == '200') {
    $data = $response->data;
  }
  else {
    throw new RulesEvaluationException('Error fetching @url - @code: @error', array('@url' => $url, '@code' => $response->code, '@error' => $response->error));
  }

  if (!$filename) {
    // Tries to get the filename from the response
    if (isset($response->headers['content-disposition']) && preg_match('/filename="(.*?)"/', $response->headers['content-disposition'], $matches)) {
      $filename = $matches[1];
    }
    else {
      preg_match('/\/?([^\/]*?)(?:$|\#|\?)/', $path, $matches);
      if (!empty($matches)) {
        $filename = $matches[1];
      }
    }
  }

  if (empty($filename)) {
    $filename = 'rules_download.unknown';
  }

  if (function_exists('transliteration_clean_filename')) {
    $filename = transliteration_clean_filename($filename, language_default('language'));
  }

  $destination = $uri_scheme . '://' . trim($file_directory, '\\/') . '/' . $filename ;

  $file = file_save_data($data, $destination, $replace);

  if (empty($file)) {
    throw new RulesEvaluationException('Error saving the downloaded content to @path', array('@path' => $destination));
  }

  if ($temporary) {
    // Make it temporary and store the file again
    $file->status &= ~FILE_STATUS_PERMANENT;
    file_save($file);
  }

  $file->display = 1;

  return array('rules_download_file' => $file);
}

/**
 * Rules action callback for validation.
 *
 * Makes sure the directory is there and writable, tries to create it as well.
 */
function rules_download_action_validate($element) {
  $destination = $element->settings['uri_scheme'] . '://' . trim($element->settings['file_directory'], '\\/');
  if (isset($destination) && !file_prepare_directory($destination, FILE_CREATE_DIRECTORY|FILE_MODIFY_PERMISSIONS)) {
    $error = t('The save directory %directory for the file could not be created or is not accessible.', array('%directory' => $destination));
    throw new RulesIntegrityException($error, array($element, 'parameter', 'file_directory'));
  }
}

/**
 * Options list callback for file replacem methods.
 *
 * @see file.inc
 */
function rules_download_file_options_list() {
  return array(
    FILE_EXISTS_RENAME => t('Rename - Append _{incrementing number} until the filename is unique'),
    FILE_EXISTS_REPLACE => t('Replace - Replace the existing file'),
    FILE_EXISTS_ERROR => t('Error - Do nothing and return FALSE'),
  );
}
