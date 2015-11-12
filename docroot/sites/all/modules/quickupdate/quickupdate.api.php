<?php

/**
 * @file
 * Hooks provided by Quick update module.
 */

/**
 * Allows to add more projects to the searching list.
 */
function hook_quickupdate_search_projects() {
  return array(
    'modules' => array(
      'views' => t('Views'),
    ),
    'themes' => array(
      'zen' => t('Zen'),
    ),
  );
}

/**
 * Allows modules to alter the projects data.
 */
function hook_quickupdate_search_projects_alter(&$projects) {
  $projects['modules']['views'] = t('Custom title');
  $projects['themes']['zen'] = t('Custom title');
}
