<?php
/**
 * @file
 * Download-related functions for the Election Candidate module.
 */

/**
 * Form builder function for "election/%election/candidates/download".
 */
function election_candidate_download_form($form, &$form_state, stdClass $election) {

  drupal_set_title(t('Download candidates'), PASS_THROUGH);
  election_set_breadcrumb($election);

  $post_name = election_get_post_name($election->type);

  $properties = array(
    'election_id' => t('Election ID'),
    'election_title' => array(
      'label' => t('Election title'),
      'default' => TRUE,
    ),
    'candidate_id' => t('Candidate ID'),
    'published' => array(
      'label' => t('Published?'),
      'default' => TRUE,
    ),
    'post_id' => t('@Post ID', array('@Post' => drupal_ucfirst($post_name))),
    'post_title' => array(
      'label' => t('@Post title', array('@Post' => drupal_ucfirst($post_name))),
      'default' => TRUE,
    ),
    'cstatus' => array(
      'label' => t('Status'),
      'default' => TRUE,
    ),
    'uid' => t('User ID'),
    'created' => array(
      'label' => t('Created date'),
      'default' => TRUE,
      'type' => 'timestamp',
    ),
    'changed' => array(
      'label' => t('Last modified date'),
      'default' => TRUE,
      'type' => 'timestamp',
    ),
    'profile_link' => array(
      'label' => t('Profile link'),
      'default' => TRUE,
    ),
  );

  foreach (election_candidate_get_types() as $bundle_name => $info) {
    $instances = field_info_instances('election_candidate', $bundle_name);
    foreach ($instances as $field_name => $instance) {
      if (!isset($properties[$field_name])) {
        $field = field_info_field($field_name);
        $properties[$field_name] = array(
          'label' => $instance['label'],
          'type' => $field['type'],
          'default' => FALSE,
        );
        if ($field_name == 'candidate_name' || $field_name == 'candidate_mail') {
          $properties[$field_name]['default'] = TRUE;
        }
      }
    }
  }

  $property_options = array();
  $default_properties = array();
  foreach ($properties as $property_key => $property) {
    $label = is_string($property) ? $property : $property['label'];
    $property_options[$property_key] = $label;
    if (isset($property['default']) && $property['default'] === TRUE) {
      $default_properties[] = $property_key;
    }
  }

  $form_state['#election'] = $election;
  $form_state['#properties'] = $properties;
  $form_state['#property_options'] = $property_options;

  $form['include_properties'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Properties to include'),
    '#options' => $property_options,
    '#default_value' => $default_properties,
  );

  $form['buttons'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );
  $form['buttons']['download'] = array(
    '#type' => 'submit',
    '#value' => t('Download candidate spreadsheet'),
  );

  return $form;

}

/**
 * Submit callback for the form election_candidate_download_form.
 */
function election_candidate_download_form_submit($form, &$form_state) {

  $election = $form_state['#election'];

  $file_name = 'candidates_election' . $election->election_id . '_' . date('Y-m-d_His', REQUEST_TIME) . '.csv';

  $include_properties = $form_state['values']['include_properties'];
  foreach ($include_properties as $key => $value) {
    if ($value === 0) {
      unset($include_properties[$key]);
    }
  }

  $properties = array_intersect_key($form_state['#properties'], $include_properties);

  $separator = ',';
  $enclosure = '"';
  $empty = '';

  $outstream = fopen('php://output', 'w');

  $candidate_ids = db_select('election_candidate')
    ->fields('election_candidate', array('candidate_id'))
    ->condition('election_id', $election->election_id)
    ->orderBy('created', 'DESC')
    ->execute()
    ->fetchCol();
  $candidates = election_candidate_load_multiple($candidate_ids);

  drupal_add_http_header('Content-Type', 'text/csv; charset=UTF-8');
  drupal_add_http_header('Cache-Control', 'no-store, max-age=0, must-revalidate');
  drupal_add_http_header('Pragma', FALSE);
  drupal_add_http_header('Content-Description', 'File Transfer');
  drupal_add_http_header('Content-Disposition', 'attachment; filename="' . $file_name . '"');

  $property_headers = array();
  foreach ($properties as $property) {
    $property_headers[] = is_string($property) ? $property : $property['label'];
  }

  fputcsv($outstream, $property_headers, $separator, $enclosure);

  foreach ($candidates as $candidate) {

    $row = array();
    foreach ($properties as $property_key => $property) {
      $value = _election_candidate_download_format_property($candidate, $property_key, $property);
      $row[$property_key] = strlen($value) ? $value : $empty;
    }

    fputcsv($outstream, $row, $separator, $enclosure);
    fpassthru($outstream);
  }

  fpassthru($outstream);
  fclose($outstream);

  drupal_exit();

}

/**
 * Format a candidate 'property', for the CSV candidate data download.
 */
function _election_candidate_download_format_property(stdClass $candidate, $property_key, $property) {
  $value = isset($candidate->{$property_key}) ? $candidate->{$property_key} : NULL;
  switch ($property_key) {
    case 'name':
      return election_candidate_get_name($candidate, FALSE);

    case 'profile_link':
      return url(election_candidate_uri_path($candidate), array('absolute' => TRUE));

    case 'election_title':
      $election = election_load($candidate->election_id);
      return $election->title;

    case 'post_title':
      if (empty($candidate->post_id)) {
        return;
      }
      $post = election_post_load($candidate->post_id);
      return $post->title;

    case 'cstatus':
      return _election_candidate_status_name($candidate);

    case 'published':
      return $candidate->published ? t('Published') : t('Not published');

  }
  if (is_array($property) && !empty($property['type'])) {
    $type = $property['type'];
  }
  else {
    return $value;
  }
  switch ($type) {
    case 'timestamp':
      return date('Y-m-d H:i:s', $value);

    case 'file':
    case 'image':
      if (is_array($value) && isset($value[LANGUAGE_NONE][0]['uri'])) {
        return file_create_url($value[LANGUAGE_NONE][0]['uri']);
      }
      break;

    case 'text_long':
      if (is_array($value) && isset($value[LANGUAGE_NONE][0]['value'])) {
        return html_entity_decode(strip_tags(check_markup($value[LANGUAGE_NONE][0]['value'], 'plain_text')));
      }
      break;

    case 'email':
      return is_array($value) && isset($value[LANGUAGE_NONE][0]) ? $value[LANGUAGE_NONE][0]['email'] : NULL;
      break;

    default:
      $values = field_get_items('election_candidate', $candidate, $property_key);
      if (!$values) {
        return;
      }
      $output = '';
      foreach ($values as $value) {
        $build = field_view_value('election_candidate', $candidate, $property_key, $value);
        $output .= drupal_render($build) . "\n";
      }
      return $output;
  }
}
