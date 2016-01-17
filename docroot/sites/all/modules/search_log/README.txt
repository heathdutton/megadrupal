
Description
-----------
The core Search module provides a simple list of top search phrases from the
watchdog log using the function dblog_top. However, most sites clear watchdog 
frequently. Search log stores search terms indefinitely and provides more 
robust reporting.

* Search log supports all modules which implement hook_search_info(). Modules 
  can be configured to be excluded from logging.

* Search log supports the search page, search block and theme search forms.
  By default, logging is performed at form submit before the search results are
  generated. Developers can implement a search_log hook to add additional 
  entries.

* (new 6.x, 7.x) Search log has an experimental feature to attempt to capture 
  failed searches. While most search reporting revolves around highlighting
  what is popular, identifying failed searches allows you to know what users 
  are looking for and not finding (and then potentially going elsewhere).

* (new 7.x) Search log provides a configurable Views block of Top searches.
  Clone the block to add additional Top Search blocks by time period, module.

* (new 7.x) Search log supports logging the user language with the query. Top 
  searches block can be configured to show only queries in the user's language.
  
* (new 7.x) Search log reporting integrates with Chart to provide some nice
  eye-candy.
  
* Search log reporting can be filtered by date, module and status.

* Search log table can be truncated at a user-specified interval or kept 
  indefinitely.


Dependencies
---------------
Search. Top searches block requires Views in 7.x.

Tested compatible with:
* Locale (core) 7.x-1.0
* Views 7.x-3.0-beta3
* Chart 7.x-1.0

Limited compatibility with:
* Custom Search 7.x-1.7 (blocks and custom_search_types)


Related Modules
---------------
Top Searches (http://drupal.org/project/top_searches), 
Zeitgeist (http://drupal.org/project/zeitgeist),
Google Analytics (http://drupal.org/project/google_analytics)
  modules that collect core search statistics. It has been asked what is the 
  difference between Search log and these modules. Fair question.
  
  If all you require is administrative reporting, Google Analytics does a 
  better job (cheaper, faster). If you want to display top searches as a block, 
  then consider the architecture differences of the modules. 
  
  * Top Searches aggregates queries by keyword and does not allow reporting
    over an arbitrary period. 
  
  * Zeitgeist records each query as a separate entry which adds database 
    overhead, but also allows it to display the last N queries. 
  
  * Search log aggregates queries by day which is similar to the core 
    Statistics module. In addition, Search log also records failed searches for 
    core search implementations without additional configuration. 
    
    It should be noted that failed search data could also be added to GA with 
    custom pageTracker code and some URL rewriting. (@see 
    http://www.google.com/support/forum/p/Google+Analytics/thread?tid=088275485caa5dce)

Apache Solr Statistics (http://drupal.org/project/apachesolr_stats) 
  beautiful administration implementation for Solr -- tip of the cap for 
  providing inspiration to support Chart module in Search log.

  
Usage
-------------
After enabling the module, Search log will automatically begin recording 
queries from the search page, search block and theme search forms. Additional
configuration is available here:
admin/config/search/search_log

Search log replaces the default Top search page at: 
admin/reports/search

Top Searches view block can be configured at:
admin/structure/views/view/search_log/edit


Author
------
John Money
ossemble LLC.
http://ossemble.com

Module development originally sponsored by Consumer Search, a service of 
About.com, a part of The New York Times Company.
http://www.consumersearch.com
