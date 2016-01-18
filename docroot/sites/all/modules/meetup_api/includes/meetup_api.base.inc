<?php

/**
 * @file
 * Wrapper for the MeetupAPIGroups class
 */

/**
 * meetup_api_request
 * Run a Meetup API Request, either with an instantiated Meetup API instance or with an instance of MeetupAPI Base if none is supplied.
 * @param mixed $request The request to send to the Meetup API.
 * @param MeetupAPIBase $mu_api
 * @param array $options
 */
function meetup_api_request($request, $mu_api = NULL, $options = NULL) {
  if (!isset($mu_api)) {
    if (isset($options['method'])) {
      require_once(MUAPILOC . '/MeetupAPIBase.php');
      $mu_api = new MeetupAPIBase(MUAPIKEY, $options['method']);
    }
    else {
      return _meetup_api_error(t('meetup_api_request() called without a valid method. Please pass a valid method as part of your $options[\'method\'] argument.'));
    }
  }
  _meetup_api_set_options($mu_api, $options);
  $mu_api->setQuery($request);
  $response = $mu_api->getResponse();
  return $response;
}

/**
 * meetup_api_set_options
 * Set options on an instantiated MeetupAPIBase instance.
 * @param MeetupAPIBase $mu_api
 * @param array $options
 */
function _meetup_api_set_options($mu_api, $options) {
  foreach ($options as $key => $option) {
    //Set options based on the key
    switch ($key) {
      case 'page_size':
      case 'page':
        $mu_api->setPageSize((int) $option);
        break;
      case 'num_pages':
      case 'pages':
        $mu_api->setNumPages((int) $option);
        break;
      case 'sort_desc':
      case 'desc':
        $mu_api->setSortDesc((bool) $option);
        break;
    }
  }
}

