<?php

/**
 * @file
 * Contains TetherStatsChartsAnalyticsStorageInterface.
 */

/**
 * Interface for an analytics storage class.
 */
interface TetherStatsChartsAnalyticsStorageInterface {

  /**
   * Gets the total number of activities for a type.
   *
   * Counts the total number of times an activity of the given type has
   * occurred over the given period.
   *
   * If $step is provided, an array of counts for each step increment
   * is returned.
   *
   * Activity is aggregated in hourly chunks. This method uses the aggregated
   * hour counts for efficiency. However, without normalizing the $start before
   * calling this method, activity that was added before the first hour
   * turnover would be missed.
   *
   * @param string $activity_type
   *   The activity type as defined in TetherStatsAnalytics.
   * @param int $start
   *   The unixtime for the period start.
   * @param int $finish
   *   The unixtime for the period finish.
   * @param string $step
   *   (Optional) The domain step as defined in TetherStatsAnalytics.
   *   This could be 'hour', 'day', 'month' or 'year'.
   *
   * @return int|array
   *   Returns the total count if the $step is NULL, otherwise returns an
   *   array of the unixtime of the domain step to the count for that step.
   */
  public function getAllActivityCount($activity_type, $start, $finish, $step = NULL);

  /**
   * Gets the total number of activities on an event for a type.
   *
   * Counts the total number of times an activity of the given type has
   * occurred over the given period on the given element.
   *
   * If $step is provided, an array of counts for each step increment
   * is returned.
   *
   * Activity is aggregated in hourly chunks. This method uses the aggregated
   * hour counts for efficiency. However, without normalizing the $start before
   * calling this method, activity that was added before the first hour
   * turnover would be missed.
   *
   * @param int $elid
   *   The element ID.
   * @param string $activity_type
   *   The activity type as defined in TetherStatsAnalytics.
   * @param int $start
   *   The unixtime for the period start.
   * @param int $finish
   *   The unixtime for the period finish.
   * @param string $step
   *   (Optional) The domain step as defined in TetherStatsAnalytics.
   *   This could be 'hour', 'day', 'month' or 'year'.
   *
   * @return int|array
   *   Returns the total count if the $step is NULL, otherwise returns an
   *   array of the unixtime of the domain step to the count for that step.
   */
  public function getElementActivityCount($elid, $activity_type, $start, $finish, $step = NULL);

  /**
   * Gets the count for an element impressed on another element.
   *
   * Counts how many times a given element was impressed on another given
   * element.
   *
   * If $step is provided, an array of counts for each step increment
   * is returned.
   *
   * @param int $elid_impressed
   *   The element ID that was impressed.
   * @param string $elid_impressed_on
   *   The element ID that was impressed upon.
   * @param int $start
   *   The unixtime for the period start.
   * @param int $finish
   *   The unixtime for the period finish.
   * @param string $step
   *   (Optional) The domain step as defined in TetherStatsAnalytics.
   *   This could be 'hour', 'day', 'month' or 'year'.
   *
   * @return int|array
   *   Returns the total count if the $step is NULL, otherwise returns an
   *   array of the unixtime of the domain step to the count for that step.
   */
  public function getElementImpressedOnElementCount($elid_impressed, $elid_impressed_on, $start, $finish, $step = NULL);

  /**
   * Gets the count for an element impressed on a node bundle.
   *
   * Counts how many times a given element was impressed on any node
   * element for the given node bundle.
   *
   * If $step is provided, an array of counts for each step increment
   * is returned.
   *
   * @param int $elid_impressed
   *   The element ID that was impressed.
   * @param string $bundle
   *   The node bundle that was impressed upon.
   * @param int $start
   *   The unixtime for the period start.
   * @param int $finish
   *   The unixtime for the period finish.
   * @param string $step
   *   (Optional) The domain step as defined in TetherStatsAnalytics.
   *   This could be 'hour', 'day', 'month' or 'year'.
   *
   * @return int|array
   *   Returns the total count if the $step is NULL, otherwise returns an
   *   array of the unixtime of the domain step to the count for that step.
   */
  public function getElementImpressedOnNodeBundleCount($elid_impressed, $bundle, $start, $finish, $step = NULL);

  /**
   * Gets the count for an element impressed on a base URL.
   *
   * Counts how many times a given element was impressed on any element
   * with a url beginning with $base_url.
   *
   * If $step is provided, an array of counts for each step increment
   * is returned.
   *
   * @param int $elid_impressed
   *   The element ID that was impressed.
   * @param string $base_url
   *   The base url that was impressed upon.
   * @param int $start
   *   The unixtime for the period start.
   * @param int $finish
   *   The unixtime for the period finish.
   * @param string $step
   *   (Optional) The domain step as defined in TetherStatsAnalytics.
   *   This could be 'hour', 'day', 'month' or 'year'.
   *
   * @return int|array
   *   Returns the total count if the $step is NULL, otherwise returns an
   *   array of the unixtime of the domain step to the count for that step.
   */
  public function getElementImpressedOnBaseUrlCount($elid_impressed, $base_url, $start, $finish, $step = NULL);

  /**
   * Gets the count for an element impressed anywhere.
   *
   * Counts how many times a given element was impressed on any other
   * element.
   *
   * If $step is provided, an array of counts for each step increment
   * is returned.
   *
   * Activity is aggregated in hourly chunks. This method uses the aggregated
   * hour counts for efficiency. However, without normalizing the $start before
   * calling this method, activity that was added before the first hour
   * turnover would be missed.
   *
   * @param int $elid_impressed
   *   The element ID that was impressed.
   * @param int $start
   *   The unixtime for the period start.
   * @param int $finish
   *   The unixtime for the period finish.
   * @param string $step
   *   (Optional) The domain step as defined in TetherStatsAnalytics.
   *   This could be 'hour', 'day', 'month' or 'year'.
   *
   * @return int|array
   *   Returns the total count if the $step is NULL, otherwise returns an
   *   array of the unixtime of the domain step to the count for that step.
   */
  public function getElementImpressedAnywhereCount($elid_impressed, $start, $finish, $step = NULL);

  /**
   * Gets the count for all elements impressed on an element.
   *
   * Counts how many times any element was impressed on a given element.
   *
   * If $step is provided, an array of counts for each step increment
   * is returned.
   *
   * @param int $elid_impressed_on
   *   The element ID that was impressed upon.
   * @param int $start
   *   The unixtime for the period start.
   * @param int $finish
   *   The unixtime for the period finish.
   * @param string $step
   *   (Optional) The domain step as defined in TetherStatsAnalytics.
   *   This could be 'hour', 'day', 'month' or 'year'.
   *
   * @return int|array
   *   Returns the total count if the $step is NULL, otherwise returns an
   *   array of the unixtime of the domain step to the count for that step.
   */
  public function getAllElementsImpressedOnElementCount($elid_impressed_on, $start, $finish, $step = NULL);

  /**
   * Gets the count for how many hit activities occurred from the referrer.
   *
   * Counts how many times a "hit" activity occurred with the $referrer string
   * somewhere in the referrer field of the activity.
   *
   * If $step is provided, an array of counts for each step increment
   * is returned.
   *
   * @param string $referrer
   *   The string to match any part of the referrer string.
   * @param int $start
   *   The unixtime for the period start.
   * @param int $finish
   *   The unixtime for the period finish.
   * @param string $step
   *   (Optional) The domain step as defined in TetherStatsAnalytics.
   *   This could be 'hour', 'day', 'month' or 'year'.
   *
   * @return int|array
   *   Returns the total count if the $step is NULL, otherwise returns an
   *   array of the unixtime of the domain step to the count for that step.
   */
  public function getHitActivityWithReferrerCount($referrer, $start, $finish, $step = NULL);

  /**
   * Gets the count for how many hit activities occurred from the referrer.
   *
   * Counts how many times a "hit" activity occurred with the $referrer string
   * somewhere in the referrer field of the activity on a given element.
   *
   * If $step is provided, an array of counts for each step increment
   * is returned.
   *
   * @param int $elid
   *   The element ID where the hit activity occurred.
   * @param string $referrer
   *   The string to match any part of the referrer string.
   * @param int $start
   *   The unixtime for the period start.
   * @param int $finish
   *   The unixtime for the period finish.
   * @param string $step
   *   (Optional) The domain step as defined in TetherStatsAnalytics.
   *   This could be 'hour', 'day', 'month' or 'year'.
   *
   * @return int|array
   *   Returns the total count if the $step is NULL, otherwise returns an
   *   array of the unixtime of the domain step to the count for that step.
   */
  public function getElementHitActivityWithReferrerCount($elid, $referrer, $start, $finish, $step = NULL);

  /**
   * Gets the count for how many hit activities occurred using a browser.
   *
   * Counts how many times a "hit" activity occurred with the $browser string
   * somewhere in the browser field of the activity.
   *
   * If $step is provided, an array of counts for each step increment
   * is returned.
   *
   * @param string $browser
   *   The string to match any part of the browser string.
   * @param int $start
   *   The unixtime for the period start.
   * @param int $finish
   *   The unixtime for the period finish.
   * @param string $step
   *   (Optional) The domain step as defined in TetherStatsAnalytics.
   *   This could be 'hour', 'day', 'month' or 'year'.
   *
   * @return int|array
   *   Returns the total count if the $step is NULL, otherwise returns an
   *   array of the unixtime of the domain step to the count for that step.
   */
  public function getHitActivityWithBrowserCount($browser, $start, $finish, $step = NULL);

  /**
   * Gets the count for how many hit activities occurred using a browser.
   *
   * Counts how many times a "hit" activity occurred with the $browser string
   * somewhere in the browser field of the activity on a given element.
   *
   * If $step is provided, an array of counts for each step increment
   * is returned.
   *
   * @param int $elid
   *   The element ID where the hit activity occurred.
   * @param string $browser
   *   The string to match any part of the browser string.
   * @param int $start
   *   The unixtime for the period start.
   * @param int $finish
   *   The unixtime for the period finish.
   * @param string $step
   *   (Optional) The domain step as defined in TetherStatsAnalytics.
   *   This could be 'hour', 'day', 'month' or 'year'.
   *
   * @return int|array
   *   Returns the total count if the $step is NULL, otherwise returns an
   *   array of the unixtime of the domain step to the count for that step.
   */
  public function getElementHitActivityWithBrowserCount($elid, $browser, $start, $finish, $step = NULL);

}
