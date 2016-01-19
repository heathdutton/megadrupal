<?php

/**
 * @file
 * Smartling views integrations.
 */

/**
 * Implements hook_views_data_alter().
 */
function smartling_views_data_alter(&$data) {
  // Node relationship.
  $data['node']['smartling_entity_data'] = array(
    'group' => t('Smartling'),
    'title' => t('Smartling Entity'),
    'help' => t('Node relationship.'),
    'relationship' => array(
      'base' => 'smartling_entity_data',
      'base field' => 'rid',
      'relationship field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Smartling related ID'),
    ),
  );

  // Term relationship.
  $data['taxonomy_term_data']['smartling_entity_data'] = array(
    'group' => t('Smartling'),
    'title' => t('Smartling Entity'),
    'help' => t('Taxonomy relationship.'),
    'relationship' => array(
      'base' => 'smartling_entity_data',
      'base field' => 'rid',
      'relationship field' => 'tid',
      'handler' => 'views_handler_relationship',
      'label' => t('Smartling related ID'),
    ),
  );

  // Comment relationship.
  $data['comment']['smartling_entity_data'] = array(
    'group' => t('Smartling'),
    'title' => t('Smartling Entity'),
    'help' => t('Comment relationship.'),
    'relationship' => array(
      'base' => 'smartling_entity_data',
      'base field' => 'rid',
      'relationship field' => 'cid',
      'handler' => 'views_handler_relationship',
      'label' => t('Smartling related ID'),
    ),
  );

  // User relationship.
  $data['users']['smartling_entity_data'] = array(
    'group' => t('Smartling'),
    'title' => t('Smartling Entity'),
    'help' => t('User relationship.'),
    'relationship' => array(
      'base' => 'smartling_entity_data',
      'base field' => 'rid',
      'relationship field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Smartling related ID'),
    ),
  );

  // Add filter.
  $data['smartling_entity_data']['status'] = array(
    'title' => t('Smartling status'),
    'help' => t('The status type (for example, "in queue", "in translate", etc).'),
    'filter' => array(
      'handler' => 'SmartlingHandlerFilterEntityStatus',
    ),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
  );

  $data['smartling_entity_data']['ranged_progress'] = array(
    'title' => t('Smartling ranged progress'),
    'help' => t('The percentage of Smartling entity translations divided into several ranges.'),
    'real field' => 'progress',
    'filter' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'field' => array(
      'handler' => 'SmartlingHandlerFieldEntityProgress',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Add views handler.
  $data['smartling_entity_data']['target_language'] = array(
    'title' => t('Target Language'),
    'help' => t('Smartling entity data "target_language" property.'),
  );
  $data['smartling_entity_data']['target_language']['field'] = array(
    'real field' => 'target_language',
    'handler' => 'SmartlingHandlerFieldTargetLanguage',
    'click sortable' => TRUE,
  );
  $data['smartling_entity_data']['target_language']['filter'] = array(
    'handler' => 'SmartlingHandlerFilterEntityLocale',
  );

  $data['smartling_entity_data']['smartling_title'] = array(
    'title' => t('Smartling entity title'),
    'help' => t('Smartling entity title link.'),
  );
  $data['smartling_entity_data']['smartling_title']['field'] = array(
    'real field' => 'eid',
    'handler' => 'SmartlingHandlerFieldSmartlingTitle',
    'click sortable' => TRUE,
  );
  $data['smartling_entity_data']['smartling_title']['sort'] = array(
    'handler' => 'views_handler_sort',
  );

  $data['smartling_entity_data']['smartling_operations'] = array(
    'title' => t('Smartling entity operations'),
    'help' => t('Smartling entity operations.'),
  );
  $data['smartling_entity_data']['smartling_operations']['field'] = array(
    'real field' => 'eid',
    'handler' => 'SmartlingHandlerFieldSmartlingOperations',
    'click sortable' => TRUE,
  );

  $data['node']['locale_info'] = array(
    'group' => t('Smartling'),
    'title' => t('Locales for node'),
    'help' => t('Smartling locales property.'),
  );
  $data['node']['locale_info']['field'] = array(
    'real field' => 'nid',
    'handler' => 'SmartlingHandlerFieldLocaleInfo',
    'click sortable' => TRUE,
  );

  $data['taxonomy_term_data']['locale_info'] = array(
    'group' => t('Smartling'),
    'title' => t('Locales for taxonomy_term_data'),
    'help' => t('Smartling locales property.'),
  );
  $data['taxonomy_term_data']['locale_info']['field'] = array(
    'real field' => 'tid',
    'handler' => 'SmartlingHandlerFieldLocaleInfo',
    'click sortable' => TRUE,
  );

  $data['comment']['locale_info'] = array(
    'group' => t('Smartling'),
    'title' => t('Locales for comment'),
    'help' => t('Smartling locales property.'),
  );
  $data['comment']['locale_info']['field'] = array(
    'real field' => 'cid',
    'handler' => 'SmartlingHandlerFieldLocaleInfo',
    'click sortable' => TRUE,
  );

  $data['users']['locale_info'] = array(
    'group' => t('Smartling'),
    'title' => t('Locales for user'),
    'help' => t('Smartling locales property.'),
  );
  $data['users']['locale_info']['field'] = array(
    'real field' => 'uid',
    'handler' => 'SmartlingHandlerFieldLocaleInfo',
    'click sortable' => TRUE,
  );

  $data['smartling_entity_data']['submitter']['relationship'] = array(
    'title' => t('Submitter'),
    'help' => t('The user submitting content for translation.'),
    'base' => 'users',
    'field' => 'submitter',
    'label' => t('Smartling submitter'),
  );

  $data['smartling_entity_data']['submission_date'] = array(
    'title' => t('Date'),
  );

  $data['smartling_entity_data']['submission_date']['field'] = array(
    'handler' => 'views_handler_field_date',
    'click sortable' => TRUE,
  );

  $data['smartling_entity_data']['submission_date']['sort'] = array(
    'handler' => 'views_handler_sort_date',
  );

  $data['smartling_entity_data']['submission_date']['filter'] = array(
    'handler' => 'views_handler_filter_date',
  );

  unset($data['smartling_entity_data']['submission_date']['argument']);
}
