<?php

/**
 * Menu callback - links admin listing.
 */
function path_checker_admin_settings_form($form, &$form_state) {

  $form['path_checker_per_page'] = array(
    '#type' => 'select',
    '#title' => t('Pagination'),
    '#options' => array(0 => t('Disabled')) + drupal_map_assoc(array(10, 20, 30, 50, 100)),
    '#default_value' => path_checker_variable_get('per_page'),
    '#description' => t('Links per page in Path Checker administration. If the'
      . ' pagination is set to "disabled", all links will be shown. WARNING: '
      . ' Disabled pagination together with hundreds or thousands of stored'
      . ' links could have negative impacts on the site performance.'),
  );

  $form['path_checker_internal_request_fallback'] = array(
    '#type' => 'radios',
    '#title' => t('Internal URL handling'),
    '#options' => array(
          0 => t('Use only internal menu router check'),
          1 => t('Use internal router check with fallback to drupal_http_request')
      ),
      '#default_value' => path_checker_variable_get('internal_request_fallback'),
    '#description' => t("Internal links could be checked using internal menu"
      . " router check which is lightning fast and resources-friendly. However"
      . " it could not handle settings like redirects defined .htaccess and"
      . " such things."),
  );

  $form['path_checker_max_redirects'] = array(
    '#type' => 'select',
    '#title' => t('Allowed redirects'),
    '#options' => drupal_map_assoc(range(0, 10)),
    '#default_value' => path_checker_variable_get('max_redirects'),
    '#description' => t("Maximum allowed redirects. When set to 0 or when the"
      . " maximum allowed redirect limit is lower than needed redirects to"
      . " reach the target, HTTP request URL will return 300 status code type."
      . " Otherwise, if target is reached before allowed redirects limit is"
      . " exhausted, it will return 200 HTTP code if the target was reached"
      . " sucessfully or 400 or 500 HTTP code types if target was reached, "
      . " but results in client (4xx) or server error (5xx)."),
  );

  $form['path_checker_original_site'] = array(
    '#type' => 'textfield',
    '#title' => t('Migration/Original site URL'),
    '#default_value' => path_checker_variable_get('original_site'),
    '#description' => t("If you are migrating from one domain to another, you"
      . " can	enter the original domain here. If not empty, original links will"
      . " be displayed in the Path Checker link administration side by side"
      . " with the internal links. Leave empty if you don't want to use this"
      . " feature. Example: http://www.myoldsite.com."),
  );

  $form['path_checker_useragent'] = array(
    '#type' => 'textfield',
    '#title' => t('Useragent'),
    '#default_value' => path_checker_variable_get('useragent'),
    '#description' => t("Agent identifier in Drupal HTTP request header."
      . " Example: Drupal (+http://drupal.org/)"),
  );

  $form['path_checker_ignore_response_codes'] = array(
    '#type' => 'textfield',
    '#title' => t('Success HTTP codes'),
    '#default_value' => path_checker_variable_get('ignore_response_codes'),
    '#description' => t("HTTP response status codes which don't increment link failed count separated by comma. Example: 200,302,304,401,403"),
  );

  $form['path_checker_response_codes_help'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTTP status codes'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['path_checker_response_codes_help']['http_codes'] = array(
    '#markup' => '<dl>
        <dt><strong>1xx Informational</strong></dt><dd>100 Continue, 101 Switching Protocols</dd>
        <dt><strong>2xx Success</strong></dt><dd>200 OK, 201 Created, 202 Accepted, 203 Non-Authoritative Information, 204 No Content, 205 Reset Content, 206 Partial Content</dd>
        <dt><strong>3xx Redirection</strong></dt><dd>300 Multiple Choices, 301 Moved Permanently, 302 Found, 303 See Other, 304 Not Modified, 305 Use Proxy, 307 Temporary Redirect</dd>
        <dt><strong>4xx Client Error</strong></dt><dd>400 Bad Request, 401 Unauthorized, 402 Payment Required, 403 Forbidden, 404 Not Found, 405 Method Not Allowed, 406 Not Acceptable, 407 Proxy Authentication Required, 408 Request Time-out, 409 Conflict, 410 Gone, 411 Length Required, 412 Precondition Failed, 413 Request Entity Too Large, 414 Request-URI Too Large, 415 Unsupported Media Type, 416 Requested range not satisfiable, 417 Expectation Failed</dd>
        <dt><strong>5xx Server Error</strong></dt><dd>500 Internal Server Error, 501 Not Implemented, 502 Bad Gateway, 503 Service Unavailable, 504 Gateway Time-out, 505 HTTP Version not supported</dd>
      </dl>',
  );

  $form['path_checker_domain_blacklist'] = array(
    '#type' => 'textarea',
    '#title' => t('Domain blacklist'),
    '#default_value' => path_checker_variable_get('domain_blacklist'),
    '#description' => t("Domains which will be skipped during test. Place each"
      . " domain on new line or separate them by comma. Example: example.com,"
      . " example.net,example.org"),
  );

  return system_settings_form($form);
}

/**
 * Return a form to filter URL redirects.
 *
 * @ingroup forms
 * @see path_redirect_filter_form_submit()
 */
function path_checker_search_form($form, &$form_state, $type = 'all', $search = '') {

  $form['#attributes'] = array('class' => 'search-form');
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $form['search']['filter'] = array(
    '#type' => 'textfield',
    '#title' => '',
    '#default_value' => $search,
    '#maxlength' => 128,
    '#size' => 25,
  );
  $form['search']['type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );
  $form['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('path_checker_search_form_submit'),
  );
  if ($search) {
    $form['search']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
      '#submit' => array('path_checker_search_form_reset'),
    );
  }
  return $form;
}

/**
 * Process filter form submission when the Filter button is pressed.
 */
function path_checker_search_form_submit($form, &$form_state) {
  $values = &$form_state['values'];
  $form_state['redirect'] = 'admin/config/search/path_checker/list/'. $values['type'] . '/' . trim($values['filter']);
}

/**
 * Process filter form submission when the Reset button is pressed.
 */
function path_checker_search_form_reset($form, &$form_state) {
  $values = &$form_state['values'];
  $form_state['redirect'] = 'admin/config/search/path_checker/list/'. $values['type'];
}

/**
 * Menu callback - links admin listing.
 */
function path_checker_admin_page($filter = 'all', $url_search = '') {
  $build = array();

  $per_page = path_checker_variable_get('per_page');
  $destination = drupal_get_destination();

  // Search form
  $build['search_form'] = array(
    '#markup' => drupal_render(drupal_get_form('path_checker_search_form', $filter, $url_search)),
  );

  // Integration with Path Redirect
  $redirect = module_exists('redirect') && user_access('administer redirects');

  // Display original link
  $original_domain = trim(path_checker_variable_get('original_site'), '/ ');

  if ($original_domain) {
    $header = array(
      array('data' => t('Url'), 'field' => 'pcl.url'),
      array('data' => t('Original URL')),
      array('data' => t('Internal'), 'field' => 'pcl.internal'),
      array('data' => t('Method'), 'field' => 'pcl.method'),
      array('data' => t('Code'), 'field' => 'pcl.code'),
      array('data' => t('Error'), 'field' => 'pcl.error'),
      array('data' => t('Fail count'), 'field' => 'pcl.fail_count'),
      array('data' => t('Last checked'), 'field' => 'pcl.last_checked'),
      array('data' => t('Operations')),
    );
  }
  else {
    $header = array(
      array('data' => t('Url'), 'field' => 'pcl.url'),
      array('data' => t('Internal'), 'field' => 'pcl.internal'),
      array('data' => t('Method'), 'field' => 'pcl.method'),
      array('data' => t('Code'), 'field' => 'pcl.code'),
      array('data' => t('Error'), 'field' => 'pcl.error'),
      array('data' => t('Fail count'), 'field' => 'pcl.fail_count'),
      array('data' => t('Last checked'), 'field' => 'pcl.last_checked'),
      array('data' => t('Operations')),
    );
  }

  $query = path_checker_query_builder('all', $filter, $per_page, $url_search);

  // Link data
  $result = $query->extend('TableSort')->orderByHeader($header)->execute();
  $rows = array();
  foreach ($result as $l) {
    $operations = array();
    $operations['test'] = array(
      'title' => t('Test'),
      'href' => 'path_checker/test/' . $l->lid,
      'query' => $destination,
    );
    if ($redirect) {
      $operations['redirect'] = array(
        'title' => t('Redirect'),
        'href' => 'admin/config/search/redirect/add',
        'query' => $destination + array('source' => urlencode($l->url)),
      );
    }
    $operations['delete'] = array(
      'title' => t('Delete'),
      'href' => 'path_checker/delete/' . $l->lid,
      'query' => $destination,
    );

    $row = array();
    $row[] = l($l->url, $l->url, array('alias' => TRUE));
    if ($original_domain) {
      $row[] = $l->internal ? l(t('Visit original'), $original_domain . '/' . $l->url, array('alias' => TRUE)) : '';
    }
    $row[] = $l->internal ? t('Yes') : t('No');
    $row[] = $l->method;
    $row[] = $l->code;
    $row[] = $l->error;
    $row[] = $l->fail_count;
    $row[] = $l->last_checked ? format_date($l->last_checked) : t('Not yet');
    $row[] = theme('links', array('links' => $operations, array('attributes' =>  array('class' => 'links inline nowrap'))));
    $rows[$l->lid] = $row;
  }

  $build['list_form'] = array('#markup' => drupal_render(drupal_get_form('path_checker_admin_form', $header, $rows)));

  if ($per_page) {
    $build['bottom_pager'] = array('#theme' => 'pager');
  }

  return $build;
}

/**
 * Alternative admin listing using tableselect element.
 * Tableselect is native in D7. Elements module is needed for D6.
 *
 * @param $form_state
 *    Form API state
 * @param type $header
 *    An array of the table header
 * @param type $options
 *    An array of the table rows
 *
 * @return
 *    Form API array
 */
function path_checker_admin_form($form, $form_state, $header, $options) {

  $form['links'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' =>  t('No links available'),
  );

  $form['pager'] = array('#value' => theme('pager'));

  $form['test'] = array(
    '#type' => 'submit',
    '#value' => t('Test'),
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  return $form;
}

/**
 * Form submit handler
 *
 * @see path_checker_admin_form()
 */
function path_checker_admin_form_submit($form, &$form_state) {
  $links = array_filter($form_state['values']['links']);
  switch ($form_state['values']['op']) {
    case t('Delete'):
      $form_state['redirect'] = 'path_checker/delete/' . implode(',', $links);
      //$form_state['redirect'] = array('path_checker/delete/' . implode(',', $form_state['values']['links']), drupal_get_destination());
      break;
    case (t('Test')):
      $responses = array();
      foreach ($links as $lid) {
        $link = path_checker_link_load($lid);
        $response = path_checker_test_url($link);
        if (path_checker_is_error_code($response->code)) {
          drupal_set_message(t('Link check of <a href="@url">@url</a> failed (status code: @code).', array('@url' => url($link->url), '@code' => $response->code)), 'warning', FALSE);
        }
        else {
          drupal_set_message(t('Link check of <a href="@url">@url</a> succeed (status code: @code).', array('@url' => url($link->url), '@code' => $response->code)), 'info', FALSE);
        }
      }
      break;
  }
}

/**
 * Page to delete a link or multiple links.
 */
function path_checker_delete_confirm($form, &$form_state, $lids) {
  $links = array();

  $lids = array_unique(array_filter(array_map('intval', explode(',', $lids))));

  if (count($lids) > 0) {
    $result = db_select('path_checker_link', 'pcl')
      ->fields('pcl', array('lid', 'url'))
      ->condition('lid', $lids, 'IN')
      ->execute();
    $lids = $result->fetchAllKeyed();
  }

  $count = count($links);

  if (!$count) {
    $form['empty'] = array('#value' => t('No valid link found.') . ' ' . l('Back', 'admin/config/search/path_checker'));
  }
  else {
    $form['lids'] = array('#type' => 'value', '#value' => $lids);
    $form['links'] = array('#type' => 'value', '#value' => $links);

    $title = format_plural($count,
        'Are you sure you want to delete the link?',
        'Are you sure you want to delete @count links?');
    $desc = format_plural($count,
        'This action cannot be undone. Following link will be deleted: %urls.',
        'This action cannot be undone. Following links will be deleted: %urls', array('%urls' => implode(', ', $links)));
    $form = confirm_form($form, $title, 'admin/config/search/path_checker', $desc, t('Delete'), t('Cancel'));
  }
  return $form;
}

/**
 * Submit handler to delete a link or multiple links.
 *
 * @see path_checker_delete_confirm()
 */
function path_checker_delete_confirm_submit($form, &$form_state) {
  path_checker_delete_link($form_state['values']['lids']);
  $form_state['redirect'] = 'admin/config/search/path_checker';
}

/**
 * Form for importing custom URL to the database.
 */
function path_checker_import_form($form, &$form_state) {

  $form['custom_links'] = array(
    '#type' => 'textarea',
    '#title' => t('Add new links'),
    '#rows' => 10,
    '#description' => t('Place one link per line. Internal paths should have the hostname part stripped (e.g. instead of "http://www.this-site.com/about" insert just "about").'),
    '#default_value' => isset($form_state['values']) ? $form_state['values']['custom_links'] : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Form submit handler
 *
 * @see path_checker_import_form()
 */
function path_checker_import_form_submit($form, &$form_state) {
  $links = preg_split('/(\r\n?|\n)/', $form_state['values']['custom_links']);

  // Try to add the links to the database.
  $added = path_checker_add_custom_links($links);
  drupal_set_message(format_plural(count($added), '1 new link was added.', '@count new links were added.'));

  $form_state['rebuild'] = TRUE;
}

/**
 * Form for importing custom URL to the database.
 */
function path_checker_export_form($form_state) {
  $form = array();

  $values = array(
    'type' => '',
  );

  if (isset($form_state['values'])) {
    $values = $form_state['values'] + $values;
  }

  $form['export'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $form['export']['type'] = array(
    '#title' => t('Choose export'),
    '#type' => 'select',
    '#options' => array(
      'path_checker' => 'Path checker links',
      'url_alias' => 'URL alias links',
    ),
    '#default_value' => $values['type'],
  );

  $form['export']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
  );

  // It's not nice to have this code in the form builder, but we can save some
  // memory by loading the content in-place. Am I right?
  if ($values['type']) {

    $links = '';
    switch ($values['type']) {
      case 'url_alias' :
        $result = db_query('SELECT dst AS link FROM {url_alias}');
        break;
      case 'path_checker' :
        $result = db_query('SELECT url AS link FROM {path_checker_link}');
        break;
    }

    foreach ($result as $row) {
      $links .= check_plain($row->link) . "\n";
    }

    $form['links'] = array(
      '#type' => 'textarea',
      '#title' => t('Exported links'),
      '#rows' => 20,
      '#default_value' => $links,
    );
  }

  return $form;
}

function path_checker_export_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Page callback, test given path_checker link object and redirect back.
 *
 * Extracted from @see path_checker_cron().
 *
 * @param $link
 *   The link object
 */
function path_checker_test_url_page($link, $report_success = FALSE) {
  $response = path_checker_test_url($link);
  if (path_checker_is_error_code($response->code)) {
    drupal_set_message(t('Link check of <a href="@url">@url</a> failed (status code: @code).', array('@url' => $link->url, '@code' => $response->code)), 'warning', FALSE);
  }
  else {
    drupal_set_message(t('Link check of <a href="@url">@url</a> succeed (status code: @code).', array('@url' => $link->url, '@code' => $response->code)), 'info', FALSE);
  }
  drupal_goto('content/path_checker');
}

/**
 * Form which sets and launches batch check of stored links.
 */
function path_checker_batch_check_form($form, $form_state) {

  $options = array(
    'all' => t('All links.'),
    'untested' => t('All untested links.'),
    'failed' => t('All links which failed during last check.'),
    'succeed' => t('All links which succeed during last check.'),
    'redirected' => t('All links which were redirected during last check.'),
  );

  $stats = array();
  foreach ($options as $type => $description) {
    $query = path_checker_query_builder('count', $type);
    $count = $query->execute()->fetchField();
    $stats[] = '<strong>' . drupal_ucfirst($type) . ':</strong> ' . l($count, 'admin/config/search/path_checker/list/' . $type);
    $options[$type] = '<strong>' . drupal_ucfirst($type) . ':</strong> ' . t('!description (@count)', array('!description' => $description, '@count' => $count));
  }

  $form['stats_title'] = array(
    '#markup' => t('Overview'),
    '#prefix' => '<h3>', '#suffix' => '</h3>',
  );
  $form['stats_list'] = array(
    '#markup' => theme('item_list', array('items' => $stats)),
  );

  $form['check'] = array(
    '#type' => 'radios',
    '#title' => t('Batch check'),
    '#options' => $options,
    '#default_value' => '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Run'),
  );

  return $form;
}

/**
 * Submits the batch check form.
 *
 * @see  path_checker_check_form()
 */
function path_checker_batch_check_form_submit($form, &$form_state) {
  $batch = path_checker_batch_check($form_state['values']['check']);
  batch_set($batch);
}

/**
 * Returns SQL query and arguments for given type, filter and search string.
 *
 * @param $type
 *   The type of select: 'lid', 'count', 'all'.
 * @param $filter
 *   The group of status codes: 'untested', 'failed', 'succeed', 'redirected', 'all'.
 * @param $search
 *   An url or its part to search for.
 *
 * @return
 *   An array which contains query string and arguments array.
 */

function path_checker_query_builder($type, $filter, $pager = 0, $search = '') {
  $query = db_select('path_checker_link', 'pcl');

  switch ($filter) {
    case 'untested':
      // "code = -1"
      $query->condition('code', -1);
      break;
    case 'failed':
      // "code LIKE '4%' OR code LIKE '5%'"
      $query->condition(db_or()
        ->condition('code', '4%', 'LIKE')
        ->condition('code', '5%', 'LIKE'));
      break;
    case 'succeed':
      // "code LIKE '2%'"
      $query->condition('code', '2%', 'LIKE');
      break;
    case 'redirected':
      // "code LIKE '3%'"
      $query->condition('code', '3%', 'LIKE');
      break;
    case 'all':
    default:
  }

  if (strlen($search)) {
    // "url LIKE '%:url%'"
    $query = $query->condition('url', '%' . db_like($search) . '%', 'LIKE');
  }

  $query->addTag('pathchecker_access');

  switch ($type) {
    case 'lid':
      // 'SELECT lid FROM {path_checker_link}'
      $query->addField('pcl', 'lid');
      break;
    case 'count':
      // 'SELECT COUNT(lid) AS count FROM {path_checker_link}'
      $query = $query->countQuery();
      break;
    case 'all':
    default:
      // 'SELECT * FROM {path_checker_link}'
      $query->fields('pcl');
  }

  if ($type != 'count' && $pager > 0) {
    $query = $query->extend('PagerDefault')->limit($pager);
  }

  return $query;
}

/**
 * Batch
 */
function path_checker_batch_check($type) {
  $operations = array();
  $query = path_checker_query_builder('lid', $type);
  $result = $query->execute();
  foreach ($result as $row) {
    $operations[] = array('path_checker_batch_check_op', array($row->lid));
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'path_checker_batch_check_finished',
    'title' => t('Checking paths'),
    'init_message' => t('Initializing.'),
    'progress_message' => t('Processed @current link out of @total.'),
    'error_message' => t('Checking of the paths has encountered an error.'),
    'file' => drupal_get_path('module', 'path_checker') . '/path_checker.admin.inc',
  );
  return $batch;
}

/**
 * Batch operation for batch
 */
function path_checker_batch_check_op($lid, &$context) {
  $link = path_checker_link_load($lid);
  $response = path_checker_test_url($link);

  // Store some result for post-processing in the finished callback.
  if (isset($context['results'][$response->code])) {
    $context['results'][$response->code]++;
  }
  else {
    $context['results'][$response->code] = 1;
  }


  // Optional message displayed under the progressbar.
  $context['message'] = t('Testing @url with status: @code.', array('@url' => $link->url, '@code' => $response->code));
}

/**
 * Batch 'finished' callback
 */
function path_checker_batch_check_finished($success, $results, $operations) {
  if ($success) {
    $total = array_reduce($results, create_function('$v, $w', 'return $v + $w;'));
    $subtotals = array();
    foreach ($results as $code => $count) {
      $subtotals[] = "status $code: $count";
    }
    $message =  '@total links processed with following HTTP status codes: @subtotals.';
    $vars = array('@total' => $total ? $total : 0, '@subtotals' => implode(', ', $subtotals));
    watchdog('path_checker', $message, $vars, WATCHDOG_INFO, 'admin/config/search/path_checker/check');
    drupal_set_message(t($message, $vars), 'error');
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = 'An error occurred while processing %operation with arguments: %arguments';
    $vars = array('%operation' => $error_operation[0], '%arguments' => print_r($error_operation[0], TRUE));
    watchdog('path_checker', $message, $vars, WATCHDOG_ERROR, 'admin/config/search/path_checker/check');
    drupal_set_message(t($message, $vars), 'error');
  }
}
