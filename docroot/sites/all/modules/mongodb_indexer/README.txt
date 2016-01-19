CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Features
 * Requirements
 * Installation & Configuration
 * Notes
 * Sponsors


INTRODUCTION
------------

Have you ever wondered if you could use Drupal as your CMS and other technology
to distribute contents to users? Mongodb indexer could well be a key part of
(such) a system that answers this question. MongoDB indexer sends your metadata
to MongoDB without interfering/altering your main storage engine. You can add
extra data to a mongo document; or alter a mongo document before it is sent to
MongoDB. In other words, you can denormalizing your data to your liking. "There
is a hook for that", of course.

A practical use case is a combination of: Drupal to manage your contents;
MongoDB indexer to send metadata to MongoDB clusters; Node.js to distribute
the contents from MongoDB clusters.


FEATURES
--------
* Selectable content types to be sent to MongoDB including custom entities,
  nodes, comments, taxonomy, users, and files.
* Ability to re-indexing a single node content type or a single bundle of
  a custom entity.
* Re-indexing operation are separated from normal indexing operation.
* Flexibility in setting up PHP MongClient connection in settings.php.
* Support for a single MongoDB instance or a Replica Set.


REQUIREMENTS
------------

* PHP 5.2 or greater
* PECL mongo >= 1.3.0


INSTALLATION & CONFIGURATION
----------------------------

* Download and install mongodb_indexer

* Edit settings.php
  - add and configure `mongodb_indexer_settings` variable. Note:
    `mongodb_indexer_settings` is an array consisting of three elements:
    `hosts`, `db`, and `options`.
  - `hosts`: an array to list your MongoDB server(s). A host string should
    contain `host:port`. For example: `127.0.0.1:27017`.
  - `db`: the name of the mongo database where the data is sent.
  - `options`: an array of options to construct MongoClient connection.
    Check <http://php.net/manual/en/mongoclient.construct.php> for more
    information.

  Setting Examples:

  - Settings for a single MongoDB instance:

        // MongoDB indexer settings.
        $conf['mongodb_indexer_settings'] = array(
          // Array of host:port of your MongoDB servers.
          // In this case, we need to list one MongoDB server.
          'hosts' => array(
            "127.0.0.1:27017"
          ),
          // The database name where data is stored.
          'db' => 'databasename',
          // Connection options
          // @link http://php.net/manual/en/mongoclient.construct.php
          'options' => array(
            // Example write concern.
            'w' => 1,
          ),
        );

  - Settings for a MongoDB Replica Set:

        // MongoDB indexer settings.
        $conf['mongodb_indexer_settings'] = array(
          // Array of host:port of your MongoDB servers.
          // You should list all of your memebers here.
          'hosts' => array(
            "127.0.0.1:27017",
            "127.0.0.1:27018",
            "127.0.0.1:27019",
          ),
          // The database name where data is stored.
          'db' => 'databasename',
          // Connection options
          // @link http://php.net/manual/en/mongoclient.construct.php
          'options' => array(
            // Replica Set name is required in this case
            'replicaSet' => 'yoursetname',
            // example write concern.
            'w' => 1,
          ),
        );

* Go to `admin/config/search/mongodb-indexer`
  - Your `mongodb_indexer_settings` variable is fine if you see the message
    "Connections to MongoDB are fine.".
  - Set the rest of the Mongodb Settings if needed.
  - Select contents you want to index to MongoDB.
  - Press `Save configuration`.


NOTES
-----

* MongoDB indexer comes with two drush commands:
  - `drush mi-mr` to select and mark content to re-indexing queue.
  - `drush mi-i` to start the Batch API operation to process re-indexing queue.

* This module does not ensure/create any MongoDB collection indexes. We think
  you would have more options/flexibility by creating your own indexes.

* The `Index items immediately` option on settings page tells the indexing
  processor to register a shutdown function to send data to MongoDB. So never
  run a large batch operation if the option is selected.

* MongoDB indexer does not take node status into account. All contents are sent
  to MongoDB regardless of their publishing status.


SPONSORS
--------

This project has been sponsored by:

* Exove Ltd <http://www.exove.com> a leading open source web design and
  development company in Finland and other Nordic countries, Baltics,
  and the UK.
