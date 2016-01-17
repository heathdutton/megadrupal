<?php

/**
 * @file
 * provides the cronributer / vcard edit form
 */

/**
 * show and process contributor form
 *
 * @param string $operation_type
 *   (chooser|standalone)
 *   default:chooser
 * @param string $js
 *   (nojs|ajax)
 *   default:nojs
 *
 * @return string html
 */
function archibald_contributor_add($operation_type = 'chooser', $js = 'nojs') {
  return archibald_contributor_edit($operation_type, $js, NULL);
}

/**
 * show and process contributor form
 *
 * @param string $operation_type
 *   (chooser|standalone) default:chooser
 * @param string $js
 *   (nojs|ajax) default:nojs
 * @param string $contributor_id
 * @param string $step
 *
 * @return string html
 */
function archibald_contributor_delete($contributor_id = '') {

  drupal_set_title(t('Delete vCard'));
  if (empty($_POST)) {
    drupal_set_message(t('If you delete the vCard you have to republish all descriptions using this vCard.'), 'warning');
  }

  if (!empty($_REQUEST['contributor_id'])) {
    $contributor_id = $_REQUEST['contributor_id'];
  }

  return drupal_get_form('archibald_contributor_delete_form', $contributor_id);
}

/**
 * Returns the delete form for a contributor.
 *
 * @param array $form
 *   The drupal form
 * @param array $form_state
 *   The drupal form state.
 * @param string $contributor_id
 *   The contributor id to delete.
 *
 * @return array
 *   The form array.
 */
function archibald_contributor_delete_form($form, &$form_state, $contributor_id) {

  $contributor = new ArchibaldLomContributor($contributor_id);
  $c_data = $contributor->getData(TRUE);
  $c_name = trim($c_data['firstname'] . ' ' . $c_data['lastname'] . ' ' . $c_data['organisation']);

  $form = array();

  $form['info'] = array(
    '#markup' => '<div style="margin-bottom: 10px;">' . t('Do you really want to delete the vCard «@name»?', array(
      '@name' => $c_name,
    )) . '</div>',
  );

  $form['contributor_id'] = array(
    '#type' => 'hidden',
    '#value' => $contributor_id,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * Deletes all entries for the contributor, reset the lom cache and mark affected national published descriptions
 * to be republished.
 *
 * @param array $form
 *   The drupal form
 * @param array $form_state
 *   The drupal form state.
 */
function archibald_contributor_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == $form_state['values']['submit']) {
    $contributor_id = $form_state['values']['contributor_id'];
    $contributor = new ArchibaldLomContributor($contributor_id);
    $c_data = $contributor->getData(TRUE);
    $c_name = $c_data['firstname'] . ' ' . $c_data['lastname'] . ' ' . $c_data['organisation'];

    $linked_res = archibald_contributor_get_linked_resources($contributor_id);
    foreach ($linked_res AS $lom_id => $type) {
      archibald_need_republication($lom_id);
      archibald_insert_change_log($lom_id, '', t('vCard "@name" deleted.', array('@name' => $c_name)), '', 'DELETED_PLAIN');
    }

    $res = db_query('SELECT l.lom_id, l.version FROM {archibald_lom} l
      LEFT JOIN {archibald_lifecycle_contributes} lc ON (lc.lifecycle_id = l.lifecycle_id)
      LEFT JOIN {archibald_meta_metadata_contributes} mc ON (mc.meta_metadata_id = l.meta_metadata_id)
      JOIN {archibald_contribute_entity} ce ON (ce.contribute_id = lc.contribute_id OR ce.contribute_id = mc.contribute_id)
      WHERE ce.entity_id = :entity_id
      GROUP BY l.lom_id, l.version', array(
        ':entity_id' => $contributor_id,
      ));

    $affected_descriptions = $res->fetchAllAssoc('version', PDO::FETCH_ASSOC);

    $res = db_select('archibald_contribute_entity', 'ce')
      ->fields('ce', array('contribute_id'))
      ->condition('entity_id', $contributor_id)
      ->execute();

    while($row = $res->fetchAssoc()) {
      db_delete('archibald_contribute_entity')->condition('contribute_id', $row['contribute_id'])->execute();
      db_delete('archibald_lifecycle_contributes')->condition('contribute_id', $row['contribute_id'])->execute();
      db_delete('archibald_meta_metadata_contributes')->condition('contribute_id', $row['contribute_id'])->execute();
      db_delete('archibald_contribute')->condition('contribute_id', $row['contribute_id'])->execute();
    }
    db_delete('archibald_contributors')->condition('contributor_id', $contributor_id)->execute();

    foreach ($affected_descriptions AS $tmp_data) {
      cache_clear_all('lom:' . $tmp_data['lom_id'] . '__version:' . $tmp_data['version'], 'cache');
    }

    drupal_set_message(t('vCard deleted'));
  }
  $form_state['redirect'] = 'admin/archibald/other';
}

/**
 * show and process contributor form
 *
 * @param string $operation_type
 *   (chooser|standalone)
 *   default:chooser
 * @param string $js
 *   (nojs|ajax) default:nojs
 * @param string $contributor_id
 * @param string $step
 *
 * @return string html
 */
function archibald_contributor_edit($operation_type = 'chooser', $js = 'nojs', $contributor_id = NULL, $step = NULL, $self_lom_id = '') {

  archibald_set_header($operation_type);

  if (!empty($_REQUEST['contributor_id'])) {
    $contributor_id = $_REQUEST['contributor_id'];
  }

  if (!empty($contributor_id)) {
    // edit mode
    $contributor = new ArchibaldLomContributor($contributor_id);
    $vcard = $contributor->getVcardObject();
    $button_label = t('Edit vCard');
    // this means you only can have one add form at same time
    $object_id = $contributor_id;
  }
  else {
    // add mode
    $vcard = new ArchibaldAppDataVcard();
    $button_label = t('Add vCard');
    // this means you only can have one add form at same time
    $object_id = 'add';
  }

  drupal_set_title($button_label);

  $form_info = array(
    'id' => 'contributor',
    'path' => 'archibald_contributor/edit/' . $operation_type . '/' . ($js ? 'ajax' : 'nojs') . '/' . $contributor_id . '/%step',
    'show trail' => TRUE,
    'show back' => TRUE,
    'show cancel' => TRUE,
    'show return' => FALSE,
    'finish text' => t('Save'),
    'next callback' => 'archibald_contributor_form_next',
    'finish callback' => 'archibald_contributor_form_finish',
    'cancel callback' => 'archibald_contributor_form_cancel',
    // this controls order, as well as form labels
    'order' => array(
      'edit' => '',
    ),
    // here we map a step to a form id.
    'forms' => array(
      // e.g. this for the step at wombat/create
      'edit' => array(
        'form id' => 'archibald_contributor_edit_form',
      ),
    ),
  );

  if (empty($step)) {
    // We reset the form when $step is NULL because that means they have
    // for whatever reason started over.
    archibald_contributor_form_cache_clear($object_id);
    $step = 'edit';
  }

  // This automatically gets defaults if there wasn't anything saved.
  $object = archibald_contributor_form_cache_get($object_id, $contributor_id, $self_lom_id);

  $form_state = array(
    'ajax' => $js,
    // Put our object and ID into the form state cache so we can easily find it.
    'object_id' => $object_id,
    'object' => &$object,
    'contributor_id' => $contributor_id,
    'vcard' => $vcard,
    'operation_type' => $operation_type,
  );

  // Send this all to our form. This is like drupal_get_form only wizardy.
  ctools_include('wizard');
  $form = @ctools_wizard_multistep_form($form_info, $step, $form_state);
  $output = drupal_render($form);

  // Only check for republication if we are "editing" a contributor
  if (!empty($contributor_id) && !empty($form_state['complete'])) {

    foreach ($form_state['object']->linked_resources as $lom_id => $locations) {
      foreach ($locations as $location => $NULL) {
        // @TODO perhabs we should do this as batch job
        // http://www.hashbangcode.com/forum/playing-drupal-queues-538.html
        archibald_need_republication($lom_id);

        $name = trim($form_state['values']['firstname'] . ' ' . $form_state['values']['lastname']);
        if (empty($name)) {
          $name = trim($form_state['values']['organisation']);
        }
        archibald_insert_change_log($lom_id, '', t('vCard "@name"', array('@name' => $name)), '', 'CHANGED');
      }
    }
  }


  // If $output is FALSE, there was no actual form.
  if ($js == 'ajax') {
    // If javascript is active, we have to use a render array.
    $commands = array();
    if ($output === FALSE || !empty($form_state['complete'])) {
      // Dismiss the modal.
      if ($operation_type == 'standalone') {
        if (!empty($form_state['redirect'])) {
          $commands = array(ctools_ajax_command_redirect($form_state['redirect']));
        }
        else {
          $commands = array(
            ctools_modal_command_dismiss(),
            ajax_command_replace(
              '#archibald_choose_contributor_search_result',
              archibald_choose_contributor_search_page(
                @$_SESSION['archibald_contributor-last_query'], @$_SESSION['archibald_contributor-last_page'], $operation_type
              )
            ),
          );
        }

        drupal_add_js(array('archibald' => array(
          'new_contributor_id' => $form_state['object']->new_contributor_id,
        )), 'setting');

      }
      else {
        // display search result and mark edited items as choosen
        // cause of this by js the modal frame will dismissed
        $commands = array(
          ctools_modal_command_display(
            t('Choose vCard'),
            archibald_choose_contributor_search(@$_SESSION['archibald_contributor-last_query'], $operation_type, $form_state['object']->new_contributor_id)
          ),
        );
      }
      drupal_add_js(array('archibald' => array(
        'new_contributor_id' => $form_state['object']->new_contributor_id,
      )), 'setting');

    }
    elseif (!empty($form_state['cancel'])) {
      // If cancelling, return to the activity.
      $commands[] = ctools_modal_command_dismiss();
    }
    else {
      $commands = ctools_modal_form_render($form_state, $output);
    }

    $commands[] = array('command' => 'archibald_contributor_preview_hide');
    print ajax_render($commands);
    exit;
  }
  else {
    if ($output === FALSE || !empty($form_state['complete'])) {
      return "";
    }
    elseif (!empty($form_state['cancel'])) {
      if ($operation_type == 'standalone') {
        $form_state['redirect'] = 'archibald_contributor/standalone';
      }
      else {
        $form_state['redirect'] = 'archibald_contributor/choose/' . $contributor_id . '/' . @$object->self_lom_id;
      }
    }
    else {
      return $output;
    }
  }
}

// ---------------------------------------------------------------------------
// Wizard caching helpers.

/**
 * Store our little cache so that we can retain data from form to form.
 */
function archibald_contributor_form_cache_set($id, $object) {
  ctools_include('object-cache');
  ctools_object_cache_set('archibald_contributor_form', $id, $object);
}

/**
 * Get the current object from the cache, or default.
 */
function archibald_contributor_form_cache_get($id, $contributor_id = '', $self_lom_id = '') {

  ctools_include('object-cache');
  $object = ctools_object_cache_get('archibald_contributor_form', $id);
  if (!$object) {
    // Create a default object.
    $object = new stdClass;
    $object->start_contributor_id = $contributor_id;
    if (!empty($self_lom_id)) {
      $object->self_lom_id = $self_lom_id;
    }
  }

  return $object;
}

/**
 * Clear the wizard cache.
 */
function archibald_contributor_form_cache_clear($id) {
  ctools_include('object-cache');
  ctools_object_cache_clear('archibald_contributor_form', $id);
}

// ---------------------------------------------------------------------------
// Wizard in-between helpers; what to do between or after forms.

/**
 * Handle the 'next' click on the add/edit pane form wizard.
 *
 * All we need to do is store the updated pane in the cache.
 */
function archibald_contributor_form_next(&$form_state) {
  archibald_contributor_form_cache_set($form_state['object_id'], $form_state['object']);
}

/**
 * Handle the 'finish' click on teh add/edit pane form wizard.
 *
 * All we need to do is set a flag so the return can handle adding
 * the pane.
 */
function archibald_contributor_form_finish(&$form_state) {
  $form_state['complete'] = TRUE;
}

/**
 * Handle the 'cancel' click on the add/edit pane form wizard.
 */
function archibald_contributor_form_cancel(&$form_state) {
  $form_state['cancel'] = TRUE;
}

// ---------------------------------------------------------------------------
// Wizard forms for our simple info collection wizard.

/**
 * Wizard start form. Edit the contributor
 */
function archibald_contributor_edit_form($form, &$form_state) {
  if (empty($form_state['contributor_id'])) {
    $form_state['title'] = t('Add vCard');
  }
  else {
    drupal_set_message(t('If you edit the vCard you have to republish all descriptions using this vCard.'), 'warning');
    $form_state['title'] = t('Edit vCard');
  }

  $form['#attributes'] = array(
    'class' => array(
      'archibald-choose-contributor-add-form-' . $form_state['operation_type'],
    ),
  );

  $form['contributor_id'] = array(
    '#type' => 'hidden',
    '#value' => $form_state['contributor_id'],
  );

  $form['operation_type'] = array(
    '#type' => 'hidden',
    '#value' => $form_state['operation_type'],
    '#attributes' => array(
      'class' => array(
        'archibald_choose_contributor_form_operation_type',
      ),
    ),
  );

  $form['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#required' => FALSE,
    '#maxlength' => 180,
    '#default_value' => $form_state['vcard']->firstname,
  );

  $form['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#required' => FALSE,
    '#maxlength' => 180,
    '#description' => t('Please enter name of person or organisation.'),
    '#default_value' => $form_state['vcard']->lastname,
  );

  $form['organisation'] = array(
    '#type' => 'textfield',
    '#title' => t('Organisation'),
    '#maxlength' => 255,
    '#default_value' => $form_state['vcard']->organization,
  );

  $address = $form_state['vcard']->address;
  $form['country'] = array(
    '#type' => 'textfield',
    '#title' => t('Country'),
    '#maxlength' => 60,
    '#default_value' => @$address[0]['country'],
  );

  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => t('ZIP'),
    '#maxlength' => 20,
    '#default_value' => @$address[0]['zip'],
  );

  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#maxlength' => 180,
    '#default_value' => @$address[0]['city'],
  );

  $form['address1'] = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
    '#maxlength' => 255,
    '#default_value' => @$address[0]['street'],
  );

  $form['address2'] = array(
    '#type' => 'textfield',
    '#title' => t('Additional address'),
    '#maxlength' => 255,
    '#default_value' => @$address[0]['extendedaddress'],
  );

  $email = $form_state['vcard']->email;
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-Mail'),
    '#required' => FALSE,
    '#maxlength' => 255,
    '#default_value' => @$email[0]['value'],
  );

  $url = $form_state['vcard']->url;
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#required' => FALSE,
    '#maxlength' => 255,
    '#default_value' => @$url[0]['value'],
  );

  $telephone = $form_state['vcard']->telephone;
  $form['tel'] = array(
    '#type' => 'textfield',
    '#title' => t('Work telephone'),
    '#required' => FALSE,
    '#description' => t('Please enter phone number like following example +41313005500.'),
    '#maxlength' => 32,
    '#default_value' => @$telephone[0]['value'],
  );

  /**
   * HH 2012-05-04 vcard icons are not longer supported
   * CHA 2012-06-15 vcard enabled again
   */
  $default_value = NULL;
  if (isset($form_state['vcard']->logo->uri)) {
    $default_value = $form_state['vcard']->logo->uri;
  }


  preg_match('/archibald_file\/([0-9]+)\//', $default_value, $old_fid);
  $default_value = isset($old_fid[1]) ? $old_fid[1] : NULL;
  $form['logo'] = array(
    '#type' => 'managed_file',
    '#title' => t('Logo'),
    '#default_value' => $default_value,
    '#upload_validators' => array(
      'file_validate_extensions' => array(
        'png jpg jpeg gif'
      ),
    ),
    '#upload_location' => 'public://files/preview_image',
    '#field_suffix' => '<div class="archibald_preview_image" style_name="thumbnail">&nbsp;</div>',
  );

  return $form;
}

/**
 * search formular for archibald contributor add validator
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_contributor_edit_form_validate($form, &$form_state) {
  archibald_set_header();

  if (empty($form_state['values']['organisation'])) {
    if (empty($form_state['values']['firstname'])) {
      form_set_error('firstname', t('Enter first name'));
    }

    if (empty($form_state['values']['lastname'])) {
      form_set_error('lastname', t('Enter last name'));
    }
  }

  if (!empty($form_state['values']['email'])) {
    if (!valid_email_address(trim($form_state['values']['email']))) {
      form_set_error('email', t('Please enter a valid E-Mail address'));
    }
  }

  if (!empty($form_state['values']['url'])) {
    if (!valid_url(trim($form_state['values']['url']), TRUE)) {
      form_set_error('url', t('Please enter a valid URL'));
    }
  }
}

/**
 * search formular for archibald contributor add save handler
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_contributor_edit_form_submit($form, &$form_state) {
  $form_state['values']['tel'] = archibald_contributor_clean_tel($form_state['values']['tel']);
  $form_state['values']['email'] = trim($form_state['values']['email']);
  $form_state['values']['url'] = trim($form_state['values']['url']);

  ##### handle the logo
  if (!empty($form_state['values']['logo'])) {
    // The new file's status is set to 0 or temporary and in order to ensure
    // that the file is not removed after 6 hours we need to change it's status
    // to 1. Save the ID of the uploaded image for later use.
    $file = file_load($form_state['values']['logo']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);

    // When a module is managing a file, it must manage the usage count.
    // Here we increment the usage count with file_usage_add().
    file_usage_add($file, 'archibald', 'contributor_logo', 1);

    $form_state['values']['logo'] = archibald_file_get_url($file);
  }

  $lom_contributor = new ArchibaldLomContributor($form_state['values']['contributor_id']);
  $lom_contributor->renderDataObject($form_state['values']);

  $contributor_id = $lom_contributor->save();

  $form_state['object']->new_contributor_id = $contributor_id;

  $form_state['object']->linked_resources = archibald_contributor_get_linked_resources($contributor_id);

  // Get all lom_id's and the version for this contributor id to clear the cache.
  $sql = db_query('
    SELECT mc.meta_metadata_id, lc.lifecycle_id FROM {archibald_contribute_entity} AS ace
    LEFT JOIN {archibald_lifecycle_contributes} lc ON (ace.contribute_id = lc.contribute_id)
    LEFT JOIN {archibald_meta_metadata_contributes} mc ON (ace.contribute_id = mc.contribute_id)
    WHERE (mc.meta_metadata_id IS NOT NULL OR lc.lifecycle_id IS NOT NULL) AND ace.entity_id = :contributor_id
  ', array(
    ':contributor_id' => $contributor_id
  ));

  $clear_cache_cids = array();
  while ($row = $sql->fetchAssoc()) {

    $version_sql = db_select('archibald_lom', 'l')
      ->fields('l', array('version', 'lom_id'));

    // Lifecycle id is not empty so the join match is lifecycle id
    if (!empty($row['lifecycle_id']) && (int)$row['lifecycle_id'] > 0) {
      $version_sql->condition('lifecycle_id', $row['lifecycle_id']);
    }
    elseif (!empty($row['meta_metadata_id']) && (int)$row['meta_metadata_id'] > 0) {
      $version_sql->condition('meta_metadata_id', $row['meta_metadata_id']);
    }
    else {
      continue;
    }

    $clear_ids = $version_sql->execute()->fetchAssoc();
    $clear_cache_cids[] = 'lom:' . $clear_ids['lom_id'] . '__version:' . $clear_ids['version'];
  }

  foreach ($clear_cache_cids AS $cid) {
    cache_clear_all($cid, 'cache');
  }

  db_update('archibald_contribute_entity')
    ->fields(array('entity' => $lom_contributor->getVcard()))
    ->condition('entity_id', $contributor_id)
    ->execute();

  // remove to lom reousce with we currently edit from the list
  if (!empty($form_state['object']->self_lom_id) && !empty($form_state['object']->linked_resources[$form_state['object']->self_lom_id])) {
    unset($form_state['object']->linked_resources[$form_state['object']->self_lom_id]);
  }
}

/**
 * Display form to let user choose witch resoruce er like to update
 * Cause updated contriubter vCard was used in given lsit of resources
 */
function archibald_contributor_update_resource_form($form, &$form_state) {
  $form_state['title'] = t('Update descriptions linked to this contributor.');

  $options = array();
  foreach ($form_state['object']->linked_resources as $lom_id => $tmp) {
    $options[$lom_id] = ArchibaldLom::determinTitle($lom_id);
  }

  return $form;
}

/**
 * process users choise
 * update all marked resources
 */
function archibald_contributor_update_resource_form_submit(&$form, &$form_state) {


}

/**
 * get list of lom_id by contributor_id
 * where actual version lom resource contains a
 * vcard mathing with contributor_id
 *
 * @param string $contributor_id
 *
 * @return array
 */
function archibald_contributor_get_linked_resources($contributor_id) {
  $version_subquery = '
    SELECT la.version
    FROM archibald_lom la
    WHERE lom.lom_id=la.lom_id
    ORDER BY la.save_time DESC
    LIMIT 1';

  $result = array();

  // cat lom_id`s of matching lom resources for lifecycle
  $sql = 'SELECT lom.lom_id
    FROM {archibald_contribute_entity} as cont_entity
    INNER JOIN {archibald_contribute} cont ON (cont_entity.contribute_id = cont.contribute_id)
    INNER JOIN {archibald_lifecycle_contributes} lc_cont ON (cont.contribute_id = lc_cont.contribute_id)
    INNER JOIN {archibald_lom} lom ON (lc_cont.lifecycle_id = lom.lifecycle_id)
    WHERE cont_entity.entity_id=:contributor_id
    AND cont.type=:type
    AND lom.version = (' . $version_subquery . ') ';

  $q_res = db_query($sql, array(
      ':contributor_id' => $contributor_id,
      ':type' => 'lifecycle',
    ));

  while ($row = $q_res->fetchObject()) {
    $result[$row->lom_id]['lifecycle'] = TRUE;
  }

  // cat lom_id`s of matching lom resources for meta meta data
  $sql = 'SELECT lom.lom_id
    FROM {archibald_contribute_entity} as cont_entity
    INNER JOIN {archibald_contribute} cont ON (cont_entity.contribute_id = cont.contribute_id)
    INNER JOIN {archibald_meta_metadata_contributes} mm_cont ON (cont.contribute_id = mm_cont.contribute_id)
    INNER JOIN {archibald_lom} lom ON (mm_cont.meta_metadata_id = lom.meta_metadata_id)
    WHERE cont_entity.entity_id=:contributor_id
    AND cont.type=:type
    AND lom.version = (' . $version_subquery . ') ';

  $q_res = db_query($sql, array(
      ':contributor_id' => $contributor_id,
      ':type' => 'meta_metadata',
    ));

  while ($row = $q_res->fetchObject()) {
    $result[$row->lom_id]['meta_metadata'] = TRUE;
  }

  return $result;
}