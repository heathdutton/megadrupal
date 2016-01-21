<?php

/**
 * @file
 * This file contains all the functions for manipulating GCC Menu.
 */

/**
 * Load a GCC Menu.
 */
function gcc_menu_load($menu_name) {

  $query = new EntityFieldQuery();
  $query->fieldCondition(GCC_MENU_FIELD_MENU, 'menu_name', $menu_name);
  $query->addTag('DANGEROUS_ACCESS_CHECK_OPT_OUT');
  $query->range(0, 1);

  $results = $query->execute();

  foreach ($results as $entity_type => $entities) {
    foreach ($entities as $entity_id => $entity) {

      $entity = entity_load($entity_type, array($entity_id));
      if (isset($entity[$entity_id]) && isset($entity[$entity_id]->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE])) {
        foreach ($entity[$entity_id]->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE] as $item) {
          if ($item['menu_name'] == $menu_name) {

            $item['entity_type'] = $entity_type;
            $item['entity'] = $entity;

            return $item;
          }
        }
      }
    }
  }

  return FALSE;
}

/**
 * Extract the menus from an entity.
 */
function gcc_menu_extract_menu($entity, $default_only = FALSE) {

  $menus = array();

  if (isset($entity->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE])) {
    foreach ($entity->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE] as $item) {

      $menus[$item['menu_name']] = $item;
      if ($default_only && $item['default_menu']) {
        return array($item['menu_name'] => $item);
      }
    }
  }

  if ($default_only) {
    return array_slice($menus, 0, 1, TRUE);
  }

  return $menus;
}
