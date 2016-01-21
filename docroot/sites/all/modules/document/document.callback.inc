<?php

require_once (drupal_get_path('module', 'document') . '/document.inc');

function document_search() {
  try {
    _document_headers();
	_document_validate_token();

    $criteria = $_REQUEST['criteria'];

    if (!isset($criteria)) {
      die(t('Invalid input.'));
    }

    $criteria = json_decode($criteria);
    $table = document_perform_search($criteria->searchFields, $criteria->searchText, $criteria->searchYear, $criteria->searchDocType);
    die($table);
  }
  catch (Exception $e) {
    die($e->getMessage() + "\n" + $e->getTraceAsString());
  }
}

function document_change_doc_status() {
  try {
    _document_headers();
	_document_validate_token();

    $ids = $_REQUEST['ids'];
    $status = $_REQUEST['status'];

    if (!isset($ids) || !isset($status)) {
      die(t('Invalid input.'));
    }
    else if ($status != DOCUMENT_STATUS_PUBLISHED && $status != DOCUMENT_STATUS_UNPUBLISHED) {
      die(t('Invalid input.'));
    }

    $ids = explode(',', $ids);

    foreach ($ids as $id) {
      $node = node_load($id, NULL, TRUE);
      $node->status = $status;
      $node->document_publishing = TRUE;
      node_save($node);

      if ($status == DOCUMENT_STATUS_PUBLISHED && variable_get('document_publish_email', TRUE)) {
        $uid = $node->uid;
        $user = user_load($uid);
        if ($user) {
          drupal_mail('document', 'publish', $user->mail, user_preferred_language($user), array(
              'node' => $node,
              'account' => $user));
        }
      }
    }

    die();
  }
  catch (Exception $e) {
    die($e->getMessage() + "\n" + $e->getTraceAsString());
  }
}

function document_delete_doc() {
  try {
    _document_headers();
	_document_validate_token();

    $ids = $_REQUEST['ids'];

    if (!isset($ids)) {
      die(t('Invalid input.'));
    }

    $ids = explode(',', $ids);

    foreach ($ids as $id) {
      node_delete($id);
    }

    die();
  }
  catch (Exception $e) {
    die($e->getMessage() + "\n" + $e->getTraceAsString());
  }
}

function document_add_type() {
  try {
    _document_headers();
	_document_validate_token();

    $type = $_REQUEST['type'];

    if (!isset($type)) {
      die(t('Invalid input.'));
    }

    $types = document_get_types(FALSE);
    if (array_key_exists($type, $types)) {
      die(t('The specified Document Type already exists.'));
    }

    $obj = array(
        'tid' => '',
        'vid' => document_get_vocid(),
        'name' => $type,
        'description' => '',
        'weight' => 0);
    drupal_write_record('taxonomy_term_data', $obj);

    $hier = array('tid' => $obj['tid'], 'parent' => 0);
    drupal_write_record('taxonomy_term_hierarchy', $hier);

    cache_clear_all();
    cache_clear_all('document_types', 'cache');
    die($obj['tid']);
  }
  catch (Exception $e) {
    die($e->getMessage() + "\n" + $e->getTraceAsString());
  }
}

function document_delete_types() {
  try {
    _document_headers();
	_document_validate_token();

    $ids = $_REQUEST['ids'];

    if (!isset($ids)) {
      die(t('Invalid input.'));
    }

    $ids = explode(',', $ids);
    $types = document_get_types(TRUE);

    foreach ($ids as $id) {
      $count = db_query("SELECT COUNT(*) FROM document AS d WHERE d.type = '%s'", $types[$id])
                ->fetchField();
      if ($count > 0) {
        die(t('The type(s) have documents associated to them. Please delete the documents first before deleting the type.'));
      }
    }

    foreach ($ids as $id) {
      taxonomy_term_delete($id);
    }

    cache_clear_all();
    cache_clear_all('document_types', 'cache');
    die();
  }
  catch (Exception $e) {
    die($e->getMessage() + "\n" + $e->getTraceAsString());
  }
}

function _document_headers() {
  header("Content-type: text/html");
  header("Expires: Wed, 29 Jan 1975 04:15:00 GMT");
  header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
  header("Cache-Control: no-cache, must-revalidate");
  header("Pragma: no-cache");
}

function _document_validate_token() {
  $validationString = $_REQUEST['validationString'];
  $validationToken = $_REQUEST['validationToken'];

  $isValid = FALSE;

  if(!empty($validationString) && !empty($validationToken)) {
    if (drupal_valid_token($validationToken, $validationString)) {
      $isValid = TRUE;
    }
  }
  
  if(!$isValid) {
	die('You are not authorized to perform this action.');
  }
}
