<?php
/**
 * @file
 * Menu callbacks for the Election module.
 */

/**
 * Page callback for listing elections.
 */
function election_page_list() {
  drupal_set_title(t('Elections'), PASS_THROUGH);

  // If the user is a site administrator, and there are no election type modules
  // enabled, prompt them to enable at least one.
  if (user_access('create elections') && user_access('administer modules')) {
    $types = election_types();
    if (!count($types)) {
      drupal_set_message(t("You need to enable an 'election type' module (see !Modules) in order to add elections.", array(
        '!Modules' => l(t('Modules'), 'admin/modules', array('fragment' => 'election_types')),
      )), 'warning');
    }
  }

  // Embed the View that lists elections.
  $view = views_get_view('elections');
  $view->override_path = current_path();
  return $view->preview('embed');
}
