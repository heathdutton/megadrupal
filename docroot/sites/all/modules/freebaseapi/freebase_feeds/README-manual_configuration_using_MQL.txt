This module provides a Feeds Fetcher that consumes data from Freebase
and a mapper+processor that helps translate the responses into Feeds-compatible
Drupal data.


NON-automated setup, for making your own data queries and mapping the results
to your own content types.

Usage
=====

Enable:

* wsclient wsclient_rest http_client
* googleapi_wsclient
* Feeds
* Feeds JSONPath Parser https://www.drupal.org/project/feeds_jsonpath_parser

First: Construct a Freebase query that will produce a list of data items with
the fields you want.

eg, using an example from the Freebase Query editor
  https://www.freebase.com/query

"Films starring Kevin Bacon":

## MQL Query:

  [{
    "type": "/film/film",
    "only:starring": {
      "actor": {
        "name": "Kevin Bacon"
      }
    },
    "name": null,
    "imdb_id": [],
    "id": null,
  }]

Run this in the Query editor, and it returns

## Freebase Results:

  {
    "result": [
      {
        "only:starring": {
          "actor": {
            "name": "Kevin Bacon"
          }
        },
        "imdb_id": [
          "tt0102138"
        ],
        "type": "/film/film",
        "id": "/en/jfk",
        "name": "JFK"
      },
      {
        "only:starring": {
          "actor": {
            "name": "Kevin Bacon"
          }
        },
        "imdb_id": [
          "tt0077975"
        ],
        "type": "/film/film",
        "id": "/en/national_lampoons_animal_house",
        "name": "National Lampoon's Animal House"
      },
      ...
  }

Now ensure that we can get the same result through googleapi_wsclient.
Enable the wsclient_tester module.

On your site, visit the Google Freebase Web Sevice settings at
/admin/config/services/wsclient/manage/freebase

Select "MQL Read" : "Test"

Paste in the above query and "Execute Request"
You should see the same results as before come back.

This means you are ready to use that data query as a data source for Feeds.
Though we still will have to do some work to map the values usefully.
Because you can make any sort of query using MQL, and get all sorts of different
shaped results, mapping data from arbitrary lookups cannot be automated yet.

Create a new Feeds importer at /admin/structure/feeds/
EG 'movies_from_freebase'
* Set the "Periodic Import" off
* Change the fetcher to "Web Service Fetcher"
* In the fetcher settings, Choose "Operation" : "MQL Read"
* "Retain raw response data"
* Save - this will now show you extra Fetcher settings
* Enter the MQL Query here.
* Save and change the "Parser" to "JSONPath Parser"
* Now you have to inspect the shape of the response data.
  The only important bits to extract right now is the name and ID

  {
    "result": [
      {
        "name": "JFK"
        "id": "/en/jfk",
      },
      {
        "name": "National Lampoon's Animal House"
        "id": "/en/national_lampoons_animal_house",
      },
  }

  Using JSONPath, we can see that the "Context" will be "$.result.*"
  and the title and guid we want is just "name" and "id"
* Skip forward to the Processor and choose a bundle there.
* Under Processor Mappings, add two mappings,
  both with a source "JSONPATH Expression".
  One for Title and one for GUID. Make the GUID Unique.
* Return to the Parser settings and there will now be space to enter:
    "Context" = "$.result.*"
    "guid" = "id"
    "title" = "name"

Mostly done. If all goes well, you can now run the import from under
/import/ (eg /import/movies_from_freebase )

That should create a load of nodes named after movies for you.
You can proceed and add more fields to the query now, and map them to fields
of your own.



The Freebase query that fetches data can either be
 * simple (using basic form parameters)
 * complex (your own query using full Freebase MQL syntax)

