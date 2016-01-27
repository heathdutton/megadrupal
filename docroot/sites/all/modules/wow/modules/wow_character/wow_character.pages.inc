<?php

/**
 * @file
 * User page callbacks for the WoW Character module.
 */

/**
 * Menu callback; Manage WoW based identities for the specified user.
 */
function wow_character_user($account) {
  $header = array(
    'name' => array('data' => t('Name'), 'field' => 'c.name'),
    'status' => array('data' => t('Status'), 'field' => 'c.status', 'sort' => 'desc'),
    'description' => array('data' => t('Character'), 'field' => 'c.level'),
    'operations' => array('data' => t('Operations')),
  );
  $options = array();

  $select = db_select('wow_characters', 'c')->extend('TableSort');

  $select->condition('uid', $account->uid);

  $cids = $select
    ->fields('c', array('cid'))
    ->orderByHeader($header)
    ->execute()
    ->fetchCol();
  $characters = wow_character_load_multiple($cids);

  // Prepare the list of characters.
  $destination = drupal_get_destination();
  foreach ($characters as $character) {
    $l_options = array('attributes' => array('class' => array('character-c' . $character->class)));
    $uri = wow_character_uri($character);
    $options[$character->cid] = array(
      'name' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $character->name,
          '#href' => $uri['path'],
          '#options' => $l_options,
        ),
      ),
      'status' => $character->status ? t('active') : t('blocked'),
      'description' => theme('wow_character_description', array('character' => $character)),
    );

    if ($character->isMain) {
      $options[$character->cid]['name']['data']['#suffix'] = ' ' . t('(Is Main)');
    }

    // Build a list of all the accessible operations for the current node.
    $operations = array();
    if ($character->status) {
      // Check if user is allowed to set as main his character.
      if (wow_character_access('set main', $character, $account)) {
        $operations['set_main'] = array(
          'title' => t('set main'),
          'href' => 'user/' . $account->uid . '/characters/set-main/' . $character->cid,
          'query' => $destination,
        );
      }
    }
    else {
      $operations['unblock'] = array(
        'title' => t('unblock'),
        'href' => 'user/' . $account->uid . '/characters/unblock/' . $character->cid,
        'query' => $destination,
      );
    }

    $operations['remove'] = array(
      'title' => t('remove'),
      'href' => 'user/' . $account->uid . '/characters/remove/' . $character->cid,
      'query' => $destination,
    );

    $options[$character->cid]['operations'] = array(
      'data' => array(
        '#theme' => 'links',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline')),
      ),
    );
  }

  $build['character_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#attributes' => array('class' => array('table')),
  );
  $build['character_user_add'] = drupal_get_form('wow_character_user_add', $account);

  return $build;
}

/**
 * Form builder; Add a WoW character.
 *
 * @ingroup forms
 * @see character_user_add_validate()
 * @see character_user_add_submit()
 */
function wow_character_user_add($form, &$form_state, $account) {
  // During initial form build, add the entity to the form state for use during
  // form building and processing.
  if (!isset($form_state['user'])) {
    $form_state['user'] = $account;
  }

  $form['realm'] = array(
    '#type' => 'wow_realm',
    '#title' => t('Realm'),
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Character'),
    '#size' => 20,
    '#maxlength' => 12,
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Add a Character'));
  return $form;
}

/**
 * Validate the wow_character_user_add form.
 *
 * @ingroup forms
 */
function wow_character_user_add_validate($form, &$form_state) {
  try {
    $realm = $form_state['values']['realm'];
    $character_name = $form_state['values']['name'];
    $fields = module_exists('wow_guild') ? array('guild') : array();
    $character = wow_character_fetch($realm->region, $realm->slug, $character_name, $fields, array(), FALSE);
  }
  catch (WoWHttpException $exception) {
    switch ($exception->getCode()) {
      case 304:
        $character = wow_character_load_by_name($realm->region, $realm->slug, $character_name);
        break;
      case 404:
        $args = array('%name' => $character_name, '!realm' => drupal_strtoupper($realm->region) . '-' . $realm->name);
        form_set_error('name', t('%name not found on !realm. Please try again with a correct name.', $args));
        return;
      default:
        watchdog_exception('wow_character', $exception, t('An exception has been thrown during the %name character validation.', array('%name' => $character_name)));
        form_set_error('name', t('The website encountered an unexpected error. Please try again later.'));
        return;
    }
  }

  $select = db_select('wow_characters', 'c')
    ->fields('c')
    ->condition('realm', $realm->name)
    ->condition('region', $realm->region)
    ->condition('name', check_plain($character_name))
    ->execute();

  if ($select->rowCount() > 0) {
    // If user exists on the website. Check its status is not active.
    $character_db = $select->fetchObject();
    if ($character_db->status) {
      // User is active and in use on the website.
      $user = user_load($character_db->uid);
      $args = array('%name' => $character_name, '@username' => $user->name);

      if ($user->uid == $form_state['user']->uid) {
        form_set_error('name', t('%name is already in your character list.', $args));
      }
      else {
        form_set_error('name', t('%name is currently in use by @username in this website. You should contact an administrator if you think your character is used by somebody else.', $args));
      }
    }
  }

  $form_state['character'] = $character;
}

/**
 * Submit the wow_character_user_add form.
 *
 * @ingroup forms
 */
function wow_character_user_add_submit($form, &$form_state) {
  $character = $form_state['character'];
  $character->uid = $form_state['user']->uid;
  $character->save();

  drupal_set_message(t('%name has been successfully added to your character list.', array('%name' => $character->name)));
}

/**
 * Menu callback; Removes the specified character from your list of characters.
 */
function wow_character_user_remove($form, $form_state, $account, $cid = 0) {
  $name = db_query("SELECT name FROM {wow_characters} WHERE uid = :uid AND cid = :cid", array(
    ':uid' => $account->uid,
    ':cid' => $cid,
  ))
  ->fetchField();
  return confirm_form(array(), t('Are you sure you want to remove the character %name ?', array('%name' => $name)), 'user/' . $account->uid . '/characters');
}

/**
 * Form handler: Submit handler for the wow_character_user_remove.
 */
function wow_character_user_remove_submit($form, &$form_state) {
  $character = wow_character_load($form_state['build_info']['args'][1]);
  $character->original = clone $character;
  $character->status = 0;
  $character->isMain = 0;
  $character->uid = 0;
  $character->save();

  drupal_set_message(t('Character removed.'));
  $form_state['redirect'] = 'user/' . $form_state['build_info']['args'][0]->uid . '/characters';
}

/**
 * Menu callback; Set a character main status.
 */
function wow_character_set_main($character) {
  $character->original = clone $character;
  $character->isMain = 1;
  $character->save();

  drupal_goto();
}

/**
 * Menu callback; Unblock the specified character by invoking a method.
 */
function wow_character_user_unblock($form, &$form_state, $account, $cid = 0) {
  $character = wow_character_load($cid);
  drupal_set_title(t('Unblock the character %name.', array('%name' => $character->name)), PASS_THROUGH);

  try {
    $character->fetch(array('items'), array(), FALSE);
  }
  catch (WoWHttpException $exception) {
    if ($exception->getCode() != 304) {
      watchdog_exception('wow_character', $exception, t('An exception has been thrown during %name character unblocking operation.', array('%name' => $character->name)));
      drupal_set_message(t('The website encountered an unexpected error. Please try again later.'), 'error');
      return $form;
    }
  }

  $form['character'] = array(
    '#type' => 'value',
    '#value' => $character,
  );

  $form['description'] = array(
    '#markup' => '<p>' . t('To unblock you character:') .
      '<ol><li>' . t('Launch World of Warcraft and log in.') . '</li>' .
      '<li>' . t('Connect your character, remove items as described bellow.') . '</li>' .
      '<li>' . t('Log out to save modifications.') . '</li>' .
      '<li>' . t('Come back to the website and click on the Unblock button bellow.') . '</li></ol>' .
      t('This will ensure you are the owner of the character and unblock it.') . '</p>',
  );

  // Build up a list of available slots.
  $slots = array();
  foreach ($character->items as $key => $item) {
    if (!in_array($key, array('averageItemLevel', 'averageItemLevelEquipped'))) {
      $slots[$key] = $item;
    }
  }

  // Only display the form if more than 7 slots are available to pick up.
  if (count($slots) > 6) {

    $form['remove']['slot_options'] = array('#type' => 'value', '#value' => array());
    foreach (array_rand($slots, 4) as $key) {
      $form['remove'][$key] = array(
        '#type' => 'item',
        '#markup' => '<p><strong>' . t('Remove !slot', array('!slot' => t($key))) . ': </strong>' . $slots[$key]['name'] . '</p>',
      );
      $form['remove']['slot_options']['#value'][] = $key;
    }

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Unblock Character'));

    // Cache the form.
    $form_state['cache'] = TRUE;
  }
  else {
    drupal_set_message(t('Your character must be equipped in all his slots in order to check that you are the owner. Log your character and stuff him, once he is full stuff, disconnect to save modifications, then reload the form.'), 'error');

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array('#type' => 'button', '#value' => t('Refresh'));
  }

  return $form;
}

/**
 * Form builder: Validation handler for the wow_character_user_unblock_form.
 */
function wow_character_user_unblock_validate($form, &$form_state) {
  $character = $form_state['values']['character'];
  try {
    $character->fetch(array('items'), array(), FALSE);

    foreach ($form_state['values']['slot_options'] as $slot) {
      if (isset($character->items[$slot])) {
        // TODO: Ensure translations copy is coherent. (FR: Votre mains est encore équipé)
        form_set_error($slot, t('Your @slot is still equipped. Please remove @item_name.', array('@slot' => t($slot), '@item_name' => $character->items[$slot]['name'])));
      }
    }
    // TODO: Also test other parts of the stuff is still equiped.
  }
  catch (WoWHttpException $exception) {
    form_set_error('remove', t('Please follow the instructions bellow to unblock your character.'));
  }
  catch (Exception $exception) {
    form_set_error('remove', t('The website encountered an unexpected error. Please try again later.'));
  }
}

/**
 * Form builder: Submit handler for the wow_character_user_unblock_form.
 */
function wow_character_user_unblock_submit($form, &$form_state) {
  $character = wow_character_load($form_state['build_info']['args'][1]);
  $character->original = clone $character;
  $character->status = 1;
  $character->save();

  $form_state['redirect'] = drupal_get_destination();
}

/**
 * Title callback.
 */
function wow_character_page_title($character) {
  return $character->name;
}

/**
 * Menu callback; view a single character.
 */
function wow_character_view_page($character) {
  drupal_set_title($character->name);
  $uri = entity_uri('wow_character', $character);
  // Set the character path as the canonical URL to prevent duplicate content.
  drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($uri['path'], $uri['options'])), TRUE);
  // Set the non-aliased path as a default shortlink.
  drupal_add_html_head_link(array('rel' => 'shortlink', 'href' => url($uri['path'], array_merge($uri['options'], array('alias' => TRUE)))), TRUE);
  return wow_character_show($character);
}

/**
 * Generate an array which displays a character detail page.
 *
 * @param object $character
 *   An character object.
 *
 * @return array
 *   A $page element suitable for use by drupal_page_render().
 */
function wow_character_show($character) {
  return wow_character_view_multiple(array($character->cid => $character), 'full');
}

/**
 * Construct a drupal_render() style array from an array of loaded characters.
 *
 * @param object $characters
 *   An array of characters as returned by character_load_multiple().
 * @param string $view_mode
 *   View mode, e.g. 'full', 'teaser'...
 * @param int $weight
 *   An integer representing the weight of the first characters in the list.
 * @param string $langcode
 *   (optional) A language code to use for rendering. Defaults to the global
 *   content language of the current request.
 *
 * @return array
 *   An array in the format expected by drupal_render().
 */
function wow_character_view_multiple($characters, $view_mode = 'teaser', $weight = 0, $langcode = NULL) {
  field_attach_prepare_view('wow_character', $characters, $view_mode, $langcode);
  entity_prepare_view('wow_character', $characters, $langcode);
  $build = array();
  foreach ($characters as $character) {
    $build['characters'][$character->cid] = wow_character_view($character, $view_mode, $langcode);
    $build['characters'][$character->cid]['#weight'] = $weight;
    $weight++;
  }
  $build['characters']['#sorted'] = TRUE;
  return $build;
}

/**
 * Generate an array for rendering the given character.
 *
 * @param object $character
 *   A character object.
 * @param string $view_mode
 *   View mode, e.g. 'full', 'teaser'...
 * @param string $langcode
 *   (optional) A language code to use for rendering. Defaults to the global
 *   content language of the current request.
 *
 * @return array
 *   An array as expected by drupal_render().
 */
function wow_character_view($character, $view_mode = 'full', $langcode = NULL) {
  if (!isset($langcode)) {
    $langcode = $GLOBALS['language_content']->language;
  }

  // Populate $character->content with a render() array.
  wow_character_build_content($character, $view_mode, $langcode);

  $build = $character->content;
  // We don't need duplicate rendering info in $character->content.
  unset($character->content);

  $build += array(
    '#theme' => 'wow_character',
    '#character' => $character,
    '#view_mode' => $view_mode,
    '#language' => $langcode,
  );

  // Allow modules to modify the structured item.
  $type = 'wow_character';
  drupal_alter(array('wow_character_view', 'entity_view'), $build, $type);

  return $build;
}

/**
 * Builds the content field of the character object.
 */
function wow_character_build_content($character, $view_mode = 'full', $langcode = NULL) {
  if (!isset($langcode)) {
    $langcode = $GLOBALS['language_content']->language;
  }

  // Remove previously built content, if exists.
  $character->content = array();

  // Build fields content.
  field_attach_prepare_view('wow_character', array($character->cid => $character), $view_mode, $langcode);
  entity_prepare_view('wow_character', array($character->cid => $character), $langcode);
  $character->content += field_attach_view('wow_character', $character, $view_mode, $langcode);

  // Allow modules to make their own additions to the character.
  module_invoke_all('wow_character_view', $character, $view_mode, $langcode);
  module_invoke_all('entity_view', $character, 'wow_character', $view_mode, $langcode);
}
