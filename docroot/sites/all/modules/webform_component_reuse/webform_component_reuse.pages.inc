<?php
/**
 * @file
 */

function webform_component_reuse_add_to_webform(&$entity, $context = array()) {
}

function webform_component_reuse_add_from_library($node){
  if($node) {
    drupal_goto('admin/structure/webform_component_reuse/' . $node->nid . '/list');
  }
}

function webform_component_reuse_list($node = array()) {
  $content = array();
  $entities = webform_component_reuse_load_multiple();
  if (!empty($entities)) {
    foreach ( $entities as $entity ) {
      $rows[] = array(
        'data' => array(
          'id' => $entity->wcid,
          'bundle' => $entity->type,
        ),
      );
    }
    $content['entity_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#header' => array(t('ID'), t('Bundle')),
    );
  }
  else {
    $content[] = array(
      '#type' => 'item',
      '#markup' => t('No webform_component_reuse currently exist.'),
    );
  }
  return $content;
}

function webform_component_reuse_info_page() {
  $content['preface'] = array(
    '#type' => 'item',
    '#markup' => t('The entity example provides a simple example entity.')
  );
  if (user_access('administer webform_component_reuse')) {
    $content['preface']['#markup'] =  t('You can administer these and add fields and change the view !link.',
      array('!link' => l(t('here'), 'admin/structure/webform_component_reuse/manage'))
    );
  }
  $content['table'] = webform_component_reuse_list();

  return $content;
}
