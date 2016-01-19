<?php
/**
 * @file
 * A Drupal interface for Luke
 *
 * More about the Luke calls see http://wiki.apache.org/solr/LukeRequestHandler
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Display information about the Solr index
 *
 * @return (String)
 *   The tables displaying the state of the index
 */
function xc_solr_info() {
  global $_xc_search_server;
  drupal_set_time_limit(0);

  xc_log_info('solr_info', 'max_execution_time: ' . ini_get('max_execution_time'));
  if ((isset($_GET['force']) && $_GET['force'] == 1)) {
    $output = xc_sorl_info_get();
  }
  else {
    $params = array(
      'search_type' => 'luke',
      'fl' => 'type',
      'numTerms' => 0,
    );
    $response = $_xc_search_server->search(NULL, 0, 0, $params, TRUE);
    $xc_solr_info_last_modified = variable_get('xc_solr_info_last_modified', NULL);
    $xc_solr_info_directory = variable_get('xc_solr_info_directory', NULL);
    if (is_null($xc_solr_info_last_modified)
        || $response->index->lastModified > $xc_solr_info_last_modified
        || $response->index->directory != $xc_solr_info_directory
        || variable_get('xc_solr_info', '') == '') {
      $output = xc_sorl_info_get();
    }
    else {
      $output = variable_get('xc_solr_info', '');
    }
  }
  return $output;
}

/**
 * Get the highest and lowest values of a given date type field
 *
 * highest value:
 * http://localhost:8983/solr/select?q=*:*&rows=1&fl=date&sort=date%20asc
 * highest value:
 * http://localhost:8983/solr/select?q=*:*&rows=1&fl=date&sort=date%20desc
 *
 * @param $date_field (String)
 *   The name of the date field we would like to use
 */
function xc_sorl_info_get_dates($date_field = 'date_df') {
  global $_xc_search_server;

  $dates = variable_set('xc_solr_info_dates', array());

  $params = array(
    'fl' => $date_field,
    'sort' => $date_field . ' asc',
  );
  $response = $_xc_search_server->search('*:*', 0, 0, $params, TRUE);
  $dates[$date_field]['min'] = $response->response->docs[0]->$date_field;

  $params['sort'] = 'date_df desc';
  $response = $_xc_search_server->search('*:*', 0, 0, $params, TRUE);
  $dates[$date_field]['max'] = $response->response->docs[0]->$date_field;

  variable_set('xc_solr_info_dates', $dates);
}

/**
 * Get information about the Solr index
 * @return unknown_type
 */
function xc_sorl_info_get() {
  global $_xc_search_server;
  drupal_set_time_limit(0);
  ini_set('default_socket_timeout', 600);

  $params = array(
    'search_type' => 'luke',
  );
  $response = $_xc_search_server->search(NULL, 0, 0, $params, TRUE);
  variable_set('xc_solr_info_last_modified', $response->index->lastModified);
  variable_set('xc_solr_info_directory', $response->index->directory);
  $fields = array_keys(get_object_vars($response->fields));

  $index_features = array(
    'numDocs' => t('Number of documents'),
    'numTerms' => t('Number of terms'),
    'version' => t('Solr version release date'),
    'optimized' => t('Index is optimized?'),
    'current' => t('Current commit user data?'),
    'hasDeletions' => t('Index has deletions?'),
    'lastModified' => t('Last modified'),
    'directory' => t('Index directory')
  );
  $types =
  $field_features = array(
    'type' => t('Type'),
    'schema' => t('Indexing properties'),
    'dynamicBase' => t('Field suffix'),
    'docs' => t('The number of documents it appears in'),
    'distinct' => t('The number of distinct values'),
  );

  // index features
  $rows = array();
  $rows[] = array(t('Number of fields'), count($fields));
  foreach ($index_features as $feature => $label) {
    if (!isset($response->index->$feature)) {
      continue;
    }
    $value = $response->index->$feature;
    switch ($feature) {
      case 'hasDeletions':
      case 'optimized':
      case 'current':
        $value = $value ? t('Yes') : t('No');
        break;

      case 'version':
        $value = format_date(intval($value / 1000), 'custom', 'Y-m-d h:m:s');
        break;

      case 'lastModified':
        $value = str_replace('T', ' ', str_replace('Z', '', $value));
        break;

      case 'numDocs':
      case 'numTerms':
        $value = number_format($value, 0, ',', ' ');
        break;

      case 'directory':
        $value = str_replace('org.apache.lucene.store.MMapDirectory:org.apache.lucene.store.MMapDirectory@', 'MMapDirectory=', $value);
        $value = preg_replace('/org.apache.lucene.store.NativeFSLockFactory@[0-9a-f]+/', 'NativeFSLockFactory', $value);
        break;
    }
    $rows[] = array($label, $value);
  }
  $output = theme('table', array(
    'header' => array(t('Feature'), t('Value')),
    'rows' => $rows,
    'attributes' => array(),
    'caption' => t('The state of Solr index')
  ));

  // field features
  $rows = array();
  sort($fields);
  foreach ($fields as $field) {

    $features = array();
    foreach ($field_features as $feature => $label) {
      if (isset($response->fields->$field->$feature)) {
        $value = $response->fields->$field->$feature;
        switch ($feature) {
          case 'docs':
          case 'distinct':
            $value = number_format($value, 0, ',', ' ');
            break;

          case 'schema':
            $human_readable_value = array();
            for ($i = 0, $l = strlen($value); $i < $l; $i++) {
              $char = drupal_substr($value, $i, 1);
              if (isset($response->info->key->$char)) {
                $human_readable_value[] = $response->info->key->$char;
              }
            }
            $value = sprintf("%s (%s)", join(', ', $human_readable_value), $value);
            break;
        }
        $features[] = array(
          'data' => array(array(
              'data' => $label,
              'width' => '25%'
            ), $value),
          'valign' => 'top',
        );
      }
    }
    if (isset($response->fields->$field->topTerms)) {
      $top_terms = array();
      foreach ($response->fields->$field->topTerms as $term => $count) {
        switch ($response->fields->$field->type) {
          case 'string':
          case 'date':
            $query = $field . ':"' . $term . '"';
            break;
          default:
            $query = $field . ':(' . $term . ')';
            break;
        }
        $link = l($term, 'xc/search/' . $query, array('query' => array('debug' => 1)));
        $top_terms[] = sprintf("%s (%s)", $link, number_format($count, 0, ',', ' '));
      }
      if (!empty($top_terms)) {
        $features[] = array(
          'data' => array(t('Top terms'), theme('item_list', array('items' => $top_terms))),
          'valign' => 'top'
        );
      }
    }
    $field_info = theme('table', array('header' => array(t('Feature'), t('Value')), 'rows' => $features));
    $rows[] = array(
      'data' => array(array(
          'data' => $field,
          'width' => '25%',
        ), $field_info),
      'valign' => 'top',
    );
  }
  $output .= theme('table', array(
    'header' => array(t('Field name'), t('Features')),
    'rows' => $rows,
    'attributes' => array(),
    'caption' => t('Properties of fields')
  ));
  variable_set('xc_solr_info', $output);

  return $output;
}

/**
 * Show statistics about searches
 *
 * @return (String)
 *   The sortable statistical table of searches.
 */
function xc_solr_search_statistics() {
  $output = '';
  $header = array(
    array('data' => t('id'), 'field' => 'id', 'valign' => 'top'),
    array('data' => t('total'), 'field' => 'total', 'valign' => 'top'),
    array('data' => t('UI'), 'valign' => 'top'),
    array('data' => t('query'), 'valign' => 'top'),
    array('data' => t('timestamp'), 'field' => 'timestamp', 'valign' => 'top'),
    array('data' => t('hits'), 'field' => 'hits', 'valign' => 'top'),
    array('data' => t('all Solr'), 'field' => 'solr_all', 'valign' => 'top'),
    array('data' => t('inside solr'), 'field' => 'solr', 'valign' => 'top'),
    array('data' => t('solr->array'), 'field' => 'solr_to_array', 'valign' => 'top'),
    array('data' => t('node creation'), 'field' => 'create_node', 'valign' => 'top'),
    array('data' => t('elements'), 'field' => 'template_elements', 'valign' => 'top'),
    array('data' => t('collect table'), 'field' => 'collect_table', 'valign' => 'top'),
    array('data' => t('theme'), 'field' => 'theme', 'valign' => 'top'),
    array('data' => t('xml_links'), 'field' => 'xml_links', 'valign' => 'top'),
    array('data' => t('image fields'), 'field' => 'images_extract_fields', 'valign' => 'top'),
    array('data' => t('image identifiers'), 'field' => 'images_identifiers', 'valign' => 'top'),
  );

  // TODO From Tom, tablesort_sql is not in drupal 7. need to conver to make it work with d7
  //$sql = 'SELECT * FROM {xc_statistics_search}' . tablesort_sql($header); // LIMIT 10
  $sql = 'SELECT * FROM {xc_statistics_search}' . tablesort_sql($header);
  $limit = 25;
  // TODO From Tom, need to check pager_query
  $result = pager_query($sql, $limit);
  $rows = array();
  $fields = array('solr_all', 'solr', 'solr_to_array', 'create_node', 'template_elements',
    'collect_table', 'theme', 'xml_links', 'images_extract_fields', 'images_identifiers');
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $cell_data = array(
      $data->id,
      $data->total,
      l($data->ui_name, 'admin/xc/search/ui/' . $data->ui_id),
      preg_replace(array('/\?$/', '/%3A/', '/%22/'), array('', ':', '"'),
        preg_replace('/(&amp;|\?)\d=-type%3Awork&amp;\d=-type%3Aexpression&amp;\d=-type%3Aholdings&amp;\d=-type%3Aitem/',
        '', $data->query)
      ),
      format_date($data->timestamp, 'custom', 'Y-m-d H:i'),
      $data->hits,
    );
    foreach ($fields as $field) {
      $value = $data->$field;
      if ($value == 0) {
        $cell_data[] = $value;
      }
      else {
        $cell_data[] = sprintf("%.2f&nbsp;%.0f%%", $value, $value * 100 / $data->total);
      }
    }

    $cells = array(
      'data' => $cell_data,
      'valign' => 'top',
    );
    $rows[] = $cells;
  }

  // TODO Please change this theme call to use an associative array for the $variables parameter.
  $pager = theme('pager');
  $output .= $pager;
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= $pager;
  return $output;
}

/**
 * Show statistics about facets
 *
 * @return (String)
 *   The sortable statistical table of facets.
 */
function xc_solr_facet_statistics() {

  $header = array(
    array('data' => t('total'), 'field' => 'total'),
    'UI',
    'query',
    array('data' => t('hits'), 'field' => 'hits'),
    array('data' => t('prepare'), 'field' => 'prepare'),
    array('data' => t('all solr'), 'field' => 'solr_all'),
    array('data' => t('solr'), 'field' => 'solr'),
    array('data' => t('display'), 'field' => 'display'),
  );

  $sql = 'SELECT * FROM {xc_statistics_facet}' . tablesort_sql($header);
  $limit = 25;
  $result = pager_query($sql, $limit);
  $rows = array();
  foreach ($result as $data) {
    $cells = array(
      'data' => array(
        array(
          'data' => sprintf("%.3f", $data->total),
          'style' => 'font-weight: bolder;'
        ),
        l($data->ui_name, 'admin/xc/search/ui/' . $data->ui_id),
        preg_replace(array('/\?$/', '/%3A/', '/%22/'), array('', ':', '"'),
          preg_replace('/(&amp;|\?)\d=-type%3Awork&amp;\d=-type%3Aexpression&amp;\d=-type%3Aholdings&amp;\d=-type%3Aitem/',
          '', $data->query)
        ),
        $data->hits,
        sprintf("%.3f (%.3f%%)", $data->prepare, $data->prepare * 100 / $data->total),
        sprintf("%.3f (%.3f%%)", $data->solr_all, $data->solr_all * 100 / $data->total),
        sprintf("%.3f (%.3f%%)", $data->solr, $data->solr * 100 / $data->total),
        sprintf("%.3f (%.3f%%)", $data->display, $data->display * 100 / $data->total),
      ),
      'valign' => 'top',
    );
    $rows[] = $cells;
  }

  // TODO Please change this theme call to use an associative array for the $variables parameter.
  $pager = theme('pager');
  $output .= $pager;
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= $pager;
  return $output;
}

/**
 * Get the number of document in Solr index
 */
function xc_solr_info_get_solr_document_number() {
  global $_xc_search_server;

  $params = array(
    'search_type' => 'luke',
    'fl' => 'type',
    'numTerms' => 0,
  );
  $response = $_xc_search_server->search(NULL, 0, 0, $params, TRUE);

  return $response->index->numDocs;
}
