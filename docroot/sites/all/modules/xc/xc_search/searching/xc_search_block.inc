<?php
/**
 * @file
 * Functions used for handlink blocks near the search result list, or the full display
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Create facet checkboxes
 *
 * @param $current_path (String)
 *   The current path
 */
function xc_search_block_facet_checkboxes($current_path) {
  global $_xc_search_no_results;
  static $states = array('checked', 'non_checked');

  if ($_xc_search_no_results) {
    return array('content' => ' ');
  }

  $checkbox_opts = xc_search_block_checkbox_options();
  $meta = xc_search_meta();
  $facet_data = xc_search_block_facet_prepare($current_path);

  $checkboxes = array(
    'online' => (object) array(
      'key' => 'online_only',
      'label' => t('online only'),
    ),
    'microform' => (object) array(
      'key' => 'exclude_microform',
      'label' => t('exclude microform'),
    ),
  );

  $checkbox_list = array();
  foreach ($checkboxes as $key => $checkbox) {

    $checkbox_state = $checkbox_opts[$checkbox->key];
    $active_filters = $facet_data->active_filters;
    $urls = array();

    foreach ($states as $state) {
      $non_filter_params = $facet_data->non_filter_params;
      if (isset($non_filter_params[$checkbox->key])) {
        unset($non_filter_params[$checkbox->key]);
      }
      $non_filter_params[$checkbox->key] = ($state == 'checked') ? 1 : 0;
      $path = $facet_data->search_path;
      if (!isset($query)) {
        $query = '';
      }

      if ($facet_data->is_prepopulated == TRUE) {
        $path .= '/' . $query;
      }

      if (is_array($active_filters) && !empty($active_filters)) {
        $url_query_params = array_merge(
          $non_filter_params,
          array('filter' => array_unique($active_filters))
        );
      }
      else {
        $url_query_params = $non_filter_params;
      }

      $urls[$state] = url($path, array('query' => xc_clean_params($url_query_params), 'absolute' => FALSE));
    }

    $checkbox_list[] = array(
      'key' => $key,
      'urls' => $urls,
      'state' => $checkbox_state,
      'label' => $checkbox->label,
    );
  }

  $block['content'] = theme('xc_search_block_facet_checkboxes', array('checkboxes' => $checkbox_list));
  return $block;
}

/**
 * Theming checkbox list.
 *
 * @param $checkbox_list (Array)
 *   The list of checkbox definitions. Keys:
 *   - key (int): identifier
 *   - urls (Array): associative array of urls with following keys:
 *     - checked: URL to jump when the checkbox is checked
 *     - non_checked: URL to jump when the checkbox is not checked
 *   - state (int): 0 means non checked, 1 means checked state
 *   - label (String): the label displayed near checkbox
 */
function theme_xc_search_block_facet_checkboxes($variables) {
  $checkbox_list = $variables['checkboxes'];
  $themed_chexboxes = array();
  foreach ($checkbox_list as $checkbox) {
    $id = 'xc_search_facet_checkbox_' . $checkbox['key'];
    $themed_chexboxes[] = '<input type="checkbox"'
      . ' id="' . $id . '"'
      . ' onclick="document.location=(this.checked == true) ? \'' . $checkbox['urls']['checked']
      . '\' : \'' . $checkbox['urls']['non_checked'] . '\'"'
      . ($checkbox['state'] ? ' checked="checked"' : '')
      . ' />'
      . '<label for="' . $id . '">' . $checkbox['label'] . '</label>';
  }
  return join('<br />', $themed_chexboxes);
  // return '<div id="xc-search-facet-checkboxes">' . join('<br />', $checkboxes) . '<div>';
}

/**
 * Prepares data which will be displayed by the facet block
 *
 * @param $current_path (string)
 *   The current path
 *
 * @return (Object)
 *   - query (string): the current query
 *   - search_path (string): the Drupal path (aka 'q' parameter)
 *   - is_prepopulated (boolean): whether the facets should be prepopulated by '*:*' query instead of an actual query
 *   - active_filters (array): the list of 'filter' parameters
 *   - statistics (array): statistics of different parts
 *   - params (array): the associative array of Solr facet parameters (these will be sent to Solr)
 *   - collections (array): associative array of information need for display. Currently it contains
 *     - active_filters: the list of active filters
 *     - facets (array): the list of facets, and theirs important features. The key
 *       is the facet name (like subject_topic_fc), and the features are
 *       'type' (like facet_fields or facet_dates),
 *       'field' (same as key),
 *       'label' (human readable name),
 *       'is_collapsed' (0/1 denotes whether the facets should collapsed or not)
 *     - subject_facets (same structure as for the facets)
 *     - hidden_facets: those facets which should be hidden
 *   - non_filter_params (array): associative array of parameters in URL other than "filter"
 *   - field_limit_modifiers: how many items per field
 *   - content (associative array): the only used key is 'filter_list'. It is an array of boxes
 *     in the "Filtering by" element of the block. It shows the boxes which shows the active
 *     filters, and the links to remove them
 */
function xc_search_block_facet_prepare($current_path) {
  global $base_path, $xc_statistics_enabled;
  static $facet_data;

  if (!isset($facet_data)) {
    $facet_data = new stdClass();
    $facet_data->query = xc_search_current_query();
    $facet_data->search_path = $_GET['q'];

    $ui = xc_search_variable('search_ui');
    if (empty($ui)) {
      $ui_id = isset($_GET['search_ui']) ? $_GET['search_ui'] : NULL;
      $ui = xc_search_ui_get($ui_id);
    }

    $facet_data->is_prepopulated = FALSE;
    if (empty($facet_data->query)) {
      if ($ui->do_prepopulate_facets == 1) {
        $facet_data->is_prepopulated = TRUE;
        $facet_data->query = '*:*';
      }
      else {
        return FALSE;
      }
    }
    $facet_data->active_filters = empty($_GET['filter']) ? array() : array_unique((array) $_GET['filter']);

    $xc_statistics_enabled = module_exists('xc_statistics');
    if ($xc_statistics_enabled) {
      $facet_data->statistics = array(
        't_start' => microtime(TRUE),
        'ui_id' => $ui->sui_id,
        'ui_name' => $ui->name,
        'query' => $facet_data->query . '?' . http_build_query($facet_data->active_filters),
      );
    }

    if ($facet_data->query != '*:*'
         && !preg_match('/^(xc__workExpressed_t|xc__expressionManifested_t|id|):/', $facet_data->query)) {
      // TODO: create a better display than raw Solr query

      /*
       $blocks['content'] = t('The query is: %query', array(
       '%query' => xc_search_display_query($facet_data->query)
       ));
       */
    }
    $facet_group = xc_search_facet_group_load($ui->facet_id);
    if (empty($facet_group)) {
      return;
    }
    $facet_objects = xc_search_facets_by_group($ui->facet_id);
    if (empty($facet_objects)) {
      return;
    }
    $default_field_params = xc_search_field_facet_properties_load(
      $facet_group->attribute_set_id
    );

    $facet_data->params = array(
      'facet' => 'true',
      'facet.limit' => $default_field_params->limitation,
      'facet.mincount' => $default_field_params->mincount,
    );

    $facet_data->collections = array(
      'active_filters' => $facet_data->active_filters,
      'facets' => array(),
    );

    xc_search_block_facets_to_parameters($facet_objects, $facet_data->params, $facet_data->collections);

    $search_type = !empty($_GET['search_type']) ? $_GET['search_type'] : 'standard';
    if ($search_type == 'standard' && !in_array('type:manifestation', $facet_data->active_filters)) {
      $facet_data->active_filters[] = 'type:manifestation';
    }
    if (!empty($facet_data->active_filters)) {
      $facet_data->params['fq'] = $facet_data->active_filters;
    }

    $base_link = $base_path . $current_path;
    $no_query_path = $base_link;
    $facet_data->non_filter_params = array();
    if (strstr($current_path, '?')) {
      $no_query_path   = preg_replace('/\?.*$/', '', $current_path);
      $no_filters_path = preg_replace('/\?.*$/', '?', $base_link);
      $appended = FALSE;
      foreach ($_GET as $key => $value) {
        if ($key != 'q' && $key != 'filter') {
          if ($appended == FALSE) {
            $appended = TRUE;
          }
          else {
            $no_filters_path .= '&';
          }
          if ($key != 'page') {
            $no_filters_path .= $key . '=' . $value;
            $facet_data->non_filter_params[$key][] = $value;
          }
        }
      }
    }

    $facet_data->field_limit_modifiers = array();
    if (!empty($facet_data->active_filters)) {
      $filter_list = array();
      foreach ($facet_data->active_filters as $filter) {

        // skipp
        if (preg_match('/^type:/', $filter)) {
          continue;
        }
        $current_non_filter_params = $facet_data->non_filter_params;

        // creating remove link
        $other_filters = array_diff($facet_data->active_filters, array($filter));

        // creating label
        list($facet_field, $facet_term) = explode(':', $filter, 2);
        $facet_data->field_limit_modifiers[$facet_field][] = preg_replace('/^"|"$/', '', $facet_term);

        // field name to display
        if (isset($facet_data->collections['facets'][$facet_field])) {
          $label = $facet_data->collections['facets'][$facet_field]['label'];
        }
        else {
          switch ($facet_field) {
            case 'dcterms__title_s':
              $label = t('Title');
              break;
            case 'xc__type007_s':
              $label = t('Type');
              break;
            case 'rdarole__director_s':
              $label = t('Director');
              break;
            case 'dcterms__identifier_t':
              $label = t('Identifier');
              break;
            case 'rdvocab__modeOfIssuance_t':
              $label = t('Mode of issuance');
              break;
            default:
              $label = $facet_field;
              break;
          }
        }
        // $label .= ': ';

        if ($facet_data->collections['facets'][$facet_field]['type'] == 'facet_dates') {
          unset($current_non_filter_params['date_df_gap']);
        }

        // get the value to display
        if (isset($facet_data->collections['facets'][$facet_field]['type'])) {
          $label = xc_search_block_get_label(
            $facet_data->collections['facets'][$facet_field]['type'],
            $facet_term,
            $facet_data->collections['facets'][$facet_field]['label'],
            TRUE
          );
        }
        elseif (isset($facet_data->collections['subject_facets'][$facet_field]['type'])) {
          $label = xc_search_block_get_label(
            $facet_data->collections['subject_facets'][$facet_field]['type'],
            $facet_term,
            $facet_data->collections['subject_facets'][$facet_field]['label'],
            TRUE
          );
        }
        else {
          $label = xc_search_block_get_label('facet_fields', $facet_term, $label, TRUE);
        }

        // hidden facets
        $filter_list[] = theme('xc_search_block_filter', array(
          'search_path' => $facet_data->search_path,
          'label' => $label,
          'get_params' => array_merge($current_non_filter_params, array('filter' => $other_filters)))
        );
      } // foreach $facet_data->active_filters

      if (!empty($filter_list)) {
        $facet_data->content['filter_list'] = $filter_list;
      }
    } // !empty($facet_data->active_filters

    foreach ($facet_data->field_limit_modifiers as $facet_field => $terms) {
      $param_name = 'f.' . $facet_field . '.facet.limit';
      if (isset($facet_data->params[$param_name])) {
        $default_limit = $facet_data->params[$param_name];
      }
      else {
        $default_limit = $facet_data->params['facet.limit'];
      }
      $facet_data->params[$param_name] = $default_limit + count($terms);
    }

    // add +2 to format_fc
    $param_name = 'f.format_fc.facet.limit';
    if (isset($facet_data->params[$param_name])) {
      $default_limit = $facet_data->params[$param_name];
    }
    else {
      $default_limit = $facet_data->params['facet.limit'];
    }
    $facet_data->params[$param_name] = $default_limit + 2;

    $checkbox_opts = xc_search_block_checkbox_options();
    if ($search_type == 'standard' && $checkbox_opts['online_only'] == 1) {
      $facet_data->params['fq'][] = 'format_fc:"Online"';
    }

    if ($search_type == 'standard' && $checkbox_opts['exclude_microform'] == 1) {
      $facet_data->params['fq'][] = 'format_fc:"Not microform"';
    }

    if ($xc_statistics_enabled) {
      $facet_data->statistics['prepare'] = microtime(TRUE) - $facet_data->statistics['t_start'];
      $t_solr = microtime(TRUE);
    }
  }

  return $facet_data;
}

/**
 * Facet block
 * @param $current_path
 */
function xc_search_block_facets($current_path) {
  global $_xc_search_server, $xc_statistics_enabled;
  include_once drupal_get_path('module', 'xc_solr') . '/XCGap.php';

  timer_start('block/view/facets/prepare');
  $facet_data = xc_search_block_facet_prepare($current_path);
  timer_stop('block/view/facets/prepare');

  //$params['debugQuery'] = 'on';
  if ($xc_statistics_enabled) {
    $t_solr = microtime(TRUE);
  }

  $search_ui = xc_search_variable('search_ui');
  if (empty($search_ui)) {
    $ui_id = isset($_GET['search_ui']) ? $_GET['search_ui'] : NULL;
    $search_ui = xc_search_ui_get($ui_id);
  }

  if ($search_ui->use_distinct_search == 0) {
    $response = xc_search_variable('search_response');
  }

  if (!isset($response) || is_null($response)) {
    try {
      $facet_data->params['XCNAME'] = 'xc_search_block_facets';
      $response = $_xc_search_server->search($facet_data->query, 0, 0, $facet_data->params, FALSE);
    }
    catch (Exception $e) {
      return array('subject' => t('Faceted navigation'), 'content' => '');
    }
  }

  if ($xc_statistics_enabled) {
    $statistics = $facet_data->statistics;
    $statistics['hits'] = $response->response->numFound;
    $statistics['solr_all'] = microtime(TRUE) - $t_solr;
    $statistics['solr'] = ($search_ui->use_distinct_search == 1) ? $response->responseHeader->QTime / 1000 : 0;
    $t_display = microtime(TRUE);
    $GLOBALS['timers']['block/view/facets/full solr'] = array(
      'counter' => 1,
      'time' => (int) ($statistics['solr_all'] * 1000),
    );
  }
  if ($search_ui->use_distinct_search == 1) {
    $GLOBALS['timers']['block/view/facets/pure solr'] = array(
      'counter' => 1,
      'time' => $response->responseHeader->QTime,
    );
  }

  // DISPLAY each facets
  $options = array(
    'search_path' => $facet_data->search_path,
    'active_filters' => $facet_data->active_filters,
    'non_filter_params' => $facet_data->non_filter_params,
    'is_prepopulated' => $facet_data->is_prepopulated,
    'field_limit_modifiers' => $facet_data->field_limit_modifiers,
  );

  $query_url = array();
  if (preg_match('/\?/', $current_path)) {
    parse_str(preg_replace('/^.*\?/', '', $current_path), $query_url);
  }

  // $facet_objects
  $hidden_facets = '';
  $normal_facets = '';
  $format_options = array('exclude' => array('Not online', 'Not microform'));

  foreach ($facet_data->collections['facets'] as $field => $facet) {
    $type  = $facet['type'];

    // get field facets
    if ($type == 'facet_fields'
         && isset($response->facet_counts->$type)
         && isset($response->facet_counts->$type->$field)) {
      if ($field == 'format_fc') {
        $links = xc_search_block_get_facet_fields(
          $field,
          $facet['label'],
          $response->facet_counts->$type->$field,
          array_merge($options, $format_options)
        );
      }
      else {
        $links = xc_search_block_get_facet_fields($field, $facet['label'],
          $response->facet_counts->$type->$field, $options);
      }

      if (!empty($links)) {
        $param_name = 'f.' . $field . '.facet.limit';

        if (isset($facet_data->params[$param_name])) {
          $default_limit = $facet_data->params[$param_name];
        }
        else {
          $default_limit = $facet_data->params['facet.limit'];
        }

        // handle hidden format
        if ($field == 'format_fc') {
          $default_limit -= count($format_options['exclude']);
        }

        if (isset($options['field_limit_modifiers'][$field])) {
          $default_limit -= count($options['field_limit_modifiers'][$field]);
        }
        if (count($links) >= $default_limit) {
          $query_url['query'] = $facet_data->query;
          $query_url['facet'] = $field;
          $links[] = l(t('More&hellip;'), 'xc_search/more_facets',
            array(
              'query' => $query_url,
              'attributes' => array('class' => array('xc-search-facet-more-link')),
              'html' => TRUE,
            )
          );
        }
      }
    }

    // get date facets
    elseif ($type == 'facet_dates') {
      // get date query facets before
      $links = array();
      $links = array_merge($links, xc_search_block_get_date_facet_query_links($field,
        $response, $facet_data->collections, $options, 'before'));

      $links = array_merge($links, xc_search_block_get_date_facet_query_links($field,
        $response, $facet_data->collections, $options, 'main'));

      // get after date query facets
      $links = array_merge($links, xc_search_block_get_date_facet_query_links($field,
        $response, $facet_data->collections, $options, 'after'));
    }

    // TODO: facet_queries
    elseif ($type == 'facet_queries') {

    }

    // theming links
    if (!empty($links)) {
      $id = 'fieldset-' . $field;
      $collapsed = module_exists('fieldset_helper')
        ? fieldset_helper_state_manager_get_state($id, $facet['is_collapsed'])
        : $facet['is_collapsed'];
      $fieldset = array(
        '#title' => $facet['label'],
        '#attributes' => array('id' => $id),
        '#collapsible' => TRUE,
        '#collapsed' => $collapsed,
        '#value' => theme('item_list', array('items' => $links)),
        '#attributes' => array('class' => array('xc-search-facet')),
        '#children' => '',
      );
      $facet_list = theme('fieldset', array('element' => $fieldset));
      if (in_array($field, $facet_data->collections['hidden_facets'])) {
        $hidden_facets .= $facet_list;
      }
      else {
        $normal_facets .= $facet_list;
      }
    }
  }

  $block['subject'] = t('');
  $block['content'] = $hidden_facets . $normal_facets;
  if ($xc_statistics_enabled) {
    $statistics['display'] = microtime(TRUE) - $t_display;
    $statistics['total'] = microtime(TRUE) - $statistics['t_start'];
    unset($statistics['t_start']);
    xc_statistics_save('facet', $statistics);
  }
  return $block;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_block_facet_activefilters($current_path) {
  $block = array();
  $facet_data = xc_search_block_facet_prepare($current_path);
  if (!empty($facet_data->content['filter_list'])) {
    $block['content'] = theme('xc_search_block_facet_activefilters', array('active_filters' => $facet_data->content['filter_list']));
  }
  return $block;
}

/**
 * Theme for active filters in the facet block.
 *
 * @param $active_filters (Array)
 *   The active filter links
 *
 * @return (String)
 *   The themed list of active filters
 */
function theme_xc_search_block_facet_activefilters($variables) {
  $active_filters = $variables['active_filters'];
  return '<div id="xc-search-active-filters">'
       .   theme('item_list', array('items' => $active_filters, 'title' => t('Filtering by:')))
       . '</div>';
}
/**
 * Get the list of field type facets as links
 * @param $field (string)
 *   The name of field
 * @param $field_label (string)
 *   The human readable field name
 * @param $terms (array)
 *   The facet terms. Its items are array or term and count number.
 * @param $options (Array)
 *   An associative array of parameters.
 *     'search_path' - (String) The current path with search term
 *     'active_filters'     - (Array) Active filters
 *     'non_filters_params' - (Array) Other parameters in URL, which are not filters
 *     'is_prepopulated' - (Boolean) Flag showing whether the facets created
 *       as prepopulated (without query term), or not
 * @return (Array)
 *   List of links which could filter the current search
 */
function xc_search_block_get_facet_fields($field, $field_label, $terms, $options) {
  $links = array();
  foreach ($terms as $term => $count) {
    // skip the active filters
    if (isset($options['field_limit_modifiers'][$field])
         && in_array($term, $options['field_limit_modifiers'][$field])) {
      continue;
    }
    if (isset($options['exclude']) && in_array($term, $options['exclude'])) {
      continue;
    }

    $query = $field . ':"' . $term . '"';
    $path = $options['search_path'];
    if ($options['is_prepopulated'] == TRUE) {
      $path .= '/*'; // . $query;
    }
    $url_query_params = array_merge(
      $options['non_filter_params'],
      array('filter' => array_merge($options['active_filters'], array($query)))
    );
    $filter_by_label = $field_label . ' = ' . $term;
    $links[] = theme('xc_search_block_facet_link', array('term' => $term, 'count' => $count, 'path' => $path, 'filter_by_label' => $filter_by_label, 'query' => $url_query_params));
  }
  return $links;
}

/**
 * Creates a date query facet link.
 *
 * @param $query (String)
 *   The Solr query
 * @param $label (String)
 *   The label of the link
 * @param $count (int)
 *   The number of documents belongs to this facet term
 * @param $options (Array)
 *   General information and URL parameters
 *
 * @return (String)
 *   The link
 */
function xc_search_block_get_facet_query_link($query, $label, $count, $options) {
  if (empty($label)) {
    $label = xc_search_block_get_label('facet_queries', $query);
  }

  $path = $options['search_path'];
  if ($options['is_prepopulated'] == TRUE) {
    $path .= '/*'; // . $query;
  }
  $url_query_params = array_merge(
    $options['non_filter_params'],
    array('filter' => array_merge($options['active_filters'], array($query)))
  );

  return theme('xc_search_block_facet_link', array('term' => $label, 'count' => $count, 'path' => $path, 'filter_by_label' => $label, 'query' => $url_query_params));
}

/**
 * Collects links displaying before and after the main date facet.
 *
 * @param $field (String)
 *   The Solr field name which is the base of the facet
 * @param $response (Object)
 *   The Solr response
 * @param $collections (Array)
 *   General informations about facets and facet properties
 * @param $options (Array)
 *   Link options, holding general additional information
 * @param $position (String)
 *   The position of the links relative to the main date facet: 'before' or 'after'.
 */
function xc_search_block_get_date_facet_query_links($field, &$response, &$collections, $options, $position = 'after') {

  $links = array();
  /*
   if (in_array($field, array_keys($options['field_limit_modifiers']))) {
   return $links;
   }
   */
  $gap_str = isset($_GET[$field . '_gap']) ? $_GET[$field . '_gap'] : '';
  if (empty($gap_str)) {
    $gap_str = $collections['facets'][$field]['params']->gap;
  }
  $gap = new XCGap($gap_str);

  foreach ($options['active_filters'] as $i => $filter) {
    if (preg_match('/^' . $field . ':/', $filter)) {
      unset($options['active_filters'][$i]);
    }
  }

  switch ($position) {
    case 'before':
    case 'after':
      // get the additional date queries for this field at this position
      $queries = &$collections['date_facet_query_display_map'][$field][$position];
      if (!empty($queries)) {
        // order by weight
        ksort($queries);
        foreach ($queries as $weight => $query_obj) {
          $options['non_filter_params'][$field . '_gap'] = $query_obj->gap;
          $query = $field . ':' . $query_obj->query;
          if (!in_array($query, $options['active_filters'])
               && $response->facet_counts->facet_queries->$query > 0) {
            $links[] = xc_search_block_get_facet_query_link(
              $query,
              $query_obj->label,
              $response->facet_counts->facet_queries->$query,
              $options
            );
          }
        }
      }
      break;
    case 'main':
    default:
      // get the otherwise handled queries
      $handled_queries = array();
      $handled_query_objects = array();
      foreach (array('before', 'after') as $key) {
        if (isset($collections['date_facet_query_display_map'][$field][$key])
             && is_array($collections['date_facet_query_display_map'][$field][$key])) {
          $handled_query_objects += array_values($collections['date_facet_query_display_map'][$field][$key]);
        }
      }
      if (!empty($handled_query_objects)) {
        foreach ($handled_query_objects as $query_obj) {
          $handled_queries[] = $field . ':' . $query_obj->query;
        }
      }
      $current_gap = $gap->text;
      $gap->scale_down();
      $new_gap = $gap->text;
      $do_link = FALSE;
      if ($new_gap != $current_gap) {
        $options['non_filter_params'][$field . '_gap'] = $new_gap;
        $do_link = TRUE;
      }

      // iterate over the facet queries
      if (!empty($response->facet_counts->facet_queries)) {
        foreach ($response->facet_counts->facet_queries as $query => $count) {
          if ($count > 0 && !in_array($query, $handled_queries)) {
            if (preg_match('/^' . $field . ':(.*)$/', $query, $matches) !== FALSE) {
              $label = xc_search_date_query_label($field, $matches[1], $collections);
              if (in_array($query, $collections['active_filters'])) {
                $links[] = sprintf('%s (%d)', $label, $count);
              }
              else {
                $links[] = xc_search_block_get_facet_query_link($query, $label, $count, $options);
              }
            }
          }
        }
      }
      break;
  }
  return $links;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_date_query_label($field, $query, &$collections) {
  $gap_str = isset($_GET[$field . '_gap']) ? $_GET[$field . '_gap'] : '';
  if (empty($gap_str)) {
    $gap_str = $collections['facets'][$field]['params']->gap;
  }
  $gap = xc_solr_parse_gap($gap_str);
  $value = preg_replace('/^(\[|\{)|(\]|\})$/', '', $query);
  list($from, $to) = explode(' TO ', $value);
  $from = drupal_substr($from, 0, $gap->date_part_length);
  $to = drupal_substr($to, 0, $gap->date_part_length);
  $label = ($from != $to) ? $from . '-' . $to : $from;

  return $label;
}
/**
 * Get the list of date type facets as links
 * @param $field (string)
 *   The name of field
 * @param $field_label (string)
 *   The human readable field name
 * @param $terms (array)
 *   The facet terms. Its items are array or term and count number.
 * @param $options (Array)
 *   An associative array of parameters.
 *     'search_path' - (String) The current path with search term
 *     'active_filters'     - (Array) Active filters
 *     'non_filters_params' - (Array) Other parameters in URL, which are not filters
 *     'is_prepopulated' - (Boolean) Flag showing whether the facets created
 *       as prepopulated (without query term), or not
 * @return (Array)
 *   List of links which could filter the current search
 */
function xc_search_block_get_facet_dates($field, $field_label, $terms, $options) {
  static $unhandled_values;

  $links = array();

  if (empty($unhandled_values)) {
    $unhandled_values = array('gap', 'end', 'before', 'after', 'between');
  }

  $date_filters = xc_search_remove_date_filter($options['active_filters'], $field);
  $keys = array_keys(get_object_vars($terms));

  // parsing gap
  $gap = xc_solr_parse_gap($terms->gap);

  // iterate over terms
  $c = count($keys);
  for ($i = 0; $i < $c; $i++) {
    $term  = $keys[$i];
    $count = $terms->$term;

    if (in_array($term, $unhandled_values)) {
      continue;
    }
    // there is no terms
    if ($count == 0) {
      continue;
    }

    if (preg_match('/^\d{4}-/', $keys[$i + 1])) {
      $next = $keys[$i + 1]; //xc_solr_date_modify($keys[$i + 1], '-1 second');
    }
    else {
      $next = $terms->end;
    }

    if ($gap !== FALSE) {
      if ($gap->date_part_length > 0) {
        $first_date  = drupal_substr($term, 0, $gap->date_part_length);
        $second_date = drupal_substr($next, 0, $gap->date_part_length);
        $term_label = ($first_date != $second_date)
          ? $first_date . '-' . $second_date
          : $first_date;
      }
      else {
        $term_label = $term . '-' . $next;
      }
    }
    else {
      $term_label = $term;
    }

    $query = $field . ':[' . $term . ' TO ' . $next . ']';
    $path = $options['search_path'];

    if ($options['is_prepopulated'] == TRUE) {
      $path .= '/*'; // . urlencode($query);
    }

    $url_query_params = array_merge(
      $options['non_filter_params'],
      array(
      'filter' => array_merge($date_filters, array($query)),
      $field . '_gap' => $terms->gap,
    )
    );

    $links[] = theme('xc_search_block_facet_link', array('term' => $term_label, 'count' => $count, 'path' => $path, 'filter_by_label' => $query, 'query' => $url_query_params));
  }
  return $links;
}

/**
 * Get displayable label based on facet term.
 *
 * @param $type (String)
 *   The type of facet ('facet_fields' or 'facet_dates' or 'facet_queries')
 * @param $facet_term (String)
 *   The facet term
 *
 * @return (String)
 *   The display label of the facet
 */
function xc_search_block_get_label($type, $facet_term, $field_label = NULL, $is_active = FALSE) {
  $content_label = '';
  switch ($type) {
    case 'facet_fields':
      $content_label = preg_replace('/^\((.*)\)$/', '$1', $facet_term);
      $content_label = preg_replace('/^"(.*?)" OR "(.*?)"$/', '$1 OR $2', $content_label);
      $content_label = preg_replace('/^"|"$/', '', $content_label);
      break;

    case 'facet_dates':
      $value = preg_replace('/^\[|\]$/', '', $facet_term);
      list($from, $to) = explode(' TO ', $value);
      $from = drupal_substr($from, 0, 4);
      $to = drupal_substr($to, 0, 4);
      $content_label = ($from != $to) ? $from . '-' . $to : $from;
      break;

    case 'facet_queries':
      $value = preg_replace('/^\[|\]$/', '', $facet_term);
      $value_parts = explode(' TO ', $value);
      $content_label = join('-', $value_parts);
      break;
  }

  return theme('xc_search_block_label', array('field_label' => $field_label, 'label' => $content_label, 'is_active' => $is_active));
}

/**
 * Theme displayable label based on facet term.
 *
 * @param $field_label (String)
 *   The field label
 * @param $content_label (String)
 *   The content label
 *
 * @return (String)
 *   The display label of the facet
 */
function theme_xc_search_block_label($variables) {
  $field_label = $variables['field_label'];
  $content_label = $variables['label'];
  $is_active = $variables['is_active'];
  return ( $field_label ? $field_label . ': ' : '' ) . $content_label;
}

/**
 * Process facet information come from database, and creates URL parameters for
 * Solr.
 *
 * It also fill up the general collection array about facets, and the
 * additional date query facets.
 *
 * @param $facet_objects (Array)
 *   List of facet object
 * @param $params (Array)
 *   Reference to URL parameters
 * @param $collections
 *   Reference to general collections
 */
function xc_search_block_facets_to_parameters($facet_objects, &$params, &$collections) {
  $type_map = array(
    'query' => 'facet_queries',
    'date' => 'facet_dates',
    'field' => 'facet_fields',
  );
  $subjects = array('subject_topic', 'subject_spatial', 'subject_temporal');
  foreach ($subjects as $subject) {
    $index_facet = xc_index_facet_load_by_name($subject);
    $field = $subject . $index_facet->type;
    $collections['subject_facets'][$field] = array(
      'type' => 'facet_fields',
      'field' => $field,
      'label' => t($index_facet->label),
      'is_collapsed' => 1,
    );
  }

  foreach ($facet_objects as $facet) {
    // TODO: handling whether it is a filed, or facet. If it is a field: $index_facet will be NULL
    $index_facet = xc_index_facet_load_by_name($facet->name);
    $field = $facet->name . $index_facet->type;
    $collections['facets'][$field] = array(
      'type' => $type_map[$facet->type],
      'field' => $field,
      'label' => t($index_facet->label),
      'is_collapsed' => $facet->is_collapsed,
    );
    if ($facet->type == 'field') {
      $params['facet.field'][] = $field;
    }
    if (isset($facet->attribute_set_id) && $facet->attribute_set_id != 0) {
      $field_params = xc_search_get_attribute_set($facet->type, $facet->attribute_set_id);
      $collections['facets'][$field]['params'] = $field_params;
      if ($facet->type == 'date') {
        foreach ($collections['active_filters'] as $filter) {
          if (preg_match('/^' . $field . ':[\{\[]([^ ]+) TO ([^ ]+)[\]\}]$/', $filter, $matches)) {
            $field_params->start = $matches[1];
            $field_params->end = $matches[2];
            $field_params->gap = $_GET[$field . '_gap'];
            break;
          }
        }
      }

      $facet_params = xc_search_facet_properties2solr_params($facet->type, $field_params, $field);

      // field base gap
      if ($facet->type == 'date') {
        // $current_gap = $_GET[$field . '_gap'];

        // get additional date quers facets
        include_once drupal_get_path('module', 'xc_search') . '/admin/xc_search_date_facet_properties.inc';
        $date_query_facets = xc_search_date_facet_properties_additionals_get_by_pid($facet->attribute_set_id);

        if (!empty($date_query_facets)) {
          foreach ($date_query_facets as $date_query_facet) {
            $query = $field . ':' . $date_query_facet->query;
            $position = $date_query_facet->position;
            $weight = $date_query_facet->weight;
            $collections['date_facet_query_display_map'][$field][$position][$weight] = $date_query_facet;
            $facet_params['facet.query'][] = $query;
          }
        }
      }
      $params += $facet_params;
    }
  }

  // hidden
  $collections['hidden_facets'] = array();
  foreach ($collections['active_filters'] as $filter) {
    if (preg_match('/^(\w+)_fc:"(.*?)"$/', $filter, $matches)) {
      // TODO: there is an assumption, that all such 'hidden' facets are
      // field facets
      $field_name = $matches[1];
      $index_facet = xc_index_facet_load_by_name($field_name);
      $field = $field_name . $index_facet->type;
      if (isset($collections['facets'][$field])) {
        continue;
      }
      $collections['hidden_facets'][] = $field;
      $collections['facets'][$field] = array(
        'type' => 'facet_fields',
        'field' => $field,
        'label' => t($index_facet->label),
        'is_collapsed' => FALSE,
      );
      $params['facet.field'][] = $field;
      /*
       * TODO: Now there is no facet info, but later...
       if (isset($facet->attribute_set_id) && $facet->attribute_set_id != 0) {
       $field_params = xc_search_get_attribute_set($facet->type, $facet->attribute_set_id);
       $collections['facets'][$field]['params'] = $field_params;
       }
       */
    }
  }

}

/**
 * Creating and theming a facet link
 *
 * @param $term (String)
 *   The facet term to display
 * @param $count (int)
 *   The number of documents matching this term
 * @param $path (string)
 *   The query URL
 * @param $filter_by_label
 *   A label in title attribute of the link
 * @param $query_params (Array)
 *   The query part of the URL
 *
 * @return (String)
 *   The facet link, which filters the content
 */
function theme_xc_search_block_facet_link($variables) {
  $term = $variables['term'];
  $count = $variables['count'];
  $path = $variables['path'];
  $filter_by_label = $variables['filter_by_label'];
  $query_params = $variables['query'];
  return '<span class="xc-search-facet-link">'
    . l('<span class="facet-term">' . t($term) . '</span>', $path, array(
    'attributes' => array('title' => t('Filter by \'!label\'', array('!label' => $filter_by_label))),
    'query' => xc_clean_params($query_params),
    'html' => TRUE,
  ))
    . ' <span class="facet-term-count">(' . $count . ')</span></span>';
}

/**
 * Makes a remove filter link
 *
 * @param $search_path (String)
 *   The Drupal path, like xc/search/query
 * @param $label (string)
 *   The displayed label of active filter, like 'Mozart, Wolfgang Amadeus, 1756-1791.'
 * @param $get_params (Array)
 *   Other URL parameters (the part of the URL after the ? sign)
 *
 * @return (String)
 *   The string to display
 */
function theme_xc_search_block_filter($variables) {
  $search_path = $variables['search_path'];
  $label = $variables['label'];
  $filter_items = explode(':', $label);

  $get_params = $variables['get_params'];
  $alt_text = t("Remove '!label' filter", array('!label' => strip_tags($label)));

  $image = l(
    theme('image', array('path' => drupal_get_path('module', 'xc_search') . '/images/delete.png', 'width' => '', 'height' => '', 'alt' => $alt_text, 'title' => $alt_text)),
    $search_path,
    array(
      'attributes' => array(
        'title' => $alt_text,
        'class' => array('xc-search-active-filter-image'),
        'html' => TRUE,
      ),
      'query' => xc_clean_params($get_params),
      'html' => TRUE,
    )
  );
  $link = l('<span class="xc-search-facet-field-label">'.$filter_items[0].': </span>
             <span class="xc-search-facet-label">'.$filter_items[1].'</span>', $search_path.'nodd', array(
    'attributes' => array(
      'title' => $alt_text.'testing',
      'class' => array('xc-search-active-filter-link'),
    ),
    'query' => xc_clean_params($get_params),
    'html' => TRUE,
  ));
  return '<span class="xc-search-active-filter">' . $image . $link . '</span>';
}


/**
 * Gets the content of result navigation block at the full record display (node/123)
 *
 * @param $node_id (int)
 *   The node identifier
 * @param $location (String)
 *   The location of the block
 *
 * @return (Array)
 *   The content of block.
 */
function xc_search_block_result_navigation_prepare($node_id, $location = 'top') {
  global $user, $_xc_search_server;
  static $cache;

  if (isset($cache[$node_id])) {
    return $cache[$node_id];
  }

  /*
   * TODO
   * the following if statement prevents creating the meta object for nodes come from
   * the More Like This list. We have to do the following things:
   * - rename caller param to 'mlt' or something like that
   * - the 'hit' parameter should be the 'hit' param of the original node, if there are
   * - keep track of the original caller
   */

  // Only show for metadata returned with a hit created by
  // xc_search_get_full_display_url()
  if (!isset($_GET['hit'])
       || $_GET['hit'] < 1
       || !is_metadata_node(node_load($node_id))
       || !isset($_SESSION['xc_search_query'])) {
    return;
  }

  $links = array();
  $caller = xc_get_caller();
  $caller_param = $caller['param'];
  $caller_module = $caller['module'];

  $meta = new stdClass();
  if (isset($_SESSION['xc_search_query'][$caller_module])) {
    $query       = $_SESSION['xc_search_query'][$caller_module]['query'];
    $params      = $_SESSION['xc_search_query'][$caller_module]['solr_params'];
    $search_page = $_SESSION['xc_search_query'][$caller_module]['search_page'];
    $meta->rows  = $rows_per_page = $_SESSION['xc_search_query'][$caller_module]['rows_per_page'];
    $meta->total = $total_results = $_SESSION['xc_search_query'][$caller_module]['total_results'];
  }

  $hit = max(1, intval($_GET['hit']));
  $meta->last = $meta->first = $hit;
  $meta->extra = array(
    'list' => array(),
    'prev' => array(),
    'next' => array(),
    'caller_module' => $caller_module,
  );

  $first = ($hit == 1) ? 0 : $hit - 2;
  if (isset($total_results)) {
    $rows  = ($hit == $total_results || $first == 0) ? 2 : 3;
  }

  if ($caller_param == 'bookmark') {
    $meta->total = xc_account_get_bookmarked_item_count();
    // $current = xc_account_get_bookmarked_item($hit);
    if ($hit > 1) {
      $prev = xc_account_get_bookmarked_previous_item($hit);
    }
    $next = xc_account_get_bookmarked_next_item($hit);

    $meta->extra['list'] = array(
      'label' => t('Back to !total results', array('!total' => $meta->total)),
      'url' => 'user/' . $user->uid . '/bookmarked-items',
      'query' => NULL,
      'fragment' => NULL,
    );

    if (isset($prev)) {
      $meta->extra['prev'] = array(
        'label' => t('Previous Result'),
        // 'doc_title' => xc_util_conditional_join(' &mdash; ', $doc->dcterms__title_t),
        'url' => 'node/' . $prev->nid,
        'query' => array(
          'hit' => ($prev->id),
          'caller' => $caller_param,
        ),
      );
    }

    if ($next) {
      $meta->extra['next'] = array(
        'label' => t('Next Result'),
        // 'doc_title' => xc_util_conditional_join(' &mdash; ', $doc->dcterms__title_t),
        'url' => 'node/' . $next->nid,
        'query' => array(
          'hit' => ($next->id),
          'caller' => $caller_param,
        ),
      );
    }
  }
  else {
    $removable_keys = array(
      'hl',
      'hl.fl',
      'hl.fragsize',
      'hl.simple.pre',
      'hl.simple.post',
      'hl.snippets',
      'facet',
      'facet.limit',
      'facet.query',
      'facet.field',
      'facet.field',
      'facet.mincount',
      'f.format_fc.facet.limit',
    );
    foreach ($removable_keys as $key) {
      unset($params[$key]);
    }
    $params += array(
      'fl' => 'dcterms__title_t,node_id_i',
    );
    $params['XCNAME'] = 'result_navigation_block';

    try {
      $response = $_xc_search_server->search($query, $first, $rows, $params, FALSE);
    }
    catch (Exception $e) {
      xc_log_error('navig_block', 'Query: ' . $query . ', params: ' . var_export($params, TRUE) . ', exception: ' . var_export($e, TRUE));
      return;
    }

    // Hit list
    $url = parse_url(urldecode($search_page['destination']));
    if (isset($url['query'])) {
      parse_str($url['query'], $url['query']);
    }
    else {
      $url['query'] = array();
    }
    if (!isset($url['fragment'])) {
      $url['fragment'] = '';
    }
    $page = intval(($hit - 1) / $rows_per_page);

    if ($page > 0) {
      $url['query']['page'] = $page;
      /*
      $url['query'] = preg_replace('/((?:^|&)page=)\d+/', '${1}' . $page, $url['query'], -1, $count);
      if ($count == 0) {
        $url['query'] .= '&page=' . $page;
      }
      */
    }

    $links['list'] = l(
      t('Back to !total results', array('!total' => $total_results)),
      $url['path'],
      array('query' => $url['query'], 'fragment' => $url['fragment'])
    );

    $meta->extra['list'] = array(
      'label' => t('Back to !total results', array('!total' => $total_results)),
      'url' => $url['path'],
      'query' => $url['query'],
      'fragment' => $url['fragment'],
    );

    // Previous
    if ($hit > 1) {
      $doc = $response->response->docs[0];
      $links['prev'] = l(
        t('Previous Result'),
        'node/' . $doc->node_id_i,
        array(
        'attributes' => array('title' => xc_util_conditional_join(' - ', $doc->dcterms__title_t)),
        'query' => array(
          'hit' => ($hit - 1),
          'caller' => $caller_param,
        ),
        'html' => TRUE,
      )
      );

      $meta->extra['prev'] = array(
        'label' => t('Previous Result'),
        'doc_title' => xc_util_conditional_join(' &mdash; ', $doc->dcterms__title_t),
        'url' => 'node/' . $doc->node_id_i,
        'query' => array(
          'hit' => ($hit - 1),
          'caller' => $caller_param,
        ),
      );
    }

    // Next
    if ($hit < $total_results) {
      $doc = $response->response->docs[$rows - 1];
      $links['next'] = l(t('Next Result'), 'node/' . $doc->node_id_i, array(
        'attributes' => array('title' => xc_util_conditional_join(' - ', $doc->dcterms__title_t)),
        'query' => array(
          'hit' => ($hit + 1),
          'caller' => $caller_param,
        ),
        'html' => TRUE,
      ));

      $meta->extra['next'] = array(
        'label' => t('Next Result'),
        'doc_title' => xc_util_conditional_join(' &mdash; ', $doc->dcterms__title_t),
        'url' => 'node/' . $doc->node_id_i,
        'query' => array(
          'hit' => ($hit + 1),
          'caller' => $caller_param,
        ),
      );
    }
  }
  $cache[$node_id] = $links;

  xc_search_meta($meta);
  return $links;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_block_result_navigation($node_id, $location = 'top') {
  $links = xc_search_block_result_navigation_prepare($node_id, $location = 'top');
  // Output
  return array(
    'subject' => '<none>',
    'content' => theme('xc_search_block_result_navigation', array('links' => $links)),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_xc_search_block_result_navigation($variables) {
  $links = $variables['links'];
  return '<div id="xc-search-result-navigation">'
       . '<div class="result-nav result-prev">' . (isset($links['prev']) ? $links['prev'] : '') . '</div>'
       . '<div class="result-nav result-next">' . (isset($links['next']) ? $links['next'] : '') . '</div>'
       . '<div class="result-nav result-list">' . (isset($links['list']) ? $links['list'] : '') . '</div>'
       . '</div>';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_block_info_bar($location, $screen = NULL) {
  if (is_null($screen)) {
    if (arg(0) == 'xc_browse') {
      $screen = arg(0);
    }
    elseif (arg(0) . '/' . arg(1) == 'xc/search'
           || arg(0) . '/' . arg(1) == 'search/xc_search') {
      $screen = 'xc_search';
    }
    elseif (preg_match('{^user/\d+/bookmarked-items$}', $_GET['q'])) {
      $screen = 'xc_account';
    }
    elseif (arg(0) == 'node') {
      $screen = 'node';
      // This function populates the $meta
      xc_search_block_result_navigation_prepare(arg(1), $location);
    }
  }

  include_once drupal_get_path('module', 'xc_search') . '/xc_search.pages.inc';
  $content = '';
  $meta = xc_search_meta();
  if ($screen != 'xc_account' && !isset($meta)) {
    return array('content' => $content);
  }

  if ($screen == 'xc_account') {
    $content = theme('xc_search_info_bar', array('meta' => $meta, 'components' => array('actions'), 'type' => 'primary', 'number' => '004', 'location' => $location, 'module' => $screen));
  }
  elseif ($screen == 'node') {
    $content = theme('xc_search_info_bar', array('meta' => $meta, 'components' => array('back_to_results', 'pager'), 'type' => 'primary', 'number' => '003', 'location' => $location, 'module' => $screen));
  }
  elseif (isset($meta->total) && $meta->total > 0 && !($location == 'bottom' && $meta->total < 5)) { //
    if ($meta->total > $meta->rows) {
      // with pager
      if ($screen == 'xc_search' || $screen == 'xc_browse') {
        if ($location == 'top') {
          $content = theme('xc_search_info_bar', array('meta' => $meta, 'components' => array('actions', 'form'), 'type' => 'primary', 'number' => '001a', 'location' => $location, 'module' => $screen));
          $content .= theme('xc_search_info_bar', array('meta' => $meta, 'components' => array('pager', 'numbers'), 'type' => 'secondary', 'number' => '001a', 'location' => $location, 'module' => $screen));
        }
        else {
          $content = theme('xc_search_info_bar', array('meta' => $meta, 'components' => array('pager', 'numbers'), 'type' => 'secondary', 'number' => '001a', 'location' => $location, 'module' => $screen));
          $content .= theme('xc_search_info_bar', array('meta' => $meta, 'components' => array('actions', 'form'), 'type' => 'primary', 'number' => '001a', 'location' => $location, 'module' => $screen));
        }
      }
      // elseif ($screen == 'xc_browse') {
//   $content = theme('xc_search_info_bar', $meta, array('actions', 'pager', 'form'), 'primary', '002a', $location, $screen);
      // }
    }
    else {
      if ($screen == 'xc_search' || $screen == 'xc_browse') {
        $content = theme('xc_search_info_bar', array('meta' => $meta, 'components' => array('actions', 'form', 'numbers'), 'type' => 'primary', 'number' => '001b', 'location' => $location, 'module' => $screen));
      }
    }
  }

  return array('content' => $content);
}

function xc_get_caller() {
  if (isset($_GET['caller'])) {
    if ($_GET['caller'] == 'xc-browse') {
      $caller['param']  = $_GET['caller'];
      $caller['module'] = 'xc_browse';
      $caller['use_default'] = FALSE;
    }
    elseif ($_GET['caller'] == 'xc-search') {
      $caller['param']  = $_GET['caller'];
      $caller['module'] = 'xc_search';
      $caller['use_default'] = FALSE;
    }
    elseif (in_array($_GET['caller'], array('bookmark', 'checked-out', 'request'))) {
      $caller['param']  = $_GET['caller'];
      $caller['module'] = 'xc_account';
      $caller['use_default'] = FALSE;
    }
  }
  else {
    $caller['param']  = 'xc-search';
    $caller['module'] = 'xc_search';
    $caller['use_default'] = TRUE;
  }
  return $caller;
}
