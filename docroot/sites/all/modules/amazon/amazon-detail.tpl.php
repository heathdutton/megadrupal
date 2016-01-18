<?php

/**
 * @file
 * Outputs a single piece of data as provided in $$detail.
 *
 * This is used primarily for amazon_filter, where any arbitrary piece of data
 * from the $item can be accessed.
 */
if (!empty($detail) && !empty($$detail)) {
  print $$detail;
} else {
  print t('%detail not found for %asin', array('%detail' => $detail, '%asin' => $asin));
}
?>