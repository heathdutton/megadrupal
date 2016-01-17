Cache Tags

An experimental project to associate cache entries with metadata, which then
enables developers to clear caches based on that metadata.

Setup (D7 version):

*  Install this project in sites/all/modules/cachetags.

*  Apply the core patch (will modify cache_set() to accept $tags):

     git apply sites/all/modules/cachetags/cachetags.patch

*  If using SQL cache, enable the cachetags_sql module, then add this to your
   settings.php:

     $conf['cache_backends'] = array('sites/all/modules/cachetags/cache-db.inc');
     $conf['cache_default_class'] = 'DrupalDatabaseCacheTagsPlugin';
     $conf['cache_tags_class'] = 'DrupalDatabaseCacheTags';

*  If using Mongo storage, install http://drupal.org/project/mongodb, (must be
   in sites/all/modules/mongodb) then add this to settings.php:

     $conf['cache_backends'] = array('sites/all/modules/cachetags/cache-mongo.inc');
     $conf['cache_default_class'] = 'DrupalMongoCacheTagsPlugin';
     $conf['cache_tags_class'] = 'DrupalMongoCacheTags';

*  To test, run the benchmark script:

     ./sites/all/modules/cachetags/benchmark.sh

See http://drupal.org/node/636454 for ongoing core development.


Cheers!
Carlos Rodriguez "carlos8f" <http://drupal.org/user/454578>
