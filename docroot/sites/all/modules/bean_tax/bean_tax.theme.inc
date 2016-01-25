<?php
/**
 * @file
 * The theme system, which controls the output of Bean Tax.
 */

/**
 * Implements theme_bean_tax_more_link().
 */
function theme_bean_tax_more_link(&$variables) {
  return l($variables['text'], $variables['path'], array('attributes' => array('class' => array('more-link'))));
}
