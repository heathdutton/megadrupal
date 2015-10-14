<?php

/**
 * @file
 * User page callbacks for the Yandex XML module.
 */

/**
 * Menu callback; presents the search form and/or search results.
 *
 * @param string $keys
 *   Keywords to use for the search.
 */
function yandex_xml_view($keys = '') {

  // The form may be altered based on whether the search was run.
  $build['search_form'] = drupal_get_form('yandex_xml_form', $keys);
  if (!$keys) {
    return $build;
  }

  try {
    $search = new YandexXml(variable_get('yandex_xml_search_url'));
    $search->query($keys);
    $search->page(isset($_GET['page']) ? (int) $_GET['page'] : 0);
    $search->site(variable_get('yandex_xml_site_autodetect') ? $GLOBALS['base_url'] : variable_get('yandex_xml_site_url'));
    $search->limit(variable_get('yandex_xml_grpoups_on_page', 10));
    $search->send(/* 'public://yandex.xml' */);
  }
  catch (Exception $e) {
    $request_failed = TRUE;
    watchdog('yandex_xml', $e->getMessage(), NULL, WATCHDOG_ERROR);
  }

  if (isset($request_failed)) {
    drupal_set_message(t('Service temporarily unavailable. Please contact the site administrator.'), 'error');
  }
  elseif ($search->error) {
    $error_message = yandex_xml_error_messages($search->error);

    if ($search->error == 15) {
      $build['search_failed'] = array(
        '#theme' => 'container',
        '#attributes' => array('id' => 'yandex-search-failed'),
        '#markup' => $error_message,
      );
    }
    else {
      drupal_set_message(t('Service temporarily unavailable. Please contact the site administrator.'), 'error');
      watchdog('yandex_xml', $error_message, NULL, WATCHDOG_ERROR);
    }
  }
  else {
    $build['search_results'] = array(
      '#theme' => 'yandex_xml_results',
      '#search' => $search,
      '#attached' => array('css' => array(drupal_get_path('module', 'yandex_xml') . '/yandex_xml.css')),
    );
    pager_default_initialize($search->total(), variable_get('yandex_xml_grpoups_on_page', 10));
    $build['pager'] = array(
      '#theme' => 'pager',
    );
  }

  return $build;
}

/**
 * Processes variables for yandex_xml-results.tpl.php.
 *
 * @see yandex_xml-results.tpl.php
 */
function template_preprocess_yandex_xml_results(&$vars) {
  global $language;
  $search = $vars['elements']['#search'];

  $vars['summary'] = format_plural($search->total(),
    '!yandex found 1 answer.',
    '!yandex found @count answers.',
    // ::first-letter doesn't work on inline elements.
    array('!yandex' => YandexXml::getLogo($language->language))
  );

  $vars['results'] = array();
  foreach ($search->results() as $result) {
    foreach ($result as $key => $value) {
      if ($key == 'passages' && $value) {
        $row[$key] = YandexXml::highlight($value->asXML());
      }
      elseif ($key == 'headline' && $value) {
        $row[$key] = YandexXml::highlight($value->asXML());
      }
      elseif ($key == 'title' && $value) {
        $row[$key] = YandexXml::highlight($value->asXML());
      }
      else {
        $row[$key] = (string) $value;
      }
    }
    $vars['results'][] = $row;
  }

}

/**
 * Convert an error code to text.
 */
function yandex_xml_error_messages($error_code = FALSE) {

  $errors[1]   = t('The query text (the value passed in the query element) contains a syntactical error. For example, a query was sent that contained only two slash symbols in a row (“//”).');
  $errors[2]   = t('An empty search query was defined (an empty value was passed in the query element).');
  $errors[15]  = t('There are no search results for the specified search query.');
  $errors[18]  = t('The XML file cannot be validated, or invalid request parameters are set');
  $errors[19]  = t('The search query contains incompatible parameters (for example, incompatible values for the groupings element).');
  $errors[20]  = t('The reason for the error is unknown. If this occurs repeatedly, contact the support service.');
  $errors[31]  = t('The user is not registered in the service.');
  $errors[32]  = t('Limit exceeded for the number of queries allowed per day. Review the information about restrictions and choose a suitable method for increasing your daily quota.');
  $errors[33]  = t('The IP address that the search request was sent from does not match the one(s) set during registration.');
  $errors[34]  = t('The user is not registered in Yandex.Passport.');
  $errors[37]  = t('Error in request parameters. Maybe mandatory parameters were omitted, or mutually exclusive parameters were defined.');
  $errors[42]  = t('The key that was issued during registration contains an error. Check whether the correct address is used for sending requests.');
  $errors[43]  = t('The version of the key that was issued during registration contains an error. Check whether the correct address is used for sending requests.');
  $errors[44]  = t('The address that requests are sent to is no longer supported. Correct the value to match the address that was given during registration.');
  $errors[48]  = t('The search type that was specified during registration does not match the search type that is being used for requesting data. Reset the domain that is being used to the correct domain. For corrections, use the URL for sending requests.');
  $errors[100] = t('The request was most likely sent by a robot. When this error appears, a CAPTCHA must be returned to the user.');

  return $error_code ? $errors[$error_code] : $errors;
}
