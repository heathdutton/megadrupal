<?php

  /*! @defgroup OSFOntologyModule OSF Ontology Drupal Module */
  //@{ 

  /*! @file getTemplates.php
      @brief Get templates applicable to a target class
      @details This file returns a list of templates that can be used to display information about instance
               of that class. This is returned to the JavaScript application to display to the user in the 
               advance pannel.
   
      @author Frederick Giasson, Structured Dynamics LLC.

      \n\n\n
   */

  $currentDir = getcwd();

  // Change the directory of the executed script to enable Drupal to bootstrap
  chdir($_SERVER['DOCUMENT_ROOT']);
  define('DRUPAL_ROOT', getcwd());  
  
  global $base_url;

  $base_url = 'http://'.$_SERVER['HTTP_HOST'];

  include_once('./includes/bootstrap.inc');
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  
  if(user_access('administer osf ontology'))
  {
    $templates = getTemplates();

    header("HTTP/1.1 200 OK");
    header("Content-Type: application/json");
    
    echo "{\n";
    echo "  \"templates\": [\n";

    for($i = 0; $i < count($templates); $i++)
    {
      if(($i + 1) == count($templates))
      {
        echo "\"" . $templates[$i] . "\"\n";
      }
      else
      {
        echo "\"" . $templates[$i] . "\",\n";
      }
    }

    echo "]}";
  }
  else
  {
    echo "{\n";

    echo "  \"templates\": [\n";

    echo "]}";    
  }


  /*! @brief Get all available templates defined on the node.

      \n

      @return returns an array of templates files defined on the node.

      @author Frederick Giasson, Structured Dynamics LLC.

      \n\n\n
  */
  function getTemplates()
  {
    $templates = array();

    // Get templates from the OSF Entities module
    if(is_dir(realpath(".") . "/" . drupal_get_path("module", "osf_entities")))
    {
      if($handle = opendir(realpath(".") . "/" . drupal_get_path("module", "osf_entities")))
      {
        while(($file = readdir($handle)) !== FALSE)
        {
          if(stripos($file, ".tpl.php") !== FALSE && stripos($file, "resource_type") !== FALSE)
          {
            array_push($templates, $file);
          }
        }
      }
    }

    // Get templates from the OSF SearchAPI module
    if(is_dir(realpath(".") . "/" . drupal_get_path("module", "osf_searchapi")))
    {
      if($handle = opendir(realpath(".") . "/" . drupal_get_path("module", "osf_searchapi")))
      {
        while(($file = readdir($handle)) !== FALSE)
        {
          if(stripos($file, ".tpl.php") !== FALSE && stripos($file, "resource_type") !== FALSE)
          {
            array_push($templates, $file);
          }
        }
      }
    }
    
    // Get templates from the theme's templates folder
    if(is_dir(realpath(".") . "/" . drupal_get_path('theme', variable_get('theme_default', NULL)) . '/templates/'))
    {
      if($handle = opendir(realpath(".") . "/" . drupal_get_path('theme', variable_get('theme_default', NULL)) . '/templates/'))
      {
        while(($file = readdir($handle)) !== FALSE)
        {
          if(stripos($file, ".tpl.php") !== FALSE && stripos($file, "resource_type") !== FALSE)
          {
            array_push($templates, $file);
          }
        }
      }
    }    

    return(array_unique($templates));
  }
    
  //@}   
?>