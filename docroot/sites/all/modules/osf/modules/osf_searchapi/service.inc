<?php

// Include struct code
use \StructuredDynamics\osf\php\api\ws\search\SearchQuery;
use \StructuredDynamics\osf\framework\Namespaces;
use \StructuredDynamics\osf\framework\Resultset;
use \StructuredDynamics\osf\framework\Subject;
use \StructuredDynamics\osf\php\api\ws\dataset\read\DatasetReadQuery;
use \StructuredDynamics\osf\php\api\ws\ontology\read\GetPropertyFunction;
use \StructuredDynamics\osf\php\api\ws\ontology\read\OntologyReadQuery;
use \StructuredDynamics\osf\php\api\ws\ontology\read\GetLoadedOntologiesFunction;

/**
 * Search service class that uses the OSF service to retrieve data from.
 */
class SearchApiStructService extends SearchApiAbstractService {

  // Allow user to select which of the Networks to use for this search.
  public function configurationForm(array $form, array &$form_state) {

    // Get list of available networks
    $networks = osf_configure_get_endpoints(NULL, FALSE, TRUE);

    // Put networks into options array ( key and value the same )
    $options = array();
    foreach ($networks as $network) {
      $options[$network->uri] = $network->uri;
    }

    // Network form select
    $form['network'] = array(
      '#type' => 'select',
      '#title' => t('Network'),
      '#description' => t('Endpoint used for searching.'),
      '#options' => $options,
      '#default_value' => $this->options,
      '#required' => TRUE,
    );

    return $form;
  }

  // List of additional features that this Service supports.
  public function supportsFeature($feature) {
    $supported = drupal_map_assoc(array(
      'search_api_facets',
      'search_api_facets_operator_or',
    ));
    return isset($supported[$feature]);
  }

  // Method that does the actual search and controls pagination.
  public function search(SearchApiQueryInterface $query) {    
    
    $time_method_called = microtime(TRUE);
    // Create the SearchQuery object
    $index = $query->getIndex();
    
    if (!empty($this->options['network'])) {
      $defaultEndpoint = osf_configure_get_endpoint_by_uri($this->options['network']);

      $search = new SearchQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());
    }
    else {
      throw new SearchApiException(t('No network set when attempting query.'));
    }

    // We only apply the global search settings if the query is not part of a Search Profile
    // We don't want to have the general search settings applied to a search profile since it
    // would defeat the purpose of the search profiles.
    if(empty($options['search_profile'])) 
    {
      _osf_searchapi_apply_search_settings($search);
    }
    else
    {
      $search->sourceInterface(variable_get("osf_searchapi_settings_interface_name", 'default'))
             ->sourceInterfaceVersion(variable_get("osf_searchapi_settings_interface_version", ''));      
    }

    // Set some basic search params
    $namespaces = new Namespaces;
    
    $search->mime("resultset");
    
    if(variable_get('osf_search_api_settings_inference', TRUE))
    {
      $search->enableInference();
    }

    $options = $query->getOptions();

    // Set pagination vars
    $max = !empty($options['limit']) ? $options['limit'] : 20;
    $search->items($max);
    $page = !empty($options['offset']) ? $options['offset'] : 0;
    $search->page($page);

    // Ensure we have at least a dataset facet for getting the total count
    $dataset_key_hash = osf_searchapi_facet_key_hash(Namespaces::$void . 'Dataset');
    if (empty($options['search_api_facets'][$dataset_key_hash])) {
      $options['search_api_facets'][$dataset_key_hash] = array(
        'field' => Namespaces::$void . 'Dataset',
        'limit' => '50',
        'operator' => 'and',
        'min_count' => 1,
        'missing' => 0,
      );
    }

    // Set the query parameter with the search keywords
    $query_keywords = &drupal_static('osf_searchapi_keywords', _osf_searchapi_sanitize_keywords($this->queryKeywords($query)));

    // If no valid keywords were submitted, return an empty result set
    if(empty($query_keywords) && empty($options['search_profile'])) {
      $results = '';
      return array(
        'results' => array(),
        'search_api_facets' => array(Namespaces::$void . 'Dataset' => array()),
        'result count' => 0,
        'performance' => array(),
      );
    }
    
    $search->includeSpellcheck();

    // Set facets
    $filters = $query->getFilter()->getFilters();
    
    if(count($filters) > 0)
    {
      $filters = $filters[0]->getFilters();
    }
    
    $datasets = array();
    $types = array();
    $properties = array();
    
    // Filter by facet
    $this->setFacets($this->options['network'], $filters, $datasets, $types, $properties);
    
    if (!empty($datasets)) {
      $search->datasetsFilters($datasets);
    }
    else {
      $filterDatasets = array();
      $datasets = osf_configure_dataset_generate_config_searchable('configured-default');

      foreach($datasets as $dataset)
      {
        $filterDatasets[] = $dataset;
      }

      $search->datasetsFilters((empty($filterDatasets) ? array('none') : $filterDatasets));
    }
    if (!empty($types)) {
      $search->typesFilters($types);
    }
    if (!empty($properties)) {
      // Here we change the labels returned by the facetapi for their possible URI reference
      $objectAttributesURIsLabels = variable_get('osf_searchapi_object_attributes_uris_labels_index', array());
      
      foreach($properties as $uri => $labelValues)
      {
        foreach($labelValues as $key => $labelValue)
        {
          $labelValue = str_replace('"', '', $labelValue);
          if(isset($objectAttributesURIsLabels[$labelValue]))
          {
            $properties[$uri][$key] = $objectAttributesURIsLabels[$labelValue];
          }
        }
      }
      
      $search->attributesValuesFilters($properties);
    }

    $time_processing_done = microtime(TRUE);

    // Adds extra information needed for Facets API
    $search->setAggregateAttributesObjectTypeToUriLiteral();

    if (module_exists('search_api_facetapi')) {
      // Add the aggregate attributes -- a URI for each facet that appears with this search.
      $searcher = 'search_api@' . $index->machine_name;
      $adapter = facetapi_adapter_load($searcher);

      foreach ($adapter->getEnabledFacets() as $key => $value) {
        $search->aggregateAttribute($value['field']);
      }
    }

    $search
      ->includeAggregates()
      ->numberOfAggregateAttributesObject(-1);

    if (!empty($options['search_profile'])) {
      $sprofile = $options['search_profile'];

      // Add the settings from the sprofile to the search query
      foreach ($sprofile->settings as $setting => $value){

        // If it's an empty array
        if (empty($value)) {
          //$search->$setting;
        }
        elseif (is_array(reset($value))) {
          // Otherwise, if the first item in $value is an array, it's multiple methods
          foreach ($value as $value_item) {
            call_user_func_array(array($search, $setting), $value_item);
          }
        }
        else {
          // And otherwise, it's one method
          call_user_func_array(array($search, $setting), $value);
        }
      }

      $search->items($options['search_profile_block']['sp_settings']['items']);
    }

    if($query_keywords != strtolower('browse'))
    {
      $search->query($query_keywords);
    }    
    
    $search->lang(get_lang('en'))
           ->send(new DrupalQuerierExtension());

    $time_query_done = microtime(TRUE);

    if (!$search->isSuccessful()) {
      // Set a generic message that there was something wrong
      drupal_set_message(
        t('There was a problem searching for \'%keywords\' because there was a problem with the system. Please try your search again later.', array('%keywords' => $query_keywords)),
        'error'
      );

      // If the user is an admin that we trust, display a more helpful message
      if (user_access('administer osf')) {
        drupal_set_message(
          t('Search API Struct query error [%status] %msg (%msg_desc).',
          array(
            '%status' => $search->getStatus(),
            '%msg' => $search->getStatusMessage(),
            '%msg_desc' => $search->getStatusMessageDescription()
          )),
          'error'
        );
      }

      // Regardless of the user, log the error message details.
      watchdog(
        'OSF SearchAPI',
        'Search API Struct query error [%status] %msg (%msg_desc).',
        array(
          '%status' => $search->getStatus(),
          '%msg' => $search->getStatusMessage(),
          '%msg_desc' => $search->getStatusMessageDescription()
        ),
        WATCHDOG_WARNING
      );

      // return an empty array rather than a PHP exception so that the page will still return.
      return array();
    }

    // Get back the resultset returned by the endpoint and make it statically
    // accessible for other modules to work with this result set
    $resultset_temp = $search->getResultset()->getResultset();
    $resultset = &drupal_static('osf_searchapi_resultset', $resultset = $search->getResultset()->getResultset());
    $resultset = $resultset_temp;

    // Iterate over resultset.  The only thing that matters here is $uri ( I think ), the other items get filled in by the loadItems method in datasource.inc
    $results = array();
    $results['results'] = array();

    if (!empty($resultset)) {
      foreach ($resultset as $items) {
        foreach ($items as $uri => $item) {

          // Skip the Aggregate records that are part of the resultset.
          if(empty($item['type']) || $item['type'][0] == Namespaces::$aggr."Aggregate") {
            continue;
          } else if ($item['type'][0] == Namespaces::$wsf."SpellSuggestion") {
            if (empty($results['spell_correction']) && !empty($item[Namespaces::$wsf."suggestion"])) {
              $results['spell_correction'] = $item[Namespaces::$wsf."suggestion"][0]['value'];
            }
            continue;
          }

          $result = array(
            'id' => $uri,
            'type' => 'osf',
          );

          $results['results'][$uri] = $result;
          
          // Now we want to create the Resource Type entity and to cache them. That way, when datasource.inc will
          // use entity_load() to load them, they will at least already be in cache.
          
          // This saves a CRUD: Read call that would otherwise be used to get the complete description of the records
          // from the CRUD: Read endpoint.
          
          // Now cache the result such that entity_load called from datasource.inc get that cached entity instead
          // of requesting it from the OSF instance
          if(!cache_get($uri, 'cache_entity_resource_type'))
          {
            $resourceTypeRecord = osf_entities_create_resource_type_class_from_resultset_subject_array($uri, $item);
            cache_set($uri, $resourceTypeRecord, 'cache_entity_resource_type');
          }
        }
      }
    }

    // Facets
    $resultset['properties'] = $properties;
    $results['search_api_facets'] = $this->getFacets($resultset, $this->options['network'], $options['search_api_facets']);

    // Cache the search facets uri/value => label used to properly create the search filter later on

    // Calculate the number of results for that search query
    $total_items = 0;
	    
    if (!empty($results['search_api_facets'][$dataset_key_hash])) {
      foreach($results['search_api_facets'][$dataset_key_hash] as $dataset) {
        $total_items += $dataset['count'];
      }
    }

    $results['result count'] = $total_items;
    
    // Compute performance
    $time_end = microtime(TRUE);
    $results['performance'] = array(
      'complete' => $time_end - $time_method_called,
      'preprocessing' => $time_processing_done - $time_method_called,
      'execution' => $time_query_done - $time_processing_done,
      'postprocessing' => $time_end - $time_query_done,
    );

    return $results;
  }

  /* Helper function to add a keyword list to the query */
  public function queryKeywords(SearchApiQueryInterface $query) {
    $q_string = '';
    $keys = $query->getKeys();
    
    if(!empty($keys))
    {
      foreach ($keys as &$key) {
        if (strpos($key, ' ')) {
          $key = '"' . $key . '"';
        }
      }
    }

    if (is_array($keys)) {
      $conjunction = $keys['#conjunction'];
      unset($keys['#conjunction']);   // Remove key from array
      if (strtoupper($conjunction) == 'AND') {
        $q_string = implode(' ', $keys);
      }
      else {
        $q_string = implode(' OR ', $keys);
      }
    }
    return $q_string;
  }

  /* Helper function to retrieve the facets from a query and create the data for facet blocks */
  public function getFacets($resultset, $network, $requested_facets) {
    $facets = array();
    $datasets = array();
    $classes = array();
    
    $isDatasetFacetRequested = FALSE;
    $isTypeFacetRequested = FALSE;
    
    foreach($requested_facets as $facet)
    {
      if($facet['field'] == 'http://www.w3.org/1999/02/22-rdf-syntax-ns#type')
      {
        $isTypeFacetRequested = TRUE;
      }
      
      if($facet['field'] == 'http://rdfs.org/ns/void#Dataset')
      {
        $isDatasetFacetRequested = TRUE;
      }
    }
    
    // Retrieve facets from results. Type and Dataset are always available in these
    if($isDatasetFacetRequested)
    {
      foreach ($resultset as $dataset => $aggregates) {
        foreach ($aggregates as $uri => $aggregate) {
          if (empty($aggregate['type']) || $aggregate['type'][0] != Namespaces::$aggr . 'Aggregate') {
            continue;
          }

          if ($aggregate[Namespaces::$aggr . 'property'][0]['uri'] == Namespaces::$void . 'Dataset') {
            array_push($datasets, array($aggregate[Namespaces::$aggr . 'object'][0]['uri'], $aggregate[Namespaces::$aggr . 'count'][0]['value']));
          }

          if ($aggregate[Namespaces::$aggr . 'property'][0]['uri'] == Namespaces::$rdf . 'type') {
            array_push($classes, array($aggregate[Namespaces::$aggr . 'object'][0]['uri'], $aggregate[Namespaces::$aggr . 'count'][0]['value']));
          }
        }
      }

      // Datasets
      $dataset_counts = array();
      foreach ($datasets as $dataset) {
        $dataset_counts[$dataset[0]] = array(
          'filter' => '"'.$this->getDatasetTitle($dataset[0]).'"',
          'count' => $dataset[1],
        );
      }
    }

    // Classes ( Types )
    if($isTypeFacetRequested)
    {
      $class_counts = array();
      
      foreach ($classes as $class) {

        if($class[0] == 'http://www.w3.org/2002/07/owl#Thing')
        {
          continue;
        }
        
        $label = get_label_from_uri($class[0]);

        $class_counts[$class[0]] = array(
          'filter' => '"'.$label.'"',
          'count' => $class[1]
        );
      }
    }

    // Get any other facets which were requested but which aren't in aggregated data by default
    if (!empty($requested_facets)) {
      foreach ($requested_facets as $fid => $facet_info) {
        switch ($facet_info['field']) {
          case Namespaces::$void . 'Dataset':
            $facets[$fid] = $dataset_counts;
            break;
          case Namespaces::$rdf . 'type':
            $facets[$fid] = $class_counts;
            break;
          default:
            $counts = $this->getFacetCounts($facet_info['field'], $results['properties']);
            if (!empty($counts)) {
              if (!isset($facets[$fid])) {
                $facets[$fid] = array();
              }
              $facets[$fid] += $counts;
            }
        }
      }
    }
    
    return $facets;
  }

  /* Apply the selected Facets to the query.  Unfortunately, the facetapi gives us the name ( prefLabel ) of the facet, 
     and we need to pass the uri into the search object. Thus, the additional query */
  public function setFacets($network, $filters, &$datasets, &$types, &$properties) {
    if (!empty($filters)) {
      $facetsPropertiesUris = cache_get('osf_searchapi_facets_properties_uris_labels');
      $facetsPropertiesUris = $facetsPropertiesUris->data;
      $facetsClassesUris = cache_get('osf_searchapi_facets_classes_uris_labels');
      $facetsClassesUris = $facetsClassesUris->data;
      $facetsDatasetsUris = cache_get('osf_searchapi_facets_datasets_uris_labels');
      $facetsDatasetsUris = $facetsDatasetsUris->data;
      
      foreach ($filters as $filter) {

        switch($filter[0])
        {
          case Namespaces::$void . 'Dataset':
            $datasetUri = array_search($filter[1], array_flip($facetsDatasetsUris));
            array_push($datasets, $datasetUri);
          break;
          
          case Namespaces::$rdf . 'type':
            $classUri = array_search($filter[1], $facetsClassesUris);        
            array_push($types, $classUri);
          break;
          
          default:
            $propertyUri = array_key_exists($filter[0], $facetsPropertiesUris);
            if($propertyUri)
            {
              $propertyUri = $filter[0];
            }
            
            if(!is_array($properties[$propertyUri]))
            {
              $properties[$propertyUri] = array();
            }
            
            $properties[$propertyUri][] = $filter[1];
          break;
        }
      }
    }
  }

  private function getDatasetTitle($dataset_uri) {
    $datasets = osf_configure_dataset_load_util('all', FALSE);
    foreach ($datasets as $dataset) {
      if($dataset_uri == $dataset->uri) {
        return($dataset->label);
      }
    }
    return FALSE;
  }

  /*
   * For a given property ($facet_property_key), perform an aggregate request and
   * return values and counts keyed as facet_api expects
   */
  private function getFacetCounts($facet_property_key, $properties = array()) {
    $is_data_property = FALSE;
    $ob = osf_structproperty_info($facet_property_key);

    if ($ob) {
      $is_data_property = (array_search(Namespaces::$owl . 'ObjectProperty', $ob->getTypes()) === FALSE);
    }
    else {
      drupal_set_message(t('Unknown property type: @k, assuming object', array('@k' => $facet_property_key)), 'error');
      return array();
    }

    $query_keywords = &drupal_static('osf_searchapi_keywords');
    $resultset = &drupal_static('osf_searchapi_resultset');
    if (empty($query_keywords) || empty($resultset)) {
      return array();
    }

    $return = array();

    $objectAttributesURIsLabels = variable_get('osf_searchapi_object_attributes_uris_labels_index', array());
    $varChanged = FALSE;
    
    if (!empty($resultset['unspecified'])) {
      foreach ($resultset['unspecified'] as $property_key => $property_value) {
        if ($property_value['http://purl.org/ontology/aggregate#property'][0]['uri'] == $facet_property_key) {
          if ($is_data_property) {
            $key = $property_value['http://purl.org/ontology/aggregate#object'][0]['value'];
            $label = $key;
          }
          else {
            $key = $property_value['http://purl.org/ontology/aggregate#object'][0]['uri'];
            $label = $property_value['http://purl.org/ontology/aggregate#object'][1]['value'];
          }
          $count = $property_value['http://purl.org/ontology/aggregate#count'][0]['value'];

          if(!key_exists($label, $objectAttributesURIsLabels) && !$is_data_property)
          {
            $varChanged = TRUE;
            $objectAttributesURIsLabels[$label] = $key;
          }
          elseif(key_exists($label, $objectAttributesURIsLabels) && $objectAttributesURIsLabels[$label] !== $key)
          {
            // The same label is used for different URIs. What we have to do here it to add something to the
            // label such that we create unique filters
            for($i = 1; $i < 128; $i++)
            {
              if(!key_exists($label.' '.$i, $objectAttributesURIsLabels))
              {
                $label = $label.' '.$i;
                break;
              }
            }
            
            $varChanged = TRUE;
            $objectAttributesURIsLabels[$label] = $key;
          }
          
          $return[$key] = array(
            'filter' => '"'.$label.'"',
            'count' => $count,
          );
        }
      }
    }
    
    if($varChanged === TRUE)
    {
      variable_set('osf_searchapi_object_attributes_uris_labels_index', $objectAttributesURIsLabels);      
    }
    
    return $return;
  }

  /* UNUSED */
  /* Since we're not writing to the Network, these don't need to do anything. */
  public function addIndex(SearchApiIndex $index) { }
  public function fieldsUpdated(SearchApiIndex $index) { }
  public function removeIndex($index) { }
  public function indexItems(SearchApiIndex $index, array $items) { }
  public function deleteItems($ids = 'all', SearchApiIndex $index = NULL) { }
}

function _osf_searchapi_sanitize_keywords($keywords) {
  $keywords = str_replace('?', '', $keywords);
  return $keywords;
}
