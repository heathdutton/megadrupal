<?php

/**
 * @file
 * Cron Support Functions for Eloqua
 *
 * @package Eloqua
 */

/**
 * Cron helper function.
 *
 * @param object $post
 *   Saved post.
 */
function _eloqua_cron($post = NULL) {
  $posts = array();

  // If post passed into this function, only process this post.
  if ($post) {
    $posts = array($post);
  }
  // Post all the data to Eloqua and fetch unposted data.
  else {
    $result_set = db_select('eloqua_saved_posts')
      ->fields('eloqua_saved_posts')
      ->condition('status', ELOQUA_STATUS_NEW)
      ->range(0, (int) variable_get('batch_size', 50))
      ->execute()
      ->fetchAll();
    $posts = _eloqua_unserialize_data_column($result_set);
  }

  // If nothnig to do, or something funky happened, bail.
  if (empty($posts) || !is_array($posts)) {
    return;
  }

  foreach ($posts as $post) {
    if (!is_array($post->data)) {
      $post->data = array();
    }

    if (!empty($post->data['form_post']['elqSiteId'])) {
      $elqSiteId = $post->data['form_post']['elqSiteId'];
    }
    else {
      // Bail if no site id, because we can't generate proper url without it.
      return;
    }

    $url = 'http://s'.$elqSiteId.'.t.eloqua.com/e/f2';

    $original_headers = $post->data['user_headers'];
    $headers = array(
      'Accept-Language' => array_key_exists('accept-language', $original_headers) ? $original_headers['accept-language'] : 'en',
      'User-Agent' => array_key_exists('user-agent', $original_headers) ? $original_headers['user-agent'] : 'User Relay',
      'Content-Type' => 'application/x-www-form-urlencoded',
    );

    // Fetch the post fields to send to Eloqua.
    $post_fields = _eloqua_cron_get_post_fields($post);

    $options = array(
      'method' => 'POST',
      'timeout' => 5,
      'headers' => $headers,
      'data' => drupal_http_build_query($post_fields),
    );

    $result = drupal_http_request($url, $options);

    // Update post data.
    $post->data['server_post'][] = array(
      'timestamp' => time(),
      'response' => $result,
      'http_status' => $result->code,
    );

    if ($result->code == '200') {
      $post->{'status'} = ELOQUA_STATUS_UPLOADED;
    }
    else {
      $post->{'status'} = ELOQUA_STATUS_FAILED;
    }

    eloqua_post_update($post);
  }
}

/**
 * Handle translating post values into what Eloqua wants in terms of structure.
 *
 * Dates are by default handled in YYYY-mm-dd
 * Times are by default handled in HH:mm
 *
 * @param array $tree
 *   The post tree name => value pairs.
 * @param array $posted_values
 *   The post tree, could be name => value pairs or index => value pairs.
 * @param array $result
 *   The re-structured tree that Eloqua will leverage.
 */
function _eloqua_get_submission_data($tree, $posted_values, &$result) {
  foreach ($tree as $name => $value) {
    // Expand fieldsets.
    if (is_array($value) && !in_array($value, $posted_values)) {
      _eloqua_get_submission_data($value, $posted_values, $result);
    }
    // Convert multi-value fields into strings.
    elseif (is_array($value)) {
      $result[$name] = implode(',', $value);
    }
    // Everything else is just passed along.
    else {
      $result[$name] = $value;
    }
  }
}

/**
 * Remaps the post fields to be used within Eloqua.
 *
 * @param array $post_fields
 *   Data to post.
 * @param array $post
 *   Post data.
 *
 * @return array
 *   $post_fields modified to have remapped keys to either the component
 *   form_key or the eloqua mapping, where defined.
 */
function _eloqua_cron_remap_post_fields($post_fields, $post) {
  $result = array();
  $nid = $post->data['form_post']['details']['nid'];
  if (empty($nid) || !is_numeric($nid)) {
    // Something is wrong with this.
    return $post_fields;
  }
  $node = node_load($nid);
  $components = $node->webform['components'];
  $map = array();
  if (is_array($components)) {
    foreach ($components as $component) {
      if (!empty($component['extra']['eloqua']['key'])) {
        $map[$component['cid']] = $component['extra']['eloqua']['key'];
      }
      else {
        $map[$component['cid']] = $component['form_key'];
      }
    }
  }

  foreach ($post_fields as $key => $value) {
    if (isset($map[$key])) {
      $result[$map[$key]] = $value;
    }
    else {
      $result[$key] = $value;
    }
  }

  return $result;
}

/**
 * Returns the cURL error for a status returned from eloqua_curl_set_opts().
 *
 * @TODO: Theme
 *
 * @param array $status
 *   Status received from eloqua_curl_set_opts().
 *
 * @return string
 *   Html formatted message.
 *
 * @see eloqua_curl_set_opts()
 */
function _eloqua_set_curl_get_error($status) {
  if (!is_array($status)) {
    return FALSE;
  }
  $messages = array();
  $message = '';
  foreach ($status as $key => $data) {
    if ($data['status']) {
      continue;
    }
    $value = var_export($data['value'], TRUE);
    $messages[] = "<li>$key - Error: {$data['message']}<br /> Value: $value</li>";
  }
  if (!empty($messages)) {
    $message = '<ul>' . implode("\n", $messages) . '</ul>';
  }
  return $message;
}

/**
 * Returns the post fields for the request, modified for Eloqua.
 *
 * @param object $post
 *   The post data.
 *
 * @return array
 *   The updated post data for Eloqua.
 */
function _eloqua_cron_get_post_fields($post) {
  // TODO: Review newest webform for exact behavior on whether submitted_tree
  // exists.
  $form_data = !empty($post->data['form_post']['submitted_tree']) ? $post->data['form_post']['submitted_tree'] : $post->data['form_post']['submitted'];

  $post_fields = array();
  // Merge any Eloqua values into the post to be submitted.
  foreach ($post->data['form_post'] as $name => $value) {
    if (substr($name, 0, 3) === 'elq') {
      $post_fields[$name] = $value;
    }
  }

  // Translates the data into a Eloqua-worthy format.
  _eloqua_get_submission_data($form_data, $post->data['form_post']['submitted'], $post_fields);

  // Remap any fields that can't be handled by webform.
  $post_fields = _eloqua_cron_remap_post_fields($post_fields, $post);
  return $post_fields;
}
