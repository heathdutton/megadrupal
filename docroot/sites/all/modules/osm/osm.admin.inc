<?php
/**
 * @file
 * Administration settings.
 */

/**
 * OSM basic settings.
 */
function osm_settings($form, &$form_state) {
  $form = array();

  $form['osm_token'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Token'),
    '#description'   => t('Your token api.'),
    '#default_value' => variable_get('osm_token', ''),
  );

  $form['osm_apiid'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Api id'),
    '#description'   => t('Your api id.'),
    '#default_value' => variable_get('osm_apiid', ''),
  );

  $form['osm_base_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Base url'),
    '#description'   => t('Base url for api calls.'),
    '#default_value' => variable_get('osm_base_url', 'https://www.onlinescoutmanager.co.uk/'),
  );

  return system_settings_form($form);
}

/**
 * OSM account manager.
 */
function osm_account_manager($form, &$form_state) {
  $form = array();

  // First setp, login the user.
  if (drupal_strlen(variable_get('osm_secret', '')) !== 32) {
    $form_state['step'] = 'user_login';

    $form['header'] = array(
      '#type'        => 'item',
      '#title'       => t('Authorisation'),
      '#description' => t('Please enter your OSM email address and password below.'),
    );

    $form['email'] = array(
      '#type'        => 'textfield',
      '#title'       => t('Email'),
      '#description' => t('Your email for OSM.'),
    );

    $form['password'] = array(
      '#type'        => 'password',
      '#title'       => t('Password'),
      '#description' => t('Your password.'),
    );

    $form['submit'] = array(
      '#type'  => 'submit',
      '#value' => t('Authorise now'),
    );
  }

  if (!variable_get('osm_active_roles', '')) {
    if ($all_roles = osm_get_users_roles()) {
      $form_state['step'] = 'role_selection';

      $form['header'] = array(
        '#type'        => 'item',
        '#title'       => t('Authorisation'),
        '#description' => t('Please select which sections should be enabled for use on this site.'),
      );

      foreach ($all_roles as $key => $value) {
        $options[$key] = $value['sectionname'] . ': ' . $value['groupname'];

        $form['osm_all_roles'] = array(
          '#type'    => 'checkboxes',
          '#title'   => t('OSM roles.'),
          '#options' => $options,
        );
      }

      $form['submit'] = array(
        '#type'  => 'submit',
        '#value' => t('Next'),
      );
    }
  }

  if (variable_get('osm_active_roles', '')) {
    $form_state['step'] = 'final_stage';

    $form['header'] = array(
      '#type'        => 'item',
      '#description' => t('Congratulations, the module has successfully connected to your Online Scout Manager account. If you want to add/remove sections, click the re-authorise button below. If you want new updates on OSM to appear immediately, use the Purge cache option below, otherwise they will appear on the site within a day.
      <p><b>If the data is not showing in the blocks or programme pages, please REMEMBER to go to Settings - External Access in OSM and give permissions to the OSM Drupal Module.</b></p>'),
    );

    /* $form['re_authorise'] = array(
    '#type'   => 'submit',
    '#value'  => t('Re-Authorise'),
    '#submit' => array('osm_reauthorise_submit'),
    );*/

    $form['purge'] = array(
      '#type'  => 'submit',
      '#value' => t('Re-Authorise & Re-sync'),
    );

    $form['blocks'] = array(
      '#type'        => 'item',
      '#title'       => t('<u>OSM Blocks</u>'),
      '#description' => t("The blocks below can be setup by going to structure > blocks, and then dragging the appropriate block onto the appropriate region. Once you've done that, save the new block settings, you can configure the block as well."),
    );

    $form['coming-up'] = array(
      '#type'        => 'item',
      '#title'       => t('Coming Up'),
      '#description' => t('Displays the next events and/or meetings.
      <p>By clicking <b>configure</b> you can select the <b>number of date entries</b>, whether to display <b>programme dates, event dates or both</b> and how you would like the <b>data format</b> shown.</p>'),
    );

    $form['patrol-points'] = array(
      '#type'        => 'item',
      '#title'       => t('Patrol Points'),
      '#description' => t('Simple block that displays your patrol/six/lodge point competition.'),
    );

    $form['osm-pages'] = array(
      '#type'  => 'item',
      '#title' => t('<u>OSM Pages</u>'),
    );

    // TODO: Ask Sam: better options to handle this list. This is an help area.
    // Better way is handle it with the hook_help drupal API.
    $form['programme'] = array(
      '#type'        => 'item',
      '#title'       => t('Programme Page'),
      '#description' => t("You can display your programme (date, title and summary) on a page.
      <p>Please follow these instructions to create a programme page:</p>
      <ul>
      <li>1. Go to 'Structure' and 'Content Types'</li>
      <li>2. Click 'Add Content Type'</li>
      <li>3. Give your new Content Type a name like 'Programme Pages'</li>
      <li>4. Click 'Save and add fields</li>
      <li>5. Add a new field, giving it a simple name like 'Section Code'</li>
      <li>6. Select the field type as 'OSM Code'</li>
      <li>7. Save the Content Type</li>
      <li>8. Go to 'Content' -  'Add Content'</li>
      <li>9. Select your newly created Content Type eg 'Programme Pages'</li>
      <li>10. Give your page a name eg 'Beaver Programme' and then in the 'Section Code' box, start typing the section name - eg Beaver</li>
      <li>11. Select the long OSM name that matches your section (eg beavers_1234_coming_up)</li>
      <li>12. Save the page - you should now have a working programme page!</li>
      <li>13. Programme settings (like layout and date types) can be edited by going to 'Content Types' - 'Manage Display'</li>
      </ul>
    "),
    );
  }

  return $form;
}

/**
 * Wizzard setup account configuration.
 */
function osm_account_manager_submit($form, &$form_state) {
  $values = $form_state['values'];

  if ($form_state['step'] == 'user_login') {
    if (empty($values['email']) && empty($values['password'])) {
      drupal_set_message(t('Please email and password need to be fill.'), 'warning');
      return FALSE;
    }
    else {
      osm_authorise($values['email'], $values['password']);
    }
  }

  if ($form_state['step'] == 'role_selection') {
    $all_roles = osm_get_users_roles();
    $active_roles = array();
    $terms = 0;

    foreach ($values['osm_all_roles'] as $role) {
      if ($role) {
        if (!$terms) {
          $terms = osm_perform_query('api.php?action=getTerms');
        }

        foreach ($terms[$role] as $term) {
          if ($term['past']) {
            $termid = $term['termid'];
          }
        }

        $active_roles[$role] = $all_roles[$role];
        $active_roles[$role]['termid'] = $termid;
      }
    }

    variable_set('osm_active_roles', $active_roles);
  }

  // TODO: FIX: bad way determine the buton. Finals action butons.
  if ($form_state['values']['op'] == t('Re-Authorise & Re-sync')) {
    variable_del('osm_active_roles');
  }

}

/**
 * Handles the 'Re-authorise' button on the account setup.
 *
 * @see osm_account_manager()
 * @see osm_account_manager_submit()
 */
function osm_reauthorise_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $node = $form['#node'];
  $form_state['redirect'] = array('admin/config/services/osm-account/reauthorise', array('query' => $destination));
}

/**
 * Re-Authorise account proccess.
 */
function osm_account_reauthorise($form, &$form_state) {

  return confirm_form($form,
    t('Are you sure you want to re-authorise the account?'),
    'admin/config/services/osm-account',
    t('Re-Authorise'),
    t('This action cannot be undone.'),
    t('Cancel')
  );
}
