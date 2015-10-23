<?php


require_once 'forms/entity.inc';
require_once 'forms/admin.inc';
require_once 'forms/bundle.inc';

require_once 'access.inc';
require_once 'ClassesBundle.inc';
require_once 'ClassesEntity.inc';
require_once 'CRUD.inc';
require_once 'CRUDBundle.inc';
require_once 'database.inc';
require_once 'entity.inc';
require_once 'exception.inc';
require_once 'metadata.inc';
require_once 'pages.inc';
require_once 'theme.inc';
require_once 'menu.inc';

if(module_exists('inline_entity_form')) {
  require_once 'InlineEntityFormController.inc';
}

/**
 * Query a property of given entities (by id). NOT EFFICIENT! Must
 * use EFQ.
 */
function entityspice_entity_load_and_extract_property($etype, array $ids, $property) {
  $extract = array();
  foreach (entity_load($etype, $ids) as $id => $entity) {
    $extract[$id] = $entity->{$property};
  }

  return $extract;
}

