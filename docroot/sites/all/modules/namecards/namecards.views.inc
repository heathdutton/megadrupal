<?php
/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 *
 * According to documentation hook_views_handlers() no longer required since class definition files included in namecards.info file.
 */
// function namecards_views_handlers() {
//   $info = array(
//     'info' => array(
//       'path' => drupal_get_path('module', 'namecards'),
//     ),
//     'handlers' => array(
//       'namecards_handler_filter_public' => array(
//         'parent' => 'views_handler_filter',
//       ),
//       'namecards_handler_argument_browse_contacts_search_terms' => array(
//         'parent' => 'views_handler_argument_string',
//       ),
//       'namecards_handler_argument_organizations_search_terms' => array(
//         'parent' => 'views_handler_argument_string',
//       ),
//       'namecards_handler_argument_events_search_terms' => array(
//         'parent' => 'views_handler_argument_string',
//       ),
//     ),
//   );

//   return $info;
// }

/**
 * Implementation of hook_views_data_alter()
 */
function namecards_views_data_alter(&$data) {
  $data['field_data_namecards_namecard_public']['visibility'] = array(
    'group' => 'Namecards',
    'title' => t('Contacts visibility'),
    'filter' => array(
      'handler' => 'namecards_handler_filter_public',
      'help' => t('Preset filter required for proper functioning of namecards module. For use with preset namecards views only.'),
    ),
  );
  $data['field_data_namecards_namecard_full_content']['search'] = array(
    'group' => 'Namecards',
    'title' => t('Add search terms to browse contacts view'),
    'argument' => array(
      'handler' => 'namecards_handler_argument_browse_contacts_search_terms',
      'help' => t('Preset filter required for proper functioning of namecards module. For use with preset namecards views only.'),
    ),
  );
  $data['node']['search_orgs'] = array(
    'group' => 'Namecards',
    'title' => t('Add search terms to organizations view'),
    'argument' => array(
      'handler' => 'namecards_handler_argument_organizations_search_terms',
      'help' => t('Preset filter required for proper functioning of namecards module. For use with preset namecards views only.'),
    ),
  );
  $data['node']['search_events'] = array(
    'group' => 'Namecards',
    'title' => t('Add search terms to events view'),
    'argument' => array(
      'handler' => 'namecards_handler_argument_events_search_terms',
      'help' => t('Preset filter required for proper functioning of namecards module. For use with preset namecards views only.'),
    ),
  );

  return $data;
}

