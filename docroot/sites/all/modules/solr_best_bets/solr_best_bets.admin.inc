<?php

/**
 * @file
 * Administrative callbacks for the Solr Best Bets module.
 */

/**
 * Configure best bets.
 *
 * @ingroup forms
 */
function solr_best_bets_admin_form($form) {

  return $form;
}

/**
 * Configure access control to endpoints.
 *
 * @ingroup forms
 */
function solr_best_bets_advanced_form($form) {
  global $base_url;

  $form['solr_best_bets:access_xml'] = array(
    '#type' => 'checkbox',
    '#title' => t('Expose endpoint to retrieve elevate.xml file'),
    '#default_value' => variable_get('solr_best_bets:access_xml', 1),
    '#description' => t('If the endpoint is exposed, the elevate.xml file can be retrieved via a web request to solr/<em>environment_name</em>/elevate.xml, where <em>environment_name</em> is the machine name of the environment defined in hook_solr_best_bets_environment_info() implementations.'),
  );

  // Get endpoint URLs.
  $endpoints = '<br/>';
  $key = variable_get('solr_best_bets:key', 'drupal');
  $environments = solr_best_bets_get_environments();
  foreach ($environments as $environment) {
    $path = $base_url . '/solr/' . $environment['name'] . '/elevate.xml';
    $options = array('external' => TRUE, 'query' => array('key' => $key));
    $url = url($path, $options);
    $endpoints .= '<br><em>' . check_plain($environment['label']) . '</em>: <a href="' . $url . '">' . check_plain($url) . '</a>';
  }

  $form['solr_best_bets:key_restrict'] = array(
    '#type' => 'checkbox',
    '#title' => t('Restrict access to endpoint by a randomly generated key'),
    '#default_value' => variable_get('solr_best_bets:key_restrict', 1),
    '#description' => t('Prevent unwanted access to the elevate.xml files by requiring the requestor to pass a valid key through the <em>key</em> query string variable. Endpoints are listed below:!endpoints', array('!endpoints' => $endpoints)),
    '#states' => array(
      'visible' => array(
        ':input[name="solr_best_bets:access_xml"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form = system_settings_form($form);

  $form['actions']['regenerate_key'] = array(
    '#type' => 'submit',
    '#value' => t('Regenerate key'),
    '#submit' => array('solr_best_bets_regenerate_key_submit'),
  );

  return $form;
}

/**
 * Form submission handler for solr_best_bets_advanced_form().
 *
 * Redirects to confirmation form to regenerate the key.
 */
function solr_best_bets_regenerate_key_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/search/solr_best_bets/keygen';
}

/**
 * Confirmation form for key regeneration.
 *
 * @ingroup forms
 */
function solr_best_bets_keygen_confirm() {
  $question = t('Are you sure you want to regenerate the authorization key?');
  $path = 'admin/config/search/solr_best_bets/advanced';
  $description = t('This action regenerates the authorization key and cannot be undone. All endpoint requests must be updated using the newly generated key.');
  return confirm_form(array(), $question, $path, $description, t('Regenerate key'), t('Cancel'));
}

/**
 * Form submission handler for solr_best_bets_keygen_confirm().
 *
 * Regenerates the authorization key.
 */
function solr_best_bets_keygen_confirm_submit($form, &$form_state) {
  $key = drupal_hash_base64(drupal_random_bytes(55));
  variable_set('solr_best_bets:key', $key);
  drupal_set_message(t('The key has been regenerated: %key', array('%key' => $key)));
  $form_state['redirect'] = 'admin/config/search/solr_best_bets/advanced';
}
