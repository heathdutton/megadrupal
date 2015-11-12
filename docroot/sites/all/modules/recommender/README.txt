Recommender API: Documentation
==============================


This documentation contains information about installation, administration, and sub-module development for Recommender API. For an overview of the module, please go to https://www.drupal.org/project/recommender.


Install
-------

First, install [Drupal Computing module](http://drupal.org/project/computing). Next, install the Recommender API module, following the general Drupal module installation process. Finally, install Recommender API sub-modules, such as [Browsing History Recommender](https://www.drupal.org/project/history_rec) or the Recommender Example sub-module.

You can choose to use either the PHP recommendation engine, or the Java recommendation engine. To use the Java engine, you also need to install these Java programs:

  * The Drupal Computing Java client library at: https://github.com/danithaca/drupal-computing
  * The Recommender API Java agent at: https://github.com/danithaca/drupal-recommender
  * Apache Mahout library at: http://mahout.apache.org/. _You might try [Machine Learning Libraries module](https://www.drupal.org/project/machine_learning)_


Compute and Display Recommendations
-----------------------------------

Make sure you have installed at least one sub-module before you can compute recommendations. You also need permissions "administer recommender" and "administer computing records".

First, submit a request to compute recommendations with required parameters (if any). Go to "Command Console" tab at "admin/config/system/computing/list", and click one of the links under the "Recommender Application" section. After specifying the parameters, click "Add Command" which will add a request to the queue to be processed later.

Next, compute recommendations using either the PHP engine or the Java engine. To use the PHP engine, run `drush recommender-run` on your Drupal server, or click "Run Recommender" button at "admin/config/system/computing/recommender", but the latter approach is not recommended because you might see PHP "Timeout" error or "Out of Memory" error. To use the Java engine, follow the instructions at https://github.com/danithaca/drupal-recommender.

Finally, display recommendations results using Views. Find the default views that shipped with the Recommender API sub-modules and enable them. You can customize the Views to fit your need.

Notes:

  * In the first step, if you set Drupal variable `recommender_show_database_option` to be `TRUE`, you will have an option to include `$database` settings in the request for recommendation computation. It is a convenient way to pass database access information to the Recommender Java agent. Use with caution because it might create potential security issues.
  * If you use the PHP engine in the second step, you might need to set `memory_limit` in php.ini to allow more RAM for recommendation computation.



Administrate
------------

Some Recommender API sub modules define "cron callback" functions that execute frequently to prepare data for the recommender engine. You can config it at "admin/config/system/computing/recommender" to run either with "Drupal Cron" or with a separate Drush command. To use the Drush command, you might need to add an entry to crontab like this:

    # runs recommender cron tasks every 10 minutes.
    */5 * * * * drush @self recommender-cron

You also need to run the "upkeep callback" functions regularly but not as frequently as the cron tasks. Such "upkeep" functions include cache refresh, database table cleanup, etc.

    # in crontab, run upkeep tasks at 5am every day
    0 5 * * * drush @self recommender-upkeep



Develop Sub-modules
-------------------

The best way to learn sub-module development is to look at the source code of Recommender Example module and/or other sub-modules such as [Browsing History Recommender](https://www.drupal.org/project/history_rec). The most important thing is to implement `hook_recommender_data()`. For example:

    function hook_recommender_data() {
      return array(
        'recommender_example_1' => array(
          'algorithm' => 'item2item',
          'data structure' => array(
            ...
          ),
          'options' => array(
            ...
          ),
        ),
        'recommender_example_2' => array(
          ...
        ),
      );
    }
    
The returned array is keyed by the machine name of recommender applications, whose values are arrays. The definition of the arrays is explained below.

  * __'title'__: The title of the recommender.
  * __'description'__: More descriptions about the recommender.

  * __'algorithm'__ (required): Specifies which recommender algorithm to be used. Currently both the PHP engine and the Java engine support 'user2user', 'item2item', 'user2user_boolean', and 'item2item_boolean'.
    - 'user2user': This is the classical user-based collaborative filtering algorithm.
    - 'user2user_boolean': This is the user-based CF algorithm where user preferences are booleans (e.g., node browsing histories).
    - 'item2item': This is the classical item-based CF algorithm.
    - 'item2item_boolean': This is the item-based CF algorithm where user preferences are booleans.
    
  * __'data structure'__ (required): Specifies how the recommendation engine accesses data, which includes the following:
    - __'preference'__: The users' preference data.
        - __'type'__: 'table' or 'file'
        - __'name'__: table name if type is 'table' or file name if type is 'file'
        - __'user field'__: the name of the 'user field' if type is 'table'
        - __'item field'__: the name of the 'item field' if type is 'table'
        - __'score field'__: the name of the 'score field' if type is 'table'
        - __'score type'__: whether the 'score field' is numeric data type (default), or 'boolean'.
        - __'timestamp field'__: the name of the 'timestamp field' if type is 'table'
        - __'no views'__ (optional): if TRUE, then Recommender API will not generate Views for this table. It is used to avoid conflict when the table is already exposed to hook_views_data().
      
    - __'user similarity'__: similar to 'preference', but uses the following fields instead:
        - __'user1 field'__: the name of the 'user1 field' if type is 'table'
        - __'user2 field'__: the name of the 'user2 field' if type is 'table'
      
    - __'item similarity'__: similar to 'preference', but uses the following fields instead:
        - __'item1 field'__: the name of the 'item1 field' if type is 'table'
        - __'item2 field'__: the name of the 'item2 field' if type is 'table'
    
    - __'prediction'__: similar to 'preference'
    
    - __'item entity type'__: Usually this is "node", or other Drupal entity type. This option is used for Views integration to display recommendations correctly.
    - __'user entity type'__: Usually this is "user", or other Drupal entity type. This option is used for Views integration to display recommendations correctly.

  * __'options'__: Specifies extra parameters. Might vary for different algorithms and implementations.
  
    - __'similarity algorithm'__ (optional, Java agent only): Specifies how to compute items/users similarities. Options are "Pearson" (default), "LogLikelihood" (default for boolean preference"), "Cosine", "Spearman", "Tanimoto", "Euclidean", and "CityBlock".
    - __'k nearest neighbors'__ (optional, Java agent only): The number of nearest neighbors to use in user/item recommendations.
    - __'min similarity score'__ (optional, Java agent only): Minimum similarity score to store in database.
    - __'max entities to keep'__ (optional, Java agent only): Specifies how many similar items, similar users, and predictions to save to the database (default: 20).
    - __'prediction skip invalid uid'__ (optional, Java agent only): TRUE or FALSE. If "TRUE" recommender will not compute predictions if uid is not valid. What counts as "invalid uid" is left for the discretion of the algorithm, usually it means non-positive number uid.

  * __'form elements callback'__ (optional): Defines a callback which returns form elements to take users input on 'options'.

  * __'cron callback'__ (optional): Defines a callback that executes in cron. Best used to prepare data for the "preference" table.

  * __'upkeep callback'__ (optional): Defines a callback that gets executed with "drush recommender-upkeep". These "upkeep" tasks should be table cleanup, cache refresh, etc that need to run periodically, but not as frequent as a cron task.

  * __'database'__ (optional, Java agent only): Specifies database connection info so Recommender Java agent can access data. You can either declare it in hook_recommender_data(), or use the `recommender_show_database_option`, or set them in the config.properties file of the Java agent. Options:
    - __'driver'__ (required): either mysql or postgresql. if it's other type, use "driveClassName" too.
    - __'username'__ (required): user name to connect to db
    - __'password'__ (required): user's password to connect to db
    - __'host'__ (required if 'url' is not set): the host name to connect to, default is 'localhost'
    - __'port'__ (optional): db port.
    - __'database'__ (required if 'url' is not set): database name to connect to.
    - __'url'__ (optional): JDBC "url" string. If set this, no need to set the others. This takes precedence over the others.


