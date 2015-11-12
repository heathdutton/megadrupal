<?php
  
  use \StructuredDynamics\osf\framework\Namespaces;
  
  /**
  * resource_type.tpl.php is an example of a generic template to display information 
  * about entities that are hosted in OSF.
  * 
  * You have to put that file into your theme's templates folder. This template
  * is the default template used to theme the resource_types. You can create
  * more templates, one for each resource_type bundle by creating new template
  * files such as: resource-type--foaf-person.tpl.php
  * 
  * You have two ways to get the description of the record being displayed:
  * 
  *   (1) You have access to the internal drupal ResourceType custom entity type instance
  *       by using this variable: $entity or $element
  *   (2) Via a Subject class instance which represents the record's description. It is
  *       accessible via the $subject template variable.
  * 
  * By using the method (1), you can do what you normally do in Drupal
  * By using the method (2), you have access to a series of utility functions
  * to help you manipulating the information of the record.
  * 
  * In this template example, we use the method (2)
  * 
  * @see https://github.com/structureddynamics/OSF-WS-PHP-API/blob/master/StructuredDynamics/osf/framework/Resultset.php
  */
  echo "<h2>".get_pref_label($subject)."</h2>";
  
  // Now we display all the information we have related to this subject, without templating any of it. 
  echo "<table>";
  
  foreach($subject->getDataPropertiesUri() as $dataPropertyURI)
  {
    // Get the description of the object property
//   if(isset($properties[$dataPropertyURI]))
     if(property_exists($element, resource_type_get_id($dataPropertyURI)))
     {             
       echo "<tr>\n";
       echo "<td>".get_label_from_uri($dataPropertyURI)."</td>\n";
       echo "<td><ul>\n";
       
       foreach($subject->getDataPropertyValues($dataPropertyURI) as $value)
       {                  
         echo "<li>".$value["value"]."</li>\n";
       }       
       
       echo "</ul></td>\n";
       echo "</tr>\n";       
     }    
  }  
  
  // First, let's get all the entities related to the object record at once. That way
  // we minimize the interaction with OSF and minimize the number of queries
  // needed to generate that page.
  $uris = array();
 
  foreach($subject->getObjectPropertiesUri() as $objPropertyURI)
  {
    foreach($subject->getObjectPropertyValues($objPropertyURI) as $value)
    {
      if($objPropertyURI == "type")
      {
        array_push($uris, $value);
      }
      else
      {
        array_push($uris, $value["uri"]);
      }
    }
  }  
  
  $objectEntities = resource_type_load($uris);

  foreach($subject->getObjectPropertiesUri() as $objPropertyURI)
  {    
     // Get the description of the object property
     //if(isset($properties[$objPropertyURI]))
     if(property_exists($element, resource_type_get_id($objPropertyURI)))
     {
       echo "<tr>\n";
       echo "<td>".get_label_from_uri($objPropertyURI)."</td>\n";
       echo "<td><ul>\n";
       
       foreach($subject->getObjectPropertyValues($objPropertyURI) as $value)
       {
         $label = "";
         
         if(isset($objectEntities[$value["uri"]]))
         {         
           if(isset($objectEntities[$value["uri"]]->obj))
           {
             $entity = $objectEntities[$value["uri"]]->obj;
             $label = $entity->getPrefLabel();
             
             if($label != "")
             {
               //echo "<li><a href=\"?uri=".urlencode($value["uri"])."\">".$label."</a></li>\n";
               echo "<li><a href=\"".osf_entities_get_resource_page_url_from_entity_uri($value["uri"])."\">".$label."</a></li>\n";
               $label = $entity->getUri();
             }
             else
             {
               echo "<li>".$entity->getUri()."</li>\n";
             }
           }
           else
           {
             echo "<li>".$value["uri"]."</li>\n";
           }
         }
         else
         { 
           echo "<li>".$value["uri"]."</li>\n";          
         }
       }
       
       echo "</ul></td>\n";
       echo "</tr>\n";
     }       
  }
  
  echo "</table>";


  function get_pref_label(&$subject)
  {
    $values = $subject->getDataPropertyValues(Namespaces::$iron.'prefLabel');
    foreach($values as $value)
    {
      if(isset($value['lang']) && $value['lang'] == get_lang(get_default_language()))
      {
        return($value['value']);
      }
    }
    
    return($values[0]['value']);
  }  
?>
