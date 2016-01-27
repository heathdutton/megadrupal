<?php

/**
 * Search LOM objects using query string and facets
 * with either SOLR of the native Drupal search
 *
 * @param string $query
 * @param array $facets
 * @param array $options
 *
 * @return
 * An associative array with element 'results' giving the search results,
 * and 'num_results' giving the total number of results found.
 *
 */
function archibald_search_notices($query = '', $facets = array(), $options = array()) {
  global $language;

  $defaults = array(
    'search_type' => 'native',
    'num_per_page' => 10,
    'restrict_lang' => TRUE
  );

  $opts = array_merge( $defaults, $options );

  if ($opts['search_type'] == 'solr') {
    // Use the SOLR engine
    $search = new ArchibaldLomSolrSearch();
    $search->init(FALSE);
    $search->setLimit($opts['num_per_page']);
    $search->setStart(pager_find_page() * $opts['num_per_page']);
    $search->enableFacets();

    if (!empty($query)) {
      $in_query_filters = array(
        'learning_resourcetype' => 'sm_learning_resourcetype',
        'intendet_enduserrole' => 'sm_intendet_enduserrole',
        'difficulty' => 'ss_difficulty',
        'education_context' => 'sm_context',
        'keywords' => 'tm_keywords',
        'coverage' => 'tm_coverage',
        'author' => 'sm_author',
        'editor' => 'tm_contributor',
        'publisher' => 'tm_contributor',
      );
      $query = strtr($query, $in_query_filters);

      $search->setQuery($query);
    }

    // By default, the option is to search only in the current language of the session
    if ($opts['restrict_lang']) {
      $search->setFilter('ss_language', $language->language);
    }

    // Fields with boost options
    $qf_values = array(
      'label',
      'content',
      'tm_keywords',
      'tm_coverage',
      'ts_otherPlattformRequirements',
      'ts_education_description',
      'tm_contributor',
      'tm_vocabular_text',
    );

    // Set up Query Filters
    $qf_base = '';
    foreach ($qf_values as $key) {
      $value = archibald_get_solr_bias_value($key);
      if (!empty($value)) {
        $qf_base .= $key . '^' . $value . ' ';
      }
    }
    $qf_base = trim($qf_base);
    $search->setQueryFields($qf_base);


    // Set facets
    foreach ($facets as $key => $val) {
      $search->setFacetFilter(
        $key,
        $val
      );
    }

    // echo '<PRE>';
    // $q = $search->search(true);
    // print_r($q->getParams());
    // print_r($q->getSolrParams());
    // print_r($q->getSolrSort());
    // echo '</PRE>';
    // die();

    $results = $search->search(TRUE);
    $facet_options = $search->getFacetOptions();
    $num_results = $search->getTotal();
    pager_default_initialize($num_results, $opts['num_per_page']);
  }
  else {
    // Use native SQL search
    $search = new ArchibaldLomSQLSearch();
    if ($opts['restrict_lang']) {
      $search_in_lang = $language->language;
      $search->setFilterLanguage($search_in_lang);
    }
    else {
      $search_in_lang = 'any';
    }

    $search->setLimit($opts['num_per_page']);
    $search->setQuery(
      $query,
      $search_in_lang
    );
    $search->setLocalPublishedFilter(TRUE);
    $search->getLocalPublishedVersion();

    $page = pager_default_initialize($num_results, $opts['num_per_page']);
    $search->setOffset($page * $opts['num_per_page']);

    $results = $search->search(TRUE);
    $facet_options = null;
    $num_results = $search->getTotalRecordCount();
  }

  return (array(
    'results' => $results,
    'facet_options' => $facet_options,
    'num_results' => $num_results
  ));
}
