<?php
/**
 * @file
 * Admin campaign form.
 */

/**
 * Form constructor for managing and viewing campaign entries.
 *
 * @see pardot_admin_campaign_validate()
 * @see pardot_admin_campaign_submit()
 *
 * @ingroup forms
 */
function pardot_admin_campaign($form, &$form_state) {
  $form = array();
  $campaigns = db_query('SELECT * FROM {pardot_campaign}');

  foreach ($campaigns as $campaign) {
    //dsm($campaign);
    $form['campaigns'][$campaign->campaign_id] = array(
      '#campaign' => $campaign,
      'campaign_id' => array(
        '#markup' => $campaign->campaign_id,
        '#value' => $campaign->campaign_id,
      ),
      'name' => array(
        '#markup' => $campaign->name,
        '#value' => $campaign->name,
      ),
      'paths' => array(
        '#markup' => $campaign->paths, // TODO: this needs some kind of replacement function to recognize <front>
        '#value' => $campaign->paths,
      ),
    );
  }

  $form['new'] = array(
    'campaign_id' => array(
      '#prefix' => '<div class="add-new-placeholder">' . t('Campaign ID') . '</div>',
      '#type' => 'textfield',
      '#size' => 10,
      //'#theme_wrapper' => 'theme_pardot_admin_campaign_textfield',
      '#suffix' => '<div class="description">' . t('Numeric campaign code(piCId) from tracking code preview in Pardot administration interface.') . '</div>',
    ),
    'name' => array(
      '#prefix' => '<div class="add-new-placeholder">' . t('Campaign name') . '</div>',
      '#type' => 'textfield',
      '#size' => 25,
      '#suffix' => '<div class="description">' . t('A short descriptive name for administration purposes. Can be the same as the campaign name in Pardot but not required.') . '</div>',
    ),
    'paths' => array(
      '#prefix' => '<div class="add-new-placeholder">' . t('Add new path') . '</div>',
      '#type' => 'textarea',
      '#wysiwyg' => FALSE,
      '#suffix' => '<div class="description">' . t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are blog for the blog page and blog/* for every personal blog. %front is the front page.", array('%front' => '<front>')) . '</div>',
    ),
  );

  $form['add'] = array(
    '#prefix' => '<div class="add-new-placeholder">&nbsp;</div>',
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * Form validation handler for pardot_admin_campaign().
 *
 * @see pardot_admin_campaign_submit()
 */
function pardot_admin_campaign_validate($form, $form_state) {
  if (db_query("SELECT campaign_id FROM {pardot_campaign} WHERE campaign_id = :campaign_id", array(':campaign_id' => $form_state['values']['campaign_id']))->fetchField()) {
    form_set_error('campaign_id', t('Your Campaign ID much be unique'));
  }
}

/**
 * Form submission handler for pardot_admin_campaign().
 *
 * @see pardot_admin_campaign_validate()
 */
function pardot_admin_campaign_submit($form, $form_state) {
  $campaign = (object) $form_state['values'];
  drupal_write_record('pardot_campaign', $campaign);
}

/**
 * Theme callback for pardot_admin_campaign.
 *
 * @see pardot_admin_campaign()
 */
function theme_pardot_admin_campaign($variables) {
  $form = $variables['form'];
  if (isset($form['campaigns'])) {
    foreach (element_children($form['campaigns']) as $id) {
      $row = array();

      $row[] = drupal_render($form['campaigns'][$id]['campaign_id']);
      $row[] = drupal_render($form['campaigns'][$id]['name']);
      $row[] = drupal_render($form['campaigns'][$id]['paths']);

      $ops = array();
      $ops[] = l(t('Edit'), 'admin/config/services/pardot/campaign/' . $form['campaigns'][$id]['#campaign']->campaign_id . '/edit');
      $ops[] = l(t('Delete'), 'admin/config/services/pardot/campaign/' . $form['campaigns'][$id]['#campaign']->campaign_id . '/delete');
      $row[] = implode(' | ', $ops);
      $rows[] = $row;
    }
  }
  $rows[] = array(
    drupal_render($form['new']['campaign_id']),
    //theme(pardot_admin_campaign_textfield, $form['new']['campaign_id']),
    //_pardot_render_td_description($form['new']['campaign_id']),
    drupal_render($form['new']['name']),
    drupal_render($form['new']['paths']),
    drupal_render($form['add']),
    //'no_striping' => TRUE,
  );

  $headers = array(t('Campaign ID'), t('Name'), t('Path'), t('Operations'));
  $output = theme('table', array('header' => $headers, 'rows' => $rows));

  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Form constructor for editing campaign entries.
 *
 * @see pardot_admin_campaign_edit_submit()
 *
 * @ingroup forms
 */
function pardot_admin_campaign_edit($form, $form_state, $campaign) {

  $form = array();

  $form['campaign_id'] = array(
    '#type' => 'value',
    '#value' => $campaign->campaign_id,
  );
  $form['name'] = array(
    '#title' => t('Campaign name'),
    '#type' => 'textfield',
    '#default_value' => $campaign->name,
    '#description' => t('A short descriptive name for administration purposes. Can be the same as the campaign name in Pardot but not required.'),
  );
  $form['paths'] = array(
    '#title' => t('Paths'),
    '#type' => 'textarea',
    '#wysiwyg' => FALSE,
    '#default_value' => $campaign->paths,
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are blog for the blog page and blog/* for every personal blog. %front is the front page.", array('%front' => '<front>')),
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submission handler for pardot_admin_campaign_edit().
 */
function pardot_admin_campaign_edit_submit($form, &$form_state) {
  $campaign = (object) $form_state['values'];
  drupal_write_record('pardot_campaign', $campaign, array('campaign_id'));
  $form_state['redirect'] = 'admin/config/services/pardot/campaign';
}

/**
 * Form constructor for deleting campaign entries.
 *
 * @see pardot_admin_campaign_delete_submit()
 *
 * @ingroup forms
 */
function pardot_admin_campaign_delete($form, &$form_state, $campaign) {
  $form = array();
  //$form['#campaign_id'] = $campaign->campaign_id;

  $description = t('Are you sure you want to delete campaign %campaign_id? These paths will no logger trigger unique campaigns in Pardot.', array('%campaign_id' => $campaign->campaign_id));

  return confirm_form($form, t('Are you sure you want to delete this campaign?'), 'admin/config/services/pardot/campaign', $description, 'Delete');
}

/**
 * Form submission handler for pardot_admin_campaign_delete().
 */
function pardot_admin_campaign_delete_submit($form, &$form_state) {
  $campaign = $form_state['build_info']['args'][0];
  db_delete('pardot_campaign')
    ->condition('campaign_id', $campaign->campaign_id)
    ->execute();
  $form_state['redirect'] = 'admin/config/services/pardot/campaign';
}
