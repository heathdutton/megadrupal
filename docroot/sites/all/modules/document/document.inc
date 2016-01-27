<?php

define('DOCUMENT_STATUS_UNPUBLISHED', 0);
define('DOCUMENT_STATUS_PUBLISHED', 1);
define('DOCUMENT_INTERNAL', 0);
define('DOCUMENT_EXTERNAL', 1);
define('DOCUMENT_SEARCH_AUTHOR', 0);
define('DOCUMENT_SEARCH_KEYWORDS', 1);
define('DOCUMENT_SEARCH_AUTHOR_KEYWORDS', 2);
define('DOCUMENT_SEARCH_NONE', 3);

function document_get_types($idAsKey = TRUE, $reset = FALSE) {
  static $types;
  if (!isset($types) || $reset) {
    if (!$reset && ($cache = cache_get('document_types')) && !empty($cache->data)) {
      $types = unserialize($cache->data);
    }
    else {
      $vocid = document_get_vocid();
      $results = db_query('SELECT * FROM {taxonomy_term_data} WHERE vid = :vid', array(':vid' => $vocid));
      $types = array();
      foreach ($results as $type) {
        $types[$type->tid] = $type->name;
      }
      uasort($types, "_document_type_comparer");

      cache_set('document_types', serialize($types));
    }
  }

  $arr = array();
  foreach ($types as $key => $value) {
    if ($idAsKey)
      $arr[$key] = $value;
    else
      $arr[$value] = $value;
  }

  return $arr;
}

function document_get_vocid() {
  $vocid = variable_get('document_vocabulary', '');
  if (empty($vocid)) {
    // Check to see if document vocabulary exists
    $vocid = db_query("SELECT vid FROM {taxonomy_vocabulary} WHERE module='document'")
              ->fetchField();
    if ($vocid) {
      // We found a vocabulary, so make sure it is associated with our content.
      $vocabulary = taxonomy_vocabulary_load($vocid);
      $vocabulary->nodes = array('document' => 1);
      $status = taxonomy_vocabulary_save($vocabulary);
    }
    else {
      // Didn't find one, so create vocabulary from scratch.
      $vocabulary = new stdClass();
      $vocabulary->name = 'Document types';
      $vocabulary->machine_name = 'document_types';
      $vocabulary->multiple = 0;
      $vocabulary->required = 1;
      $vocabulary->hierarchy = 2;
      $vocabulary->relations = 0;
      $vocabulary->module = 'document';
      $vocabulary->nodes = array('document' => 1);
      $status = taxonomy_vocabulary_save($vocabulary);
      $vocid = $vocabulary->vid;
    }
    variable_set('document_vocabulary', $vocid);
  }
  return $vocid;
}

function document_get_vocab() {
  $vocid = variable_get('document_vocabulary', '');
  $vocab = NULL;
  if (empty($vocid)) {
    $vocid = document_get_vocid();
    $vocab = taxonomy_vocabulary_load($vocid);
  }
  else {
    $vocab = taxonomy_vocabulary_load($vocid);
    if (!$vocab) {
      $vocid = document_get_vocid();
      $vocab = taxonomy_vocabulary_load($vocid);
    }
  }
  return ($vocab);
}

function document_get_years() {
  $years = array();
  for ($i = 1950; $i <= date('Y'); $i++) {
    $years[] = $i;
  }

  return ($years);
}

function document_get_path() {
  $path = variable_get('document_path', '');
  $path = variable_get('file_public_path', conf_path() . '/files')  . '/' . $path;

  return ($path);
}

function document_perform_search($searchFields, $searchText, $searchYear = NULL, $searchDocType = NULL) {
  $conditions = db_and();
  $conditions->condition('n.status', DOCUMENT_STATUS_PUBLISHED);
  
  switch ($searchFields) {
    case 0:
      $conditions->condition('author', '%' . $searchText . '%', 'LIKE');
      break;
    case 1:
      $conditions->condition('keywords', '%' . $searchText . '%', 'LIKE');
      break;
    case 2:
      $or = db_or();
      $or->condition('keywords', '%' . $searchText . '%', 'LIKE')
          ->condition('author', '%' . $searchText . '%', 'LIKE');
      $conditions->condition($or);
      break;
    case 3:
      break;
    default:
      die('Invalid Input');
  }

  if (!empty($searchYear)) {
    $conditions->condition('publish_year', $searchYear);
  }

  if (!empty($searchDocType) > 0) {
    $conditions->condition('d.type', '%' . $searchDocType . '%', 'LIKE');
  }

  return (document_search_table($conditions));
}

function document_search_table($conditions) {
  //http://www.rahulsingla.com/projects/drupal-document-module#comment-194
  //During Ajax search, the path is document/search, and hence clicking table sort headers after a search
  //take you to a blank page with only the document table.
  //So, need to change the query to the document page.
  $q = $_GET['q'];
  $_GET['q'] = 'document';

  $headers = array(
      array(
          'data' => t('Type'),
          'field' => 'd.type',
          'class' => array('search-result-header col-type')),
      array(
          'data' => t('Title'),
          'field' => 'title',
          'sort' => 'asc',
          'class' => array('search-result-header col-title')),
      array(
          'data' => t('Author'),
          'field' => 'author',
          'class' => array('search-result-header col-author')),
      array(
          'data' => t('Year of Publication'),
          'field' => 'publish_year',
          'class' => array('search-result-header col-publish-year')),
      array(
          'data' => t('Keywords'),
          'class' => array('search-result-header col-keywords')),
      array(
          'data' => '',
          'class' => array('search-result-header col-download')));

  $query = db_select('node', 'n');
  $query->join('document', 'd', 'n.vid = d.vid');
  $query
        ->condition($conditions)
        ->extend('PagerDefault') 	//Pager Extender
          ->limit(10)				//10 results per page
        ->extend('TableSort')		//Sorting Extender
          ->orderByHeader($headers)//Field to sort on is picked from $header
        ->fields ('n')
        ->fields('d');
  $results = $query
        ->execute();

  $moderate = user_access('moderate document');
  if ($moderate) {
    array_unshift($headers, '');
  }

  $imgUnpublish = theme_image(array(
      'path' => document_image_url('spacer.gif'), 
      'alt' => t('Unpublish'),
      'title' => t('Unpublish'),
      'attributes' => array(
          'onclick' => 'doc.changeDocStatus(this, %1$d, \'icon-unpublish\', false);',
          'class' => array('icon-unpublish'),
          'width' => 16,
          'height' => 16,
          )
      ));
  $imgDelete = theme_image(array(
      'path' => document_image_url('spacer.gif'), 
      'alt' => t('Delete'),
      'title' => t('Delete'),
      'attributes' => array(
          'onclick' => 'doc.deleteDoc(this, %1$d, \'icon-delete\');',
          'class' => array('icon-delete'),
          'width' => 16,
          'height' => 16,
          )
      ));

  $rows = array();
  foreach($results as $doc) {
    $row = array(
        check_plain($doc->type),
        l($doc->title, 'node/' . $doc->nid),
        check_plain($doc->author),
        $doc->publish_year,
        check_plain($doc->keywords),
        l(t('Download'), $doc->url, array(
            'attributes' => array(
                'target' => '_blank'))));

    if ($moderate) {
      array_unshift($row, sprintf($imgUnpublish . '&nbsp;&nbsp;&nbsp;' . $imgDelete, $doc->nid));
    }
    $rows[] = $row;
  }
  
  $table = theme('table', array(
      'header' => $headers, 
      'rows' => $rows
      )
    );

  //Restore the original query.
  $_GET['q'] = $q;

  return ($table);
}

function document_image_url($name) {
  $url = drupal_get_path('module', 'document') . '/images/' . $name;
  return ($url);
}

function document_mail_text($key, $language = NULL, $variables = array()) {
  $langcode = isset($language) ? $language->language : NULL;

  if ($admin_setting = variable_get('document_' . $key, FALSE)) {
    // An admin setting overrides the default string.
    return strtr($admin_setting, $variables);
  }
  else {
    // No override, return default string.
    switch ($key) {
      case 'publish_subject':
        return t('Your Document got published at !site', $variables, array('langcode' => $langcode));
      case 'publish_body':
        return t("!username,\n\nThank you for uploading a document at !site. Your document, \"!node_title\" has been published by the moderators. You can now view your document at: !node_title_link.\n\n\n--  !site team", $variables, array('langcode' => $langcode));
    }
  }
}

function document_mail_tokens($account, $node, $language) {
  global $base_url;
  $node_link = 'node/' . $node->nid;
  $tokens = array(
      '!username' => $account->name,
      '!doc_link' => l($node->document_url, $node->document_url),
      '!node_title' => $node->title,
      '!node_title_link' => l($node->title, $node_link),
      '!node_link' => l($node_link, $node_link),
      '!site' => variable_get('site_name', 'Drupal'),
      '!uri' => $base_url,
      '!uri_brief' => preg_replace('!^https?://!', '', $base_url),
      '!mailto' => $account->mail,
      '!date' => format_date(time(), 'medium', '', NULL, $language->language));
  return $tokens;
}

function _document_type_comparer($a, $b) {
  return (strcasecmp($a['type'], $b['type']));
}

function _document_register_status() {
  drupal_add_js(array(
      'document' => array(
          'STATUS_UNPUBLISHED' => DOCUMENT_STATUS_UNPUBLISHED,
          'STATUS_PUBLISHED' => DOCUMENT_STATUS_PUBLISHED)), 'setting');
}

function _document_register_validation_token () {
  // makes a random alpha numeric string of a given lenth
  $chars = array_merge(range('A', 'Z'), range('a', 'z'), range(0, 9), array(
      '@',
      '%',
      '^',
      '*'));
  $validationString = '';
  for ($c = 0; $c < 10; $c++) {
    $validationString .= $chars[mt_rand(0, count($chars) - 1)];
  }
  
  $validationToken = drupal_get_token($validationString);
  
  drupal_add_js("doc.validationString = '$validationString';
	doc.validationToken = '$validationToken';", 'inline');  
}
