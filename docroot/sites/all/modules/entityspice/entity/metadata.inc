<?php

/**
 * Get hook_entityspcie_info of an specific entity type.
 *
 * @param string @key
 *   If provided, return $key from info hook array otherwise return the array
 *   Itself.
 */
function _entityspice_getinfo($etype, $key = NULL) {
  $info = _entityspice_getinfo_all();
  if(!$etype) {
    return $info;
  }
  if(!isset($info[$etype])) {
    // @FIXME
    // When calling hook_schema in module.install, module's
    // hook_entityspcie_info does not exist yet.
    $info[$etype] = _entityspice_info_defaults($etype);
  }
  return $key ? $info[$etype][$key] : $info[$etype];
}

/**
 * Same as _entityspice_getinfo() but overrides provided values in $overrides.
 *
 * Only first level array is supported.
 *
 * @TODO replace with simpleter array_merge or something.
 */
function _entityspice_getinfo_override($etype, array $overrides, $key=NULL) {
  if($key) {
    return isset($overrides['key']) ?
      $overrides['key'] : _entityspice_getinfo($etype, $key);
  }

  $info = _entityspice_getinfo($etype);
  foreach ($overrides as $name => $value) {
    if(is_array($value)) {
      foreach($value as $k => $v) {
        $info[$name][$k] = $v;
      }
    }
    else {
      $info[$name] = $value;
    }
  }

  return $info ;
}

/**
 * Get information of all modules implementing hook_entityspcie_info().
 */
function _entityspice_getinfo_all($reset = FALSE) {
  $drupal_static_fast = &drupal_static(__FUNCTION__, array('info' => array()));
  $info = &$drupal_static_fast['info'];

  if(empty($info) || $reset) {
    $info = _entityspice_gather_hook_info('info', 'entityspice', FALSE, '');
    $final = array();
    foreach ($info as $etype => $data) {
      $final[$etype] = $data + _entityspice_info_defaults($etype);
    }
    $info = $final;
  }

  return $info;
}

function _entityspice_get_entity_name($etype, $entity, array $overrides = array()) {
  $info = _entityspice_getinfo_override($etype, $overrides);
  if(isset($overrides['_name'])) {
    return $overrides['_name'];
  }

  if($info['has title']) {
    return $entity->{$info['title key']};
  }

  return $entity->getID();
}

/**
 * Fill in default values for hook_entityspcie_info().
 *
 * If implementing module has not provided any peace of information a default
 * value if provided here. This makes isset() all over the code unnecessary.
 */
function _entityspice_info_defaults($etype) {
  $d = array();

  $d['module'] = $etype;

  // Names.
  $d['machine name'] = $etype;
  $d['concat camel'] =
    str_replace(' ', '', ucwords(str_replace('_', ' ', $etype)));
  $d['label'] =
    ucwords(str_replace('_', ' ', $etype));

  // Properties and Bundles.
  $d['bundle machine name'] = $etype . '_bundle';
  $d['bundle idkey'] = 'bundle_id';
  $d['bundle key name'] = 'bundle';
  $d['idkey'] = 'eid';
  $d['ridkey'] = 'reid';
  $d['title key'] = 'title';
  $d['title label'] = 'Title';

  // Database tables.
  $d['base table'] = $etype;
  $d['revision table'] = $etype . '_revision';
  $d['bundle table'] = $etype . '_bundle';

  // Forms.
  $d['add form'] = $etype . '_form';
  $d['edit form'] = $etype . '_form';
  $d['delete form'] = $etype . '_delete_form';
  $d['forms file'] = array(
    // Can be '' but must be set. Final slash is needed.
    'directory' => 'inc/',
    'file' => "${etype}.forms",
    'suffix' => 'inc',
  );

  // Something.
  $d['default uri base'] = 'es/' . $etype;

  // Is/Has.
  $d['has title']      = TRUE;
  $d['has uid']        = TRUE;
  $d['has updated']    = TRUE;
  $d['has created']    = TRUE;
  $d['has title']      = TRUE;
  $d['has revision']   = TRUE;
  $d['has bundle']     = TRUE;
  $d['has forms file'] = TRUE;

  $d['is fieldable']        = TRUE;
  $d['is bundle fieldable'] = FALSE;

  // Files.
  $d['bundle admin file'] = $d['bundle machine name'] . '.admin.inc';

  // Functions, Hooks and Callbacks.
  $d['access callback'] = $etype . '_access';
  $d['bundle access callback'] = $etype . '_bundle_access';
  $d['bundle form submit delete'] = '_entityspice_entity_bundle_form_submit_delete';
  $d['bundle submit delete'] =  '_entityspice_entity_bundle_form_submit_delete';
  $d['bundle exists callback'] =  $etype . '_get_bundles';
  $d['entity form submit callback'] = $etype . '_form_submit';
  $d['entity delete form submit callback'] = $etype . '_delete_form_submit';
  $d['entity form validation callback'] = $etype . '_form_validate';


  // Paths.
  $d['parent admin path'] = 'admin/structure/' . $etype;
  $d['parent userland path'] = 'es/' . $etype;
  // Redirected to these pages after adding/removing bundles.
  $d['bundle submit redirect'] = $d['parent admin path'];
  $d['bundle delete redirect'] = $d['parent admin path'];
  $d['entity delete redirect'] = '/';

  // Various stuff needed by this function (info alter):
  $d['_entityspice_entity_info_alter'] = array(
    'bundle argument' => 4,
    'parent' => 'admin/structure/' . $etype,
    'name' =>  $etype . '_' . $d['bundle key name'],
    'path' => 'admin/structure/' . $etype . '/manage/%' . $etype . '_' . $d['bundle key name'],
    'real path base' => 'admin/structure/' . $etype . '/manage',
  );

  // This variable needed in hook_menu so that ES will know what will be the
  // position of arg in each path. It is determined by how many words between
  // slashes (es/mytype/% : 2) appear before the arg.
  $d['parent user path arg no'] =
    count(explode('/', $d['parent userland path']));

  $d['has inine entity form support'] = TRUE;
  $d['inline inline entity form'] = array(
    'controller' => $d['concat camel'] . 'InlineEntityFormController',
  );
  return $d;
}

