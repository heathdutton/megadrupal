<?php

/**
 * @file
 * Data integrity checker admin pages.
 */

/**
 * Data integrity report page.
 */
function di_checker_report_page() {
  $output = '';

  $di_data = module_invoke_all('di_checker_info');

  if (count($di_data)) {
    foreach ($di_data as $cur_di) {
      if (!empty($cur_di['title'])) {
        $output .= '<h2>' . $cur_di['title'] . '</h2>';
      }
      if (!empty($cur_di['markup'])) {
        $output .= $cur_di['markup'];
      }
    }
  }
  else {
    $output = t('Data integrity issues not found');
  }

  return $output;
}
