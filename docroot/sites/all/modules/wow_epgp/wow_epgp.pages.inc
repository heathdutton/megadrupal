<?php

/**
 * @file
 * User page callbacks for the EPGP module.
 */

/**
 * Menu callback; Displays EPGP table.
 */
function wow_epgp_page() {
  // TODO: use #attached property.
  drupal_add_css(drupal_get_path('module', 'wow') . '/css/wow.css');
  drupal_add_css('.non-eligible {opacity:.5}', 'inline');

  $header = array(
    'name' => array('data' => t('Name'), 'field' => 'c.name'),
    'ep' => array('data' => t('Effort Points'), 'field' => 'e.ep'),
    'gp' => array('data' => t('Gear Points'), 'field' => 'e.gp'),
    'pr' => array('data' => t('Loot priority'), 'field' => 'e.pr', 'sort' => 'desc'),
  );

  $query = db_select('wow_characters', 'c');
  $query->condition('gid', wow_guild_default('gid'))
        ->join('wow_epgp', 'e', '%alias.cid = c.cid');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query->fields('c', array('cid', 'region', 'realm', 'name', 'class'))
        ->fields('e', array('ep', 'gp', 'pr'))
        ->limit(50)
        ->orderByHeader($header);
  $result = $query->execute();

  $epgp = variable_get('wow_epgp', (object) array('uid' => 0, 'min_ep' => 0, 'base_gp' => 0, 'decay_p' => 0));
  $rows = array();
  $non_eligible = array();
  foreach ($result as $character) {
    $uri = wow_character_uri($character);
    $data = array('data' => array(
      '<strong>' . theme('wow_character_name', array('character' => $character, 'path' => "$uri[path]/epgp")) . '</strong>',
      $character->ep,
      $character->gp,
      round($character->pr, 2),
    ));

    if ($character->ep < $epgp->min_ep) {
      // Displays only significant accounts.
      if ($character->ep > $epgp->min_ep / 10) {
        $non_eligible[$character->cid] = $data + array('class' => array('non-eligible'));
      }
    }
    else {
      $rows[$character->cid] = $data;
    }
  }

  if (!empty($epgp->uid)) {
    $user = user_load($epgp->uid)->name;
    $time = format_interval(REQUEST_TIME - $epgp->timestamp);

    $build['user'] = array(
      '#markup' => '<p>' . t('Imported by !user @time ago', array('!user' => $user, '@time' => $time)) . '</p>',
    );
  }

  $build['characters'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows + $non_eligible,
    '#empty' => t('No data avialable.'),
    '#attributes' => array('class' => array('table')),
  );
  $build['pager'] = array('#markup' => theme('pager'));

  $build['min_ep'] = array(
    '#type' => 'fieldset',
    '#title' => '<strong>' . t('Min EP:') . '</strong> ' . $epgp->min_ep,
    '#collapsible' => TRUE,
  );
  $build['min_ep']['help'] = array(
    '#markup' => t('Min EP puts a limit, which members should exceed in order to have any loot priority. This is more or less an attendance check and a road block for new members to reach a point after which they can need on loot. Anyone below MinEP has in effect lower PR then everyone above MinEP even if their effective PR is higher.'),
  );

  $build['base_gp'] = array(
    '#type' => 'fieldset',
    '#title' => '<strong>' . t('Base GP:') . '</strong> ' . $epgp->base_gp,
    '#collapsible' => TRUE,
  );
  $build['base_gp']['help'] = array(
    '#markup' => t('Base GP adds a certain GP to each member of a guild. Base GP prevents new recruits or long time inactive members from trumping your dedicated members just because their GP is 0 and thus their PR infinite.'),
  );

  $build['decay'] = array(
    '#type' => 'fieldset',
    '#title' => '<strong>' . t('Decay:') . '</strong> ' . $epgp->decay_p . '%',
    '#collapsible' => TRUE,
  );
  $build['decay']['help'] = array(
  // TODO. Update doc with real value.
    '#markup' => t("In order to award recent effort more than past effort, which in effect avoids PR hoarding for veterans and enables new coming and dedicated members of a guild to be awarded properly, EPGP supports a decay mechanism. Decay simply removes a chunk of EP and GP from the totals, effectively leaving PR unchanged. For example with a decay of 10% applied each week, for each 7 days that pass your effort's rewards get diminished by 10%. The effort you put 10 weeks ago will only be worth 34.8% of what it did when you received it."),
  );

  return $build;
}

/**
 * Menu callback; Displays EPGP Character history.
 *
 * @param Character $character
 */
function wow_epgp_character_history_page($character) {
  $select = db_select('wow_epgp_history', 'e')
    ->fields('e', array('iid', 'created', 'gp'))
    ->condition('cid', $character->cid)
    ->orderBy('created', 'DESC')
    ->extend('PagerDefault');

  $ids = $select->execute()->fetchAllAssoc('iid');

  if (!empty($ids)) {
    $items = wow_item_load_multiple(array_keys($ids));
    $views = wow_item_view_multiple($items);
    $header = array(
      t('Date'),
      t('Item'),
      t('Price'),
    );

    $rows = array();
    foreach ($ids as $id => $loot) {
      $rows[] = array(
        t('!time ago', array('!time' => format_interval(REQUEST_TIME - $loot->created, 1))),
        drupal_render($views['items'][$id]),
        $loot->gp,
      );
    }

    $build['loots'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }
  else {
    $default_message = '<p>' . t('No items has been found yet.') . '</p>';

    $build['default_message'] = array(
      '#markup' => $default_message,
      '#prefix' => '<div id="first-time">',
      '#suffix' => '</div>',
    );
  }

  return $build;
}
