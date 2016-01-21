<?php

/**
 * @file
 * Utils functions for Smartling module.
 */

/**
 * Checks entity. If entity have translatable field return TRUE.
 *
 * @param string $entity_type
 *   Entity type.
 * @param string $bundle
 *   Entity bundle.
 *
 * @return bool
 *   Return TRUE if entity has translatable fields.
 */
function smartling_entity_has_translatable_field($entity_type, $bundle) {
  foreach (field_info_instances($entity_type, $bundle) as $instance) {
    if (smartling_field_is_translatable($instance['field_name'], $entity_type)) {
      return TRUE;
    }
  }
  return FALSE;
}

function smartling_nodes_method($bundle) {
  return drupal_container()
    ->get('smartling.wrappers.smartling_utils')
    ->isNodesMethod($bundle);
}

function smartling_fields_method($bundle) {
  return drupal_container()
    ->get('smartling.wrappers.smartling_utils')
    ->isFieldsMethod($bundle);
}

/**
 * Returns node status for nodes method by node id.
 *
 * @param object $node
 *   Node object.
 *
 * @return string
 *   Return unical node status for nodes method translate.
 */
function smartling_nodes_method_node_status($node) {
  if (isset($node->tnid) && $node->tnid != '0' && ($node->tnid != $node->nid)) {
    return SMARTLING_TRANSLATE_NODE;
  }
  elseif (isset($node->tnid) && $node->tnid != '0' && ($node->tnid == $node->nid)) {
    return SMARTLING_ORIGINAL_NODE;
  }
  elseif (isset($node->tnid) && $node->tnid == '0') {
    return SMARTLING_ORIGINAL_WITHOUT_TRANSLATION_NODE;
  }
}

function smartling_is_configured() {
  return drupal_container()
    ->get('smartling.wrappers.smartling_utils')
    ->isConfigured();
}

/**
 * Checks settings for node content type.
 *
 * @param string $bundle
 *   Bundle.
 * @param string $entity_type
 *   Entity type.
 *
 * @return bool
 *   Return TRUE if fields for translation are set.
 */
function smartling_translate_fields_configured($bundle, $entity_type) {
  $container = drupal_container();
  if (empty($container)) {
    return FALSE;
  }
  $val = $container->get('smartling.settings')
    ->getFieldsSettingsByBundle($entity_type, $bundle);
  return !empty($val);
}


function smartling_field_is_translatable($field_name, $entity_type) {
  return drupal_container()
    ->get('smartling.wrappers.smartling_utils')
    ->fieldIsTranslatable($field_name, $entity_type);
}

/**
 * Get full language name.
 *
 * @param string $d_locale
 *   Locale in drupal format (ru, en).
 *
 * @return string
 *   Return full language name or empty string.
 */
function smartling_get_full_language_name($d_locale) {
  $languages = language_list();
  return (isset($languages[$d_locale])) ? $languages[$d_locale]->name : '';
}

/**
 * This functions convert locale format. Example: 'en' => 'en-US'.
 *
 * @param string $locale
 *   Locale string in Drupal format: 'en'.
 *
 * @return string
 *   Return locale or empty string.
 */
function smartling_convert_locale_drupal_to_smartling($locale) {
  $locales = smartling_settings_get_handler()->getLocalesConvertArray();
  return (isset($locales[$locale])) ? $locales[$locale] : '';
}

/**
 * This functions convert locale format. Example: 'en-US' => 'en'.
 *
 * @param string $locale
 *   Locale string in Smartling format: 'en-US'.
 *
 * @return string
 *   Return locale or empty string.
 */
function smartling_convert_locale_smartling_to_drupal($locale) {
  $locales = smartling_settings_get_handler()->getLocalesConvertArray();
  return (string) array_search($locale, $locales);
}

function smartling_get_dir($file_name = '') {
  return drupal_container()->get('smartling.settings')->getDir($file_name);
}

function smartling_clean_filename($filename, $allow_dirs = FALSE) {
  return drupal_container()
    ->get('smartling.wrappers.smartling_utils')
    ->cleanFileName($filename, $allow_dirs);
}


function smartling_get_entity_processor($smartling_entity) {
  return drupal_container()
    ->get('smartling.entity_processor_factory')
    ->getProcessor($smartling_entity);
}

/**
 * Smartling language options list.
 * It's prepared for the list/checkboxes-item on the form.
 *
 * @return array
 *   Return language options list or empty array.
 */
function smartling_language_options_list() {
  $languages = smartling_language_list();

  foreach ($languages as $lang => $language) {
    $languages[$lang] = check_plain($language->name);
  }
  return $languages;
}

/**
 * Smartling language list.
 *
 * @return array
 *   Return language list or empty array.
 */
function smartling_language_list() {
  $languages_list = array();
  $languages = language_list('language');

  $smartling_target_locales = variable_get('smartling_target_locales', array());

  foreach ($languages as $langcode => $language) {
    if (/* $language->enabled != '0' && */
    (in_array($langcode, $smartling_target_locales))
    ) {
      $languages_list[$langcode] = $language;
    }
  }
  return $languages_list;
}

/**
 * Get link to entity.
 *
 * @param string $entity_type
 *   Entity type.
 * @param object $entity
 *   Smartling data object
 * @param string $link_title
 *
 * @todo Looks like we have similar function in GenericEntityProcessor.linkToContent()
 *
 * @return string
 *   Return link to entity.
 */
function smartling_get_link_to_entity($entity_type, $entity, $link_title = '') {
  $wrapper = entity_metadata_wrapper($entity_type, $entity);

  $link_title = (!empty($link_title)) ? $link_title : $wrapper->label();
  $link_title = (!empty($link_title)) ? $link_title : t('Related entity');

  $uri = entity_uri($entity_type, $entity);
  $path = $uri['path'];

  return l($link_title, $path);
}

/**
 * Get original entity.
 *
 * @param object $term
 *
 * @deprecated
 *
 * @return object
 *   Return entity object or NULL.
 */
function smartling_get_original_taxonomy_term($term) {
  $entity = NULL;
  if ($term->language == language_default()->language) {
    $entity = $term;
  }
  elseif (isset($term->i18n_tsid) && is_numeric($term->i18n_tsid)) {
    $entity = i18n_taxonomy_term_get_translation($term, language_default()->language);
  }

  return $entity;
}

function smartling_get_original_node($node) {
  if (smartling_nodes_method($node->type)) {
    if (isset($node->tnid) && $node->tnid != '0' && ($node->tnid != $node->nid)) {
      return node_load($node->tnid);
    }
  }
  return $node;
}

/**
 * Get settings handler.
 *
 * @return object
 *   Return handler object.
 */
function smartling_settings_get_handler() {
  return drupal_container()->get('smartling.settings');
}

/**
 * Get log handler.
 *
 * @return Drupal\smartling\Log\SmartlingLog
 *   Return handler object.
 */
function smartling_log_get_handler() {
  return drupal_container()->get('smartling.log');
}

/*
 * Creates an HTML-ready translation status of an entity.
 *
 * @param string $entity_name
 * @param int $status
 * @param string $language_name
 * @param int $progress
 *
 * @return string
 */
function smartling_entity_status_message($entity_name, $status, $language_name, $progress) {
  $res = '';
  $entity_name = check_plain($entity_name);
  $language_name = check_plain($language_name);
  $progress = intval($progress);
  switch ($status) {
    case SMARTLING_STATUS_IN_QUEUE:
      $res = '<span title="' . $entity_name . ' ' . t('is awaiting translation') . '"><span class="lang-name">' . $language_name . '</span><span class="lang-status in-queue">' . $progress . '%</span></span>';
      break;

    case SMARTLING_STATUS_IN_TRANSLATE:
      $res = '<span title="' . t('Translation in progress') . '"><span class="lang-name">' . $language_name . '</span><span class="lang-status in-translate">' . $progress . '%</span></span>';
      break;

    case SMARTLING_STATUS_TRANSLATED:
      $res = '<span title="' . $entity_name . ' ' . t('is translated') . '"><span class="lang-name">' . $language_name . '</span><span class="lang-status translated">' . $progress . '%</span></span>';
      break;

    case SMARTLING_STATUS_CHANGE:
      $res = '<span title="' . $entity_name . ' ' . t('was changed after submitting for translation') . '"><span class="lang-name">' . $language_name . '</span><span class="lang-status status-changed">' . $progress . '%</span></span>';
      break;

    case SMARTLING_STATUS_FAILED:
      $res = '<span title="' . t('Request failed') . '"><span class="lang-name">' . $language_name . '</span><span class="lang-status status-failed">' . $progress . '%</span></span>';
      break;
  }

  return $res;
}


/**
 * Create Smartling entity.
 *
 * @param stdClass $drupal_entity
 *   Drupal content entity.
 * @param string $entity_type
 *   Drupal entity type machine name.
 * @param string $origin_language
 *   Language key of source drupal content entity.
 * @param string $target_language
 *   Target language key for smartling entity.
 * @param array $default_options
 *   Optional params that will override the ones in a function.
 *   This feature is use in interface_translation module to set a bundle for example.
 *
 * @return SmartlingEntityData
 *
 * @todo try to avoid original language in arguments.
 */
function smartling_create_from_entity($drupal_entity, $entity_type, $origin_language, $target_language, $default_options = array()) {
  return drupal_container()
    ->get('smartling.wrappers.smartling_submission_wrapper')
    ->createFromDrupalEntity($drupal_entity, $entity_type, $origin_language, $target_language, $default_options)
    ->getEntity();
}


/**
 * Smartling cancel submission.
 *
 * @param int $limit
 *  The maximum items to cancel.
 */
function smartling_cancel_submission($limit = 5) {
  drupal_container()->get('smartling.utils.cancel_submission')->cancel($limit);
}

/**
 * Add items to "smartling_check_status" queue.
 */
function smartling_add_to_check_status() {
//  $settings = smartling_settings_get_handler();
//  $cron_last = $settings->getCheckStatusCronLast();
//  if ( $cron_last + SMARTLING_CRON_RUN_INTERVAL > time()) {
//    return;
//  }
//  $settings->setCheckStatusCronLast(time());

  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'smartling_entity_data')
    ->propertyCondition('status', array(SMARTLING_STATUS_IN_TRANSLATE, SMARTLING_STATUS_TRANSLATED), 'IN')
    // @TODO move 'original' to constant.
    ->propertyCondition('target_language', 'original', '<>')
    ->execute();

  if (!empty($entities['smartling_entity_data'])) {
    $eids = array_keys($entities['smartling_entity_data']);
    $submissions = smartling_entity_data_load_multiple($eids);
    $file_names = array();
    foreach ($submissions as $subm) {
      $file_names[] = $subm->file_name;
    }
    $file_names = array_unique($file_names);
    //drupal_container()->get('smartling.queue_managers.check_status')->add($eids);

    $result = db_select('queue', 'q')
      ->fields('q', array('data'))
      ->condition('name', 'smartling_bulk_check_status', '=')
      ->execute()
      ->fetchAll();

    $in_queue = array();
    foreach ($result as $queue_item) {
      //@todo: add support for arrays of ints in queue, when
      $in_queue[] = (string) unserialize($queue_item->data);
    }
    $in_queue = array_unique($in_queue);

    $file_names = array_diff($file_names, $in_queue);
    if (!empty($file_names)) {
      foreach ($file_names as $file_name) {
        drupal_container()
          ->get('smartling.queue_managers.bulk_check_status')
          ->add($file_name);
      }
    }
  }
}