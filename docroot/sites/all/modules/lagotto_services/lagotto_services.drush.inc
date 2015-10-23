<?php
/**
 * @file
 * Drush command integration for Lagotto services.
 */

/**
 * Implements hook_drush_command().
 */
function lagotto_services_drush_command() {
  $items['lagotto-lookup-id'] = array(
    'description' => dt('Lookup an article ID on a Lagotto server.'),
    'required-arguments' => 1,
    'aliases' => array('lgid'),
    'arguments' => array(
      'id' => 'The ID being looked up. More than one ID of the same type can be supplied.',
    ),
    'options' => array(
      'type' => 'The type of ID supplied. One of doi, pmc, pmid, default doi.',
      'format' => 'Set the output format. One of csv, plain, default plain.',
      'stats' => 'Enable output of article metrics instead of the title and IDs.',
      'server' => 'The URL of the Lagotto server. Overrides the Drupal config.',
      'key' => 'The API Key for the Lagotto server. Overrides the Drupal config.',
    ),
    'examples' => array(
      'drush lagotto-lookup-id 10.7554/eLife.00471' => 'Check the currently defined Lagotto server for details of eLife article e00471',
      'drush lagotto-lookup-id --type=pmc PMC4066238' => 'Check the currently defined Lagotto server for details of a PubMedCentral article',
      'drush lgid --server=http://alm.example.org/lagotto PMC4066238' => 'Check the currently defined Lagotto server for details of PMC ',
    ),
  );
  return $items;
}

/**
 * Drush callback to lookup a work ID.
 */
function drush_lagotto_services_lagotto_lookup_id() {

  $options = array();

  // Support drush-setup of server.
  $stats = drush_get_option('stats');
  $stats = ($stats == '1');

  $server = drush_get_option('server', '');
  if (preg_match('/^https?:\/\//i', $server)) {
    $server_url = $server . '/api/v5/articles';
    $options['service_url'] = $server_url;
  }

  $key = drush_get_option('key', '');
  if (!empty($key)) {
    $options['service_key'] = $key;
  }

  // Get the ID type, validate and convert to Lagotto names.
  switch (drush_get_option('type', 'doi')) {
    default:
    case 'doi':
      $options['id_type'] = 'doi';
      break;

    case 'pmc':
    case 'pmcid':
      $options['id_type'] = 'pmcid';
      break;

    case 'pm':
    case 'pmid':
      $options['id_type'] = 'pmid';
      break;
  }

  // Get the format, validate and convert to Lagotto names.
  switch (drush_get_option('format', 'plain')) {
    default:
    case 'plain':
      $format = 'plain';
      break;

    case 'csv':
      $format = 'csv';
      break;
  }

  $how = array(
    'head' => TRUE,
    'stats' => $stats,
    'format' => $format,
  );

  // The API can take no more than 50 at a time.
  $ids = array();
  $args = func_get_args();
  $results = array();
  foreach ($args as $arg) {
    $ids[] = $arg;
    if (count($ids) >= 48) {
      $partial = lagotto_services_drush_lookup($ids, $options);
      if ($partial) {
        foreach ($partial['result'] as $v) {
          $results[] = $v;
        }
      }
      $ids = array();
    }
  }
  // Stragglers...
  if (count($ids) > 0) {
    $partial = lagotto_services_drush_lookup($ids, $options);
    if ($partial) {
      foreach ($partial['result'] as $v) {
        $results[] = $v;
      }
    }
  }

  if (count($results) > 0) {
    switch ($how['format']) {
      case 'plain':
        lagotto_services_drush_format_plain($how, $results);
        break;

      case 'csv':
        lagotto_services_drush_format_csv($how, $results);
        break;
    }
  }
}

/**
 * Submit an API fetch to the server for the IDs provided.
 *
 * Results are printed on the console using drush_print()
 *
 * @param array $ids
 *   Array of ID values to request. All will be interpreted according
 *   to the 'id_type' key in $options, defaulting to 'doi'.
 * @param array $options
 *   API options defining the fetch. See lagotto_services_fetch_work() for
 *   possible values.
 *
 * @return array
 *   The records returned from the API call, or an empty array on error.
 *
 * @see lagotto_services_fetch_work
 */
function lagotto_services_drush_lookup(array $ids, array $options) {
  $default_options = array(
    'include_detail' => FALSE,
    'quiet' => TRUE,
  );
  $config = array_merge($default_options, $options);

  $result = lagotto_services_fetch_work($ids, $config);

  if ($result == NULL) {
    drush_print(t('Permission denied.'));
    return NULL;
  }

  $records = lagotto_services_records($result);
  if (!empty($records['error'])) {
    if ($records['error'] == 3000) {
      drush_print(t('No record found.'));
    }
    else {
      drush_print(t('Server returned error : @code : @msg',
          array(
            '@code' => $records['error'],
            '@msg' => $records['message'],
          ))
      );
    }
    return NULL;
  }

  return $records;
}


/**
 * Output format function for plain text (human readable) output.
 *
 * @param array $how
 *   Options defining how the output is generated. Can contain:
 *   - head - TRUE if asked to output a header.
 *   - format - One of 'csv' or 'plain', defining the output style.
 *   - stats - TRUE if metric stats should be output, otherwise general info.
 * @param array $records
 *   Record array from lagotto API.
 */
function lagotto_services_drush_format_plain(array $how, array $records) {
  $fields = array();

  // Set up the table header row.
  if ($how['stats']) {
    if ($how['head']) {
      $fields[] = array(
        'doi' => 'DOI',
        'date' => 'Date',
        'view' => 'Viewed',
        'save' => 'Saved',
        'disc' => 'Discussed',
        'cite' => 'Cited',
        'url' => 'Url',
      );
    }
    $widths = array(
      'doi' => 22,
      'date' => 12,
      'view' => 6,
      'save' => 6,
      'disc' => 6,
      'cite' => 6,
    );
  }
  else {
    if ($how['head']) {
      $widths = array(
        'doi' => 22,
        'pmid' => 6,
        'pmcid' => 6,
      );
    }
    $fields[] = array(
      'doi' => 'DOI',
      'pmid' => 'PMID',
      'pmcid' => 'PMCID',
      'ttl' => 'Title',
    );
  }

  // And now the table data.
  foreach ($records as $record) {
    if ($how['stats']) {
      $fields[] = array(
        'doi' => check_plain($record['doi']),
        'date' => date('Y-m-d', lagotto_services_issue_date($record)),
        'view' => check_plain($record['viewed']),
        'save' => check_plain($record['saved']),
        'disc' => check_plain($record['discussed']),
        'cite' => check_plain($record['cited']),
        'url' => check_plain($record['canonical_url']),
      );
    }
    else {
      $fields[] = array(
        'doi' => check_plain($record['doi']),
        'pmid' => check_plain($record['pmid']),
        'pmcid' => check_plain($record['pmcid']),
        'ttl' => check_plain($record['title']),
      );
    }
  }

  // And let drush print it.
  drush_print_table($fields, $how['head'], $widths);
}

/**
 * Output format function for plain text (human readable) output.
 *
 * @param array $how
 *   Options defining how the output is generated. Can contain:
 *   - first - TRUE  when appropriate to output a header or open a file.
 *   - last - TRUE when appropriate to output a footer or close a file.
 *   - format - One of 'csv' or 'plain', defining the output style.
 *   - stats - TRUE if metric stats should be output, otherwise general info.
 * @param array $records
 *   Record array from lagotto API.
 */
function lagotto_services_drush_format_csv(array $how, array $records) {
  if ($how['head']) {
    if ($how['stats']) {
      drush_print('doi,date,viewed,saved,discussed,cited,url');
    }
    else {
      drush_print('doi,pmid,pmcid,title');
    }
  }

  foreach ($records as $record) {
    if ($how['stats']) {
      $fields = array(
        '@doi' => check_plain($record['doi']),
        '@date' => date('Y-m-d', lagotto_services_issue_date($record)),
        '@view' => check_plain($record['viewed']),
        '@save' => check_plain($record['saved']),
        '@disc' => check_plain($record['discussed']),
        '@cite' => check_plain($record['cited']),
        '@url' => check_plain($record['canonical_url']),
      );
    }
    else {
      $fields = array(
        '@doi' => check_plain($record['doi']),
        '@pmid' => check_plain($record['pmid']),
        '@pmcid' => check_plain($record['pmcid']),
        '@ttl' => check_plain($record['title']),
      );
    }
    drush_print(lagotto_services_output_tocsv($fields));
  }
}

/**
 * Format a fields array as a line of CSV text.
 *
 * @param array $fields
 *   An array of values to print. Key names are not important.
 * @param string $delimiter
 *   The character that separates fields on a line.
 * @param string $wrap
 *   The character that wraps each field's value (e.g. ") if the
 *   field contains whitespace or the delimiter character. If this
 *   character also appears within a field's value, it is doubled.
 * @param bool $null_ok
 *   If True, and a field's value is php-NULL, then output
 *   NULL as the field's value; otherwise output nothing.
 *
 * @return string
 *   A line of the CSV output corresponding to the fields supplied.
 */
function lagotto_services_output_tocsv(array $fields, $delimiter = ',', $wrap = '"', $null_ok = FALSE) {
  $delimiter_esc = preg_quote($delimiter, '/');
  $enclosure_esc = preg_quote($wrap, '/');

  $output = array();
  foreach ($fields as $field) {
    if ($field === NULL && $null_ok) {
      $output[] = 'NULL';
      continue;
    }

    if (preg_match("/(?:${delimiter_esc}|${enclosure_esc}|\\s)/", $field)) {
      $output[] = ($wrap . str_replace($wrap, $wrap . $wrap, $field) . $wrap);
    }
    else {
      $output[] = $field;
    }
  }

  return implode($delimiter, $output);
}
