<?php

/**
 * Field handler to translate a record type into its readable form.
 */
class osf_views_field_osf_type extends views_handler_field_node 
{
  function option_definition() 
  {    
    $options = parent::option_definition();

    return $options;
  }

  /**
   * Provide machine_name option for to node type display.
   */
  function options_form(&$form, &$form_state) 
  {
    parent::options_form($form, $form_state);
  }
  
  function options_submit(&$form, &$form_state) 
  {
    parent::options_submit($form, $form_state);

    // Change the UI name to make it easier to read to the Views user
    $form_state['values']['options']['ui_name'] = "OSF ";
  }  

  function render($values) 
  {
    $value = $this->get_value($values);

    $uri = $value[0];
    
    $labels = variable_get("osf_views_class_labels", array());
    $network = variable_get("osf_views_views_target_osf_web_services_".$this->view->name, "http://localhost/ws/");   
    
    // Get the preferred label for this type
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
      
      $label = osf_views_generate_label_from_uri($label);

      // Now save the new label into the cache array
      $labels[$network][$uri] = $label;
      
     // If there are new labels generated from this procedure, we simply save them into the cache.
      variable_del("osf_views_class_labels");      
      variable_set("osf_views_class_labels", $labels); 
    } 
    
    return($label);
  }
}
