This module provides a number of presets for web services provided by
Google and found at https://developers.google.com/apis-explorer/#p/

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
  