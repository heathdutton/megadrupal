<?php

/**
 * @file
 * Contains page callbacks and related functions for the Visitors Voice module.
 */

/**
 * Page callback: Responds to requests from Visitors Voice.
 *
 * This doesn't use Drupal's normal access mechanism but "manually" checks
 * whether the request has been properly authenticated to show that it actually
 * comes from Visitors Voice, using the "visitorsvoice_access_key" variable.
 *
 * This page callback will output a JSON (or, in case of access failure, plain
 * text) response and then terminate the page request directly.
 *
 * @param string $operation
 *   The operation to execute. The following are recognized:
 *   - CheckAuthentication: Checks whether authentication is set up correctly.
 *   - GetSearchResult: Retrieves one page of normal search results for a
 *     specific set of keywords, possibly along with facets.
 *   - GetPageSearchResult: Retrieves all results matching certain keywords when
 *     only searching the title, or otherwise accomplishing a very good match.
 *   - UpdateSearchResult: Updates the search results for a specific set of
 *     keywords by adding, removing and re-ordering the results.
 *   - GetPageIdFromUrl: Retrieves the internal item ID for a certain URL.
 *   All other operations will trigger an error message to be printed.
 *
 * @return null
 *   Returns NULL, to terminate the page request directly.
 *
 * @see visitorsvoice_menu()
 * @see visitorsvoice_request_authenticate()
 */
function visitorsvoice_request_handler($operation) {
  try {
    $nonce = visitorsvoice_request_authenticate();

    switch ($operation) {
      case 'CheckAuthentication':
        // Since we got this far, authentication seems to work.
        $response = array(
          'Message' => 'ok',
        );
        break;
      case 'GetModuleVersion':
        $response = array(
          'Version' => VISITORSVOICE_VERSION,
        );
        break;
      case 'GetSearchResult':
        $response = visitorsvoice_get_search_result($_POST);
        break;
      case 'GetPageSearchResult':
        $response = visitorsvoice_get_page_search_result($_POST);
        break;
      case 'UpdateSearchResult':
        $response = visitorsvoice_update_search_result($_POST);
        break;
      case 'GetPageIdFromUrl':
        $response = visitorsvoice_get_page_id_from_url($_POST);
        break;

      default:
        throw new VisitorsvoiceException(t('Operation unknown.'), 404);
    }
    if ($response) {
      if (!is_scalar($response)) {
        $response = json_encode($response);
      }
      $hmac = hash_hmac('sha1', $nonce . $response, variable_get('visitorsvoice_access_key', ''));
      header("Pragma: hmac_digest=$hmac;");
      echo $response;
    }
  }
  catch (VisitorsvoiceException $e) {
    $e->terminateRequest();
  }
  return NULL;
}

/**
 * Authenticates a request made to the Visitors Voice request handler.
 *
 * @return string
 *   The nonce used for this request.
 *
 * @throws VisitorsvoiceException
 *   If authentication for this request failed.
 *
 * @see visitorsvoice_authentication_fail()
 */
function visitorsvoice_request_authenticate() {
  $access_key = variable_get('visitorsvoice_access_key', '');
  if (!$access_key) {
    throw new VisitorsvoiceException(t('Access denied: no access key set in Drupal.'), 403);
  }

  $last_id = variable_get('visitorsvoice_last_access_id', '');
  if (empty($_GET['id']) || $last_id >= $_GET['id']) {
    $args['!given_id'] = isset($_GET['id']) ? '"' . $_GET['id'] . '"' : t('none');
    $args['!last_id'] = $last_id;
    throw new VisitorsvoiceException(t('Access denied: ID needs to be greater than "!last_id", !given_id given.', $args), 403);
  }

  if (empty($_COOKIE['visitorsvoice_nonce']) || empty($_COOKIE['visitorsvoice_hmac'])) {
    throw new VisitorsvoiceException(t('Access denied: necessary cookies visitorsvoice_nonce and visitorsvoice_hmac not present.'), 403);
  }
  $nonce = $_COOKIE['visitorsvoice_nonce'];
  $hmac = $_COOKIE['visitorsvoice_hmac'];

  $string = $nonce;
  $string .= current_path();
  $string .= $_GET['id'];
  // Get request body. (Since this is also used by, e.g., Symphony's Request
  // class, this seems the best way to do this, even though it looks rather
  // shaky/hacky.)
  $string .= file_get_contents('php://input');

  $computed_hmac = hash_hmac('sha1', $string, $access_key);
  if (!$computed_hmac) {
    throw new VisitorsvoiceException(t('Internal error: SHA1 not supported on this machine.'), 500);
  }
  if ($hmac !== $computed_hmac) {
    throw new VisitorsvoiceException(t('Access denied: HMAC did not match.'), 403);
  }

  // Only now set the last ID to the one used here, so unauthorized requests
  // don't change this value.
  variable_set('visitorsvoice_last_access_id', $_GET['id']);

  return $nonce;
}

/**
 * Gets the ID of the index to use for this request.
 *
 * @param array $parameters
 *   The parameters for the current REST request. Not used at the moment.
 *
 * @return string
 *   The ID of the search index to work with for this request.
 *
 * @throws VisitorsvoiceException
 *   If the index could not be determined.
 */
function visitorsvoice_get_index_id(array $parameters) {
  $index_id = variable_get('visitorsvoice_index');
  if ($index_id) {
    return $index_id;
  }
  throw new VisitorsvoiceException(t('Search index to use not configured.'));
}

/**
 * Loads the index to use for this request.
 *
 * @param array $parameters
 *   The parameters for the current REST request. Not used at the moment.
 *
 * @return SearchApiIndex
 *   The search index to work with for this request.
 *
 * @throws VisitorsvoiceException
 *   If the index could not be determined or loaded.
 */
function visitorsvoice_load_index(array $parameters) {
  $index_id = visitorsvoice_get_index_id($parameters);
  $index = search_api_index_load($index_id);
  if (!$index) {
    throw new VisitorsvoiceException(t('Could not load search index with ID "!index_id".', array('!index_id' => $index_id)));
  }
  return $index;
}

/**
 * REST request handler for retrieving search results.
 *
 * @param array $parameters
 *   The parameters for this request. The following parameters are recognized:
 *   - SearchTerm: The keywords to search for.
 *   - PageNumber: (optional) The number of the page to retrieve, starting at 1.
 *     Defaults to 1.
 *   - Facet: (optional) An array of facet filters to set, as fields mapped to
 *     an array of values they should be filtered to.
 *
 * @return array
 *   An associative array of search results.
 *
 * @throws VisitorsvoiceException
 *   If the search couldn't be executed properly.
 */
function visitorsvoice_get_search_result(array $parameters) {
  if (empty($parameters['SearchTerm'])) {
    throw new VisitorsvoiceException(t('No search term specified.'));
  }
  $index = visitorsvoice_load_index($parameters);

  try {
    $options['search id'] = 'visitorsvoice-search';
    $query = $index->query($options);
    $query->keys($parameters['SearchTerm']);
    if (isset($parameters['Facet']) && module_exists('facetapi')) {
      $searcher = 'search_api@' . $index->machine_name;
      $adapter = facetapi_adapter_load($searcher);
      $adapter->setParams($parameters, 'Facet');
    }
    $page = isset($parameters['PageNumber']) ? $parameters['PageNumber'] - 1 : 0;
    $query->range(10 * $page, 10);
    $response = $query->execute();
  }
  catch (SearchApiException $e) {
    throw new VisitorsvoiceException($e->getMessage(), 500, $e);
  }

  return array('SearchTerm' => $parameters['SearchTerm']) + visitorsvoice_format_search_results($response, $index);
}

/**
 * REST request handler for retrieving particularly good matches for a search.
 *
 * @param array $parameters
 *   The parameters for this request. The following parameters are recognized:
 *   - SearchTerm: The keywords to search for.
 *
 * @return array
 *   An associative array of search results.
 *
 * @throws VisitorsvoiceException
 *   If the search couldn't be executed properly.
 */
function visitorsvoice_get_page_search_result(array $parameters) {
  if (empty($parameters['SearchTerm'])) {
    throw new VisitorsvoiceException(t('No search term specified.'));
  }
  $index = visitorsvoice_load_index($parameters);

  // Determine the fulltext fields to try for "good matches".
  // As the first approach, we simply sort by boost and take those fields not
  // having the lowest used boost value.
  // First, retrieve the fulltext fields along with their boost values.
  $fields = isset($index->options['fields']) ? $index->options['fields'] : array();
  $fulltext_fields = array();
  foreach ($index->getFulltextFields() as $field) {
    $boost = isset($fields[$field]['boost']) ? $fields[$field]['boost'] : '1.0';
    $fulltext_fields[$field] = $boost;
  }
  // Then, sort them by boost (ascending).
  asort($fulltext_fields);
  // Now, remove all fields sharing the lowest boost value.
  $lowest_boost = reset($fulltext_fields);
  foreach ($fulltext_fields as $field => $boost) {
    if ($boost != $lowest_boost) {
      break;
    }
    unset($fulltext_fields[$field]);
  }
  $fulltext_fields = array_keys($fulltext_fields);

  // If that failed (i.e., all fields have the same boost), we instead look for
  // fields with specific names often used for titles.
  if (!$fulltext_fields) {
    // Fields in ascending importance (since "name" is often just used for the
    // machine-readable name, and "title" is most common since it is used for
    // nodes.
    foreach (array('name', 'label', 'title') as $field) {
      if (isset($fields[$field]['type']) && search_api_is_text_type($fields[$field]['type'])) {
        $fulltext_fields[] = $field;
      }
    }
  }

  try {
    $options['search id'] = 'visitorsvoice-lookup';
    $query = $index->query($options);
    if ($fulltext_fields) {
      $query->keys($parameters['SearchTerm']);
      $used_fields = array();
      while (empty($response['result count']) && $fulltext_fields) {
        $used_fields[] = array_pop($fulltext_fields);
        $query->fields($used_fields);
        $response = $query->execute();
      }
    }
    else {
      // If we didn't manage to get any particularly "important-looking" fields,
      // we instead use a phrase query to find the best matches.
      $query->keys(array('#conjunction' => 'AND', $parameters['SearchTerm']));
      $response = $query->execute();
    }
  }
  catch (SearchApiException $e) {
    throw new VisitorsvoiceException($e->getMessage(), 500, $e);
  }

  // Remove any facets.
  unset($response['search_api_facets']);

  return array('SearchTerm' => $parameters['SearchTerm']) + visitorsvoice_format_search_results($response, $index);
}

/**
 * Formats search results in the format used by Visitors Voice.
 *
 * @param array $response
 *   The search results as specified by SearchApiQueryInterface::execute().
 * @param SearchApiIndex $index
 *   The index that was searched.
 *
 * @return array
 *   A search results array in the Visitor Voice format. "FacetList" will only
 *   be included if $fields was not given.
 *
 * @throws VisitorsvoiceException
 *   If the search couldn't be executed properly.
 */
function visitorsvoice_format_search_results(array $response, SearchApiIndex $index) {
  $return = array(
    'TotalHitsCount' => $response['result count'],
    'HitList' => array(),
  );

  if (!empty($response['results'])) {
    try {
      $num = 0;
      $datasource = $index->datasource();
      $items = $datasource->loadItems(array_keys($response['results']));
      foreach ($response['results'] as $item_id => $result) {
        $hit = array(
          'HitNumber' => ++$num,
          'HitId' => $item_id,
        );
        if (isset($items[$item_id])) {
          $label = $datasource->getItemLabel($items[$item_id]);
          if (isset($label)) {
            $hit['HitTitle'] = $label;
          }
          $url = $datasource->getItemUrl($items[$item_id]);
          if (isset($url)) {
            $url['options']['absolute'] = TRUE;
            $hit['HitUrl'] = url($url['path'], $url['options']);
          }
        }
        $hit['Meta'] = array();
        $return['HitList'][] = $hit;
      }
    }
    catch (SearchApiException $e) {
      throw new VisitorsvoiceException($e->getMessage(), 500, $e);
    }
  }

  if (module_exists('search_api_facetapi') && !empty($response['search_api_facets'])) {
    $searcher = 'search_api@' . $index->machine_name;
    $adapter = facetapi_adapter_load($searcher);
    if ($adapter) {
      $return['FacetList'] = array();
      $adapter->getUrlProcessor()->setParams(array(), 'Facet');
      foreach ($response['search_api_facets'] as $facet_name => $filters) {
        $facet = facetapi_facet_load($facet_name, $searcher);
        if (!$facet) {
          continue;
        }
        $facet_object = new FacetapiFacet($adapter, $facet);
        $processor = new FacetapiFacetProcessor($facet_object);
        $processor->process();
        $build = $processor->getBuild();
        if (!$build) {
          continue;
        }
        $returned_facet = array(
          'Caption' => $facet['label'],
          'Operator' => 'and',
          'FacetItemList' => array(),
        );
        $settings = $facet_object->getSettings('block');
        if (isset($settings->settings['soft_limit'])) {
          $returned_facet['SoftLimit'] = $settings->settings['soft_limit'];
        }
        $settings = $facet_object->getSettings();
        if (isset($settings->settings['operator'])) {
          $returned_facet['Operator'] = $settings->settings['operator'];
        }
        foreach ($build as $facet_item) {
          _visitorsvoice_add_facet_item($returned_facet['FacetItemList'], $facet_item, $adapter, $facet);
        }
        $return['FacetList'][] = $returned_facet;
      }
    }
  }

  return $return;
}

/**
 * Adds a Facet API facet item to a list of Visitors Voice facets.
 *
 * @param array $facet_list
 *   The array of facets, as a reference, in the format used by Visitors Voice.
 * @param array $facet_item
 *   The facet item that should be added, as a Facet API facet build array. The
 *   additional "#filter" parameter represents the query that should be added
 *   for the facet item in the Visitors Voice format.
 * @param FacetapiAdapter $adapter
 * @param array $facet
 */
function _visitorsvoice_add_facet_item(array &$facet_list, array $facet_item, FacetapiAdapter $adapter, array $facet) {
  $item = array(
    'Name' => $facet_item['#markup'],
    'HitNumber' => $facet_item['#count'],
    'IsSelected' => $facet_item['#active'],
    'Value' => $facet_item['#indexed_value'],
  );
  $item['Query'] = drupal_http_build_query($adapter->getQueryString($facet, array($facet_item['#indexed_value']), FALSE));
  $item['Query'] = preg_replace('/^Facet\[\d+\]/', 'Facet%5B%5D', $item['Query']);
  if (!empty($facet_item['#item_parents'])) {
    $item['Parent'] = reset($facet_item['#item_parents']);
  }
  $facet_list[] = $item;
  if (!empty($facet_item['#item_children'])) {
    foreach ($facet_item['#item_children'] as $child) {
      _visitorsvoice_add_facet_item($facet_list, $child, $adapter, $facet);
    }
  }
}

/**
 * REST request handler for customizing search results.
 *
 * @param array $parameters
 *   The parameters for this request. The following parameters are recognized:
 *   - SearchTerm: The keywords of the search that should be customized.
 *   - PagesToAdd: (optional) An array of IDs of items that should be added to
 *     the search results for that keyword.
 *   - PagesToReorder: (optional) An array of IDs of the items that should be
 *     ordered first in searches for that keyword, in the order they should be
 *     returned in.
 *   - PagesToRemove: (optional) An array of IDs of items that should be removed
 *     from the search results for that keyword.
 *   If none of the optional parameters are present, a RESET is executed for the
 *   given search term.
 *
 * @return array
 *   An associative array summarizing the result of the operation.
 *
 * @throws VisitorsvoiceException
 *   If the operations couldn't be executed properly.
 */
function visitorsvoice_update_search_result(array $parameters) {
  if (empty($parameters['SearchTerm'])) {
    throw new VisitorsvoiceException(t('No search term specified.'));
  }
  $index_id = visitorsvoice_get_index_id($parameters);
  $transaction = db_transaction();
  $customization_id = visitorsvoice_customization_id($index_id, $parameters['SearchTerm'], TRUE);
  // When no IDs are specified, this is a RESET request.
  $reset = TRUE;
  // The order here is important, as ADDs should be executed before re-ordering.
  $operations = array(
    'PagesToAdd' => 'visitorsvoice_customization_add',
    'PagesToReorder' => 'visitorsvoice_customization_order',
    'PagesToRemove' => 'visitorsvoice_customization_remove',
  );
  $executed_operations = array();
  $changed_item_ids = array();
  foreach ($operations as $key => $function) {
    if (!empty($parameters[$key])) {
      $reset = FALSE;
      $changed_item_ids += $function($customization_id, $parameters[$key]);
      $executed_operations[] = $key;
    }
  }
  if ($reset) {
    $changed_item_ids += visitorsvoice_customization_delete($index_id, $parameters['SearchTerm']);
    $executed_operations[] = 'RESET';
  }

  if ($changed_item_ids) {
    $index = search_api_index_load($index_id);
    try {
      if (!$index) {
        throw new SearchApiException(t('Could not load search index with ID "!index_id".', array('!index_id' => $index_id)));
      }
      $index->datasource()->trackItemChange($changed_item_ids, array($index));

      // For indexes with the index_directly option set, queue the items to be
      // indexed at the end of the request.
      // Taken from search_api_track_item_change() – we can't call that, though,
      // since we only want to mark items as changed for that specific index.
      if (!empty($index->options['index_directly'])) {
        try {
          search_api_index_specific_items_delayed($index, $changed_item_ids);
        }
        catch (SearchApiException $e) {
          watchdog_exception('search_api', $e);
        }
      }
    }
    catch (SearchApiException $e) {
      $transaction->rollback();
      throw new VisitorsvoiceException($e->getMessage(), 500, $e);
    }
  }

  return array(
    'Message' => 'ok',
    'Operations' => $executed_operations,
  );
}

/**
 * REST request handler for mapping a URL to a search item.
 *
 * Since neither Drupal nor the Search API have any support for this, this is a
 * rather complex heap of heuristics, but should at least catch nearly all
 * (actually existing) cases.
 *
 * @param array $parameters
 *   The parameters for this request. The following parameters are recognized:
 *   - PageUrl: The URL of a search item.
 *
 * @return mixed
 *   The ID of the search item located at the given URL.
 *
 * @throws VisitorsvoiceException
 *   If no match could be found or another error occurred.
 */
function visitorsvoice_get_page_id_from_url(array $parameters) {
  if (empty($parameters['PageUrl'])) {
    throw new VisitorsvoiceException(t('No URL specified.'));
  }

  $index = visitorsvoice_load_index($parameters);
  try {
    $datasource = $index->datasource();
  }
  catch (SearchApiException $e) {
    throw new VisitorsvoiceException($e->getMessage(), 500, $e);
  }

  // In the (unlikely) case of an index of files, there won't be a menu item, so
  // we have to catch that case early.
  if ($index->getEntityType() === 'file') {
    return _visitorsvoice_get_item_id(_visitorsvoice_path_to_file($parameters['PageUrl']), $datasource);
  }

  $url_parts = parse_url($parameters['PageUrl']);
  if (!$url_parts) {
    throw new VisitorsvoiceException(t('Invalid URL specified.'));
  }
  $path = NULL;

  // First off, we need the path within the Drupal installation. It could be
  // given via a "q" GET parameter directly, or with clean URLs as an addition
  // to the HTTP resource path.
  if (isset($url_parts['query'])) {
    parse_str($url_parts['query'], $query);
    if (isset($query['q'])) {
      $path = $query['q'];
    }
  }
  if (!$path) {
    global $base_path;
    $path = isset($url_parts['path']) ? $url_parts['path'] : '/';
    if ($base_path && strpos($path, $base_path) === 0) {
      $path = substr($path, strlen($base_path));
    }
  }
  if ($path === '') {
    $path = variable_get('site_frontpage', 'node');
  }
  // Resolve any path alias that might be in effect here.
  $path = drupal_get_normal_path($path);

  // Get the menu item for the path.
  $menu_item = menu_get_item($path);
  if (!$menu_item) {
    throw new VisitorsvoiceException(t('Could not find a menu item for the given URL.'));
  }
  // If the current user (which is anonymous) cannot access the menu item, the
  // page arguments won't be unserialized and mapped.
  if (!$menu_item['access']) {
    $menu_item['page_arguments'] = menu_unserialize($menu_item['page_arguments'], $menu_item['original_map']);
  }

  // If the menu item doesn't have any arguments, it is probably safe to assume
  // that it doesn't represent any search item.
  if (!$menu_item['page_arguments']) {
    throw new VisitorsvoiceException(t("The given URL doesn't seem to represent a single item."));
  }

  $item = NULL;

  if ($entity_type = $index->getEntityType()) {
    $item = _visitorsvoice_menu_item_to_entity($menu_item, $entity_type, $url_parts);
  }
  elseif (is_array($menu_item['page_arguments'])) {
    foreach ($menu_item['page_arguments'] as $arg) {
      if (is_array($arg)) {
        continue;
      }
      if (is_scalar($arg)) {
        $arg = $index->loadItems(array($arg));
        $arg = reset($arg);
        if (!$arg) {
          continue;
        }
      }
      $url = $datasource->getItemUrl($arg);
      if ($url && $url['path'] == $path) {
        $item = $arg;
        break;
      }
    }
  }

  if (!$item) {
    $item = _visitorsvoice_path_to_item_brute_force($path, $index);
  }

  return _visitorsvoice_get_item_id($item, $datasource);
}

/**
 * Tries to find the file matching the given URL.
 *
 * @param string $url
 *   The absolute URL of the file.
 *
 * @return object
 *   If successful, the file object the given URL belongs to.
 *
 * @throws VisitorsvoiceException
 *   If no match could be found.
 */
function _visitorsvoice_path_to_file($url) {
  $files = file_load_multiple(FALSE, array('uri' => $url));
  if (!$files) {
    $public_base = file_create_url('public://');
    if (strpos($url, $public_base) === 0) {
      $url = str_replace($public_base, 'public://', $url);
      $files = file_load_multiple(FALSE, array('uri' => $url));
    }
  }
  if ($files) {
    return reset($files);
  }
  throw new VisitorsvoiceException(t('Could not find a matching item for the given URL.'));
}

/**
 * Tries to find an entity of the given type for the given menu item.
 *
 * @param array $menu_item
 *   The menu item to map to an entity.
 * @param string $entity_type
 *   The type of entity to look for.
 * @param array $url_parts
 *   The complete URL of the entity, as parsed by parse_url().
 *
 * @return object|null
 *   If successful, the entity the given URL belongs to. NULL if no known entity
 *   type matches the menu item.
 *
 * @throws VisitorsvoiceException
 *   If the menu item belongs to an entity of another type.
 */
function _visitorsvoice_menu_item_to_entity(array $menu_item, $entity_type, array $url_parts) {
  $types = entity_get_info();
  if ($entity_type === 'comment' && $menu_item['path'] === 'node/%') {
    // See if there is a fragment set for the comment.
    if (!empty($url_parts['fragment']) && substr($url_parts['fragment'], 0, 8) === 'comment-') {
      $comment = comment_load(substr($url_parts['fragment'], 8));
      return $comment ? $comment : NULL;
    }
    $vars['!url_type'] = $types['node']['label'];
    $vars['!index_type'] = $types['comment']['label'];
    throw new VisitorsvoiceException(t('The given URL points to an entity of type "!url_type", but index contains entities of type "!index_type".', $vars));
  }

  // For most of the entity types (especially in core), the menu path contains
  // the entity type as a prefix.
  $pos = strpos($menu_item['path'], '/%');
  if ($pos) {
    $path_prefix = substr($menu_item['path'], 0, $pos);
    $path_prefix = str_replace(array('/', '-'), '_', $path_prefix);

    if (isset($types[$path_prefix])) {
      if ($path_prefix != $entity_type) {
        // One important exception here is profile2, which uses the user's base
        // path as the path for its own entities and will often be indexed.
        if ($entity_type === 'profile2' && $path_prefix === 'user' && !empty($url_parts['fragment'])) {
          if (module_exists('profile2') && substr($url_parts['fragment'], 0, 8) === 'profile-') {
            $item = profile2_load_by_user($menu_item['page_arguments'][0], substr($url_parts['fragment'], 8));
            return $item ? $item : NULL;
          }
        }
        $vars['!url_type'] = $types[$path_prefix]['label'];
        $vars['!index_type'] = $types[$entity_type]['label'];
        throw new VisitorsvoiceException(t('The given URL points to an entity of type "!url_type", but index contains entities of type "!index_type".', $vars));
      }
      // We have a match! However, even though we know we want the first item,
      // we can still just fall-through to the code below that tries all the
      // page arguments.
    }
  }

  // Even if we don't have a clear prefix matching our type, we still try
  // whether any page argument could be the desired item.
  if (!is_array($menu_item['page_arguments'])) {
    return NULL;
  }
  foreach ($menu_item['page_arguments'] as $item) {
    if (is_array($item)) {
      continue;
    }
    if (is_scalar($item)) {
      $item = entity_load_single($entity_type, $item);
      if (!$item) {
        continue;
      }
    }
    $url = entity_uri($entity_type, $item);
    if ($url && $url['path'] == $menu_item['href']) {
      return $item;
    }
  }

  return NULL;
}

/**
 * Determines the search item that the given path belongs to.
 *
 * Uses a brute-force algorithm and is only used as a last ressort.
 *
 * @param string $path
 *   The path to map to an item.
 * @param SearchApiIndex $index
 *   The index in which the item should be contained.
 *
 * @return object
 *   The search item the path belongs to.
 *
 * @throws VisitorsvoiceException
 *   If no match could be found.
 */
function _visitorsvoice_path_to_item_brute_force($path, SearchApiIndex $index) {
  // Sadly, there is no possibility to just load all items of a given datasource
  // (definitely an API shortcoming). We also cannot look inside the tracking
  // table of the datasource cleanly. So we even have to use a heuristic at this
  // step.
  // First, we see whether we have an entity type and can therefore just use
  // entity_load().
  $items = array();
  if ($entity_type = $index->getEntityType()) {
    $items = entity_load($entity_type, FALSE);
  }
  else {
    // Our next guess is that the "search_api_item" table is used to track the
    // items.
    $item_ids = db_select('search_api_item', 'i')
      ->fields('i', array('item_id'))
      ->condition('index_id', $index->id)
      ->execute()
      ->fetchCol();
    // If even that failed, we have to get really dirty and do a raw query on
    // the server.
    if (!$item_ids && $index->enabled) {
      // We just want all results, without facets or other filters. Therefore we
      // don't use the query's execute() method but pass it straight to the
      // server for evaluation. Since this circumvents the normal preprocessing,
      // which sets the fields (on which some service classes might even rely
      // when there are no keywords), we set them manually here.
      // @see _search_api_get_items_on_server()
      try {
        $query = $index->query()
          ->fields(array());
        $response = $index->server()->search($query);
        if (!empty($response['results'])) {
          $item_ids = array_keys($response['results']);
        }
      }
      catch (SearchApiException $e) {
        // We can ignore this here.
      }
    }
    if ($item_ids) {
      $items = $index->loadItems($item_ids);
    }
  }

  if (!$items) {
    throw new VisitorsvoiceException(t('Could not find any items for the configured index.'));
  }

  $datasource = $index->datasource();
  foreach ($items as $item) {
    $url = $datasource->getItemUrl($item);
    if ($url && $url['path'] === $path) {
      return $item;
    }
  }
  throw new VisitorsvoiceException(t('Could not find a matching item for the given URL.'));
}

/**
 * Retrieves the ID of a given search item.
 *
 * @param object $item
 *   The item whose ID should be retrieved.
 * @param SearchApiDataSourceControllerInterface $datasource
 *   The datasource of that item.
 *
 * @return mixed
 *   The item's ID.
 *
 * @throws VisitorsvoiceException
 *   If the item's ID could not be determined.
 */
function _visitorsvoice_get_item_id($item, SearchApiDataSourceControllerInterface $datasource) {
  $id = $datasource->getItemId($item);
  if ($id !== NULL) {
    return $id;
  }
  throw new VisitorsvoiceException(t('Found item, but could not retrieve its ID.'));
}
