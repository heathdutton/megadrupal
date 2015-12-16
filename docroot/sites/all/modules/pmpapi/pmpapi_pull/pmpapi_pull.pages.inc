<?php

/**
 * Form constructor for the PMPAPI pull form that gets doc by GUID.
 *
 * @see pmpapi_pull_pull_doc_by_guid_validate()
 * @see pmpapi_pull_pull_doc_by_guid_submit()
 *
 * @ingroup forms
 */
function pmpapi_pull_pull_doc_by_guid() {

  $form = array();

  $form['guid'] = array(
    '#type' => 'textfield',
    '#title' => t('PMP GUID'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Get document'),
  );

  return $form;
}

/**
 * Form validation handler for pmpapi_pull_pull_doc_by_guid().
 *
 * @see pmpapi_pull_pull_doc_by_guid_submit()
 *
 * @ingroup forms
 */
function pmpapi_pull_pull_doc_by_guid_validate($form, &$form_state) {
  $guid = trim($form_state['values']['guid']);
  $pattern = '/^[0-9A-F]{8}-[0-9A-F]{4}-4[0-9A-F]{3}-[89AB][0-9A-F]{3}-[0-9A-F]{12}$/i';
  if (!preg_match($pattern, $guid)) {
    form_set_error('guid', t('Not a valid GUID.'));
  }
}

/**
 * Form submission handler for pmpapi_pull_pull_doc_by_guid().
 *
 * @see pmpapi_pull_pull_doc_by_guid_validate()
 *
 * @ingroup forms
 */
function pmpapi_pull_pull_doc_by_guid_submit($form, &$form_state) {
  $guid = trim($form_state['values']['guid']);
  pmpapi_pull_pull_doc($guid);
}

/**
 * Form constructor for the PMPAPI pull search form.
 *
 * @see pmpapi_pull_filter_form_submit()
 *
 * @ingroup forms
 */
function pmpapi_pull_filter_form($form, &$form_state, $search_term = '') {

  $limit = (!empty($_GET['limit'])) ? $_GET['limit'] : 10;
  $params = array('limit' => $limit);
  $per_page = 10;

  if ($search_term) {
    $params['text'] = $search_term;
  }

  if (!empty($_GET['tag'])) {
    $tag = $_GET['tag'];
    $params['tag'] = $tag;
  }

  if (!empty($_GET['profile'])) {
    $profile = $_GET['profile'];
    $params['profile'] = $profile;
  }
  $profiles = array('All') + pmpapi_get_profile_list();

  if (!empty($_GET['creator'])) {
    $creator = $_GET['creator'];
    $params['creator'] = $creator;
  }
  $creators = array('All') + array_flip(pmpapi_get_creators());

  if (!empty($_GET['property'])) {
    $program = $_GET['property'];
    // query by 'collection=', not 'property='
    $params['collection'] = $program;
  }
  $programs = array(0 => t('All')) + pmpapi_get_properties();

  if (!empty($_GET['page'])) {
    // doing this drupal-style, wherein pages are 0-indexed
    // looks like PMP offset is 0-indexed as well
    $params['offset'] = $_GET['page'] * $per_page;
  }

  $form['text'] = array(
    '#type' => 'fieldset',
    '#title' => t('Text search'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $text_t_args = array('@syntax' => url('http://www.lucenetutorial.com/lucene-query-syntax.html'));
  $form['text']['search_term'] = array(
    '#title' => 'Search term',
    '#type' => 'textfield',
    '#default_value' => $search_term,
    '#description' => t("Limit query to search snippet (using the <a href='@syntax'>Lucene query syntax</a>)", $text_t_args),
  );

  $form['text']['tag'] = array(
    '#title' => 'Tag(s)',
    '#type' => 'textfield',
    '#default_value' => !empty($tag) ? $tag : '',
    '#description' => t('Limit query to the listed tags (space-separated).'),
  );

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter by'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['filter']['profile'] = array(
    '#title' => 'Profile',
    '#type' => 'select',
    '#default_value' => !empty($profile) ? $profile : 0,
    '#options' => $profiles,
  );

  $form['filter']['creator'] = array(
    '#title' => 'Creator',
    '#type' => 'select',
    '#default_value' => !empty($creator) ? $creator : 0,
    '#options' => $creators,
  );

  $form['filter']['property'] = array(
    '#title' => 'Program',
    '#type' => 'select',
    '#default_value' => !empty($program) ? $program : 0,
    '#options' => $programs,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  $pmp = pmpapi_query($params, FALSE); // don't cache

  if (!$pmp->errors) {
    $docs = pmpapi_pull_parse_doc_metadata($pmp->query->results->docs);

    $form['doc_list'] = array(
      '#markup' => pmpapi_pull_create_doc_list_markup($docs),
    );

    $form['pager'] = array (
       '#markup' => pmpapi_pull_create_pager($pmp->query->links->navigation['self']->totalitems),
    );
  }
  return $form;
}

/**
 * Form submission handler for pmpapi_pull_filter_form().
 *
 * @ingroup forms
 */
function pmpapi_pull_filter_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == 'Search') {
    $path = 'admin/content/pmp/search';
    $options = array('query' => array());
    if (!empty($form_state['values']['search_term'])) {
      $path = $path . '/' . $form_state['values']['search_term'];
    }
    if ($form_state['values']['tag']) {
      $options['query']['tag'] = $form_state['values']['tag'];
    }
    if ($form_state['values']['profile']) {
      $options['query']['profile'] = $form_state['values']['profile'];
    }
    if ($form_state['values']['creator']) {
      $options['query']['creator'] = $form_state['values']['creator'];
    }
    if ($form_state['values']['property']) {
      $options['query']['property'] = $form_state['values']['property'];
    }
    $form_state['redirect'] = array($path, $options);
  }
}

/**
 * Parses out metadata from PMP docs.
 *
 * @param $docs array
 *   PMP docs to be parsed
 *
 * @return array
 *   All parsed metadata, one element per doc
 */
function pmpapi_pull_parse_doc_metadata($docs) {
  $metadata = array();
  $pulling = variable_get('pmpapi_pull_pull_active');
  foreach($docs as $doc) {
    $title = $doc->attributes->title;
    $pieces = explode('/', $doc->links->profile[0]->href);
    $profile = array_pop($pieces);
    $guid = $doc->attributes->guid;
    $api_date =  $doc->attributes->published;
    $timestamp = strtotime($api_date);
    $pub_date =  format_date($timestamp, 'custom', 'D, M jS Y \a\t g:i a');

    $been_pulled = pmpapi_pull_doc_has_been_pulled($guid);
    $mapped_profile = pmpapi_pull_find_mapped_entity($profile);
    if (!$pulling || !$mapped_profile || (pmpapi_get_eid_from_guid($guid) && !$been_pulled)) {
      $preview = check_plain($title);
    }
    elseif ($been_pulled) {
      $preview = check_plain($title) . ' ' . l('[' . t('Re-pull') . ']', 'admin/content/pmp/preview/' . $guid);
    }
    else {
      $preview = l($title, 'admin/content/pmp/preview/' . $guid);
    }
    $metadata[] = array($preview, $profile, $guid, $pub_date);
  }
  return $metadata;
}

/**
 * Creates a table of PMP doc metadata.
 *
 * @param $docs array
 *   PMP docs metadata
 *
 * @return string
 *   A themed HTML table of metadata
 */
function pmpapi_pull_create_doc_list_markup($docs) {
  $vars['header'] = array(
    'preview_link' => array(
      'data' => t('Headline (click to preview)'),
    ),
    'profile' => array(
      'data' => t('Profile'),
    ),
    'guid' => array(
      'data' => t('GUID'),
    ),
    'pub_date' => array(
      'data' => t('Publish Date'),
      'sort' => 'desc',
    ),
  );
  $data = array();
  
  $vars['rows'] = $docs;
  return theme('table', $vars);
}

/**
 * Creates a pager for a PMP doc metadata table.
 *
 * @param $total int
 *   Total number of results in the paged table
 *
 * @return string
 *   A themed HTML pager
 */
function pmpapi_pull_create_pager($total) {
  $page = (isset($_GET['page'])) ? abs((int)$_GET['page']) : 0;
  // 'hacking' the pager
  // See inpiration (D6) at:
  // http://www.jacobsingh.name/content/how-use-themepager-without-sql-query-and-go-hell
  global $pager_page_array, $pager_total;
  $pager_total[0] = ceil($total/10);
  $pager_page_array[0] = $page;
  return theme('pager', array('quantity' => 4));
}

/**
 * Page callback: Previews a pulled doc without creating a node.
 *
 * Callback for admin/content/pmp/preview/%
 *
 * @return array
 *   A Drupal form array
 *
 * @see pmpapi_pull_menu()
 */
function pmpapi_pull_preview($form, &$form_state, $guid = '') {
  if (variable_get('pmpapi_pull_pull_active')) {
    $pmp = pmpapi_fetch($guid);
    if ($pmp && !$pmp->errors) {
      $pmp_doc = $pmp->query->results->docs[0];

      // hackalicious
      $profile_href =$pmp->query->results->docs[0]->links->profile[0]->href;
      $pieces = explode('/', $profile_href);
      $profile = array_pop($pieces);

      // parse out the bits we want for preview
      // define (fake) $entity
      $entity_values = array();

      $mapped_entity = pmpapi_pull_find_mapped_entity($profile);
      $entity_type = $mapped_entity['entity_type'];
      $bundle_name = $mapped_entity['bundle_name'];
      $uname = $entity_type . '__' . $bundle_name;

      $map = variable_get('pmpapi_pull_mapping_' . $uname . '_' . $profile);

      if ($entity_type == 'file') {
        $file_uri = pmpapi_pull_get_enclosure_url($pmp_doc->links->enclosure[0]);
        $entity_values += pmpapi_pull_create_mock_file($file_uri);
      }

      $entity_info = entity_get_info($entity_type);
      $key = $entity_info['entity keys']['id'];
      $entity_values[$key] = 0;

      if (!empty($entity_info['entity keys']['bundle'])) {
        $key = $entity_info['entity keys']['bundle'];
        $entity_values[$key] = $bundle_name;
      }

      $label = '';
      if (!empty($entity_info['entity keys']['label'])) {
        $label = $entity_info['entity keys']['label'];
        if ($pmp_key = array_search($label, $map)) {
          $entity_values[$label] = $pmp_doc->attributes->$pmp_key;
        }
        else {
          $entity_values[$label] = $pmp_doc->attributes->title;
        }
      }
      $entity_values['created'] = REQUEST_TIME;

      $uid = variable_get('pmpapi_pull_pull_user', 1);
      $account = user_load($uid);
      $entity_values['uid'] = $uid;
      $entity_values['name'] = format_username($account);
      $default_format = filter_default_format($account);

      $special_fields = array('image', 'audio', 'tags');
      foreach ($map as $pmp_field => $local_field) {
        $local_ok = $local_field && $local_field !== $label;
        $pmp_ok = !in_array($pmp_field, $special_fields) && !empty($pmp_doc->attributes->$pmp_field);
        if ($local_ok && $pmp_ok) {
          $entity_values[$local_field] = array(LANGUAGE_NONE => array(array('value' => $pmp_doc->attributes->$pmp_field, 'format' => $default_format)));
        }
      }

      $entity = entity_create($entity_type, $entity_values);
      $entity_view = drupal_render(entity_view($entity_type, array($entity)));
      $markup = '<div class="preview">' . $entity_view . '</div>';

      $form['doc'] = array(
        '#markup' => $markup,
      );
      $form['publish'] = array(
        '#type' => 'submit',
        '#submit' => array('pmpapi_pull_publish_from_preview'),
        '#value' => t('Pull'),
      );
    }
    return $form;
  }
  else {
    $message = t('If you like to preview and/or pull this PMP doc, please activate the pull process by mapping PMP doc(s) to an existing entity: !link', array('!link' => l('Pull Settings', 'admin/config/services/pmp/pull')));
    drupal_set_message($message, 'warning');
  }
}


function pmpapi_pull_publish_from_preview() {
  return drupal_get_form('pmpapi_pull_publish_from_preview_form');
}

function pmpapi_pull_publish_from_preview_form($form, &$form_state) {
  // don't do this arg() crap; use hidden field form field
  $guid = arg(4);
  pmpapi_pull_pull_doc($guid);

  $form_state['redirect']  = 'admin/content/pmp/search';
  drupal_redirect_form($form_state);
}

/**
 * Creates a mock file object (i.e., without touching the database) from a file
 *
 * @param string $uri
 *   The URI of the file
 *
 * @return object
 *   A mock file object.
 */
function pmpapi_pull_create_mock_file($uri) {
  // Use just path, in case there's a query_string on the URL
  // it will confuse file_get_mimetype()
  $pieces = drupal_parse_url($uri);
  $file = array();
  $file['fid'] = 0;
  $file['uid'] = variable_get('pmpapi_pull_pull_user', 1);
  $file['filename'] = end(explode('/', $uri));
  $file['uri'] = $uri;
  $file['filemime'] = file_get_mimetype($pieces['path']);
  $file['filesize'] = 1000; // let's just fake this
  $file['status'] = 1;
  $file['timestamp'] = REQUEST_TIME;
  return $file;
}