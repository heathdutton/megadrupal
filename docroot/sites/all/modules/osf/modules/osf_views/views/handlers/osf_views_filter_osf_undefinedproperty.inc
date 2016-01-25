<?php

use \StructuredDynamics\osf\framework\Namespaces;

/**
 * Filter by any undefined property
 */
class osf_views_filter_osf_undefinedproperty extends views_handler_filter 
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
      $undefinedProperties = variable_get("osf_views_undefinedproperty_labels", array());
      
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
              if(isset($undefinedProperties[$network][$propertyUri]))
              {
                if(strlen($undefinedProperties[$network][$propertyUri]) > 75)
                {
                  $undefinedProperties[$network][$propertyUri] = substr($undefinedProperties[$network][$propertyUri], 0, 75) + "...";
                }
                
                array_push($properties, $undefinedProperties[$network][$propertyUri]);
              }
            }
          }
        }
      }  
         
      $form['undefined_property_action'] = array(
        '#type' => 'radios',
        '#title' => t('Comparison action to perform'),
        '#options' => array("contains", "starts with"),
        '#default_value' => 0
      );
        
      $form['undefined_property'] = array(
        '#type' => 'select',
        '#title' => t('Property to use'),
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

    $undefinedProperties = variable_get("osf_views_undefinedproperty_labels", array());
    $network = variable_get("osf_views_views_target_osf_web_services_".$this->view->name, "http://localhost/ws/");   
    
    $uri = array_search($form['undefined_property']['#options'][$form['undefined_property']['#value']], $undefinedProperties[$network]);

    // We change the operator of this handler, for the one selected by the user.
    $this->options['operator'] = $uri."::".$form['undefined_property_action']['#options'][$form['undefined_property_action']['#value']];
        
    $form_state['values']['options']['ui_name'] = $form['undefined_property']['#options'][$form['undefined_property']['#value']];
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
