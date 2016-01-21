<?php

use \StructuredDynamics\osf\framework\Namespaces;

/**
 * Filter by record type
 */
class osf_views_filter_osf_type extends views_handler_filter_in_operator 
{  
  var $typeFilters = array();
  
  function option_definition() 
  {
    $options = parent::option_definition();

    // Set the default operator radiobutton
    $options['operator']['default'] = 'has type';
    
    // Select the first value by default
    $options['value']['default'] = 0;
    
    return $options;
  }  
  
  function options_submit(&$form, &$form_state) 
  {
    parent::options_submit($form, $form_state);

    // Change the UI name to make it easier to read to the Views user
    $form_state['values']['options']['ui_name'] = "OSF: type";
  }  
  
  function operators() 
  {    
    $operators = parent::operators();
    
    // Make sure all the options are radios buttons. 
    // We only want the user to select a single option
    $this->value_form_type = "radios";
    
    // Operators that can be used by the users to match the type(s)
    $operators = array(
      'has type' => array(
        'title' => t('has type'),
        'short' => t('a'),
        'method' => 'op_hasType',
        'values' => 1
      )  
    );

    return $operators;
  }  
  
  function admin_summary() 
  {
    $labels = variable_get("osf_views_class_labels", array());    
    $network = variable_get("osf_views_views_target_osf_web_services_".$this->view->name, "http://localhost/ws/");   
    
    // Change the summary that is between parenthesis in the Views UI
    return(trim($labels[$network][$this->value]));
  }
  
  /**
  * Get the list of all types defined, and accessible to the user, on the OSF instance
  * 
  * @TODO Cache this query somewhere in Drupal since it is called at each pageview
  */
  function get_value_options() 
  {
    if(isset($this->value_options)) 
    {
      return($this->value_options);
    }    
    
    $search = osf_views_get_attributes_and_filters($this->view);
    
    if (!$search->isSuccessful()) {
      drupal_set_message(t("Can't get value options for the types: [@error] @errorMsg - @errorDescription", 
                         array("@error" => $search->getStatus(),
                               "@errorMsg" => $search->getStatusMessage(),
                               "@errorDescription" => $search->getStatusMessageDescription())), 
                         "error", 
                         TRUE);
    }
    else
    {       
      $resultset = $search->getResultset()->getResultset();
      
      $types = array();
      
      $labels = variable_get("osf_views_class_labels", array());
      $network = variable_get("osf_views_views_target_osf_web_services_".$this->view->name, "http://localhost/ws/");   

      $newLabels = FALSE;
      
      // Get all the types from the aggregates records of the Search resultset
      foreach($resultset as $dataset => $subjects)
      {
        foreach($subjects as $uri => $subject)
        {
          if(array_search(Namespaces::$aggr."Aggregate", $subject["type"]) !== FALSE)
          {
            if($subject[Namespaces::$aggr."property"][0]["uri"] == Namespaces::$rdf."type")
            {
              // Get the preferred label for this type
              $uri = $subject[Namespaces::$aggr."object"][0]["uri"];
              
              $label = $uri;
              
              if(isset($labels[$network]) && isset($labels[$network][$uri]))
              {
                $label = $labels[$network][$uri];
              }
              else
              {
                // We are facing a type without a label from the ontology.
                // When this happen, we try to create one from the ending of
                // the URI, and save it into the cache
                
                // Now create a better label by splitting at the upper case letters
                $label = osf_views_generate_label_from_uri($label);

                // Now save the new label into the cache array
                $labels[$network][$uri] = $label;
                
                $newLabels = TRUE;
              }            
              
              array_push($types, $label);
            }
          }
        }
      } 
      
      if($newLabels)
      {
        // If there are new labels generated from this procedure, we simply save them into the cache.
        variable_del("osf_views_class_labels");
        
        variable_set("osf_views_class_labels", $labels);
      }
      
      // Feed the list to the Views module
      $this->value_options = $types; 
    }
  }
  
  function value_form(&$form, &$form_state) 
  {
    parent::value_form($form, $form_state);
    
    // Remove the default "Select all" option to the user
    unset($form['value']["#options"]["all"]);
    
    // Remove the expose checkbox. We don't want people to have the choice to change this filter.
    unset($form['expose_button']);
    
    // Select the default item in the list of radio buttons
    $labels = variable_get("osf_views_class_labels", array());
    $network = variable_get("osf_views_views_target_osf_web_services_".$this->view->name, "http://localhost/ws/");   
        
    $form['value']["#default_value"] = (!isset($this->value) || $this->value == '' ? 0 : array_search($labels[$network][$this->value], $form['value']['#options']));
  }
  
  /**
   * Perform any necessary changes to the form values prior to storage.
   * There is no need for this function to actually store the data.
   */
  function value_submit($form, &$form_state) 
  {
    $labels = variable_get("osf_views_class_labels", array());
    $network = variable_get("osf_views_views_target_osf_web_services_".$this->view->name, "http://localhost/ws/");   
    
    $uri = array_search($form['value']['#options'][$form['value']['#value']], $labels[$network]);
    
    // Change the value that is selected from the value of the radio button in the list
    // to the actual URI value of the type related to this radio button
    $form_state['values']['options']['value'] = $uri;
  }  
        
  /**
  * Keep track of all the type filters for this handler
  */    
  function op_hasType()
  {  /*
    $this->get_value_options();
    
    $this->typeFilters = array();
    
    $labels = variable_get("osf_views_class_labels", array());
    $network = variable_get("osf_views_views_target_osf_web_services_".$this->view->name, "http://localhost/ws/");   

    foreach($labels[$network] as $uri => $label)
    {
      if($label == $this->value_options[$this->value])
      {
        // Get the URI of the time from its label from the cache.
        array_push($this->typeFilters, $uri);
      }
    } */
    
    array_push($this->typeFilters, $this->value);     
  }
}
