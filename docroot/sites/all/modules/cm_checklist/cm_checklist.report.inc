<?php
// $Id$

/**
 * @file
 * Admin page callbacks for the open media module.
 */

/**
 * Menu callback; prints a page listing steps to configure community media modules.
 */
function cm_checklist_main() {
  $output = '<h2>' . t('Status of Community Media Configuration:') . '</h2>' . cm_checklist_steps_as_list();
  return $output;
}

/**
 * Menu callback; prints a page listing general help for a module.
 */
function cm_checklist_page($name) {
  $output = t('steps...');
  return $output;
}

function cm_checklist_steps_as_list() {
  
  //defines constants for REQUIREMENT_INFO, REQUIREMENT_OK, REQUIREMENT_WARNING, REQUIREMENT_ERROR
  include_once DRUPAL_ROOT . '/includes/install.inc';
  
  define('REQUIREMENT_NOT_INSTALLED', -1);
  
  //Format status report using theme_status_report
  //$requirements[0] = array('title' => 'Drupal', 'value' => '7.0-dev', 'severity' => 2, 'weight' => -10, 'description' => 'You must upgrade PHP to 5.2.8 or greater.');
    
  //PHP
  $version = phpversion();
  $description = t('You are using version @version of PHP.', array('@version' => $version));
  $requirements['php'] = array(
    'title' => 'PHP', 
    'value' => $version, 
    'weight' => -10, 
  );
     
  if (version_compare(phpversion(), '5.2') < 0) {
   $requirements['php']['severity'] = 2; 
   $requirements['php']['description'] = $description . ' ' . t('You must upgrade to PHP 5.3 or greater for several core Community Media functions to work correctly.');
  }
  else {
    $requirements['php']['severity'] = 0; 
    $requirements['php']['description'] = $description;   
  }
  
  //RAM
  //max_execution_time
  //memory_limit
  //ini_get();

  $memory_limit = ini_get('memory_limit');
  $description = t('You have @memory_limit of memory allocated PHP.', array('@memory_limit' => $memory_limit));
  $requirements['memory'] = array(
    'title' => 'Memory', 
    'value' => $memory_limit, 
    'weight' => 1, 
  );
  
  $memory_needs = 64;
  
  if (module_exists('reservations')) {
    $memory_needs = ($memory_needs + 32);
  }
  
  if (module_exists('civicrm')) {
    $memory_needs = ($memory_needs + 32);
  }
  
  $memory_limit_int = str_replace('M', '', $memory_limit);
   
  if ($memory_limit_int > ($memory_needs + 100)) {
   $requirements['memory']['severity'] = REQUIREMENT_OK; 
   $requirements['memory']['description'] = $description . ' ' . t('You have more than enough memory allocated to PHP to run the enabled modules.');
  }
  
  if ($memory_limit_int > $memory_needs) {
   $requirements['memory']['severity'] = REQUIREMENT_OK; 
   $requirements['memory']['description'] = $description . ' ' . t('You have enough memory allocated to PHP to run the enabled modules.');
  }
  
  if ($memory_limit_int = $memory_needs) {
   $requirements['memory']['severity'] = REQUIREMENT_WARNING; 
   $requirements['memory']['description'] = $description . ' ' . t('You have just enough memory allocated to PHP to run the enabled modules.  If you experience performance issues, consider allocating more memory to PHP.');
  }
  
  if ($memory_limit_int < $memory_needs) {
    $requirements['memory']['severity'] = REQUIREMENT_ERROR; 
    $requirements['memory']['description'] = $description . ' ' . t('You will likely have performance problems unless you allocate at least $memory_needs of RAM to PHP.', array('$memory_needs' => $memory_needs));   
  }
  
  //Documentation Edit - Check #2
  $requirements['documentation'] = array(
    'title' => 'Shared Documentation', 
    'weight' => 3, 
  );
  
  $documentation_status = cm_checklist_documentation_status();
  $requirements['documentation']['value'] = t('Installed');
  $requirements['documentation']['severity'] = $documentation_status['severity'];
  $requirements['documentation']['description'] = $documentation_status['description'];
 
  
  
  //Legal - Check #3
  $requirements['legal'] = array(
    'title' => 'Legal', 
    'weight' => 3, 
  );
  
  if (module_exists('legal')) {
    $legal_status = cm_checklist_legal_status();
    $requirements['legal']['value'] = t('Installed');
    $requirements['legal']['severity'] = $legal_status['severity'];
    $requirements['legal']['description'] = $legal_status['description'];
  } 
  else {
    $requirements['legal']['value'] = t('Not Installed');
    $requirements['legal']['severity'] = REQUIREMENT_NOT_INSTALLED; 
    $requirements['legal']['description'] = t('The <a href="@url">Legal module</a> is not installed.', array('@url' => url('http://drupal.org/project/legal'))) . ' ' . t('This module should be installed and configured before you configure anything else that allows members to log in to your site.'); 
    $requirements['legal']['description'] .= '<p>' . t('Installation Difficulty') .': <b>' . t('Easy') . '</b></p>';
  }
  
  //Show (without VOD) - Check #4
  $requirements['cm_show'] = array(
    'title' => 'Show', 
    'weight' => 4, 
  );
   
  if (module_exists('cm_show')) {
    $cm_show_status = cm_show_status();
    $requirements['cm_show']['value'] = t('Installed');
    $requirements['cm_show']['severity'] = $cm_show_status['severity']; 
    $requirements['cm_show']['description'] = $cm_show_status['description'];
  } 
  else {
    $requirements['cm_show']['value'] = t('Not Installed');
    $requirements['cm_show']['severity'] = REQUIREMENT_NOT_INSTALLED; 
    $requirements['cm_show']['description'] = t('The <a href="@url">Community Media Show</a> module is not installed.', array('@url' => url('http://drupal.org/project/cm_show'))) . ' ' . t(' Show manages the core metadata required to exchange video with other sites.  Show can be used with an existing encoding workflows or with a workflow manage by other Drupal modules.'); 
    $requirements['cm_show']['description'] .= '<p>' . t('Installation Difficulty') .': <b>' . t('Moderate') . '</b></p>';
   
  }
  
  //PBCore - Check #5
  $requirements['pbcore'] = array(
    'title' => 'PBCore', 
    'weight' => 5, 
  );
  
  if (module_exists('pbcore')) {
    $pbcore_status = pbcore_status();
    $requirements['pbcore']['value'] = t('Installed');
    $requirements['pbcore']['severity'] = $pbcore_status['severity'];
    $requirements['pbcore']['description'] = $pbcore_status['description'];
  } 
  else {
    $requirements['pbcore']['value'] = t('Not Installed');
    $requirements['pbcore']['severity'] = REQUIREMENT_NOT_INSTALLED; 
    $requirements['pbcore']['description'] = t('The <a href="@url">PBCore</a> module enables producers to categorize their shows based on this metadata dictionary. It is required Show, but is not automatically configured.', array('@url' => url('http://drupal.org/project/pbcore'))); 
    $requirements['pbcore']['description'] .= '<p>' . t('Installation Difficulty') .': <b>' . t('Easy') . '</b></p>';
  }
  
  //Creative Commons - Check #6
  $requirements['creativecommons'] = array(
    'title' => 'Creative Commons', 
    'weight' => 6, 
  );
  
  if (module_exists('creativecommons')) {
    $creativecommons_status = creativecommons_status();
    $requirements['creativecommons']['value'] = t('Installed');
    $requirements['creativecommons']['severity'] = $creativecommons_status['severity'];
    $requirements['creativecommons']['description'] = $creativecommons_status['description'];
  } 
  else {
    $requirements['creativecommons']['value'] = t('Not Installed');
    $requirements['creativecommons']['severity'] = REQUIREMENT_NOT_INSTALLED; 
    $requirements['creativecommons']['description'] = t('<a href="@url">Creative Commons</a> adds licensing options to Show. Managing rights is complicated and you should understand the difference is the Creative Commons license before enabling this module.', array('@url' => url('http://drupal.org/project/creativecommons'))); 
    $requirements['creativecommons']['description'] .= '<p>' . t('Installation Difficulty') .': <b>' . t('Easy') . '</b></p>';
  }

  
  //Crew Connect - Check #7
  $requirements['om_crew_connect'] = array(
    'title' => 'Crew Connect', 
    'weight' => 7, 
  );
  
  if (module_exists('om_crew_connect')) {
    $om_crew_connect_status = om_crew_connect_status();
    $requirements['om_crew_connect']['value'] = t('Installed');
    $requirements['om_crew_connect']['severity'] = $om_crew_connect_status['severity'];
    $requirements['om_crew_connect']['description'] = $om_crew_connect_status['description'];
  } 
  else {
    $requirements['om_crew_connect']['value'] = t('Not Installed');
    $requirements['om_crew_connect']['severity'] = REQUIREMENT_NOT_INSTALLED; 
    $requirements['om_crew_connect']['description'] = t('The <a href="@url">Crew Connect</a> module is not installed.', array('@url' => url('http://drupal.org/project/om_crew_connect'))) . t(' Use this is a simple tool to help producers connect with each other.  Enabling Crew Connect gives your producers a reason to login to your site and gives you experience managing users and permissions.'); 
    $requirements['om_crew_connect']['description'] .= '<p>' . t('Installation Difficulty') .': <b>' . t('Easy') . '</b></p>';
  }
  
  //Airing - Check #8
  $requirements['cm_airing'] = array(
    'title' => 'Airing', 
    'weight' => 8, 
  );
  
  if (module_exists('cm_airing')) {
    $cm_airing_status = cm_airing_status();
    $requirements['cm_airing']['value'] = t('Installed');
    $requirements['cm_airing']['severity'] = $cm_airing_status['severity'];
    $requirements['cm_airing']['description'] = $cm_airing_status['description'];
  } 
  else {
    $requirements['cm_airing']['value'] = t('Not Installed');
    $requirements['cm_airing']['severity'] = REQUIREMENT_NOT_INSTALLED; 
    $requirements['cm_airing']['description'] = t('The <a href="@url">Community Media Airing</a> module is not installed.', array('@url' => url('http://drupal.org/project/cm_airing'))) . t(' Airing nodes are used to display scheduling information from playback server RSS. It includes FullCalendar based Views and Blocks that display upcoming Airings.  When Airing Show Sync is configured, Upcoming airings of a Show or all Shows in a Project can be displayed.'); 
    $requirements['cm_airing']['description'] .= '<p>' . t('Installation Difficulty') .': <b>' . t('Moderate') . '</b></p>';
  }
  
   //CiviCRM - Check #9
  $requirements['civicrm'] = array(
    'title' => 'CiviCRM', 
    'weight' => 9, 
  );
  
  if (module_exists('civicrm')) {
    $cm_checklist_civicrm_status = cm_checklist_civicrm_status();
    $requirements['civicrm']['value'] = t('Installed.');
    $requirements['civicrm']['severity'] = $cm_checklist_civicrm_status['severity'];
    $requirements['civicrm']['description'] = $cm_checklist_civicrm_status['description'];
  } 
  else {
    $requirements['civicrm']['value'] = t('Not Installed');
    $requirements['civicrm']['severity'] = REQUIREMENT_NOT_INSTALLED; 
    $requirements['civicrm']['description'] =  t('<a href="@url">CiviCRM</a>', array('@url' => url('http://drupal.org/project/civicrm'))); 
    $requirements['civicrm']['description'] .= '<p>' . t('Installation Difficulty') .': <b>' . t('Moderate') . '</b></p>';
  }
  
  //Project - Check #9
  $requirements['cm_project'] = array(
    'title' => 'Project', 
    'weight' => 9, 
  );

  if (module_exists('cm_project')) {
    $cm_project_status = cm_project_status();
    $requirements['cm_project']['value'] = t('Installed');
    $requirements['cm_project']['severity'] = $cm_project_status['severity']; 
    $requirements['cm_project']['description'] = $cm_project_status['description'];
  } 
  else {
    $requirements['cm_project']['value'] = t('Not Installed');
    $requirements['cm_project']['severity'] = REQUIREMENT_NOT_INSTALLED; 
    $requirements['cm_project']['description'] = t('The <a href="@url">Community Media Project</a> module is not installed.', array('@url' => url('http://drupal.org/project/cm_project'))) . t(' Use this module to group Shows and Reservations.  The module leverages Organic Groups (OG).  If you decide to allow producers to maintain public Project pages, there are dozens of popular OG modules that extend social networking features.'); 
    $requirements['cm_project']['description'] .= '<p>' . t('Installation Difficulty') .': <b>' . t('Moderate') . '</b></p>';
  }
   
    
  //Reservations - Check #10
  $requirements['reservations'] = array(
    'title' => 'Reservations', 
    'weight' => -8, 
  );
  if (module_exists('reservations')) {
    $cm_checklist_reservations_status = cm_checklist_reservations_status();
    $requirements['reservations']['value'] = t('Installed.');
    $requirements['reservations']['severity'] = $cm_checklist_reservations_status['severity'];
    $requirements['reservations']['description'] = $cm_checklist_reservations_status['description'];
  } 
  else {
    $requirements['reservations']['value'] = t('Not Installed');
    $requirements['reservations']['severity'] = REQUIREMENT_NOT_INSTALLED; 
    $requirements['reservations']['description'] = t('The <a href="@url">Reservations</a> module is not installed.', array('url' => url('http://drupal.org/project/reservations'))) . t(' Used to Manage Equipment Reservations and Checkout process'); 
    $requirements['reservations']['description'] .= '<p>' . t('Installation Difficulty') .': <b>' . t('Difficult') . '</b></p>';
   
  }
    
  return theme('status_report', array('requirements' => $requirements));
}


