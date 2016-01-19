The DigitalNZ API is a collection of modules providing metadata search of
New Zealand digital content via the DigitalNZ web services API.

For more information about DigitalNZ see http://digitalnz.org

A DigitalNZ API key is required to use this module.

For API information and API keys, see http://digitalnz.org/developer

This module was produced at the DigitalNZ-sponsored Christchurch Hackfest, 12 September 2009.
DigitalNZ sponsored the DigitalNZ Block module.

Modules
- DigitalNZ API
This module provides settings for the DigitalNZ API endpoint including your API key.
It provides functions that directly call the web services API, but doesn't provide
end-user features.

- DigitalNZ Block
Complements standard Drupal search (e.g search/node) with results from DigitalNZ available in a block.

- DigitalNZ Search
This module enables an alternative search tab beside the default node and user search tabs to conduct
searches against the DigitalNZ metadata repository.

Installation
 - Copy the module to the desired directory, e.g sites/all/modules/digitalnz.
 - Go to Administer > Modules.
 - Enable the DigitalNZ API module.
 - Enter your API key at Administer > Site configuration > DigitalNZ API.
 - Enable DigitalNZ Search if you want to offer search to your site users.
 - Visit Administer > Configuration > Search settings to enable the DigitalNZ search.
 - You can change the number of rows returned at Administer > Configuration > DigitalNZ Search.

Usage
Once enabled, do a search on your Drupal site. Select the DigitalNZ tab to submit the same
search terms to DigitalNZ.

If you enable Digital Search, I recommend enabling the DigitalNZ Sorting block to allow a user to
sort search results returned from DigitalNZ. Set to show only on search/digitalnz/* paths.

If results are returned, you can sort them by various criteria:
Relevancy
Title
Type
Provider (DigitalNZ content partner)
Date

Hooks
DigitalNZ API offers a 'digitalnzapi_search' hook for overriding the search_text.
This can be used to limit the set of providers, or only return results of a certain type.
For example,

/**
 * Implementation of hook_digitalnzapi_search().
 */
function digitalnzblock_digitalnzapi_search(&$search_text) {
  $search_text .= ' AND category:Audio';
}

Roadmap
There are many features that could be added to this module. Feel free to offer patches or
suggestions via the Drupal.org issue queue for this module.

Jonathan Hunt
digitalnz_api.module@huntdesign.co.nz
http://huntdesign.co.nz