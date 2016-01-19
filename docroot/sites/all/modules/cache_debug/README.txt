## REQUIREMENTS ##

- PHP 5.3 or greater

## INSTALLATION ##

Cache Debug is a Drupal Cache Handler. While it is not actually a cache storage system,
it wraps around an existing cache storage handler and logs the calls being made to it.

If you've used the Memcache or Redis modules before, this installation process should be
familiar to you.

 1. Edit settings.php to make Debug Cache the default cache class, for example:
      $conf['cache_backends'][] = 'sites/all/modules/cache_debug/cache_debug.inc';
      $conf['cache_default_class'] = 'DrupalDebugCache';

    Alternately, if you only want debug a specific cache bin, you can set DrupalDebugCache for that
    bin rather than for all of Drupal caching.
      $conf['cache_bootstrap_class'] = 'DrupalDebugCache';

2. Configure Cache Debug
   Cache Debug can be configured in one of two ways:
    * Through the UI by enabling the Cache Debug module. This gives you a user friendly interface
      to configure Cache Debug with but it does limit how grainular you can configure Cache Debug.
      This is good enough for most use cases.

    * Hard set configuration in settings.php using $conf.
      Cache debug lets you simply log cache calls and also stacktrace cache calls so that you can see
      what inside Drupal is making that cache call. This is really useful for tracking down calls to
      cache_get or cache_clear_all.

      Logging cache calls can be configured like this:
        $conf['cache_debug_log_set'] = TRUE;
        $conf['cache_debug_log_clear'] = TRUE;

      Logging stacktraces can be configured like this:
        $conf['cache_debug_stacktrace_set'] = TRUE;
        $conf['cache_debug_stacktrace_clear'] = TRUE;

      * NOTE: For stacktraces to work, logging for that cache call type must also be enabled.

3. (OPTIONAL): By default, Cache Debug will use the DrupalDatabaseCache Cache Handler as the storage
   mechanism. But if you want to use something else like memcache you can add it as the cache_debug_class.
      $conf['cache_backends'][] = 'sites/all/modules/memcache/memcache.inc';
      $conf['cache_debug_class'] = 'MemCacheDrupal';
      // cache_form bin should remain using Database caching.
      $conf['cache_cache_debug_form'] = 'DrupalDatabaseCache';


## ADDITIONAL CONFIGURATION ##
- Per-URL Debugging
  Cache Debug can be very verbose and can fill up log files very quickly. To reduce the logging, you can
  choose to only use cache debug on demand via a query parameter:

  // settings.php: 
  if (isset($_REQUEST['cache_debug'])) {
    $conf['cache_backends'][] = 'sites/all/modules/cache_debug/cache_debug.inc';
    $conf['cache_default_class'] = 'DrupalDebugCache';
  }

  With this setting, you can append ?cache_debug=<cache_breaking_token> to the end of any Drupal URL
  and get cache debugging output.

- Logging Modes
  Cache debug allows you to output logs to three different log types:
    * Watchdog
    * error_log
    * Arbitrary file

  error_log is the default choice as it logs to a file in a known location. Watchdog could also be used, BUT
  is strongly recommended against if you have dblog module enabled and the verbose logging will likely have 
  significant performance impact to your sight. However, watchdog method will work will with syslog module
  especially if you're aggregating syslogs across web clusters (for production use).

  Arbitrary file allows you to log to a location outside of the Drupal known filesystem. Beaware that Cache Debug
  log files can get very big, especially when left on for long periods of time and when used in production. Ensure you
  have log rotation setup for the log file and either compress and/or remove the files after a given period to ensure
  your servers to not run out of space.

