-- SUMMARY --

This module allows user-defined user-agents to bypass Drupal Core's page cache.
Additionally, when the page cache is bypassed, the existing page cache entry
for the page in question is updated with the latest rendering.


-- USE CASES --

* Setting a Googlebot user-agent will allow Google to always see fresh content
  no matter what. As an added bonus, by simply crawling your content, Google
  will be freshening up your page cache.
* Setting your minimum cache lifetime very high and regularly running a crawler
  using a custom user-agent, you can ensure that users always load a page from
  the cache and that the cache is always up-to-date.
* Use a browser plugin to modify your user-agent to always see fresh content on
  your site.


-- INSTALLATION --

* Enable the module
* Give appropriate roles the "administer ua cache bypass" permission.
* Add user-agents via the configuration page at
  /admin/config/development/ua_cache_bypass.
