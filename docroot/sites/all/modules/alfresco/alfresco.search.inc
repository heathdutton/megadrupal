<?php

/**
 * @file
 * Search hooks.
 *
 * @todo: Crear un súbmodulo para búsquedas independiente ?
 */

/**
 * Implements hook_search_info().
 */
function alfresco_search_info() {
  return array(
    'title' => 'Alfresco',
    'path' => 'alfresco',
  );
}

/**
 * Implements hook_search_access().
 */
function alfresco_search_access() {
  return user_access('search alfresco repository');
}

/**
 * Implements hook_search_execute().
 *
 * @todo: Mover la lógica de las búsquedas a un método de alfresco.service.inc
 */
function alfresco_search_execute($keys = NULL, $conditions = NULL) {
  $find = array();

  // @todo search_parse_query()
  if (drupal_strlen($keys) < variable_get('minimum_word_size', 3)) {
    form_set_error('keys', t('You must include at least one positive keyword with @count characters or more.', array('@count' => variable_get('minimum_word_size', 3))));
    return $find;
  }

  // @todo: Configurar si mostrar contenido con/sin acceso
  //$repository_access = alfresco_repository_access();
  $repository_access = FALSE;

  try {
    alfresco_include('soap');

    $session = alfresco_soap_get_session();
    $store = new AlfStore($session, 'SpacesStore');

    $query = _alfresco_search_build_query($keys);

    //dpm('query: '. $query);
    $results = $session->query($store, $query);

    foreach ($results as $alf_node) {
      $reference = $alf_node->__toString();

      $nid = alfresco_exists_node($reference);
      if (!$nid && !$repository_access) {
        continue;
      }

      // @todo: Añadir hook para filtrar los resultados de la búsqueda
      if ($node = node_load($nid)) {
        if (!node_access('view', $node)) {
          continue;
        }
      }

      $name = $alf_node->cm_name;
      $title = $alf_node->cm_title;
      $description = $alf_node->cm_description;
      $extension = substr(strrchr($alf_node->cm_name, '.'), 1);
      $type = $alf_node->type;
      //$path = $alf_node->getFolderPath() . '/'. $name;

      if ($repository_access && !$node) {
        $store_node = $alf_node->getStore();
        $link = "alfresco/download/$store_node->scheme/$store_node->address/$alf_node->id/$alf_node->cm_name";
      }
      else {
        $link = alfresco_url_download($node);
      }

      if ($alf_node->cm_content instanceof AlfContentData) {
        $content = $alf_node->cm_content;
      }

      $find[] = array(
        'link'    => url($link, array('absolute' => TRUE)),
        'title'   => !empty($title) ? $title : $name,
        //'snippet' => $description,
        'user'    => $alf_node->cm_creator,
        'date'    => strtotime($alf_node->cm_modified),
        'extra'   => array(
          'size'     => isset($content) ? format_size($content->size) : '',
          'mimetype' => isset($content) ? $content->mimetype : '',
        ),
      );
    }
  }
  catch (SoapFault $exception) {
    alfresco_error_handler('alfresco', $exception);
  }

  return $find;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
// function alfresco_form_search_form_alter(&$form, &$form_state) {
//   alfresco_include('search');
//   alfresco_search_form_alter($form, $form_state);
// }


/**
 *
 */
function alfresco_search_form_alter(&$form, &$form_state) {

  if ($form['module']['#value'] == 'alfresco') {
    // @todo: Implements advanced alfresco search
    // - Search only alfresco item
    // - Search all repository
  }
}

function _alfresco_search_build_query($keys) {
  $words = explode(' ', $keys);

  $query = '+ISNODE:T +TYPE:"cm:content"';

  foreach ($words as $word) {
    $query .= ' +ALL:"*'. check_plain($word) .'*"';
  }

  return $query;
}
