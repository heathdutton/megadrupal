<?php

/**
 * Implements hook_entity_info_alter().
 *
 * We are adding the info about the entity bundles via a hook to avoid a
 * recursion issue as loading the model types requires the entity info as well.
 */
function _entityspice_entity_info_alter($etype, &$entity_info) {
  // @TODO make sure ${etypes}s is actually the path in $base_path.
  // @todo: Testing to ensure the schema exists; needed because running gui
  // install profile was hitting this BEFORE the schema was installed.
  if (drupal_get_schema($etype)) {
    $i = _entityspice_getinfo($etype);
    $base_path = $i['_entityspice_entity_info_alter']['path'];
    $real_path = $i['_entityspice_entity_info_alter']['real path base'];

    foreach (_entityspice_entity_get_bundles($etype) as $bundle => $info) {
      $entity_info[$etype]['bundles'][$bundle] = array(
        'label' => $info->label,
        'admin' => array(
          'path' => $base_path,
          'real path' => $real_path . '/' . "$bundle",
          'bundle argument' => $i['_entityspice_entity_info_alter']['bundle argument'],
          'access arguments' => array("administer $etype bundles"),
        ),
      );
    }
  }
}

/**
 * Title callback, returns title of entity.
 */
function _entityspice_entity_title($etype, $entity, $check_plain = TRUE) {
  if(!_entityspice_getinfo($etype, 'has title')) {
    throw new EntitySpiceOperationNotSupported('Entity type ' . $etype . ' does not have a title property.');
  }
  if($entity = _entityspice_get_entity($etype, $entity)) {
    $label = _entityspice_get_entity($etype, $entity)->label();
    return $check_plain ? check_plain($label) : $entity->label();
  }
}

/**
 * Implements hook_field_extra_field_info().
 *
 * Needs bundle support on entity type.
 */
function _entityspice_field_extra_field_info($etype, array $form = array(), array $display = array()) {
  $extra = array();

  if(_entityspice_getinfo($etype, 'has title')) {
    foreach (_entityspice_entity_get_bundles($etype) as $bundle => $info) {
      $extra[$etype][$bundle] = array(
        'form' => array(
          'title' => array(
              'label' => t('Title'),
              'description' => t('Entity title'),
              'weight' => 0,
            ) + $form,
        ),
        'display' => array(
            'title' => array(
              'label' => t('Title'),
              'weight' => 0,
            ),
          ) + $display,
      );
    }
  }

  return $extra;
}

/**
 * Helper function to load entity if it's ID is passed.
 *
 * @param string $etype
 *   machine name of entity type.
 * @param int/object $entity
 *   Entity object itself or id of it.
 *
 * @return entity
 *   Loaded entity.
 */
function _entityspice_get_entity($etype, $entity) {
  if(!is_object($entity)) {
    $entity = _entityspice_entity_load($etype, $entity);
  }

  return is_object($entity) ? $entity : NULL;
}

/**
 * Gets an array of all entity types, keyed by the name.
 *
 * @param string $name
 *   If set, the bundle with the given name is returned.
 */
function _entityspice_entity_get_bundles($etype, $name = NULL) {
  if(!_entityspice_getinfo($etype, 'has bundle')) {
    throw new EntitySpiceOperationNotSupported('Entity type ' . $etype . ' does not implement a bundle system.');
  }

  $bundle_name = _entityspice_getinfo($etype, 'bundle machine name');
  $bundles =
    entity_load_multiple_by_name($bundle_name, $name ? array($name) : FALSE);

  return $name ? reset($bundles) : $bundles;
}

/**
 * Generate a list of bundle names of a given entity type.
 *
 * Default arguments MUST be in a way so that calling the function without any
 * arguments generates an apropirate list of names for form API.
 *
 * @param string $etype
 *   Entity type.
 * @param string $with_label
 *   Return associative array instead, Which has bundle name as key and label
 *   as value. Will be good for form API.
 * @param string $check_plain
 *   Whether to pass bundle label through checkp_plain or not.
 * @return array bundle names.
 */
function _entityspice_entity_get_bundles_names($etype, $with_label = TRUE, $check_plain = TRUE) {
  if(!_entityspice_getinfo($etype, 'has bundle')) {
    throw new EntitySpiceOperationNotSupported('Entity type ' . $etype . ' does not implement a bundle system.');
  }

  $bundles = _entityspice_entity_get_bundles($etype);

  if ($with_label) {
    $ret = array();
    foreach ($bundles as $bundle => $info) {
      $ret[$bundle] = $check_plain ? check_plain($info->label) : $info->label;
    }
  }
  else {
    $ret = array_keys($bundles);
  }

  return $ret;
}

/**
 * Implements hook_entity_info().
 */
function _entityspice_entity_info($etype) {
  $info = _entityspice_getinfo($etype);

  $path         = $info['parent admin path'];
  $concat_camel = $info['concat camel'];
  $label        = $info['label'];
  $type_idkey   = $info['bundle idkey'];
  $bundle       = $info['bundle machine name'];
  $module       = $info['module'];

  $ret_info = array(
    $etype => array(
      'label' => $label,
      'controller class' => $concat_camel . 'EntityController',
      'metadata controller class' => $concat_camel . 'MetadataController',
      'entity class' => $concat_camel,
      'views controller class' => $concat_camel . 'ViewsController',
      'base table' => $info['base table'],
      'fieldable' => $info['is fieldable'],
      'entity keys' => array(
        'id' => $info['idkey'],
        'bundle' => $info['bundle key name'],
        'label' => 'title',
      ),
      'bundle keys' => array(
        'bundle' => 'name',
      ),
      'bundles' => array(),
      'view modes' => array(
        'full' => array(
          'label' => t('Full'),
          'custom settings' => FALSE,
        ),
      ),

      'uri callback' => 'entity_class_uri',
      'token type' => $etype,
      'module' => $module,
      'label callback' => 'entity_class_label',
      'access callback' => $info['access callback'],
    ),

    $bundle => array(
      'label' => t("$label Bundle"),
      'entity class' => $concat_camel . 'Bundle',
      'controller class' => $concat_camel . 'BundleController',
      'base table' => $info['bundle table'],
      'fieldable' => $info['is bundle fieldable'],
      'bundle of' => $etype,
      'exportable' => TRUE,
      'token type' => $bundle,
      'entity keys' => array(
        'id' => $type_idkey,
        'name' => 'name',
        'label' => 'label',
      ),
      'access callback' => $info['bundle access callback'],
      'module' => $module,

      'admin ui' => array(
        'path' => $path,
//         'file' => $file,
//         'file path' => $file_path,
        'controller class' => $concat_camel . 'BundleUIController',
      ),
    ),
  );

  if($info['has revision']) {
    $ret_info[$etype]['revision table'] = $info['revision table'];
    $ret_info[$etype]['entity keys']['revision'] = $info['ridkey'];
  }

  return $ret_info;
}
