<?php

/**
 * @file
 * A module to export private vocabularies and terms using Features.
 *
 * Vocabularies can be exported as private and private terms can be exported
 * preserving their connection to the user.
 */

/**
 * Implements hook_features_export_options().
 */
function private_vocabulary_features_export_options() {
  $vocabularies = array();
  foreach (private_taxonomy_get_private_vocabularies() as $vocabulary) {
    $vocabularies[$vocabulary->machine_name] = $vocabulary->name;
  }
  asort($vocabularies);
  return $vocabularies;
}

/**
 * Implements hook_features_export().
 */
function private_vocabulary_features_export($data, &$export,
  $module_name = '') {

  $pipe = array();

  $export['dependencies']['private_taxonomy'] = 'private_taxonomy';

  // Add dependencies for each vocabulary.
  $map = features_get_default_map('taxonomy');
  foreach ($data as $machine_name) {
    if (isset($map[$machine_name]) && $map[$machine_name] != $module_name) {
      $export['dependencies'][$map[$machine_name]] = $map[$machine_name];
    }
    else {
      $export['features']['private_vocabulary'][$machine_name] = $machine_name;

      $fields = field_info_instances('taxonomy_term', $machine_name);
      foreach ($fields as $name => $field) {
        $pipe['field'][]
          = "taxonomy_term-{$field['bundle']}-{$field['field_name']}";
      }
    }
  }
  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function private_vocabulary_features_export_render($module, $data) {
  $vocabularies = private_taxonomy_get_private_vocabularies();
  $code = array();
  foreach ($data as $machine_name) {
    foreach ($vocabularies as $vocabulary) {
      if ($vocabulary->machine_name == $machine_name) {
        // We don't want to break the entity cache, so we need to clone the
        // vocabulary before unsetting the id.
        $vocabulary = clone $vocabulary;
        unset($vocabulary->vid);
        $code[$machine_name] = $vocabulary;
      }
    }
  }
  $code = "  return " . features_var_export($code, '  ') . ";";
  return array('private_vocabulary_default_vocabularies' => $code);
}

/**
 * Implements hook_features_revert().
 */
function private_vocabulary_features_revert($module) {
  private_vocabulary_features_rebuild($module);
}

/**
 * Implements hook_features_enable_feature().
 */
function private_vocabulary_features_enable_feature($module) {
  private_vocabulary_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function private_vocabulary_features_rebuild($module) {
  if ($vocabularies = features_get_default('private_vocabulary', $module)) {
    $existing = taxonomy_get_vocabularies();
    foreach ($vocabularies as $vocabulary) {
      $vocabulary = (object) $vocabulary;
      foreach ($existing as $existing_vocab) {
        if ($existing_vocab->machine_name === $vocabulary->machine_name) {
          $vocabulary->vid = $existing_vocab->vid;
        }
      }
      // Need to mark the vocabulary as private.
      $vocabulary->private = TRUE;
      taxonomy_vocabulary_save($vocabulary);
    }
  }
}

/**
 * Implements hook_features_export_options().
 */
function private_term_features_export_options() {
  $options = array();
  foreach (private_taxonomy_get_private_vocabularies() as $vocabulary) {
    $sql = db_select('taxonomy_term_data', 'ttd');
    $sql->join('taxonomy_vocabulary', 'tv', 'tv.vid = ttd.vid');
    $sql->join('user_term', 'ut', 'ut.tid = ttd.tid');
    $sql->join('users', 'u', 'ut.uid = u.uid');
    $sql->addField('u', 'name', 'user');
    $terms = $sql->condition('tv.vid', $vocabulary->vid)
      ->fields('ttd', array('uuid', 'name'))
      ->orderBy('ttd.name', 'ASC')
      ->execute();
    foreach ($terms as $term) {
      $options[$term->uuid] = $vocabulary->name . ' - ' . $term->name .
        ' (' . $term->user . ')';
    }
  }
  natcasesort($options);

  return $options;
}

/**
 * Implements hook_features_export().
 */
function private_term_features_export($data, &$export, $module_name = '') {

  $pipe = array();
  $export['dependencies']['private_taxonomy'] = 'private_taxonomy';
  $export['dependencies']['uuid'] = 'uuid';

  $tids = array();
  foreach ($data as $uuid) {
    $tid = private_taxonomy_get_term_tid($uuid);
    $tids[$tid] = $tid;
  }
  $terms = taxonomy_term_load_multiple($tids);
  private_taxonomy_add_terms($export, $terms, $pipe);

  return $pipe;
}

/**
 * Recursive function to add terms to the export.
 */
function private_taxonomy_add_terms(&$export, $terms, &$pipe) {
  foreach ($terms as $term) {
    if (isset($term->tid) && private_taxonomy_is_term_private($term->tid)) {
      $export['features']['private_term'][$term->uuid] = $term->uuid;
      $machine_name = $term->vocabulary_machine_name;
      if (!isset($export['features']['private_vocabulary'][$machine_name])) {
        $export['features']['private_vocabulary'][$machine_name]
          = $machine_name;
        $fields = field_info_instances('taxonomy_term', $machine_name);
        foreach ($fields as $name => $field) {
          $pipe['field'][]
            = "taxonomy_term-{$field['bundle']}-{$field['field_name']}";
        }
      }
      // Recursively add all parents and the references of the parents.
      $parents = taxonomy_get_parents($term->tid);
      private_taxonomy_add_terms($export, $parents, $pipe);
      // Get term references.
      $instances = field_info_instances('taxonomy_term', $machine_name);
      foreach ($instances as $field_name => $instance) {
        $field = field_info_field($field_name);
        if (array_key_exists('taxonomy_term', $field['bundles'])) {
          if (isset($term->$field_name)) {
            foreach ($term->$field_name as $lang => $values) {
              foreach ($values as $value) {
                $tid = $value['tid'];
                $uuid = private_taxonomy_get_term_uuid($tid);
                if (!in_array($uuid, $export['features']['private_term'])) {
                  if (private_taxonomy_is_term_private($tid)) {
                    $export['features']['private_term'][$uuid] = $uuid;
                    private_taxonomy_add_terms($export, array($tid), $pipe);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

/**
 * Implements hook_features_export_render().
 */
function private_term_features_export_render($module, $data) {
  $code = array();
  $tids = array();
  foreach ($data as $uuid) {
    $tid = private_taxonomy_get_term_tid($uuid);
    if ($tid) {
      $tids[$tid] = $tid;
    }
  }
  $terms = taxonomy_term_load_multiple($tids);
  foreach ($terms as $term) {
    // We don't want to break the entity cache, so we need to clone the
    // vocabulary before unsetting the id.
    $item = clone $term;
    $machine_name = $term->vocabulary_machine_name;
    $instances = field_info_instances('taxonomy_term', $machine_name);
    foreach ($instances as $field_name => $instance) {
      $field = field_info_field($field_name);
      if (array_key_exists('taxonomy_term', $field['bundles'])) {
        if (property_exists($item, $field_name) &&
          count($item->{$field_name})) {

          foreach ($item->{$field_name} as $language => $values) {
            foreach ($values as $key => $value) {
              $tid = $item->{$field_name}[$language][$key]['tid'];
              $uuid = private_taxonomy_get_term_uuid($tid);
              $item->{$field_name}[$language][$key]['uuid'] = $uuid;
              unset($item->{$field_name}[$language][$key]['tid']);
            }
          }
        }
      }
    }
    unset($item->tid);
    unset($item->vid);
    $uid = private_taxonomy_term_get_user($term->tid);
    $account = user_load($uid);
    $item->uid = $account->uuid;
    $parents = taxonomy_get_parents($term->tid);
    if (count($parents) > 0) {
      $item->parent = array();
      foreach ($parents as $tid => $value) {
        $item->parent[] = private_taxonomy_get_term_uuid($tid);
      }
    }
    $code[$term->uuid] = $item;
  }
  $code = "  return " . features_var_export($code, '  ') . ";";
  return array('private_term_default_terms' => $code);
}

/**
 * Implements hook_features_revert().
 */
function private_term_features_revert($module) {
  private_term_features_rebuild($module);
}

/**
 * Implements hook_features_enable_feature().
 */
function private_term_features_enable_feature($module) {
  private_term_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function private_term_features_rebuild($module) {
  // Make sure that the vocabularies have been created.
  private_vocabulary_features_rebuild($module);
  if ($terms = features_get_default('private_term', $module)) {
    // Create terms so tids exist.
    foreach ($terms as $term) {
      $machine_name = $term['vocabulary_machine_name'];
      $instances = field_info_instances('taxonomy_term', $machine_name);
      foreach ($instances as $field_name => $instance) {
        $field = field_info_field($field_name);
        if (array_key_exists('taxonomy_term', $field['bundles'])) {
          if (isset($term[$field_name]) && count($term[$field_name])) {
            unset($term[$field_name]);
          }
        }
      }
      $tid = db_select('taxonomy_term_data', 'ttd')
        ->condition('ttd.uuid', $term['uuid'])
        ->fields('ttd', array('tid'))
        ->execute()
        ->fetchField();
      $vid = db_select('taxonomy_vocabulary', 'tv')
        ->condition('tv.machine_name', $term['vocabulary_machine_name'])
        ->fields('tv', array('vid'))
        ->execute()
        ->fetchField();
      $accounts = entity_uuid_load('user', array($term['uid']));
      foreach ($accounts as $account) {
        if ($account) {
          $term['uid'] = $account->uid;
        }
        else {
          unset($term['uid']);
        }
        break;
      }
      if ($vid) {
        if ($tid) {
          $term['tid'] = $tid;
        }
        $term['vid'] = $vid;
        if (isset($term['parent'])) {
          $parent = array();
          foreach ($term['parent'] as $uuid) {
            $tid = db_select('taxonomy_term_data', 'ttd')
              ->condition('ttd.uuid', $uuid)
              ->fields('ttd', array('tid'))
              ->execute()
              ->fetchField();
            if ($tid) {
              $parent[] = $tid;
            }
          }
          $term['parent'] = $parent;
        }
        else {
          if (isset($term['tid'])) {
            db_delete('taxonomy_term_hierarchy')
              ->condition('tid', $term['tid'])
              ->execute();
            db_insert('taxonomy_term_hierarchy')
              ->fields(array(
                'tid' => $term['tid'],
                'parent' => 0,
              ))
              ->execute();
          }
        }
        $item = (object) $term;
        taxonomy_term_save($item);
      }
    }
    // Add references to tids.
    foreach ($terms as $term) {
      $tid = private_taxonomy_get_term_tid($term['uuid']);
      $item = taxonomy_term_load($tid);
      $machine_name = $term['vocabulary_machine_name'];
      $instances = field_info_instances('taxonomy_term', $machine_name);
      foreach ($instances as $field_name => $instance) {
        $field = field_info_field($field_name);
        if (array_key_exists('taxonomy_term', $field['bundles'])) {
          if (isset($term[$field_name]) && count($term[$field_name])) {
            foreach ($term[$field_name] as $language => $values) {
              foreach ($values as $key => $value) {
                $uuid = $term[$field_name][$language][$key]['uuid'];
                $term[$field_name][$language][$key]['tid']
                  = private_taxonomy_get_term_tid($uuid);
                unset($term[$field_name][$language][$key]['uuid']);
              }
            }
            $item->{$field_name} = $term[$field_name];
          }
        }
      }
      taxonomy_term_save($item);
    }
  }
}

/**
 * Retrieve the term ID using the uuid.
 *
 * @param string $uuid
 *   Universally unique ID.
 *
 * @return int
 *   Term ID.
 */
function private_taxonomy_get_term_tid($uuid) {
  $tid = db_select('taxonomy_term_data', 'ttd')
    ->condition('ttd.uuid', $uuid)
    ->fields('ttd', array('tid'))
    ->execute()
    ->fetchField();
  return $tid;
}

/**
 * Retrieve the uuid using the term ID.
 *
 * @param int $tid
 *   Term ID.
 *
 * @return int
 *   Universally unique ID.
 */
function private_taxonomy_get_term_uuid($tid) {
  $uuid = db_select('taxonomy_term_data', 'ttd')
    ->condition('ttd.tid', $tid)
    ->fields('ttd', array('uuid'))
    ->execute()
    ->fetchField();
  return $uuid;
}
