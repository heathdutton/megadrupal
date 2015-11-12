<?php

namespace Ming\Examples;

/**
 * API Documentation
 *
 * Ming v1.x
 *
 * Ming is a wrapper around the PHP Mongo classes, which adds some additional
 * capabilities and behaviours. In addition to what raw Mongo support provides,
 * Ming has the following features:
 *
 *  - Preconfigure and connect to multiple persistent connections, from multiple
 *    Mongo database sources
 *  - Simple wrappers around common Mongo tasks (Insert, Upsert, Delete, Find)
 *  - Shortcuts for accessing objects by MongoID, without the need to instantiate
 *    MongoID classes
 *  - A scaffold Class (Item) for building, saving and working with data for
 *    storage.
 *  - Optional integration with the MongoDB module
 */

/**
 * Example 1: Connect to a Mongo database
 *
 * Settings should be provided as a keyed array, documented in hook_ming_settings(),
 * however this array can be provided as a simple settings array passed to
 * ming_db() as wel.
 *
 * @return \Ming\Core\Database $db
 */
function ming_example_connect() {

  // Start with your settings
  $settings = array(
    'mongo_host' => 'localhost', // default
    'mongo_port' => '27017', // default
    'mongo_user' => 'my_mongo_user', // defaults to NULL
    'mongo_pass' => 'my_mongo_password', // defaults to NULL
    'mongo_db' => 'my_db_name', // a default databse for this connection
  );

  // Load our MongoDB
  $db = ming_db(NULL, $settings);

  return $db;
}

/**
 * Example 2: Working with data in Ming
 */
function ming_example_working_with_data() {
  // Get our \Ming\Core\Database object
  $db = ming_example_connect();

  // Choose a collection to work with
  $db->collection('cars');

  // Prepare some data
  $data = array(
    'marque' => 'Rolls Royce',
    'model' => 'Silver Shadow',
    'year' => '1975',
  );

  // Insert some data
  $db->insert($data);

  // Insert some data in a 'safe' way
  // This returns an array containing the status of the insert.
  // See http://php.net/manual/en/mongocollection.insert.php for possible
  // return values, but note that at some point Ming will do its own error
  // handling.
  $result = $db->insert($data);

  // Update the first matched item
  // We now have two records for Silver Shadow's, this will only update one.
  $data = array(
    'marque' => 'Rolls Royce',
    'model' => 'Silver Shadow II',
    'year' => '1976',
  );
  $filter = array('marque' => 'Rolls Royce');
  $db->update($filter, $data);

  // Update all items
  // There are still two records, however they are now different. We'll do a
  // partial update on both. This will update only the field specified.
  $data = array(
    'origin' => 'United Kingdom',
  );
  $db->updateAll($filter, $data, TRUE);

}

