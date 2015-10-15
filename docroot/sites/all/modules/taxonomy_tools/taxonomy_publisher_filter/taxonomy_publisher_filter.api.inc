<?php

/**
 * @file
 * API functions.
 */

define('TAXONOMY_PUBLISHER_FILTER_CACHE', variable_get('taxonomy_publisher_filter_cache', TRUE));
define('TAXONOMY_PUBLISHER_FILTER_CACHE_TIME', variable_get('taxonomy_publisher_filter_cache_time', 60 * 60 * 24));

/**
 * API function for custom forms.
 *
 * @param int $vid
 *   Taxonomy vocabulary ID
 * @param array $settings
 *   Settings to use cache and to filter or not by roles
 *
 * @return array
 *   Return filtered result
 */
function _taxonomy_publisher_filter_custom_form($vid, $settings = array()) {
  global $user;
  $options = array();
  $terms = taxonomy_get_tree($vid);
  if (count($terms)) {
    foreach ($terms as $key => $term) {
      if ($key == 0) {
        $vocab = taxonomy_vocabulary_load($term->vid);
      }
      $options[$term->tid] = $term->name;
    }
  }
  else {
    return $options;
  }
  if (isset($settings['by_role']) && $settings['by_role'] === TRUE) {
    $roles = array_values($user->roles);
    $settings['by_role'] = end($roles);
    if (!user_access('taxonomy_publisher_filter_custom_' . $vocab->machine_name)) {
      return $options;
    }
  }
  else {
    $settings['by_role'] = 'ALL';
  }
  if (isset($settings['cache'])) {
    $settings['vocabulary'] = $vocab->machine_name;
    _taxonomy_publisher_filter_term_list($options, TRUE, $settings);
  }
  else {
    _taxonomy_publisher_filter_term_list($options);
  }
  return $options;
}

/**
 * Check if result already cached , if not filter term by term.
 *
 * @param array $options
 *   array with widget options where tid = array key
 * @param bool $cache
 *   use(TRUE) / not (FALSE) cache options
 * @param array $settings
 *   if $cache = TRUE settings to create $cache_name
 */
function _taxonomy_publisher_filter_term_list(&$options, $cache = FALSE, $settings = array()) {
  $cached = '';

  if ($cache) {
    $cache_name = "tpf_widget";
    if (count($settings)) {
      foreach ($settings as $value) {
        $cache_name .= "_" . $value;
      }
    }
    if ($cached = _taxonomy_publisher_filter_get_cache($cache_name)) {
      $options = $cached;
      return;
    }
  }

  if (empty($cached)) {
    _taxonomy_publisher_filter_term_list_by_tid($options);
    if ($cache) {
      _taxonomy_publisher_filter_set_cache($cache_name, $options);
    }
  }
}

/**
 * Unset unpublished terms from field options.
 *
 * @param array $options
 *   array with widget options where tid = array key
 */
function _taxonomy_publisher_filter_term_list_by_tid(&$options) {
  if (count($options)) {
    foreach ($options as $tid => $value) {
      if (is_int($tid)) {
        $term = taxonomy_term_load($tid);
        if (_taxonomy_publisher_filter_check_term_is_unpublished($term)) {
          unset($options[$tid]);
        }
      }
    }
  }
}

/**
 * Check and return fields using Taxonomy Vocabulary.
 *
 * @param array $form_info
 *   array containing form information
 *
 * @return array
 *   return the fields from the form that are a taxonomy term reference
 */
function _taxonomy_publisher_filter_return_taxonomy_fields($form_info) {
  $field_names = array();
  foreach ($form_info as $key => $value) {
    if ((strpos($key, '_tid') !== FALSE) && (strpos($key, 'filter-') !== FALSE)) {
      preg_match('/^filter-(.*?)$/', $key, $matches);
      $field_names[$matches[1]] = $matches[1];
    }
  }
  return $field_names;
}

/**
 * Filter views exposed form field witch use taxonomy vocabulary.
 *
 * @param array $field_options
 *   form array with term options
 * @param string $field_name
 *   name of the field to filter
 * @param array $vids
 *   taxonomy Vocabulary list used by the Publisher module
 * @param array $settings
 *   settings to create an uniq cache name
 */
function _taxonomy_publisher_filter_filter_options(&$field_options, $field_name, $vocabularies, $settings) {

  $i = 0;
  foreach ($field_options as $tid => $value) {
    if (is_int($tid)) {
      $term_id = $tid;
      /* If is hierarchy dropdown */
      if (is_object($value)) {
        $array = $value->option;
        $term_id = key($array);
      }
      $term = taxonomy_term_load($term_id);
      if ($i == 0) {
        if (!in_array($term->vocabulary_machine_name, $vocabularies)) {
          return;
        }
        else {
          $tvl = taxonomy_vocabulary_load($term->vid);
          if (user_access("taxonomy_publisher_filter_views_$tvl->machine_name")) {
            if (TAXONOMY_PUBLISHER_FILTER_CACHE === TRUE) {
              $cache_name = "tpf_views";
              if (count($settings)) {
                foreach ($settings as $value) {
                  $cache_name .= "_" . $value;
                }
              }
              if ($cached = _taxonomy_publisher_filter_get_cache($cache_name)) {
                $field_options = $cached;
                return;
              }
            }
          }
          else {
            return;
          }
        }
      }
      if (_taxonomy_publisher_filter_check_term_is_unpublished($term)) {
        unset($field_options[$tid]);
      }
      $i++;
    }
  }
  if (empty($cached) && TAXONOMY_PUBLISHER_FILTER_CACHE === TRUE) {
    _taxonomy_publisher_filter_set_cache($cache_name, $field_options);
  }
}

/**
 * Check if term is unpublished.
 *
 * @param int $term
 *   Taxonomy Term Object
 *
 * @return bool
 *   Return true if term is unpublished
 */
function _taxonomy_publisher_filter_check_term_is_unpublished($term) {
  if (isset($term->field_taxonomy_term_status[LANGUAGE_NONE][0]['value']) && $term->field_taxonomy_term_status[LANGUAGE_NONE][0]['value'] == 0) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Sett cache.
 *
 * @param string $cid
 *   Cache id
 * @param array $value 
 *   Cache value to be sett
 */
function _taxonomy_publisher_filter_set_cache($cid, $value) {
  cache_set($cid, $value, 'cache', time() + TAXONOMY_PUBLISHER_FILTER_CACHE_TIME);
}

/**
 * Check for cache.
 *
 * @param string $cid
 *   Cache id
 *
 * @return bool
 *   Return cache
 */
function _taxonomy_publisher_filter_get_cache($cid) {
  $cache = cache_get($cid, 'cache');
  if (!empty($cache)) {
    if ($cache->expire > time()) {
      return $cache->data;
    }
  }
  return FALSE;
}
