<?php
/**
 * @file
 * SSGA: method and functions
 */

/**
 * GA Push Method callback: SSGA (php).
 */
function ga_push_method_ssga($push, $type, $options) {

  if (class_exists('ssga')) {

    $ga = new ssga();

    // Set your Google Analytics key:
    $id = !empty($options['tid']) ? $options['tid'] : variable_get('googleanalytics_account', '');
    $ga->setAccountId($id);

    // Set your charset:
    $ga->setCharset("UTF-8");

    // Set your hostname:
    // @TODO: domain integration.
    $ga->setHostName($GLOBALS['base_url']);

    // Set language:
    $ga->setLanguage($GLOBALS['language']->language);

    // Set an event based on:
    // http://code.google.com/apis/analytics/docs/tracking/eventTrackerGuide.html

    switch ($type) {
      case GA_PUSH_TYPE_EVENT:

        // Complete optional parameters:
        $push += array(
          'label' => '',
          'value' => 1,
        );

        $ga->setEvent($push['category'], $push['action'], $push['label'], $push['value']);
        break;

      /*
      case '_trackPageview': //TODO: is useful?

        //Set a pageview
        $ga->setPageView("/de/serverside/test");
        //Set page title
        $ga->setPageTitle("Test");

        break;
      */
    }

    // Push the event.
    $ga->create();

  }
  else {
    // @TODO: Throw an error
  }

}
