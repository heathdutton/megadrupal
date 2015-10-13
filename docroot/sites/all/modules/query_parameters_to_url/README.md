# Query Parameters to Clean URLs

Introduction
------------
This module provides the ability to rewrite URL query parameters into Clean URL components on specified paths.

Motivation
----------
Views exposed filters generates URLs with multiple query parameters, and the URL path gets unwieldy
fast if there are multiple filters and filter values used. Furthermore because the path contains
query parameters, it might impact SEO results.

An example of a dirty URL like that could be:

[http://example-site.com/events?field_category_id[0]=100&field_category_id[1]=101&field_author_name=John&field_author_surname=Doe]
(http://example-site.com/events?field_category_id[0]=100&field_category_id[1]=101&field_author_name=John&field_author_surname=Doe)

Using this module you can transform the URL into:

[http://example-site.com/events/p/field_category_id/0__100--1__101/field_author_name/John/field_author_surname/Doe]
(http://example-site.com/events/p/field_category_id/0__100--1__101/field_author_name/John/field_author_surname/Doe)

Isn't that a joy to look at?

Required modules
----------------
No additional module dependencies, except having Clean URLs enabled.


Configuration
-------------
Configuration form can be found at Admin -> Configuration -> Search And metadata -> Query Parameters To URL settings.

(URL -> [/admin/config/search/query-parameters-to-url](/admin/config/search/query-parameters-to-url)).

The following configuration options are present:

* Allow disabling all path rewriting by un-checking checkbox.
* Allow configuring which characters should be used for delimiting query parameters. Care should be taken, to only use
 characters that are valid in an URI path component, as well as making sure the characters don't normally appear in
 the URL, so that the decoding process is correct
 (refer to [RFC 3986](https://tools.ietf.org/html/rfc3986#section-3.3) for allowed characters details).
* Allow setting a regular expression which is used to determine on which paths query parameter rewriting should occur.
* Additional rewrite-enabled paths can be added by implementing **hook_query_parameters_to_url_rewrite_access()**.
* Allow rewriting the final encoded URLs by implementing **hook_query_parameters_to_url_rewrite_alter()**.
  This allows you renaming the query parameter keys and values to be shorter or more SEO friendly.
  See query_parameters_to_url.api.php for documentation and an example.
* Experimental feature to allow saving menu items with rewritten URLs that contain encoded query parameters.
* Enable caching of the rewritten URL pages by appling a patch to core.


Usage
-----
Enable the module, go to the configuration form, configure on which paths should rewriting occur, 
using a regular expression.

Example regular expressions:

* "{}" or "{.+}" - Enable query parameter rewriting on all Drupal paths.
* "" (empty) - Disable query parameter rewriting on all Drupal paths.
* "{^events|^news}" - Enable query parameter rewriting on all paths that start with events or news.
* "{^node/([0-9]+)/(.+)}" - Enable query parameter rewriting on all node paths (view, edit, etc).

Caching (how to fix)
-------
By default caching of rewritten URL pages will **NOT** work, because Drupal uses the Request URI as the key for the page 
cache, but the URI is rewritten by this module to make it work with Views, Better Exposed Filters, and other potential
modules. 

To make caching work again, a patch has to be applied to Drupal core, that allows rewriting the page cache key.
This patch can be found in patches/enhanced_page_cache-D7.34.patch or altenatively bundled with another module found at
[Enhanced Page Cache](https://www.drupal.org/project/enhanced_page_cache). Once the patch is applied,
the hook_page_cache_object_alter() will be picked up, and the rewritten page URL will be served from cache.
The bundled patch is a re-roll against the latest stable version of drupal core, of the one bundled with module above.


Implementation
--------------
The module uses three hooks to achieve its behavior:

* hook_url_outbound_alter().
* hook_url_inbound_alter().
* hook_init().

The first one is used to inspect links that go through l() or url(), check if they have query parameters, 
and rewrite the query parameters into clean URL components.

The second one does the opposite, when a user accesses a URL in the browser, it checks if the url contains any encoded
query parameters, and sets them back into $_GET. Because some modules (Better Exposed Filters for example) 
don't use the $_GET variable directly, and instead use request_uri(), the new path is also set into 
$_SERVER['REQUEST_URI'], as well as a few more server global variables.

The last hook is the most important one, because it is the one that makes clean urls work for Views Exposed Filters. 
Because Views uses a <form> tag with the action set to GET, there is no way to rewrite the URL, except in Javascript. 
That's why in hook_init() we check whether the entered URL has query parameters, and if it does, we simply issue a
redirect to the Clean URL version (using a Location header).


Considerations
--------------
* Because the module uses the url outbound hook, performance of the site can be slightly slower, depending on the 
 number of links that will be rewritten. This is a necessary slow-down, so that the module can actually work.
* To speed up things a bit, you can disable "additional path" hook executions, so that only the regular expression is
 used.
* Using the hook_query_parameters_to_url_rewrite_alter to rewrite the final encoded URLs, can decrease performance
 significantly, depending on what you do in the hook. For example calling node_loads for every outbound url will incur
 a performance hit. To mitigate this you can try and use static or DB caching depending on your business logic.
* When nested query parameter arrays are used, like [(/events?a[0][1][2]=3&a[3]=4)](/events?a[0][1][2]=3&a[3]=4), 
 the resulting clean URL has a lot of nested delimiters [(/events/p/a/0__1__2__3--3__4)](/events/p/a/0__1__2__3--3__4),
 which looks a bit ugly, but is necessary to decode the URL back into query parameters.

TODOs
-----
* Add DB caching for rewritten URLs, if this will improve speed, bypassing all hook invocations.
* Add tests for url rewriting hooks.
