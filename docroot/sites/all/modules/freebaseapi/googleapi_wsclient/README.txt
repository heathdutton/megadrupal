This module provides a number of presets for web services provided by
Google and found at https://developers.google.com/apis-explorer/#p/

These services are then made available to the Drupal wsclient module,
so that they can be interacted with using that API.

These can be built from the front-end UI also, and are provided here as
a convenience as examples of pre-cooked recipes.

To use the Freebase API 'Topic' lookup.
---------------------------------------

* Enable the this module, wsclient, wsclient_rest, and wsclient_tester
* Visit 
  /admin/config/services/wsclient/manage/freebase_topic_lookup/operation/topic/test
* Lookup an ID. A Freebase ID is usually of the form
  /en/bob_dylan - for wikipedia-style entries
  or 
  /m/0dgs16b - for every other topic

  
To use the MQL lookup
---------------------
  
Sample and experimental MQL queries can be run in the slightly better
interactive tool online at https://www.freebase.com/query

Once you have figured out the question to ask, you can now run such MQL 
yourself.
See the Freebase Schema request to see how to do this from code.

Eg: what are the properties of a Politician?

  $query_string = '[{
    "type": "/type/property",
    "schema": { "id": "/government/politician" },
    "id": null,
    "name": null
  }]';
  $fb_schema = freebase_api_run_mql_query($query_string);

or: Fetch a list of many music genres

  $query_string = '[{
    "name": null,
    "id": null,
    "type": "/music/genre",
    "limit": 500
  }]';
  $service = wsclient_service_load('freebase');
  $response = $service->invoke('mqlread', $array('query' => $query_string));
  $genres = $response['result']
  
