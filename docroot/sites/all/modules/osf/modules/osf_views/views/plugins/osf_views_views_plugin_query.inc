<?php
     
use \StructuredDynamics\osf\framework\Namespaces; 
use \StructuredDynamics\osf\framework\Subject;    

/**
* Views 3 Query Plugin used to send queries to the related OSF instances. 
*/
class osf_views_views_plugin_query extends views_plugin_query 
{
  /**
   * A pager plugin that should be provided by the display.
   *
   * @var views_plugin_pager
   */
  var $pager = NULL;

  /**
   * Add anything to the query that we might need to.
   */
  //function query() { }

  /**
   * Builds the necessary info to execute the query.
   */
  function build(&$view) 
  {
    // Initialize the pager    
    $view->init_pager();

    // Let the pager modify the query to add limits.
    $this->pager->query();

    $view->build_info['query'] = $this->query();
    $view->build_info['count_query'] = $this->query(TRUE);
  }

  /**
   * Utility function to return the total number of results. 
   */
  function get_count_records($resultset) {
    $total = 0;
    
    foreach($resultset as $datasetUri => $subjects) {
      foreach($subjects as $aggregateUri => $aggr) {
        if($aggr['type'][0] == Namespaces::$aggr.'Aggregate' &&
           $aggr[Namespaces::$aggr.'property'][0]['uri'] == Namespaces::$void."Dataset") {
          $total += $aggr[Namespaces::$aggr.'count'][0]['value'];
        }
      }
    }
    return($total);
  }
   
  /**
   * Execute the view's query.
   */  
  function execute(&$view) 
  {

    $start = microtime();

    $results = array();
    
    /*
    
      Here we are saving the view session into a drupal variable. The goal is to have
      access to the fields and filters that the user is currently defining for the view.
      This is the only way we found to get that information within the different
      handlers. We need to know the fields and filters such that we can filter the properties
      and type values within each filter. This should be replaced by a call, somewhere in
      views that would re-generate that information.
      
      Also, we have to serialize/encode this information otherwise Views throws this error:
      
          "variable_set("osf_views_filters_".$view->name, urlencode(serialize($view->filter)));"
          
      Because there is some conflicts somewhere.
    
    */    
    
    variable_del("osf_views_filters_".$view->name);
    variable_set("osf_views_filters_".$view->name, urlencode(serialize($view->filter)));

    // Get the OSF Search object
    $search = osf_views_get_search($view);

    // Apply the pagination options to the query
    $items_per_page = $view->query->pager->options['items_per_page'];
    $current_page = $view->query->pager->current_page;
    $offset = $items_per_page * $current_page;
    $search->items($items_per_page); // Set the limit
    $search->page($offset); // Set the item count offset
    
    // Needed to be able to count the total results
    $search->includeAggregates();

    // Send the actual query to the Search endpoint
    $search->send(new DrupalQuerierExtension());

    if ($search->isSuccessful()) {

      $resultset = $search->getResultset()->getResultset();
      $view->query->pager->total_items = $this->get_count_records($resultset);      
      
      foreach ($resultset as $dataset => $subjects) {
        foreach ($subjects as $uri => $subject) {
          // Skip the Aggregate records that are part of the resultset.
          if ($subject["type"][0] == Namespaces::$aggr."Aggregate") {
            continue;
          }
            
          $obj = new Subject($uri);
          $obj->setSubject($subject);
          
          $results[$uri]["obj"] = $obj;
          
          foreach($subject["type"] as $type)
          {
            
            if(!isset($results[$uri]["type"]))
            {
              $results[$uri]["type"] = array();
            }

            if(!isset($results[$uri][resource_type_get_id(Namespaces::$rdf."type")]))         
            {
              $results[$uri][resource_type_get_id(Namespaces::$rdf."type")] = array();
            }
                
            array_push($results[$uri]["type"], resource_type_get_id($type));  
            array_push($results[$uri][resource_type_get_id(Namespaces::$rdf."type")], $type);                
          }
          
          if(isset($subject["prefLabel"]) && $subject["prefLabel"] != "")
          {
            $results[$uri]["preflabel"] = array($subject["prefLabel"]);
            $results[$uri][resource_type_get_id(Namespaces::$iron."prefLabel")] = array($subject["prefLabel"]);
          }
          
          if(isset($subject["altLabel"]) && $subject["altLabel"] != "")
          {
            $results[$uri]["altLabel"] = array($subject["altLabel"]);
            $results[$uri][resource_type_get_id(Namespaces::$iron."altLabel")] = array($subject["altLabel"]);
          }          
              
          if(isset($subject["description"]) && $subject["description"] != "")
          {
            $results[$uri]["description"] = array($subject["description"]);
            $results[$uri][resource_type_get_id(Namespaces::$iron."description")] = array($subject["description"]);
          }              
          
          if(isset($subject["prefURL"]) && $subject["prefURL"] != "")
          {
            $results[$uri]["prefURL"] = array($subject["prefURL"]);
            $results[$uri][resource_type_get_id(Namespaces::$iron."prefURL")] = array($subject["prefURL"]);
          }          
          
          if(isset($subject["lat"]) && $subject["lat"] != "")
          {
            $results[$uri]["lat"] = array($subject[Namespaces::$geo."lat"][0]["value"]);
            $results[$uri][resource_type_get_id(Namespaces::$geo."lat")] = array($subject[Namespaces::$geo."lat"][0]["value"]);
          }
          
          if(isset($subject["long"]) && $subject["long"] != "")
          {              
            $results[$uri]["long"] = array($subject[Namespaces::$geo."long"][0]["value"]);
            $results[$uri][resource_type_get_id(Namespaces::$geo."long")] = array($subject[Namespaces::$geo."long"][0]["value"]);
          }
           
          foreach($subject as $attributeUri => $values)
          {
            /*
            if($attributeUri == "type" ||
               $attributeUri == "prefLabel" ||
               $attributeUri == Namespaces::$geo."lat" ||
               $attributeUri == Namespaces::$geo."long")
            {
              continue;     
            }
            */
            
            if(is_array($values))
            {
              foreach($values as $value)
              {
                if(isset($value["value"]))
                {
                  if(!isset($results[$uri][resource_type_get_id($attributeUri)]))
                  {
                    $results[$uri][resource_type_get_id($attributeUri)] = array();
                  }
                  
                  array_push($results[$uri][resource_type_get_id($attributeUri)], $value["value"]);
                }
                else
                {
                  if(isset($value["uri"]))
                  {
                    if(!isset($results[$uri][resource_type_get_id($attributeUri)]))
                    {
                      $results[$uri][resource_type_get_id($attributeUri)] = array();
                    }

                    array_push($results[$uri][resource_type_get_id($attributeUri)], $value["uri"]);
                    
                    if(isset($value['reify']))
                    {
                      $results[$uri]["reify_".resource_type_get_id($attributeUri)] = array($value['reify'][Namespaces::$wsf.'objectLabel'][0]);                      
                    }
                  }
                }
              }
            }
          }

          // Use entity_create(...) to create the proper Entity object to feed to the requester           
          $results[$uri] = entity_create("resource_type", $results[$uri]);
        }
      }            
      
      $view->result = $results;
      $view->total_rows = count($results);

      $view->execute_time = microtime() - $start;     
    }
    else
    {
      $view->total_rows = 0;

      $view->execute_time = microtime() - $start;     
    }
  }

  /**
   * Information about options for all kinds of purposes will be held here.
   * @code
   * 'option_name' => array(
   *  - 'default' => default value,
   *  - 'translatable' => TRUE/FALSE (wrap in t() on export if true),
   *  - 'contains' => array of items this contains, with its own defaults, etc.
   *      If contains is set, the default will be ignored and assumed to
   *      be array()
   *
   *  ),
   *  @endcode
   * Each option may have any of the following functions:
   *  - export_option_OPTIONNAME -- Special export handling if necessary.
   *  - translate_option_OPTIONNAME -- Special handling for translating data
   *    within the option, if necessary.
   */  
  function option_definition() 
  {
    $options = parent::option_definition();
    $options['osf_web_services_network'] = array('default' => '');

    return $options;
  }

  /**
   * Provide a form to edit options for this plugin.
   */  
  function options_form(&$form, &$form_state) 
  {    
    $instances = osf_configure_get_endpoints(NULL, FALSE, TRUE);
    
    $options = array();
    $defaultOption = 0;
    
    foreach($instances as $key => $instance)
    {
      if($instance->uri == variable_get("osf_views_views_target_osf_web_services_".$form_state['view']->name, 0))
      {
        $defaultOption = $key;
      }
      
      array_push($options, $instance->uri);      
    }       
    
    // Create the form to setup de settings of this Query Plugin
    $form['osf_web_services_network'] = array(
      '#type' => 'select',
      '#title' => t('OSF Network to query'),
      '#options' => $options,
      '#default_value' => $defaultOption,
      '#description' => t(''),
      '#required' => TRUE,
    );        
  }
  
  /**
   * Validate the options form.
   */  
  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);
    
    // Save the selected network of the user
    variable_set("osf_views_views_target_osf_web_services_".$form_state['view']->name, $form['options']['osf_web_services_network']['#options'][$form['options']['osf_web_services_network']['#value']]);
  }

  function add_filter($filter) 
  {
    $this->filter[] = $filter;
  }
  
  /**
   * Add a field to the query table, possibly with an alias. This will
   * automatically call ensure_table to make sure the required table
   * exists, *unless* $table is unset.
   *
   * @param $table
   *   The table this field is attached to. If NULL, it is assumed this will
   *   be a formula; otherwise, ensure_table is used to make sure the
   *   table exists.
   * @param $field
   *   The name of the field to add. This may be a real field or a formula.
   * @param $alias
   *   The alias to create. If not specified, the alias will be $table_$field
   *   unless $table is NULL. When adding formulae, it is recommended that an
   *   alias be used.
   * @param $params
   *   An array of parameters additional to the field that will control items
   *   such as aggregation functions and DISTINCT.
   *
   * @return $name
   *   The name that this field can be referred to as. Usually this is the alias.
   */
  function add_field($table, $field, $alias = '', $params = array()) 
  {
    $alias = $field;

    // Create a field info array.
    $field_info = array(
      'field' => $field,
      'table' => $table,
      'alias' => $field,
    ) + $params;

    if (empty($this->fields[$field])) {
      $this->fields[$field] = $field_info;
    }

    return $field;
  }

  
  /**
   * Needed because it may be called by Views and needs to be defined.
   */  
  function ensure_table($table, $relationship = NULL, $join = NULL) {}
  
  /**
   * Needed because it may be called by Views and needs to be defined.
   */  
  function add_where() {}

  /**
   * Render the pager, if necessary.
   */
  function render_pager($exposed_input) {
    if (!empty($this->pager) && $this->pager->use_pager()) {
      $page = pager_find_page();
      $num_per_page = $this->pager->options['items_per_page'];
      $offset = $num_per_page * $page;
      $result = $this->pager->total_items;

      // Now that we have the total number of results, initialize the pager.
      pager_default_initialize($result, $num_per_page);

      // Finally, display the pager controls, and return.
      $output .= theme('pager');
      return $output;

    }
    return '';
  }
}
