<?php

/**
 * @file
 * Helper method used to lookup CMIS object from request.
 */

/**
 * Produce a list of theme types.
 *
 * @return array
 *   An array with a list of themes for formatting cmis views.
 *
 */
function _cmis_views_theme_types() {
  $themes[0] = t('Bulleted list with file type and size');
  $themes[1] = t('Paragraphs with description, file type, size and date');
  $themes[2] = t('Table with file name, title, description, type, size and date');
  $themes[3] = t('Display title and description details with thumbnail');
  return $themes;
}

/**
 * Return a folder object from a file path
 *
 * @param object $repository
 *   An object containing details of the CMIS repository
 *
 * @param string $querystring
 *   A CMIS folder object
 *
 * @return string
 *   A string with all formatting needed to display in a block
 *
 */
function _cmis_views_content_object_from_path($repository, $querystring) {

  $object_path = NULL;
  $bcarray = explode('/', $querystring);
  $repoid = !empty($repository->repositoryId) ? $repository->repositoryId : 'default';

  if (!empty($bcarray)) {
    $object_path = drupal_encode_path('/' . implode('/', $bcarray));
    $object = cmisapi_getObjectByPath($repoid, $object_path);
  }
  return $object->properties['cmis:objectId'];
}

/**
 * Returns a friendly file size from byte value
 *
 * @param string $bytes
 *   A string containing the file size in bytes e.g. 1073744939.
 *
 * @param int $precision
 *   An integer specifying the decimal places bytes should be rounded to.
 *
 * @return string
 *   A string containing a friendly file size e.g 1MB.
 */
function _cmis_views_tidySize($bytes, $precision = 0) {
  $units = array('B', 'KB', 'MB', 'GB', 'TB');

  $bytes = max($bytes, 0);
  $pow = floor(($bytes ? log($bytes) : 0) / log(1024));
  $pow = min($pow, count($units) - 1);

  $bytes /= pow(1024, $pow);

  return round($bytes, $precision) . ' ' . $units[$pow];
}

/* This isn't working properly, need a unique identifier to use with cache */

function _cmis_views_query($uuid, $repoid){
  $query_result = cache_get('cmis_views_'.$uuid, 'cache');
  if (!$query_result) {
    $query_result = '';
    try {
      $repoid = !empty($repository->repositoryId) ? $repository->repositoryId : 'default';      
      $quickquery = 'select d.*, t.* from cmis:document as d join cm:titled as t on d.cmis:objectId = t.cmis:objectId ';
      $quickquery .= "where in_folder(d, '%s') ";
      $quickquery .= "ORDER BY d.cmis:name ASC";
      $query_result = cmisapi_query($repoid, sprintf($quickquery, $uuid), $options = array('renditionFilter' => "cmis:thumbnail"));
      cache_set('cmis_views_'.$uuid, $query_result, 'cache', $expire = CACHE_TEMPORARY);
    }
      catch (CMISException $e) {
      cmis_error_handler('cmis_query', $e);
      $contents = t('Error');
    }
  }else{
    $query_result = $query_result->data;
  }
  return $query_result;
}

/**
 * Returns a friendly file size from byte value
 *
 * @param string $mimetype
 *   A string containing the file mimetype.
 *
 * @return array
 *   An array containing the image file name and a friendly filetype PDF/HTML.
 */
function _cmis_views_tidyMime($mimetype) {
  switch ($mimetype) {
    case "text/plain":
      return array("image" => "text-64.png", "type" => "text");
      break;
    case "application/vnd.openxmlformats-officedocument.wordprocessingml.template":
    case "application/msword":
    case "application/vnd.openxmlformats-officedocument.wordprocessingml.document":
      return array("image" => "doc-64.png", "type" => "MS Word");
      break;
    case "application/vnd.ms-excel":
    case "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet":
    case "application/vnd.openxmlformats-officedocument.spreadsheetml.template":
      return array("image" => "xls-64.png", "type" => "MS Excel");
      break;
    case "application/vnd.openxmlformats-officedocument.presentationml.slideshow":
    case "application/vnd.ms-powerpoint":
    case "application/vnd.openxmlformats-officedocument.presentationml.presentation":
    case "application/vnd.openxmlformats-officedocument.presentationml.template":
      return array("image" => "ppt-64.png", "type" => "MS Powerpoint");
      break;
    case "application/vnd.oasis.opendocument.chart":
      return array("image" => "odg-64.png", "type" => "Open Office Chart");
      break;
    case "application/vnd.oasis.opendocument.presentation":
    case "application/vnd.oasis.opendocument.presentation-template":
      return array("image" => "odp-64.png", "type" => "Open Office Presentation");
      break;
    case "application/vnd.oasis.opendocument.spreadsheet":
    case "application/vnd.oasis.opendocument.spreadsheet-template":
      return array("image" => "ods-64.png", "type" => "Open Office Spreadsheet");
      break;
    case "application/vnd.oasis.opendocument.text":
    case "application/vnd.oasis.opendocument.text-template":
      return array("image" => "odt-64.png", "type" => "Open Office Spreadsheet");
      break;
    case "text/html":
    case "application/xhtml+xml":
      return array("image" => "html-64.png", "type" => "HTML");
      break;
    case "video/mp4":
    case "video/mpeg":
    case "video/mpeg2":
    case "video/x-flv":
    case "video/x-msvideo":
    case "video/x-ms-wmv":
      return array("image" => "video-64.png", "type" => "Video");
      break;
    case "application/pdf":
      return array("image" => "pdf-64.png", "type" => "PDF");
      break;
    case "image/png":
    case "image/jpeg":
    case "image/gif":
      return array("image" => "img-64.png", "type" => "Image");
      break;
    case "application/zip":
      return array("image" => "zip-64.png", "type" => "Zip archive");
      break;
    default:
      return array("image" => "generic-64.png", "type" => $mimetype);
  }
}
