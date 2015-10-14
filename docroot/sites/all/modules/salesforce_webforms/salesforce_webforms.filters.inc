<?php

/**
 * @file
 * Data conversion filters.
 */

/**
 * Converts a webform date field into Salesforce format.
 *
 * @param string $date
 *   A date to be converted.
 *
 * @returns the filtered value in the form YYYY-MM-DD
 */
function salesforce_webforms_date_filter($date) {
  return date('Y-m-d', strtotime($date));
}

/**
 * Converts a date/time field into Salesforce format.
 *
 * @param string $datetime
 *   A date to be converted.
 *
 * @returns the filtered value in the form YYYY-MM-DDTHH:MM:SSZ
 */
function salesforce_webforms_datetime_filter($datetime) {
  return date('Y-m-d\TH:i:s\Z', strtotime($datetime));
}
