<?php

/**
 * @file
 * Documentation and code examples for API developers.
 */

/**
 * This example shows how to use the BlizzardApiWowAuction class to save an
 * auction house dump file to the local system and then read the contents.
 *
 * If all is successful, output should be similar to:
 * @code
 * ItemID: 52185
 * Seller: ???
 * Bid: 33g 6s 0c
 * Buyout: 34g 80s 0c
 * Quantity: 20
 * TimeLeft: Less than 12 hours
 * @endcode
 *
 * @return mixed
 *   A string containing information on the first alliance auction listed, or
 *   FALSE if there was a problem retrieving the information.
 */
function blizzardapi_auction_example() {
  $auction = new BlizzardApiWowAuction('medivh');
  try {
    $auction->sendRequest();
  }
  catch (BlizzardApiException $e) {
    drupal_set_message(t('Unable to send API request.'), 'error');
    return FALSE;
  }
  
  // Get a default file path.
  $path = $auction->prepareDefaultPath();
  if ($path === FALSE) {
    drupal_set_message(t('Unable to setup destination directory.'), 'error');
    return FALSE;
  }
  
  if ($auction->isModified()) {
    // This method can also return a file object when using Drupal's managed
    // file system. For simplicity, this example uses an unmanaged file, and
    // therefore a path is retured.
    $path = $auction->downloadAuctionFile($path);
    if ($path === FALSE) {
      drupal_set_message(t('Unable to download auction file.'), 'error');
      return FALSE;
    }
  }
  
  // Reminder: Loading a large JSON file consumes a high amount of memory. For
  // example, a 3.75 MB file can consume 10x that amount in memory. Make sure
  // your PHP installation has enough available and if needed, do not load files
  // greater than a predetermined size.
  $json = drupal_json_decode(file_get_contents($path));
  $sample = $json['alliance']['auctions'][0];
  
  $bid = BlizzardApiWow::unpackCurrency($sample['bid']);
  $buyout = BlizzardApiWow::unpackCurrency($sample['buyout']);
  $timeleft = array(
    BlizzardApiWowAuction::TIME_LEFT_SHORT => 'Less than 30 minutes',
    BlizzardApiWowAuction::TIME_LEFT_MEDIUM => 'Less than 2 hours',
    BlizzardApiWowAuction::TIME_LEFT_LONG => 'Less than 12 hours',
    BlizzardApiWowAuction::TIME_LEFT_VERY_LONG => 'Greater than 12 hours'
  );
  
  $output = array(
    'ItemID: ' . $sample['item'],
    'Seller: ' . $sample['owner'],
    "Bid: {$bid->gold}g {$bid->silver}s {$bid->copper}c",
    "Buyout: {$buyout->gold}g {$buyout->silver}s {$buyout->copper}c",
    'Quantity: ' . $sample['quantity'],
    'TimeLeft: ' . $timeleft[$sample['timeLeft']]
  );
  
  return implode("\n", $output);
}

/**
 * This example shows how to customize API request caching.
 */
function blizzardapi_cache_example() {
  // Default cache lifetime of 1 day.
  $leaderboard = new BlizzardApiWowPvp(BlizzardApiWowPvp::BRACKET_RATEDBG);
  try {
    // First an example of how normal caching works.
    $data = $leaderboard->sendRequest()->getData();  // Retrieved from Blizzard.
    $data = $leaderboard->sendRequest()->getData();  // Retrieved from local cache.
    
    // When attempting to bypass the local cache and retrieve data from Blizzard,
    // the API will still try to optimize the request by checking if data has
    // been modified since the last request. If it has not, then data is loaded
    // from the cache and given an updated expiration time. Otherwise, new data
    // from Blizzard is retrieved and processed.
    $data = $leaderboard->sendRequest(TRUE)->getData();
    // Determine if new data was retrieved and perform an expensive operation.
    if ($leaderboard->isModified()) {
      drupal_set_message(t('Updating leaderboard...'));
    }
    
    // Now, an example when using a custom cache lifetime.
    $leaderboard->setCacheLifetime(172800);
    // It it also important to specify a cache prefix when specifying a
    // customized cache period. This prevents other modules from getting stale
    // data when making standard requests and also allows you to create data
    // groups if necessary. The prefix can be any string, but should usually
    // just be your module's name.
    $leaderboard->setCachePrefix('example:');
    $data = $leaderboard->sendRequest()->getData();
  }
  catch (BlizzardApiException $e) {
    watchdog_exception('blizzardapi', $e);
    return FALSE;
  }
  return $data;
}

/**
 * This example shows how to create a generic wrapper for sending API requests
 * that also displays a custom error message to the user and logs any exceptions
 * that occur.
 *
 * @param BlizzardApiWow $api
 *   A BlizzardApiWow object (which can be created using the factory class
 *   BlizzardApiWowResource) to obtain data from.
 * @param bool $refresh
 *   (optional) See BlizzardApi::sendRequest().
 * @param array $options
 *   (optional) See BlizzardApi::sendRequest().
 *
 * @return mixed
 *   Data from the specified API or FALSE if the request could not be completed.
 */
function blizzardapi_resource_example(BlizzardApiWow $api, $refresh = FALSE, $options = array()) {
  try {
    $api->sendRequest($refresh, $options);
  }
  catch (BlizzardApiException $e) {
    drupal_set_message(t('Unable to complete the Blizzard API request.'), 'error');
    watchdog('blizzardapi', 'API request from %path failed: %message.',
      array('%path' => $api->getApiPath(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
    return FALSE;
  }
  
  return $api->getData();
}
