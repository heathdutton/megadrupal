<?php
 
  /**
  * resource_type_search.tpl.php is an example of a generic template to display information 
  * about entities that are hosted in OSF and that are returned in a search resultset.
  * 
  * You have to put that file into your theme's templates folder. This template
  * is the default template used to theme the resource_types. You can create
  * more templates, one for each resource_type bundle by creating new template
  * files such as: resource_type__foaf_person__search.tpl.php
  * 
  * You have two ways to get the description of the record being displayed:
  * 
  *   (1) You have access to the internal drupal ResourceType custom entity type instance
  *       by using this variable: $entity
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
 
?>
 
<h3><?php print "<a href=\"/resources/".urlencode(urlencode($entity->uri))."\">".(isset($entity->preflabel[get_lang(get_default_language())][0]['value']) ? $entity->preflabel[get_lang(get_default_language())][0]['value'] : $entity->label)."</a>"; ?></h3>

<div><?php print (isset($entity->description[get_lang(get_default_language())][0]['value']) ? $entity->description['und'][0]['value'] : ''); ?></div>

