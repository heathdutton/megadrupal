<?php

/**
 * Timers interface.
 */
interface Profiling_Backend_Interface
{
  /**
   * Initiliaze the backend while the core is bootstraping.
   * 
   * @return int
   *   Timer delta. If multiple timers with the same name are being run then
   *   this will give you a unique index of the timer in order to stop the
   *   right one.
   */
  public function init();

  /**
   * Start single timer.
   * 
   * @param string $name
   *   Timer name.
   * @param string $collection
   *   (optional) Collection name. The collection name is display information
   *   only and do not interfere in anyway with timer behavior.
   * 
   * @return int
   *   The timer delta. Delta is a unique identifier related to name and will
   *   allow you to shutdown a single identifier timer if more than once are
   *   runned under the same name.
   */
  public function startTimer($name, $collection = PROFILING_DEFAULT_COLLECTION);

  /**
   * Stop single timer.
   * 
   * @param string $name
   *   Timer name.
   * @param int $delta = NULL
   *   (optional) Timer delta to stop, if non given, it will shutdown the
   *   last one. If no delta is given, it will shutdown the last one that
   *   has been started with the same name.
   * 
   * @return boolean
   *   FALSE in case of error, TRUE else.
   */
  public function stopTimer($name, $delta = NULL);

  /**
   * Stop all the timers and close whatever resources you opened while the
   * registered shutdown function is being called.
   * 
   * @return Profiling_Backend_Interface
   *   Self reference for chaining.
   */
  public function stopAllTimers();

  /**
   * Get all timers prior to database save, this is likely to be called after
   * the stopAllTimers() method has been called.
   * 
   * You can return an empty array here if your backend stores and manipulate
   * data itself without using the profiling database schema.
   * 
   * @return array
   *   Keys are internal name strings. Values are ordered numeric keyed arrays
   *   of timers using this name.
   *   Timers can be be either arrays either stdClass objects who's properties
   *   matches the profiling_timers table fields. The 'delta' property will
   *   be overriden.
   */
  public function getTimers();

  /**
   * Check if the current running environment can run this backend. This will
   * be called in administration section screen in order to guide the UI, but
   * also at hook_boot() time, this must have the minimal performance impact
   * possible.
   * 
   * In case of FALSE returned at hook_boot() the system won't proceed to
   * measures.
   * 
   * @return boolean
   *   TRUE if this backend can run on the current environment.
   */
  public function checkEnv();
}
