<?php
// $Id: bing.search.inc,v 1.1.2.4 2010/08/20 01:19:19 mfb Exp $

/**
 * Set a maximum number of search results to page through.
 */
define('BING_MAX_TOTAL', 1000);

/**
 * Returns array of Bing highlighting characters.
 */
function bing_highlight() {
  return array('/\x{e000}/u', '/\x{e001}/u');
}

/**
 * Converts WebResult object to result array.
 */
function bing_result_Image($item) {
  return array(
    'link' => isset($item->Url) ? $item->Url : NULL,
    'title' => isset($item->Title) ? bing_title($item->Title) : NULL,
    'snippet' => isset($item->Thumbnail) ? l(bing_thumbnail($item->Thumbnail), $item->Url, array('html' => TRUE)) : NULL,
    'date' => isset($item->DateTime) ? strtotime($item->DateTime) : NULL,
    'extra' => isset($item->MediaUrl) ? array(bing_snippet($item->MediaUrl)) : NULL,
  );
}

/**
 * Converts NewsResult object to result array.
 */
function bing_result_News($item) {
  return array(
    'link' => isset($item->Url) ? $item->Url : NULL,
    'title' => isset($item->Title) ? bing_title($item->Title) : NULL,
    'snippet' => isset($item->Snippet) ? bing_snippet($item->Snippet) : NULL,
    'date' => isset($item->Date) ? strtotime($item->Date) : NULL,
    'extra' => isset($item->Source) ? array(bing_snippet($item->Source)) : NULL,
  );
}

/**
 * Converts VideoResult object to result array.
 */
function bing_result_Video($item) {
  return array(
    'link' => isset($item->PlayUrl) ? $item->PlayUrl : NULL,
    'title' => isset($item->Title) ? bing_title($item->Title) : NULL,
    'snippet' => isset($item->StaticThumbnail) ? l(bing_thumbnail($item->StaticThumbnail), $item->PlayUrl, array('html' => TRUE)) : NULL,
    'extra' => isset($item->SourceTitle) ? array(bing_snippet($item->SourceTitle)) : NULL,
  );
}

/**
 * Converts WebResult object to result array.
 */
function bing_result_Web($item) {
  return array(
    'link' => isset($item->Url) ? $item->Url : NULL,
    'title' => isset($item->Title) ? bing_title($item->Title) : NULL,
    'snippet' => isset($item->Description) ? bing_snippet($item->Description) : NULL,
    'date' => isset($item->DateTime) ? strtotime($item->DateTime) : NULL,
    'extra' => isset($item->DisplayUrl) ? array(bing_snippet($item->DisplayUrl)) : NULL,
  );
}

/**
 * Submits search query and processes search results.
 */
function bing_service($keys, $conditions) {
  global $language;
  $limit = max(1, intval(variable_get('bing_limit', 15)));
  $results = array();
  if (variable_get('bing_query', '')) {
    $keys .= ' AND (' . variable_get('bing_query', '') . ')';
  }
  try {
    $webservice = new SoapClient('http://api.bing.com/search.wsdl', array('compression' => SOAP_COMPRESSION_ACCEPT | SOAP_COMPRESSION_DEFLATE));
    if ($webservice) {
      $params = array('parameters' => array(
        'AppId' => variable_get('bing_appid', ''),
        'Market' => variable_get('bing_locale_' . $language->language, 'en-US'),
        'Options' => array('SearchOption' => array('EnableHighlighting')),
        'Query' => $keys,
        'Sources' => array(variable_get('bing_source', 'Web')),
        'Adult' => variable_get('bing_safesearch', 'Strict'),
      ));
      // @todo Allow searching multiple sources simultaneously, returning
      // multiple result sets with multiple pagers.
      foreach ($params['parameters']['Sources'] as $element => $source) {
        $page = pager_default_initialize(BING_MAX_TOTAL, $limit, $element);
        $params['parameters'][$source] = array('Count' => $limit, 'Offset' => $limit * $page);
      }
      $response = $webservice->Search($params);
      foreach ($params['parameters']['Sources'] as $element => $source) {
        $callback = 'bing_result_' . $source;
        $items = isset($response->parameters->$source->Results->{$source . 'Result'}) ? $response->parameters->$source->Results->{$source .'Result'} : array();
        $items = is_array($items) ? $items : array($items);
        foreach ($items as $item) {
          $results[] = $callback($item);
        }
        pager_default_initialize(min(isset($response->parameters->$source->Total) ? $response->parameters->$source->Total : 0, BING_MAX_TOTAL), $limit, $element);
      }
    }
  }
  catch (Exception $e) {
    watchdog('bing', t('Web service error: %faultstring. <pre>%detail</pre>'), array('%faultstring' => $e->faultstring, '%detail' => print_r($e->detail->Errors, TRUE)), WATCHDOG_ERROR);
    if (variable_get('error_level', ERROR_REPORTING_DISPLAY_ALL)) {
      drupal_set_message(t('Web service error: %faultstring.', array('%faultstring' => $e->faultstring)), 'error');
    }
  }
  return $results;
}

/**
 * HTML-encodes a snippet or other result string.
 */
function bing_snippet($snippet) {
  // Run check_plain() just in case of XSS payload.
  return preg_replace(bing_highlight(), array('<strong>', '</strong>'), check_plain($snippet));
}

/**
 * Renders a themed thumbnail image.
 */
function bing_thumbnail($thumbnail) {
  return theme('image', array('path' => $thumbnail->Url, 'alt' => '', 'title' => '', 'attributes' => array('width' => $thumbnail->Width, 'height' => $thumbnail->Height), 'getsize' => FALSE));
}

/**
 * Prepares a result title, which will be HTML-encoded later in the theme layer.
 */
function bing_title($title) {
  // Title will be HTML-encoded by l().
  return preg_replace(bing_highlight(), '', $title);
}
