<?php

/**
 * Processes variables for aggregator-wrapper.tpl.php.
 *
 * @see aggregator-wrapper.tpl.php
 */
function twbs_preprocess_aggregator_wrapper(&$variables) {
  $variables['pager'] = theme('pager');
}
