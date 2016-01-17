<?php
/**
 * @file
 * Page callbacks and forms for shared domain list module.
 */

/**
 * Overview page for domain entities.
 */
function mailadmin_shareddomains_overview_page() {
  $build = array();

  // Provide list of links to all the domains.
  $links = array();
  foreach (mailadmin_shareddomain_load_all() as $id => $name) {
    $links[] = array(
      'title' => $name,
      'href' => 'mailadmin/shareddomain/' . $id,
    );
  }

  if (!empty($links)) {
    $build['links'] = array(
      '#theme' => 'links',
      '#links' => $links,
      '#attributes' => array('class' => 'domains'),
      '#heading' => array(
        'level' => 'h2',
        'text' => t('Select a domain to edit or delete it'),
      ),
    );
  }

  $build['add_new'] = array(
    '#markup' => l(t('Add new domain'), 'mailadmin/shareddomain/new'),
  );

  return $build;
}

/**
 * Editing form for domains.
 */
function mailadmin_shareddomains_edit_form($form, &$form_state, $domain = NULL) {
  if (is_null($domain)) {
    $domain = mailadmin_shareddomain_new();
  }

  $form['domain'] = array(
    '#type' => 'value',
    '#value' => $domain,
  );

  $form['domain_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain name'),
    '#default_value' => (!empty($domain->domain_name)) ? $domain->domain_name : NULL,
    '#required' => TRUE,
  );

  $form['buttons'] = array();

  $form['buttons']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );
  
  if (!empty($domain->is_new)) {
    $form['buttons']['save']['#value'] = t('Create domain');
  }
  else {
    $form['buttons']['delete'] = array(
      '#markup' => l(t('Delete this domain'), 'mailadmin/shareddomain/' . $domain->domain_id . '/delete'),
    );
  }

  return $form;
}

/**
 * Validation before saving a new domain.
 */
function mailadmin_shareddomains_edit_form_validate(&$form, &$form_state) {
  $name = trim(drupal_strtolower($form_state['values']['domain_name']));

  // Validate the domain name syntax.
  if (!preg_match('/^[a-z0-9][a-z0-9\.-]*$/', $name)) {
    form_set_error('domain_name', t('%name is not a valid domain name.', array('%name' => $name)));
  }

  // Enforce unique domains.
  $query = db_query('SELECT domain_id FROM {mailadmin_shareddomains} WHERE domain_name = :name AND domain_id != :id', array(
    ':name' => $name,
    ':id' => (integer) @$form_state['values']['domain']->domain_id,
  ));

  if ($query->rowCount()) {
    form_set_error('domain_name', t('%name already exists in the database.', array('%name' => $name)));
  }

  $form_state['values']['domain_name'] = $name;
}

/**
 * Submit handler saving a domain, new or existing.
 */
function mailadmin_shareddomains_edit_form_submit($form, &$form_state) {
  $domain = $form_state['values']['domain'];

  $new = (boolean) (isset($domain->is_new) && $domain->is_new);
  // Update the stored domain name.
  $domain->domain_name = $form_state['values']['domain_name'];

  mailadmin_shareddomain_save($domain);

  if ($new) {
    drupal_set_message(t('Domain “%name” was created.', array('%name' => $domain->domain_name)));
    watchdog('mailadmin_shareddomain', 'Domain “%name” was created.', array('%name' => $domain->domain_name));
  }
  else {
    drupal_set_message(t('Domain “%name” was updated.', array('%name' => $domain->domain_name)));
    watchdog('mailadmin_shareddomain', 'Domain “%name” was updated.', array('%name' => $domain->domain_name));
  }

  $form_state['redirect'] = 'mailadmin/shareddomain';
}

/**
 * Editing form for domains.
 */
function mailadmin_shareddomains_delete_form($form, &$form_state, $domain) {
  $form['domain'] = array(
    '#type' => 'value',
    '#value' => $domain,
  );

  return confirm_form($form, t('Delete the domain “%name”?', array('%name' => $domain->domain_name)), 'mailadmin/shareddomain/' . $domain->domain_id, t('Be aware that this will not remove any e-mail addresses and forwards enabled for this domain, but it will prevent creation of new ones.'), t('Delete'));
}

/**
 * Submit handler for the domain delete form.
 */
function mailadmin_shareddomains_delete_form_submit($form, &$form_state) {
  entity_delete('mailadmin_shareddomain', $form_state['values']['domain']->domain_id);

  drupal_set_message(t('Domain “%name” was deleted.', array('%name' => $form_state['values']['domain']->domain_name)));

  $form_state['redirect'] = 'mailadmin/shareddomain';

  watchdog('mailadmin_shareddomain', 'Domain “%name” was deleted.', array('%name' => $domain->domain_name));
}

