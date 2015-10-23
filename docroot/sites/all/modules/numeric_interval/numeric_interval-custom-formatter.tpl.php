<?php
/**
 * @file
 * Template file of the custom formatter.
 *
 * Available variables:
 * - $pre
 *   The prefix what the admin added in the formatter settings page.
 * - $between
 *   The string between the first and the second value.
 * - $suff
 *   The suffix what the admin added in the formatter settings page.
 * - $val1
 *   - The first value.
 * - $val2
 *   - The last value.
 *
 * @author Kálmán Hosszu - http://drupal.org/user/267481
 */
?>
<span class="numeric-interval-prefix"><?php print $pre; ?></span>
<span class="numeric-interval-val1"><?php print $val1; ?></span>
<span class="numeric-interval-between"><?php print $between; ?></span>
<span class="numeric-interval-val2"><?php print $val2; ?></span>
<span class="numeric-interval-suffix"><?php print $suff; ?></span>