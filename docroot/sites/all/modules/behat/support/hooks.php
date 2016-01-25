<?php
/**
 * Set up the test database environment
 */
 
$hooks->beforeFeature('', function($event) {
  // TODO
  // We would neither need this hook nor no evil globals
  // if there was a way to determine the feature name
  // in beforeScenario()
  
	$feature = $event->getFeature();
  $file = $feature->getFile();
  preg_match('/([\w]+).feature$/', $file, $matches);
  $GLOBALS['behat_current_feature'] = $matches[1];
});

$hooks->beforeScenario('', function($event) {
  $context = $event->getContext();
  // Set up Simpletest
  $test_id = db_insert('simpletest_test_id')
   ->fields(array('test_id' => rand(1000, 10000000)))
   ->execute();

  // Initialize Drupal context object from our own class
  $context->d = new BehatWebTestCase($test_id);
  $context->d->setUp($GLOBALS['behat_current_feature']);
});

$hooks->afterScenario('', function($event) {
  $context = $event->getContext();
  try {
    // Clean up database
    $context->d->tearDown();
  } catch (Exception $e) {
      return;
  }
});