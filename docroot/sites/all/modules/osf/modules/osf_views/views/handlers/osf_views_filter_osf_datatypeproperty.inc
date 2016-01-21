<?php

use \StructuredDynamics\osf\framework\Namespaces;

/**
 * Filter by any data property
 */
class osf_views_filter_osf_datatypeproperty extends views_handler_filter 
{ 
  function option_definition() 
  {
    $options = parent::option_definition();

    return $options;
  }    
        
  public function operator_form(&$form, &$form_state) 
  {
    parent::operator_form($form, $form_state);

    // Get all the properties used to define records in this OSF instance
    // and return them to Views to display to the user    
    $search = osf_views_get_search($form_state['view']);                     
    
    $search->query("");
    
    $search->page(0);
    $search->items(0);
    
    $search->includeAggregates();
    $search->attributesValuesFilters("all");   
    
    $search->send(new DrupalQuerierExtension());
    
    if(!$search->isSuccessful())
    {
      drupal_set_message(t("Can't get operator form for this property: [@error] @errorMsg - @errorDescription", 
                         array("@error" => $search->getStatus(),
                               "@errorMsg" => $search->getStatusMessage(),
                               "@errorDescription" => $search->getStatusMessageDescription())), 
                         "error", 
                         TRUE);
    }
    else
    {       
      $resultset = $search->getResultset()->getResultset();
      
      $properties = array();
      
      $network = variable_get("osf_views_views_target_osf_web_services_".$form_state['view']->name, "http://localhost/ws/");   
      $datatypeProperties = variable_get("osf_views_datatypeproperty_labels", array());
      
      foreach($resultset as $dataset => $subjects)
      {
        foreach($subjects as $uri => $subject)
        {
          if(array_search(Namespaces::$aggr."Aggregate", $subject["type"]) !== FALSE)
          {
            if($subject[Namespaces::$aggr."property"][0]["uri"] == Namespaces::$rdf."Property")
            {
              $propertyUri = $subject[Namespaces::$aggr."object"][0]["uri"];
              
              // Check if the property is a Datatype Property
              // Only the properties defined in the ontologies will be make available
              // to the user. If the node where the data come from hasn't defined a property
              // then it won't appear there neither, even if some records do use it
              if(isset($datatypeProperties[$network][$propertyUri]))
              {
                if(strlen($datatypeProperties[$network][$propertyUri]) > 75)
                {
                  $datatypeProperties[$network][$propertyUri] = substr($datatypeProperties[$network][$propertyUri], 0, 75) + "...";
                }
                
                array_push($properties, $datatypeProperties[$network][$propertyUri]);
              }
            }
          }
        }
      }  
         
      $form['datatype_property_action'] = array(
        '#type' => 'radios',
        '#title' => t('Comparison action to perform'),
        '#options' => array("contains", "starts with"),
        '#default_value' => 0
      );
        
      $form['datatype_property'] = array(
        '#type' => 'select',
        '#title' => t('Data property to use'),
        '#options' => $properties
      ); 
    } 
  }
  
  /**
   * Display the filter on the administrative summary
   */
  function admin_summary() 
  {
    return check_plain((string) substr($this->operator, strpos($this->operator, "::") + 2)) . ' ' . check_plain((string) $this->value);
  }  
  
  function options_submit(&$form, &$form_state) 
  {
    parent::options_submit($form, $form_state);

    $datatypeProperties = variable_get("osf_views_datatypeproperty_labels", array());
    $network = variable_get("osf_views_views_target_osf_web_services_".$this->view->name, "http://localhost/ws/");   
    
    $uri = array_search($form['datatype_property']['#options'][$form['datatype_property']['#value']], $datatypeProperties[$network]);

    // We change the operator of this handler, for the one selected by the user.
    $this->options['operator'] = $uri."::".$form['datatype_property_action']['#options'][$form['datatype_property_action']['#value']];
        
    $form_state['values']['options']['ui_name'] = $form['datatype_property']['#options'][$form['datatype_property']['#value']];
  }  
  
  public function value_form(&$form, &$form_state) 
  {
    parent::value_form($form, $form_state);
    
    // Remove the expose checkbox. We don't want people to have the choice to change this filter.
    unset($form['expose_button']);    
    
    while(is_array($this->value)) 
    {
      $this->value = $this->value ? array_shift($this->value) : NULL;
    }
    
    $form['value'] = array(
      '#type' => 'textfield', 
      '#title' => 'Value of the property', 
      '#default_value' => isset($this->value) ? $this->value : '',
    );
  }  
}
