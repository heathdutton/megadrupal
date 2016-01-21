--- REQUIREMENTS ---
Before you can install this module you need to have downloaded the following contrib modules:
* Entity API 7.x-1.x (https://www.drupal.org/project/entity).
* Libraries API 7.x-2.x (https://www.drupal.org/project/libraries).
* X Autoload 7.x-5.x (https://www.drupal.org/project/xautoload).

To enable highlighting of the search term in the suggestions box you also need to install the highlight jQuery plugin
(http://github.com/bartaz/sandbox.js/raw/master/jquery.highlight.js) as a library in the sites/all/libraries/highlight
folder.

Optional modules that can provide suggestions to the Fast Autocomplete module are:
* Search API (https://www.drupal.org/project/search_api).
* Apachesolr (https://www.drupal.org/project/apachesolr).

--- INSTALLATION ---
The installation of the Fast Autocomplete is like any other Drupal module. After enabling the module you need to
configure it.

--- CONFIGURATION ---
After enabling the Fast Autocomplete module you can find the configuration form at /admin/config/search/fac. The first
thing you need to do is select a backend service for the module and on which input you want the Fast Autocomplete
suggestions to be enabled. You can also configure some other settings like cleaning up the JSON files here.

NOTE: the backend services for Search API and Apachesolr will only show up if you have actually installed the
corresponding modules.

After saving the basic configuration you can optionally configure the backend service at
/admin/config/search/fac/backend. The options in the form will differ per backend service.

The view mode of entities in the suggestions will be "default" unless you enable custom field settings for the "Fast
Autocomplete" view mode for the appropriate bundles using the Field UI module. For instance: to enable the "Fast
Autocomplete" view mode for the "Page" bundle of the "Node" entity_type you would go to:
/admin/structure/types/manage/page/display, open the "Custom display settings" vertical tab and enable the "Fast
Autocomplete" view mode.

--- ALTERS AND CUSTOM BACKEND SERVICE CLASSES ---
There are alter hooks available that you can implement in your own custom module to alter the queries of backend
services before they are executed or you can add your own custom backend service class.

To add your own custom backend service class you have to implement hook_fac_service_info() in your custom module and
create a service class that implements the SearchServiceInterface (or extends an existing service class).

More information on the available hooks is in the fac.api.php file and you can use the provided backend services as a
reference.

--- HOW THE MODULE WORKS ---
Basically the module is a jquery plugin that adds a behavior on inputs that uses the input to retrieve a JSON file with
suggestions for the term that is in the input through AJAX and show them in a suggestion box. The JSON files are
requested from the public files directory. Only when a suggestion file doest not exist, Drupal is bootstrapped and a
page callback will create the suggestions, store the JSON file and return them in the AJAX response.

The suggestions are created using a configurable backend service. The backend service can be basically anything that
returns an array of suggestions with a suggestion being an array of entity_type and entity_id. The "Fast Autocomplete"
view mode is used to render the suggestion in the result list, so the presentation of suggestions is configurable.

--- LIMITATIONS ---
Because of the architecture of the module to use public JSON files to provide suggestions as fast as possible there is
a security-based limitation that the search query for suggestions is performed as an anonymous user. Otherwise there
could potentially be an information leakage issue because one would be able to retrieve restricted information by
requesting the public JSON files.

If you deem the risk of information leakage to be mitigated or the effect of leakage low or non-existent you are able to
override this behavior by setting the variable "fac_anonymous_search" to FALSE in settings.php.