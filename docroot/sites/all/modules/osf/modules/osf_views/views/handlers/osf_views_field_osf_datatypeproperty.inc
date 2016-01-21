<?php

use \StructuredDynamics\osf\framework\Namespaces;

/**
 * Field handler to translate any data property into its readable form.
 */
class osf_views_field_osf_datatypeproperty extends views_handler_field 
{
  function option_definition() 
  {    
    $options = parent::option_definition();

    $options['datatype_property'] = array('default' => NULL);    
    
    return $options;
  }
  
  function options_submit(&$form, &$form_state) 
  {
    parent::options_submit($form, $form_state);

    $targetURI = $form['datatype_property']['#value'];
    $label = osf_views_get_label("osf_views_datatypeproperty_labels", $targetURI);
    if (!empty($label)) {
      // Change the UI name to use the label for the Views admin name if we have one
      $form_state['values']['options']['ui_name'] = $form['datatype_property']['#title'] . ' (' . $label . ')';
    }
  }  
  
  public function get_value($values, $field = NULL) 
  {
    // Check if we have a value for this selected field (property)
    // These values have been created by the Query Plugin
    $this->get_value_options();
    
    $uri = $this->options["datatype_property"];

    if(isset($values->{resource_type_get_id($uri)}))
    {
      return($values->{resource_type_get_id($uri)});
    }
    
    if(isset($values->{resource_type_get_id($uri)}))
    {
      return($values->{resource_type_get_id($uri)});
    }
   
    return(array());
  } 
  
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }    
    // Get the list of all the datatype properties that are used
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
                
                $this->value_options[$propertyUri] = $datatypeProperties[$network][$propertyUri];
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
    
    $form['datatype_property'] = array(
      '#type' => 'select',
      '#title' => t('Data property'),
      '#options' => $this->value_options,
      '#default_value' => $this->options['datatype_property'],
    );
  }  

  function render($values) 
  {
    $value = $this->get_value($values);

    if(count($value) == 0)
    {
      return("");
    }
    elseif(!is_array($value[0]))
    {
      return($value[0]);
    }
    else
    {
      return($value[0][0]);
    }
  }
}
