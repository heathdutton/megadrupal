<?php

/**
 * @file
 * Admin page callback file for the epgp module.
 */

/**
 * Menu callback; Creates the import EPGP bulk data form.
 */
function wow_epgp_bulk_import() {
  $form['data'] = array(
    '#type' => 'textarea',
    '#title' => t('EPGP bulk data'),
    '#rows' => 20,
    '#required' => TRUE,
    '#description' => t('Copy/past the EPGP report directly from World of Warcraft.')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import data'),
  );

  return $form;
}

/**
 * Form validation handler for the epgp_bulk_import() form.
 */
function wow_epgp_bulk_import_validate($form, &$form_state) {
  try {
    $data = json_decode($form_state['values']['data']);

    if (empty($data)) {
      throw new Exception();
    }

    $guild = wow_guild_load_by_name($data->region, $data->realm, $data->guild);

    if (wow_guild_default('gid') == $guild->gid) {
      $members = db_select('wow_characters', 'c')
        ->fields('c', array('name', 'cid'))
        ->condition('c.gid', $guild->gid)
        ->execute()
        ->fetchAllAssoc('name');
      foreach ($data->roster as &$roster) {
        $name = $roster[0];
        if (array_key_exists($name, $members)) {
          $roster[0] = $members[$name]->cid;
        }
        else {
          $args = array('%name' => $name);
          form_set_error('data', t('Member %name was not found in the database. If the entry is recent please try again in a few hours, also, you can empty the cache and try again.', $args));
        }
      }

      $form_state['temporary']['data'] = $data;
      $form_state['temporary']['members'] = $members;
    }
    else {
      $guild = variable_get('wow_guild');

      $args = array(
        '%data_realm' => $data->realm, '!guild_realm' => $guild->realm,
        '%data_guild' => $data->guild, '!guild_name' => $guild->name,
      );
      form_set_error('data', t('These data are for another guild. Found %data_guild on %data_realm instead of !guild_name on !guild_realm.', $args));
    }
  }
  catch (Exception $exception) {
    form_set_error('data', t('Data are not valid.'));
    return;
  }
}

/**
 * Form submit handler for the epgp_bulk_import() form.
 */
function wow_epgp_bulk_import_submit($form, &$form_state) {
  $data = $form_state['temporary']['data'];
  $members = $form_state['temporary']['members'];

  $epgp = variable_get('wow_epgp', new stdClass());

  if (!isset($epgp->timestamp) || $epgp->timestamp < $data->timestamp) {
    $transaction = db_transaction();

    try {
      // Import roster data.
      foreach ($data->roster as $roster) {
        $cid = $roster[0];
        $ep = $roster[1];
        $gp = $roster[2];

        db_merge('wow_epgp')
          ->key(array('cid' => $cid))
          ->fields(array(
            'ep' => $ep,
            'gp' => $gp,
            'pr' => $ep / $gp,
          ))
          ->execute();
      }

      // Import loot data.
      $items = array();
      foreach ($data->loot as $loot) {
        $created = $loot[0];
        $name = $loot[1];
        $iid = $loot[2];
        $gp = $loot[3];

        db_merge('wow_epgp_history')
          ->key(array('created' => $created, 'cid' => key_exists($name, $members) ? $members[$name]->cid : 0))
          ->fields(array(
            'iid' => $iid,
            'gp' => $gp,
          ))
          ->execute();

        $items[$iid] = $iid;
      }

      $entities = wow_item_load_multiple(array_keys($items));

      // Fetch items which are not known yet from the database.
      foreach (array_diff_key($items, $entities) as $iid) {
        // TODO: Creates a batch process.
        $item = wow_item_fetch(wow_guild_default('region'), $iid, $GLOBALS['language_content']->language);
        $item->save();
      }

      $epgp->uid = $GLOBALS['user']->uid;
      foreach (array('min_ep', 'base_gp', 'decay_p', 'timestamp', 'extras_p') as $key) {
        $epgp->{$key} = $data->{$key};
      }
      variable_set('wow_epgp', $epgp);

      drupal_set_message(t('Data successfully imported.'));
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('wow_epgp', $e);
      throw $e;
    }
  }
  else {
    drupal_set_message(t('Data are already up to date.'));
  }
}
