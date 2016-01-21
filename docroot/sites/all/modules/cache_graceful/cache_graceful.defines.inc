<?php
/**
 * @file
 */

/**
 * Default time, in seconds, to wait for cache
 */
define('CACHE_GRACEFUL_WAIT_FOR_CACHE', 5);

/**
 * Default time, in microseconds, to wait between cache get attempts
 * when waiting for cache
 */
define('CACHE_GRACEFUL_WAIT_FOR_CACHE_DELAY', 20000);

/**
 * Default time, in seconds, to lock prefetch
 */
define('CACHE_GRACEFUL_LOCK_TIMEOUT', 3600);

/**
 * Default time, in seconds, for expiration
 */
define('CACHE_GRACEFUL_EXPIRE', 60);

/**
 * Default time, in seconds, for prefetch
 */
define('CACHE_GRACEFUL_PREFETCH', 30);

/**
 * Default queueing option
 */
define('CACHE_GRACEFUL_QUEUE', FALSE);

/**
 * Default time, in seconds, for stale period
 */
define('CACHE_GRACEFUL_STALE', 30);

