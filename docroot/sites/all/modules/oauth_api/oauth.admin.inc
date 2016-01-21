<?php

/**
 * @file
 * Identity administration and module settings UI.
 */


/**
 * Return form for identity administration filters.
 */
function oauth_identity_filter_form($type) {
  $session = isset($_SESSION['oauth_identity_filter']) ? $_SESSION['oauth_identity_filter'] : array();
  $filters = oauth_identity_filters($type);

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only identities where'),
    '#theme' => 'exposed_filters',
  );
  $form['filters']['type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    if ($type = 'oid') {
      $value = oauth_load($value);
      $value = $value->title;
    }
    $t_args = array('%property' => $filters[$type]['title'], '%value' => $value);
    if ($i++) {
      $form['filters']['current'][] = array('#markup' => t('and where %property is %value', $t_args));
    }
    else {
      $form['filters']['current'][] = array('#markup' => t('where %property is %value', $t_args));
    }
    if (in_array($type, array('oid', 'user'))) {
      // Remove the option if it is already being filtered on.
      unset($filters[$type]);
    }
  }

  $form['filters']['status'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('clearfix')),
    '#prefix' => ($i ? '<div class="additional-filters">' . t('and where') . '</div>' : ''),
  );
  $form['filters']['status']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  $form['filters']['status']['filters']['oid'] = array(
    '#type' => 'select',
    '#options' => $filters['oid']['options'],
    '#title' => $filters['oid']['title'],
    '#default_value' => $type,
  );

  $form['filters']['status']['filters']['username'] = array(
    '#type' => 'textfield',
    '#title' => $filters['user']['title'],
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => $GLOBALS['user']->name,
  );

  $form['filters']['status']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['status']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => count($session) ? t('Refine') : t('Filter'),
  );
  if (count($session)) {
    $form['filters']['status']['actions']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['status']['actions']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  }

  drupal_add_library('system', 'drupal.form');

  return $form;
}

/**
 * Validate result from oauth identity administration filter form.
 */
function oauth_identity_filter_form_validate($form, &$form_state) {
  $account = user_load_by_name($form_state['values']['username']);

  if (empty($account)) {
    form_set_error('username', t('User name does not exists.'));
  }
}

/**
 * Process result from oauth identity administration filter form.
 */
function oauth_identity_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  $filters = oauth_identity_filters($form_state['values']['type']);

  switch ($op) {
    case t('Filter'):
    case t('Refine'):
      // Apply every filter that has a choice selected other than 'any'.
      foreach ($filters as $filter => $options) {
        if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
          $options = $filters[$filter]['options'];
          // Only accept valid selections offered on the dropdown, block bad input.
          if (isset($options[$form_state['values'][$filter]])) {
            $_SESSION['oauth_identity_filter'][] = array($filter, $form_state['values'][$filter]);
          }
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['oauth_identity_filter']);
      break;
    case t('Reset'):
      $_SESSION['oauth_identity_filter'] = array();
      break;
    case t('Update'):
      return;
  }

  $form_state['redirect'] = 'admin/people/identities';
  return;
}

/**
 * Menu callback: identity administration.
 */
function oauth_admin_people($form, $form_state, $type = '[any]') {
  $form['filter'] = oauth_identity_filter_form($type);
  $form['#validate'][] = 'oauth_identity_filter_form_validate';
  $form['#submit'][] = 'oauth_identity_filter_form_submit';
  $form['admin'] = oauth_admin_account();

  return $form;
}

/**
 * Form builder: Builds the identities administration overview.
 */
function oauth_admin_account() {
  $admin_access = user_access('administer oauth account');
  $header = array(
    'username' => array('data' => t('Username'), 'field' => 'o.uid'),
    'oauth' => array('data' => t('Application'), 'field' => 'o.oid'),
    'authname' => array('data' => t('Authname'), 'field' => 'o.authname'),
  );

  $query = db_select('oauthmap', 'o');
  oauth_identity_build_filter_query($query);

  $count_query = clone $query;
  $count_query->addExpression('COUNT(o.uid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('o', array('aid', 'uid', 'oid', 'authname'))
    ->limit(50)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();

  /*
  // Build the 'Update options' form.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
    '#access' => $admin_access,
  );
  $options = array();
  foreach (module_invoke_all('oauth_operations') as $operation => $array) {
    $options[$operation] = $array['label'];
  }
  $form['options']['operation'] = array(

  );

  */
  $options = array();
  foreach ($result as $identity) {
    $options[$identity->aid] = array(
      'username' => $identity->uid,
      'oauth' => $identity->oid,
      'authname' => $identity->authname,
    );
  }

  $form['identities'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No external identity available.'),
  );
  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}

/**
 * Extends a query object for oauth identity administration filters based on session.
 *
 * @param $query
 *   Query object that should be filtered.
 */
function oauth_identity_build_filter_query(SelectQuery $query) {
  // Build query
  $filter_data = isset($_SESSION['oauth_identity_filter']) ? $_SESSION['oauth_identity_filter'] : array();
  foreach ($filter_data as $index => $filter) {
    list($key, $value) = $filter;
    switch ($key) {
      case 'oid':
        if (is_numeric($value)) {
          $query->condition('o.oid', $value);
        }
        else {
          // Look-up IDs in oauth_type.
          $alias = $query->join('oauth_type', 'ot', "o.oid = %alias.oid");
          $query->condition($alias . '.type', $value);
        }
        break;
      case 'username':
        $query->condition('o.uid', user_load_by_name($value)->uid);
        break;
    }
  }
}

/**
 * Form builder: Builds the application administration overview.
 */
function oauth_admin_applications($type) {

  // Enable language column if translation module is enabled or if we have any
  // node with language.
  $multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {oauth} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());

  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'o.title'),
  );
  if ($multilanguage) {
    $header['language'] = array('data' => t('Language'), 'field' => 'o.language');
  }
  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('oauth', 'o')->extend('TableSort');
  $applications = $query
    ->fields('o', array('oid', 'title', 'language'))
    ->orderByHeader($header)
    ->execute()
    ->fetchAllAssoc('oid');

  // Prepare the list of applications.
  $languages = language_list();
  $destination = drupal_get_destination();
  $options = array();
  foreach ($applications as $application) {
    $l_options = $application->language != LANGUAGE_NONE && isset($languages[$application->language]) ? array('language' => $languages[$application->language]) : array();
    $options[$application->oid] = array(
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $application->title,
          '#href' => 'oauth/' . $application->oid,
          '#options' => $l_options,
        ),
      ),
    );
    if ($multilanguage) {
      if ($application->language == LANGUAGE_NONE || isset($languages[$application->language])) {
        $options[$application->oid]['language'] = $application->language == LANGUAGE_NONE ? t('Language neutral') : t($languages[$application->language]->name);
      }
      else {
        $options[$application->oid]['language'] = t('Undefined language (@langcode)', array('@langcode' => $node->language));
      }
    }
    // Build a list of all the accessible operations for the current node.
    $operations = array();
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => 'oauth/' . $application->oid . '/edit',
      'query' => $destination,
    );
    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => 'oauth/' . $application->oid . '/delete',
      'query' => $destination,
    );

    // Render an unordered list of operations links.
    $options[$application->oid]['operations'] = array(
      'data' => array(
        '#theme' => 'links__node_operations',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline')),
      ),
    );
  }

  // TODO: Add status column.
  $form['applications'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#empty' => t('No application available.'),
  );

  return $form;
}

/**
 * List identity administration filters that can be applied.
 */
function oauth_identity_filters($type = '[any]') {
  if ($type === '[any]') {
    $title = t('any');
  }
  else {
    $title = t('any !type application', array('!type' => $type));
  }

  // Application filter
  $filters['oid'] = array(
    'title' => t('Application'),
    'options' => array($type => $title) + oauth_application_get_titles($type),
  );

  // User filter
  $filters['user'] = array(
    'title' => t('User associated'),
  );

  return $filters;
}

/**
 * Returns a list of available application names.
 *
 * @param $type
 *   The application type to display.
 *
 * @return
 *   An array of oauth names, keyed by their ID.
 */
function oauth_application_get_titles($type = '[any]') {
  $query = db_select('oauth', 'o')
    ->fields('o', array('oid', 'title'));

  if ($type != '[any]') {
    $query->condition('type', $type);
  }

  return $query
    ->execute()
    ->fetchAllKeyed();
}

/**
 * OAuth identity selector form.
 *
 * @param $type
 *   The type of the application.
 */
function oauth_identity_form($type = '[any]') {
  $applications = oauth_application_get_titles($type);
  // If we have a value for the oid dropdown from $form_state['values'] we use
  // this both as the default value for the oid dropdown and also as a
  // parameter to pass to the function that generate the autocomplete path for
  // the authname textfield.
  $selected = isset($form_state['values']['oid']) ? $form_state['values']['oid'] : key($applications);

  $form['oid'] = array(
    '#type' => 'select',
    '#title' => t('Application'),
    '#options' => $applications,
    '#default_value' => $selected,
    // Bind an ajax callback to the change event (which is the default for the
    // select form type) of the oid dropdown. It will replace the authname
    // textfield when rebuilt
    '#ajax' => array(
      'callback' => 'oauth_identity_form_callback',
      'wrapper' => 'identity-authname',
    ),
  );

  $form['authname'] = array(
    '#type' => 'textfield',
    '#title' => t('Authname'),
    // The entire enclosing div created here gets replaced when wow_region
    // is changed.
    '#prefix' => '<div id="identity-authname">',
    '#suffix' => '</div>',
    '#autocomplete_path' => "oauth/$selected/autocomplete",
  );

  return $form;
}

/**
 * Selects just the authname textfield to be returned for re-rendering
 *
 * Since the controlling logic for populating the form is in the form builder
 * function, all we do here is select the element and return it to be updated.
 *
 * @return array
 *   The authname textfield.
 */
function oauth_identity_form_callback($form, $form_state) {
  return $form['authname'];
}