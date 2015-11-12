<?php

use \StructuredDynamics\osf\framework\Namespaces;

/**
 * Field handler to translate any object property into its readable form.
 */
class osf_views_field_osf_objectproperty extends views_handler_field 
{
  function option_definition() 
  {    
    $options = parent::option_definition();

    $options['object_property'] = array('default' => NULL);    
    
    return $options;
  }  
  
  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);
    
    $this->options["object_property_show_uri"] = $form['object_property_show_uri']['#value'];
    
    $targetURI = $form['object_property']['#value'];
    $label = osf_views_get_label("osf_views_objectproperty_labels", $targetURI);
    if (!empty($label)) {
      // Change the UI name to use the label for the Views admin name if we have one
      $form_state['values']['options']['ui_name'] = $form['object_property']['#title'] . ' (' . $label . ')';
    }
  }  
  
  public function get_value($values, $field = NULL) 
  {
    // Check if we have a value for this selected field (property)
    // These values have been created by the Query Plugin
    $this->get_value_options();
    
    $uri = $this->options["object_property"];

    if(isset($values->{resource_type_get_id($uri)}))
    {
      return($values->{resource_type_get_id($uri)});
    }
    
    if($this->options['object_property_show_uri'])
    {
      // If required by the user, we automatically display the URI for this object reference.
      if(isset($values->{resource_type_get_id($uri)}))
      {
        return($values->{resource_type_get_id($uri)});
      }
    }
    else
    {
      // By default, we display a possible label for this object reference
      if(isset($values->{"reify_".resource_type_get_id($uri)}))
      {
        return($values->{"reify_".resource_type_get_id($uri)});
      }
      else
      {
        // In the worse case, we return the ending of the URI
        if(isset($values->{resource_type_get_id($uri)}))
        {
          return(osf_views_generate_label_from_uri($uri));
        }        
      }      
    }
    
   
    return(array());
  } 
  
  function get_value_options() 
  {
    if (isset($this->value_options)) {
      return;
    }    
    // Get the list of all the object properties that are used
    // to describe records of this OSF instance
    $search = osf_views_get_attributes_and_filters($this->view);
   
    if (!$search->isSuccessful()) {
      drupal_set_message(t("Can't get values options for this property: [@error] @errorMsg - @errorDescription", 
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
      
      $network = variable_get("osf_views_views_target_osf_web_services_".$this->view->name, "http://localhost/ws/");   
      $objectProperties = variable_get("osf_views_objectproperty_labels", array());    
      
      foreach($resultset as $dataset => $subjects)
      {
        foreach($subjects as $uri => $subject)
        {
          if(array_search(Namespaces::$aggr."Aggregate", $subject["type"]) !== FALSE)
          {
            if($subject[Namespaces::$aggr."property"][0]["uri"] == Namespaces::$rdf."Property")
            {
              $propertyUri = $subject[Namespaces::$aggr."object"][0]["uri"];
              
              // Check if the property is a Object Property
              // Only the properties defined in the ontologies will be make available
              // to the user. If the node where the data come from hasn't defined a property
              // then it won't appear there neither, even if some records do use it
              if(isset($objectProperties[$network][$propertyUri]))
              {
                if(strlen($objectProperties[$network][$propertyUri]) > 75)
                {
                  $objectProperties[$network][$propertyUri] = substr($objectProperties[$network][$propertyUri], 0, 75) + "...";
                }
                
                $this->value_options[$propertyUri] = $objectProperties[$network][$propertyUri];
              }
            }
          }
        }
      }
    }    
  }  

  public function options_form(&$form, &$form_state) 
  {
    parent::options_form($form, $form_state);

    $this->get_value_options();
    
    $form['object_property'] = array(
      '#type' => 'select',
      '#title' => t('Object property'),
      '#options' => $this->value_options,
      '#default_value' => $this->options['object_property'],
    );
    
    $form['object_property_show_uri'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show URI of the object reference instead of its label'),
      '#default_value' => (isset($this->options['object_property_show_uri']) ? $this->options['object_property_show_uri'] : 0)
    );
  }  

  function render($values) 
  {
    $value = $this->get_value($values);
    
    if(count($value) == 0)
    {
      return("");
    }
    else
    {
      return($value[0]);
    }
  }
}
