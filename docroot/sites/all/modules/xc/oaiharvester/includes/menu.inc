<?php

/**
 * Implements hook_menu().
 *
 * The menus of OAI Harvester module.
 *
 * Supported URLs are:
 * harvester - opening page
 * admin/xc/harvester/repository - the repository page
 * admin/xc/harvester/repository/list - list of repositories
 * admin/xc/harvester/repository/add - add a new repository
 * admin/xc/harvester/repository/%oaiharvester_repository - show a repository
 * admin/xc/harvester/repository/%oaiharvester_repository/view - show the properties of a repository
 * admin/xc/harvester/repository/%oaiharvester_repository/edit - edit a repository
 * admin/xc/harvester/repository/%oaiharvester_repository/revalidate - revalidate the respository
 * admin/xc/harvester/repository/%oaiharvester_repository/sets - show the sets supported by a repository
 * admin/xc/harvester/repository/%oaiharvester_repository/formats - show the metadata formats supported by a repository
 * admin/xc/harvester/set - show sets
 * admin/xc/harvester/set/list - list of sets
 * admin/xc/harvester/set/%oaiharvester_set - show the properties of a set
 * admin/xc/harvester/set/%oaiharvester_set/view - show the properties of a set
 * admin/xc/harvester/format - the page of metadata formats
 * admin/xc/harvester/format/list - list of metadata formats
 * admin/xc/harvester/format/%oaiharvester_format - show the properties of a format
 * admin/xc/harvester/format/%oaiharvester_format/view - show the properties of a format
 * admin/xc/harvester/schedule - the page of schedules
 * admin/xc/harvester/schedule/list - list of schedules
 * admin/xc/harvester/schedule/step1 - Add scheduled harvest
 * admin/xc/harvester/schedule/%oaiharvester_schedule - properties of a schedule
 * admin/xc/harvester/schedule/%oaiharvester_schedule/view - properties of a schedule
 * admin/xc/harvester/schedule/%oaiharvester_schedule/edit - modify a schedule
 * admin/xc/harvester/schedule/%oaiharvester_schedule/start - manual start of schedule harvesting
 *
 * @return array The menu array
 */
function _oaiharvester_menu() {

  $items['admin/xc/harvester'] = array(
    'title' => 'Metadata Harvester',
    'description' => 'Register remote (OAI-PMH) metadata repositories and schedule harvesting of those repositories.  This feature enables loading and synchronization of metadata stored in your ILS and other servers. Prerequisite: Setup a storage location (see Storage menu).  The XC Metadata Services Toolkit and XC OAI Toolkit are installed and running.  Your ILS is configured with an ILS Export script to load metadata into the OAI Toolkit.  The MST is configured to load metadata from the OAI Toolkit, and has processed it into XC Schema format.  Your metadata is in XC schema format and available for harvesting by Drupal through the MST Transformation or Aggregation service (either one will work).',
    'page callback' => 'system_admin_menu_block_page',
    'access arguments' => array(ADMINISTER_HARVESTER),
    'file' => 'system.admin.inc',
    'file path' => drupal_get_path('module', 'system'),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 3,
  );
  $items['admin/xc/harvester/repository'] = array(
    'title' => 'Repositories',
    'description' => 'Add, modify and view remote metadata repositories',
    'access arguments' => array(ADMINISTER_HARVESTER),
    'page callback' => 'oaiharvester_repository_list',
    'type' => MENU_NORMAL_ITEM,
    'weight' => -1,
  );
  $items['admin/xc/harvester/repository/list'] = array(
    'title' => 'List',
    'description' => 'List OAI-PMH repositories',
    'page callback' => 'oaiharvester_repository_list',
    'access arguments' => array(ADMINISTER_HARVESTER),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 0,
  );
  $items['admin/xc/harvester/repository/add'] = array(
    'title' => 'Add Repository',
    'description' => 'Register a new OAI-PMH repository',
    'page callback' => 'oaiharvester_repository_add',
    'access arguments' => array(ADMINISTER_HARVESTER),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
  );

  $repositories = oaiharvester_provider_get_all();
  $weight = 2;
  foreach ($repositories as $repository) {
    $id = $repository->provider_id;
    $items['admin/xc/harvester/repository/' . $id] = array(
      'title' => $repository->name,
      'title callback' => 'check_plain',
      'description' => "OAI-PMH repository $repository->name",
      'page callback' => 'oaiharvester_repository_view',
      'page arguments' => array($id),
      'access arguments' => array(ADMINISTER_HARVESTER),
      'type' => MENU_NORMAL_ITEM,
      'weight' => $weight++,
    );
    $items['admin/xc/harvester/repository/' . $id . '/view'] = array(
      'title' => 'View',
      'description' => 'View the properties of a OAI-PMH repository',
      'access arguments' => array(ADMINISTER_HARVESTER),
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
    );
    $items['admin/xc/harvester/repository/' . $id . '/edit'] = array(
      'title' => 'Edit',
      'description' => 'Edit the properties of a OAI-PMH repository',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('oaiharvester_repository_edit_form', $id),
      'access arguments' => array(ADMINISTER_HARVESTER),
      'type' => MENU_LOCAL_TASK,
      'weight' => 3,
    );
    $items['admin/xc/harvester/repository/' . $id . '/revalidate'] = array(
      'title' => 'Revalidate',
      'description' => 'Revalidate an OAI-PMH repository. It refresh the basic information, the list of sets and format supported by the repository.',
      'page callback' => 'oaiharvester_repository_validate',
      'page arguments' => array($id),
      'access arguments' => array(ADMINISTER_HARVESTER),
      'type' => MENU_LOCAL_TASK,
      'weight' => 4,
    );
    /*
     $items['admin/xc/harvester/repository/' . $id . '/delete'] = array(
     'title' => 'Delete',
     'page callback' => 'oaiharvester_repository_delete',
     'page arguments' => array($id),
     'access arguments' => array(ADMINISTER_HARVESTER),
     'type' => MENU_LOCAL_TASK,
     'weight' => 5,
     );
     */
    $items['admin/xc/harvester/repository/' . $id . '/sets'] = array(
      'title' => 'Sets',
      'description' => 'Show the sets supported by an OAI-OMH repository',
      'page callback' => 'oaiharvester_repository_sets',
      'page arguments' => array($id),
      'access arguments' => array(ADMINISTER_HARVESTER),
      'type' => MENU_LOCAL_TASK,
      'weight' => 6,
    );
    $items['admin/xc/harvester/repository/' . $id . '/formats'] = array(
      'title' => 'Formats',
      'description' => 'Show the formats supported by an OAI-OMH repository',
      'page callback' => 'oaiharvester_repository_formats',
      'page arguments' => array($id),
      'access arguments' => array(ADMINISTER_HARVESTER),
      'type' => MENU_LOCAL_TASK,
      'weight' => 7,
    );
  }

  $items['admin/xc/harvester/set'] = array(
    'title' => 'Sets',
    'description' => 'View all sets supported by any OAI-PMH repository.',
    'page callback' => 'oaiharvester_set_list',
    'access arguments' => array(ADMINISTER_HARVESTER),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 1,
  );
  $items['admin/xc/harvester/set/list'] = array(
    'title' => 'List',
    'description' => 'View all sets supported by any OAI-PMH repository.',
    'page callback' => 'oaiharvester_set_list',
    'access arguments' => array(ADMINISTER_HARVESTER),
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );
  $items['admin/xc/harvester/set/%oaiharvester_set'] = array(
    'title callback' => 'oaiharvester_set_title',
    'title arguments' => array(4),
    'description' => 'Display the properties of a given set.',
    'page callback' => 'oaiharvester_set_view',
    'page arguments' => array(4),
    'access arguments' => array(ADMINISTER_HARVESTER),
    'type' => MENU_CALLBACK,
  );
  $items['admin/xc/harvester/set/%oaiharvester_set/view'] = array(
    'title' => 'View',
    'description' => 'Display the properties of a given set.',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'access arguments' => array(ADMINISTER_HARVESTER),
    'weight' => -10,
  );
  $items['admin/xc/harvester/format'] = array(
    'title' => 'Formats',
    'description' => 'View a list of repositories organized by the metadata formats they offer',
    'page callback' => 'oaiharvester_format_list',
    'access arguments' => array(ADMINISTER_HARVESTER),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 2,
  );
  $items['admin/xc/harvester/format/list'] = array(
    'title' => 'List',
    'description' => 'View a list of repositories organized by the metadata formats they offer',
    'page callback' => 'oaiharvester_format_list',
    'access arguments' => array(ADMINISTER_HARVESTER),
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );
  $items['admin/xc/harvester/format/%oaiharvester_format'] = array(
    'title callback' => 'oaiharvester_format_title',
    'title arguments' => array(4),
    'description' => 'Display the properties of a given format.',
    'page callback' => 'oaiharvester_format_view',
    'page arguments' => array(4),
    'access arguments' => array(ADMINISTER_HARVESTER),
    'type' => MENU_CALLBACK,
  );
  $items['admin/xc/harvester/format/%oaiharvester_format/view'] = array(
    'title' => 'View',
    'description' => 'Display the properties of a given format.',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'access arguments' => array(ADMINISTER_HARVESTER),
    'weight' => -10,
  );

  $items['admin/xc/harvester/schedule'] = array(
    'title' => 'Scheduled harvests',
    'description' => 'Create, modify, and view OAI-PMH harvest schedules',
    'page callback' => 'oaiharvester_schedule_list',
    'access arguments' => array(ADMINISTER_HARVESTER),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 0,
  );

  $items['admin/xc/harvester/schedule/list'] = array(
    'title' => 'List',
    'description' => 'List of harvest schedules',
    'page callback' => 'oaiharvester_schedule_list',
    'access arguments' => array(ADMINISTER_HARVESTER),
    'type' => MENU_LOCAL_TASK,
    'weight' => 0,
  );
  $items['admin/xc/harvester/schedule/step1'] = array(
    'title' => 'Add scheduled harvest',
    'description' => 'Create a new harvest schedule',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('oaiharvester_schedule_multiform'),
    'access arguments' => array(ADMINISTER_HARVESTER),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
  );
  $items['admin/xc/harvester/schedule/emptyqueue'] = array(
    'title' => 'Empty harvester queue',
    'description' => 'Empty the harvester queue',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('oaiharvester_schedule_emptyqueue_form'),
    'access arguments' => array(ADMINISTER_HARVESTER),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );
  $items['admin/xc/harvester/schedule/unlock'] = array(
    'title' => 'Unlock harvests',
    'description' => 'Unlock harvests by setting the oaiharvester_processing_cron variable to default',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('oaiharvester_schedule_unlock_form'),
    'access arguments' => array(ADMINISTER_HARVESTER),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
  );
  $items['admin/xc/harvester/schedule/step2'] = array(
    'title' => 'Add Schedule Step 2',
    'page callback' => 'oaiharvester_schedule_step2',
    'access arguments' => array(ADMINISTER_HARVESTER),
    'type' => MENU_CALLBACK,
    'weight' => 3,
  );
  $items['admin/xc/harvester/schedule/continue/%oaiharvester_saved_batch/%/%'] = array(
    'title' => 'Continue harvesting',
    'description' => 'Continue a stopped harvest',
    'page callback' => 'oaiharvester_schedule_continue',
    'page arguments' => array(5, 6, 7),
    'load arguments' => array(5),
    'access arguments' => array(HARVEST_RECORDS),
    'type' => MENU_CALLBACK,
    'weight' => 3,
  );
  $items['admin/xc/harvester/schedule/restart/%oaiharvester_saved_batch/%'] = array(
    'title' => 'Restart harvesting',
    'description' => 'Restart a stopped harvest',
    'page callback' => 'oaiharvester_schedule_restart',
    'page arguments' => array(5, 6),
    'load arguments' => array(5),
    'access arguments' => array(HARVEST_RECORDS),
    'type' => MENU_CALLBACK,
    'weight' => 3,
  );

  $schedules = _oaiharvester_schedule_get_all();
  $weight = 4;
  foreach ($schedules as $schedule) {
    $id = $schedule->harvest_schedule_id;
    $items['admin/xc/harvester/schedule/' . $id] = array(
      'title' => $schedule->schedule_name,
      'title callback' => 'check_plain',
      'description' => 'Display the settings of a given schedule',
      'page callback' => 'oaiharvester_schedule_view',
      'page arguments' => array(4, TRUE),
      'access arguments' => array(ADMINISTER_HARVESTER),
      'type' => MENU_NORMAL_ITEM,
      'weight' => $weight++,
    );
    $items['admin/xc/harvester/schedule/' . $id . '/view'] = array(
      'title' => 'View',
      'description' => 'Display the settings of a given schedule',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'access arguments' => array(ADMINISTER_HARVESTER),
      'weight' => -10,
    );
    $items['admin/xc/harvester/schedule/' . $id . '/edit'] = array(
      'title' => 'Edit',
      'description' => 'Edit the harvest schedule',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('oaiharvester_schedule_edit_form', $schedule),
      'access arguments' => array(ADMINISTER_HARVESTER),
      'type' => MENU_LOCAL_TASK,
      'weight' => 4,
    );
    $items['admin/xc/harvester/schedule/' . $id . '/resettime'] = array(
      'title' => 'Reset time',
      'description' => 'Reset last ran',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('oaiharvester_schedule_resettime_form', $schedule),
      'access arguments' => array(ADMINISTER_HARVESTER),
      'type' => MENU_LOCAL_TASK,
      'weight' => 5,
    );
    $items['admin/xc/harvester/schedule/' . $id . '/list_http_cache'] = array(
      'title' => 'List HTTP cache',
      'description' => 'List HTTP cache',
      'page callback' => 'oaiharvester_list_http_cache',
      'page arguments' => array(4),
      'access arguments' => array(ADMINISTER_HARVESTER),
      'type' => MENU_LOCAL_TASK,
      'weight' => 10,
    );
    $items['admin/xc/harvester/schedule/' . $id . '/clear_cache'] = array(
      'title' => 'Clear cache',
      'description' => 'Clear cache',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('oaiharvester_schedule_clear_cache_form', $schedule),
      'load arguments' => array(4, TRUE),
      'access arguments' => array(ADMINISTER_HARVESTER),
      'type' => MENU_LOCAL_TASK,
      'weight' => 12,
    );
    $items['admin/xc/harvester/schedule/' . $id . '/report'] = array(
      'title' => 'Harvest Reports',
      'description' => 'Display reports about a schedules',
      'page callback' => 'oaiharvester_schedule_report_list',
      'page arguments' => array(4),
      'access arguments' => array(HARVEST_RECORDS),
      'type' => MENU_LOCAL_TASK,
      'weight' => 13,
    );
    $items['admin/xc/harvester/schedule/' . $id . '/set_passive'] = array(
      'title' => 'Set passive',
      'description' => 'Set status passive (not running)',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('oaiharvester_schedule_set_passive_form', $schedule),
      'load arguments' => array(4, TRUE),
      'access arguments' => array(ADMINISTER_HARVESTER),
      'type' => MENU_LOCAL_TASK,
      'weight' => 14,
    );
    $items['admin/xc/harvester/schedule/' . $id . '/start'] = array(
      'title' => 'Harvest',
      'description' => 'Start harvester',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('oaiharvester_schedule_start_form', $schedule),
      'load arguments' => array(4, TRUE),
      'access arguments' => array(ADMINISTER_HARVESTER),
      'type' => MENU_LOCAL_TASK,
      'weight' => 20,
    );

    $items['admin/xc/harvester/schedule/' . $id . '/batch/%oaiharvester_saved_batch'] = array(
      'title' => 'Harvest report',
      'description' => 'Harvest report',
      'page callback' => 'oaiharvester_saved_batch_view',
      'page arguments' => array(4, 6),
      'load arguments' => array(4, TRUE),
      'access arguments' => array(ADMINISTER_HARVESTER),
      'type' => MENU_CALLBACK,
      'weight' => 10,
    );

    $items['admin/xc/harvester/schedule/' . $id . '/report/%oaiharvester_saved_batch'] = array(
      'title' => 'Report #',
      'description' => 'Display report about a harvest',
      'page callback' => 'oaiharvester_schedule_report',
      'page arguments' => array(6),
      'load arguments' => array(6),
      'access arguments' => array(HARVEST_RECORDS),
      'type' => MENU_CALLBACK,
      'weight' => 1,
    );
  }

  /*
   $items['admin/xc/harvester/logs'] = array(
   'title' => 'Logs',
   'description' => 'View harvest logs',
   'type' => MENU_NORMAL_ITEM,
   'access arguments' => array(ADMINISTER_HARVESTER),
   'weight' => 4,
   );
   */

  return $items;
}
