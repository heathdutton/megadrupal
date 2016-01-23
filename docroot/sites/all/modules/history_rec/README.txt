Browsing History Recommender
============================


Installation & Configuration
----------------------------

You need to install the Recommender API (7.x-6.x) module and Drupal Computing module before install this module.

After installation, follow these steps to compute recommendations:

  1. Go to admin/config/system/computing/recommender/history_rec to review the settings, and make changes if necessary. Note that the "Use accesslog table" option is only valid if you enable the "Statistics" module in Core.
  2. Run Drupal Cron to feed data into recommender.
  3. Go to admin/config/system/computing/list, and click "Browsing History Recommender" to add a computing command.
  4. Compute recommendations using either of the following approaches:
    - Open a command line terminal and run "drush recommender-run".
    - Open a command line terminal and execute the Recommender Java agent.
    - Go to admin/config/system/computing/recommender and click "Run Recommender" (not recommended).
  5. You can view the execution results at admin/config/system/computing/records.
  6. Display recommendation results using the default Views.

For more information about how the module works, please read the documentation of Recommender API.


Technology Explanation
----------------------

This module uses Recommender API to compute recommendations, using the item-based collaborative filtering algorithm. The basic idea is that two items are related because users always browse them together, and items should be recommended to me because I have viewed other related items before. For more details about the algorithm, see the paper at http://portal.acm.org/citation.cfm?id=642471.

Users browsing history data are saved either in Drupal's built-in "history" table, or in the "accesslog" table if you enable the "statistics" module in core. The "history" table tracks registered users browsing history on nodes in the past 30 days. The "accesslog" table tracks all users (including anonymous users) browsing history on any links on the site in the past X days. Regardless of which table you use, this module will dump data in its own table for the purpose of recommendation computation. The benefit of using "accesslog" table is that you can use browsing history data from anonymous users.


Limitations
-----------

This module is **not** able to:
  
  * Make recommendations for entity types other than "node".
  * Use "comments" as browsing history (the "boost comments" feature was in previous releases, but not yet added here).
