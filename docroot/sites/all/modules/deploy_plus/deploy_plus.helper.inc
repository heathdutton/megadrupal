<?php
/**
 * @file
 * Deploy Plus helper function.
 */

/**
 * Get entity dependencies.
 *
 * @param object $entity
 *   Entity object.
 * @param string $type
 *   Entity type.
 *
 * @return array
 *   Array of entityes links.
 */
function deploy_plus_get_entity_dependencies($entity, $type) {
  $dependencies = array();

  if ($type == 'taxonomy_term' || $type == 'file') {
    return $dependencies;
  }

  // Node from link_path.
  if ($type == 'menu_link') {
    $path = $entity->link_path == '<front>' ? drupal_lookup_path('source', 'home') : $entity->link_path;
    $node = menu_get_object('node', 1, $path);
    if ($node) {
      $title = deploy_plus_build_entity_title('node', $node);
      $label = deploy_plus_entity_type_label('node', $node);
      $dependencies[$label][] = $title;
    }
    return $dependencies;
  }

  // Entity references.
  $info = field_info_instances($type, $entity->type);
  $wrapped = entity_metadata_wrapper($type, $entity);
  foreach ($info as $field_name => $value) {
    // Related entity type.
    $rtype = $wrapped->{$field_name}->type();
    $info = field_info_field($field_name);
    if ($info['type'] == 'entityreference') {
      $rtype = $info['settings']['target_type'];
      $related = $wrapped->{$field_name}->value();
      if ($related) {
        $related = is_object($related) ? array($related) : $related;
        if (is_array($related)) {
          foreach ($related as $rel) {
            if ($rel) {
              $title = deploy_plus_build_entity_title($rtype, $rel);
              $label = deploy_plus_entity_type_label($rtype, $rel);
              $dependencies[$label][] = $title;
            }
          }
        }
      }
    }
    // File.
    elseif ($info['type'] == 'file') {
      $rtype = $info['type'];
      $files = $wrapped->{$field_name}->value();
      if (is_array($files)) {
        // Wrap file in array if only one present.
        if (isset($files['fid'])) {
          $files = array($files);
        }
        foreach ($files as $file) {
          $f = file_load($file['fid']);
          $title = deploy_plus_build_entity_title($rtype, $f);
          $label = deploy_plus_entity_type_label($rtype, $f);
          $dependencies[$label][] = $title;
        }
      }
    }
    // Field Collection.
    elseif ($info['type'] == 'field_collection') {
      $rtype = 'field_collection_item';
      $fcs = $wrapped->{$field_name}->value();
      foreach ($fcs as $fc) {
        if ($fc) {
          $title = deploy_plus_build_entity_title($rtype, $fc);
          $label = deploy_plus_entity_type_label($rtype, $fc);
          $dependencies[$label][] = $title;
        }
      }
    }
  }

  // Translation.
  if ($entity->tnid) {
    $translation = translation_node_get_translations($entity->tnid);
    foreach ($translation as $value) {
      if ($value->nid != $entity->nid) {
        // Translated entity.
        $tentity = entity_load_single($type, $value->nid);
        $title = deploy_plus_build_entity_title($type, $tentity);
        $label = t('Translation');
        $dependencies[$label][] = "{$tentity->language} $title";
      }
    }
  }

  // Author.
  if ($wrapped->author) {
    // Skip global admin.
    if ($wrapped->author->uid->value() != 1) {
      $rtype = 'user';
      $user = user_load($wrapped->author->uid->value());
      $label = deploy_plus_entity_type_label($rtype, $user);
      $title = deploy_plus_build_entity_title($rtype, $user);
      $dependencies[$label][] = $title;
    }
  }

  return $dependencies;
}

/**
 * Helper function to build title.
 *
 * @param string $type
 *   Entity type.
 * @param object $entity
 *   Entity object.
 *
 * @return string
 *   URL strign.
 */
function deploy_plus_build_entity_title($type, $entity) {
  $title = entity_label($type, $entity);
  $items = array();
  $ids = array(
    'nid',
    'tid',
    'mlid',
    'fid',
    'item_id',
    'field_name',
    'uid',
    'uuid',
    'link_path',
    'menu_name',
    'vocabulary_machine_name',
  );
  foreach ($ids as $key) {
    $value = FALSE;
    if (is_object($entity) && isset($entity->{$key})) {
      $value = $entity->{$key};
    }
    elseif (is_array($entity) && isset($entity[$key])) {
      $value = $entity[$key];
    }
    if ($value) {
      switch ($key) {
        case 'menu_name':
          $menus = menu_get_menus(FALSE);
          $value = $menus[$value];
          break;

        case 'vocabulary_machine_name':
          $vocabulary = taxonomy_vocabulary_machine_name_load($value);
          $value = $vocabulary->name;
          $key = 'vocabulary_name';
          break;

        default:
          break;
      }
      $items[] = t('@key: !value', array('@key' => $key, '!value' => $value));
    }
  }

  if ($type == 'file') {
    $url = is_array($entity) ? $entity['uri'] : $entity->uri;
    $title = l($title, file_create_url($url), array(
      'attributes' => array('target' => '_blank'),
    ));
  }
  else {
    $uri = entity_uri($type, $entity);
    if ($uri) {
      $uri['options']['attributes']['target'] = '_blank';
      $title = l($title, $uri['path'], $uri['options']);
    }
  }
  return $title . theme('item_list', array('items' => $items));
}

/**
 * Get entity type label.
 *
 * @param string $type
 *   Entity type.
 * @param object $entity
 *   Entity object.
 *
 * @return string
 *   Entity type human readable name.
 */
function deploy_plus_entity_type_label($type, $entity) {
  $label = '';
  if ($type == 'node') {
    $wrapped = entity_metadata_wrapper($type, $entity);
    $label = $wrapped->type->label();
  }
  else {
    $info = entity_get_info($type);
    $label = $info['label'];
  }
  return $label;
}
