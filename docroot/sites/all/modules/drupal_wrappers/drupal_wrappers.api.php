<?php

/**
 * @file
 * API Documentation for our awesome Drupal wrappers.
 */

/**
 * drupal_sorta_equals takes two arguments and checks to see if they sorta (==)
 * equal each other. But, other modules may know better if these two things
 * equal each other so we give modules the ability to alter the result.
 *
 * @param bool $does_it_sorta_equal
 *   The current status of if the two things are equal.
 * @param mixed $cash
 *   The first thing in the comparison.
 * @param mixed $money
 *   The second thing in the comparison.
 */
function hook_sorta_equals_alter($does_it_sorta_equal, $cash, $money) {
  // Philosophically speaking, isn't everything equal in its own right, man.
  $does_it_sorta_equal = TRUE;
}