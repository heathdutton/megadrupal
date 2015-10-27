Fivestar Recommender Documentation
==================================


Installation & Configuration
----------------------------

You need to install the Recommender API (7.x-6.x) module and Drupal Computing module before install this module.

After installation, follow these steps to compute recommendations:

  1. Run Drupal Cron to feed users voting data into recommender.
  2. Go to admin/config/system/computing/list, and click "Fivestar Recommender" to add a computing command.
  3. Compute recommendations using either of the following approaches:
    - Open a command line terminal and run "drush recommender-run".
    - Open a command line terminal and execute the Recommender Java agent.
    - Go to admin/config/system/computing/recommender and click "Run Recommender" (not recommended).
  . You can view the execution results at admin/config/system/computing/records.
  4. Display recommendation results using the default Views.

For more information about how the module works, please read the documentation of Recommender API.


Customization
-------------

Customization is required to be able to:

  1. Support entity types other than "node".
  2. Support tags other than "vote".
  3. Filter "content types" or "entity bundles" before recommendations computation.
  4. Use algorithms other than item-based collaborative filtering.
  
Customization requires mainly two things. First, you might need to create new tables in `hook_schema()` to save voting data and recommendation data. Second, you might need to change `fivestar_rec_process_vote()` to prepare voting data. Recommender API will take care of recommendation computation once voting data is prepared.
