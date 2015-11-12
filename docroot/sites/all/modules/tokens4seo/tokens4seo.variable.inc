<?php

/**
 * @file
 *
 * This is the file with the variable options to fill as:
 * 
 * Google News Keywords
 * Copyright
 * Image
 * Publisher URL
 * Author URL
 * 
 */ 

/**
 * Implemente hook_variable_info()
 */

function tokens4seo_variable_info($options) {

  /* Google news keywords */
  $variables['token4seo_variables_google_news_keywords'] = array(
    'type' => 'text',
    'title' => t('Google news keywords', array(), $options),
    'default' => '',
    'description' => t('A comma-separated list of keywords about the page. This meta tag is used as an indicator in Google News.', array(), $options),
    'required' => FALSE,
    'group' => 'token4seo_variables',
    'localize' => TRUE,
    'token' => TRUE,
  );

  /* Copyright */
  $variables['tokens4seo_variables_copyright'] = array (
    'type' => 'text',
    'title' => t('copyright', array(),$options),
    'default' => '',
    'description' => t('Details a copyright, trademark, patent, or other information that pertains to intellectual property about this page. Note that this will not automatically protect your site\'s content or your intellectual property.', array(), $options),
    'required' => FALSE,
    'group' => 'token4seo_variables',
    'localize' => TRUE,
    'token' => TRUE,
  );

  /* Image */

  /* Publisher url */
  $variables['tokens4seo_variables_publisher_url'] = array (
    'type' => 'url',
    'title' => t('Publisher url', array(),$options),
    'default' => '',
    'description' => t('Add the url for the publisher', array(), $options),
    'required' => FALSE,
    'group' => 'token4seo_variables',
    'localize' => TRUE,
    'token' => TRUE,
  );

  /* Author url  */
  $variables['tokens4seo_variables_author_url'] = array (
    'type' => 'url',
    'title' => t('Author url', array(),$options),
    'default' => '',
    'description' => t('Used by some search engines to confirm authorship of the content on a page. Should be either the full URL for the author\'s Google+ profile page or a local page with information about the author.', array(), $options),
    'required' => FALSE,
    'group' => 'token4seo_variables',
    'localize' => TRUE,
    'token' => TRUE,
  );

  return $variables;
}



/**
 * Implements hook_variable_group_info()
 */
function tokens4seo_varaible_group_info() {

  $groups['token4seo_variables'] = array (
    'title' => t('Token for seo'),
    'descripcion' => t('Variables for token for seo module'),
    'access' => 'administer configuration',
    'path' => array('admin/config/search/token4seo'),
  
  );


  return $groups;
}




