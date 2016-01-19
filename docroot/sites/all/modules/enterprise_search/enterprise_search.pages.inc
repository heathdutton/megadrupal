<?php
/**
 * @file
 * The intererface implementations for the enterprise_search module.
 */

/**
 * Form callback for the index and core selection.
 */
function enterprise_search_index_form() {
  $solr = apachesolr_get_solr();
  $indicies = $solr->getIndicies();
  if (!$indicies) {
    return enterprise_search_no_indicies_message();
  }

  $options = array();
  $form = array();
  $markup = '<h3>Search Server</h3>';
  $markup = 'Create and manage your index(s) through your account at ';
  $markup = l(t('www.axistwelve.coml'), 'http://www.axistwelve.com/user', array('attributes' => array('target' => '_blank')));
  $markup = ". If you don't see an index you're expecting to, it may be that the Access Key you are using does not have permission to view it. You can amend this through your account.";
  $markup = "<br /><br />";
  $markup = "If you are using 'Enterprise Search' on more than one website, remember that you can either create a separate index for each site, or configure all sites to use a single common index for multi-site searching.";

  $form['a12_label'] = array(
    '#markup' => $markup,
  );

  $options[0] = t('Disabled');
  foreach ($indicies as $machine_name => $data) {
    $label = $data['label'];
    $description = $data['description'];
    if ($description) {
      $label .= ":";
    }

    $options[$machine_name] = "$label $description";
  }

  // If the stored index no longer exists, we need to default back to none.
  $default_index = variable_get('enterprise_search_apachesolr_index', 0);
  if (!isset($options[$default_index])) {
    variable_set('enterprise_search_apachesolr_index', 0);
    $default = 0;
  }

  $form['name'] = array(
    '#type' => 'radios',
    '#title' => "Index",
    '#options' => $options,
    '#default_value' => $default_index,
    '#required' => TRUE,
  );

  $core_options = array(
    "DEV" => "Development",
    "TEST" => "Test",
    "LIVE" => "Production",
  );

  $default_core = variable_get("enterprise_search_apachesolr_core", "DEV");
  if (isset($coreoptions[$default_core])) {
    $default_core = "DEV";
  }

  foreach ($indicies as $index) {
    $form['core'] = array(
      '#type' => 'radios',
      '#title' => 'Environment',
      '#options' => $core_options,
      '#default_value' => $default_core,
      '#required' => TRUE,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Set search server',
  );

  return $form;
}

/**
 * Form submit handler to save the current index and core the user is using.
 */
function enterprise_search_index_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  if (strtolower($values['name']) != variable_get
    ('enterprise_search_apachesolr_index') || $values['core'] != variable_get
    ('enterprise_search_apachesolr_core')
  ) {

    if (module_exists('solr_best_bets')) {
      // Deleted terms marked for deletion.
      db_delete('solr_best_bets')
        ->condition('status', 3)
        ->execute();

      // Set all best bets term status to undefined.
      $update = db_update('solr_best_bets')
        ->fields(array(
          'status' => 4,
        ));
      $update->execute();
      // Set global best bets synched flag.
      variable_set('enterprise_search_synonyms_sync', '0');
    }
  }

  variable_set('enterprise_search_apachesolr_index', strtolower($values['name']));
  variable_set('enterprise_search_apachesolr_core', $values['core']);

  try {
    $solr = apachesolr_get_solr();
    drupal_set_message(t("Index and core successfully selected."));
  } catch (A12ConnectionException $e) {
    drupal_set_message($e->getMessage(), 'error');
  }
}

/**
 * Provides a message to display when no indicies have been found.
 */
function enterprise_search_no_indicies_message() {
  $solr_link = l(t('Apache Solr module'), 'http://www.http://drupal.org/project/apachesolr');
  $a12_link = l(t('www.axistwelve.com'), 'http://www.axistwelve.com');
  $reasons = array(
    "Your subscription has expired or is currently set to 'offline'.",
    "The Access Key you are using is currently 'disabled' or has been deleted.",
    "The Access Key you are using does not have permissions to see any of your indices.",
    "You are not using the most recent version of the $solr_link",
  );

  $markup = "No indicies were found. This could be due to one of the following reasons:";
  $markup .= theme('item_list', array('items' => $reasons));
  $markup .= "Please review your settings through your account at $a12_link and try again.";

  $message = array(
    'message' => array(
      '#type' => 'markup',
      '#markup' => $markup,
    ),
  );

  return $message;
}

/**
 * Implements hook_form().
 */
function enterprise_search_features_page() {
  if (!module_exists('a12_elevated_search_results')) {
    $output = t('This enables you to configure the top results for any given query by associating content with specific keywords. To enable this feature please enable the !url module or
    click here to !readmore.',
      array(
        '!url' => l('Elevated Search Results', 'admin/modules',
          array('attributes' => array('target' => '_blank'))),
        '!readmore' => l('read more', 'http://axistwelve.com/elevated-search-results',
          array('attributes' => array('target' => '_blank')))
      ));
    $variables['element']['#attributes']['class'][] = 'collapsible';
    $variables['element']['#children'] = '';
    $variables['element']['#description'] = '';
    $variables['element']['#id'] = 'Elevated Search Results';
    $variables['element']['#title'] = 'Elevated Search Results';
    $variables['element']['#value'] = $output;
    $output = theme('fieldset', $variables);
  }
  else {
    $output = a12_elevated_search_results_markup();
  }
  return $output;
}
