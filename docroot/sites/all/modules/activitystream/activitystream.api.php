<?php

/**
 * @file
 * Hooks provided by the Activity Stream module and how to add your own stream.
 */

/**
 * Define an Activity Stream service.
 *
 * Services are simply sites which have activity you'd like to import into
 * an activity stream. A module can define as many services as they'd like.
 *
 * @return
 *   An array whose keys are internal service IDs and whose values identify
 *   properties of the service. The available properties are listed below:
 *   - type: (required) The internal service ID. Same as the key.
 *   - name: (required) The displayed-to-humans name of the Service.
 *   - verb: The action the user is performing. Examples include
 *     "tweeted", "posted", "started watching", "read", "reviewed", etc.
 *     By default, Activity Stream items will display as "ACTOR VERB
 *     OBJECT",where object is the title of an Activity Stream item. So,
 *     "Morbus posted An Mispelt Title" or "sbp tweeted RT Sadness." You
 *     can tweak this default statement display with theme preprocessing.
 *   - icon: (required) The file path to an icon for this service. Higher
 *     res is better, but the default UI will CSS scale down to 24x24.
 */
function hook_activitystream_services() {
  return array(
    'flutter' => array(
      'type'  => 'flutter',
      'name'  => t('Flutter'),
      'verb'  => t('fluttered'),
      'icon'  => drupal_get_path('module', 'activitystream_flutter') . '/flutter.png',
    ),
  );
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Every service requires some configuration, usually being, at the least,
 * the username to fetch the activity of. An Activity Stream accounts form is
 * available at user/#/edit/activitystream-accounts, and modules can use the
 * existing Drupal core hook_form_FORM_ID_alter() to add their required fields
 * and custom submit function.
 *
 * In the example below, we're requesting just a username for the fictional
 * Flutter service, and then adding our own submit handler so that we can
 * act upon and then save to the database.
 *
 * Also note the use of activitystream_account_load(). In conjunction with
 * activitystream_account_save(), they handle all I/O relevant to a service's
 * per-user account configuration, serializing and unserializing as needed.
 * If you need to save an array's worth of service configuration data per-
 * user, you can do that too.
 */
function hook_form_activitystream_accounts_form_alter(&$form, &$form_state) {
  $form['MODULE']['MODULE_flutter_username'] = array(
    '#default_value'  => activitystream_account_load('flutter', $form['#user']->uid),
    '#title'          => t('Flutter username'),
    '#type'           => 'textfield',
  );

  $form['#submit'][] = 'MODULE_form_activitystream_accounts_form_submit';
}

/**
 * Form submit handler for a user's Activity Stream accounts.
 *
 * This shows an example of saving a service's per-user account information or
 * deleting it should they clear out the value in the form. The following is
 * just a simple username field, but you can add as much configuration as
 * necessary for your particular service.
 */
function MODULE_form_activitystream_accounts_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['MODULE_flutter_username'])) {
    activitystream_account_save('flutter', $flutter_username, $form['#user']->uid);
  }
  else {
    activitystream_account_delete('flutter', $form['#user']->uid);
  }
}

/**
 * Implements hook_activitystream_SERVICE_items_fetch().
 *
 * When it's time to find new activity, we ask modules for an array of $items
 * fetched from a particular service. In our fictional Flutter example herein,
 * hook_activitystream_flutter_items_fetch() would be called.
 *
 * @param $uid
 *   The Drupal user whose items are being fetched.
 * @param $data
 *   The user's saved account information for the service. For our Flutter
 *   example, $data would contain the entered Flutter username (as saved
 *   in MODULE_form_activitystream_accounts_form_submit(). If you've saved
 *   an array's worth of account info, you'd receive that array in $data.
 *
 * @return
 *   An array of $items, with each individual item array containing:
 *    - title: (required) The title of the activity item.
 *    - body: (required) The body of the activity item.
 *    - link: (required) A URL to the original item on the service.
 *    - timestamp: (required) A UNIX timestamp for when the activity occurred.
 *    - guid: (required) A globally unique ID for this activity item. This
 *      lets Activity Stream know if this is a new item or an update to an
 *      an already-saved item. If the guid already exists for the passed $uid,
 *      but the title or body are different, the saved item will be updated.
 *    - raw: (desired) One of the core goals behind Activity Stream is to
 *      provide an archive of your activity spread throughout the web. To help
 *      this along, 'raw' allows a module to save the verbatim service data
 *      for this activity item. For an item created from an RSS feed, this
 *      would be the full XML downloaded from the service, or the full JSON
 *      received from somewhere else, and so forth. 'raw' data should be
 *      able to recreate the activity item even if the service doesn't exist
 *      any more, and might even contain data that hasn't been massaged into
 *      Drupal (such as GPS or user-agent strings, keywords, XML namespaces
 *      that weren't supported at the original time of fetching, etc.).
 */
function hook_activitystream_SERVICE_items_fetch($uid, $data) {
  // If your service provides RSS or Atom feeds, you can use the SimplePIE
  // parser stored within activitystream_feed.module. Just pass in the Drupal
  // user ID and the feed URL (or URLs, as an array), and you'll receive an
  // an array of $items suitable for returning to Activity Stream.
  $feed_url = 'http://example.com/user/' . $data . '.rss';
  $items = activitystream_feed_items_fetch($uid, $feed_url);

  // A hardcoded example of an item's structure:
  $items[] = array(
    'title'     => "Major-General's Song",
    'link'      => "http://en.wikipedia.org/wiki/Major-General's_Song",
    'body'      => "I've information vegetable, animal, and mineral, ...",
    'guid'      => 'http://en.wikipedia.org/w/index.php?title=Major-General%27s_Song&oldid=509475615',
    'raw'       => '<?xml ... ?>', // Or whatever else your raw data is.
    'timestamp' => 1346435609,
  );

  return $items;
}

/**
 * Implements hook_activitystream_SERVICE_item_alter().
 *
 * Allows you to modify the fetched items prior to node saving.
 *
 * @param $items
 *   An array of fetched items from the service.
 * @param $uid
 *   The Drupal user whose items are being fetched.
 * @param $data
 *   The user's saved account information for the service.
 */
function hook_activitystream_SERVICE_items_alter(&$items, &$uid, &$data) {
  foreach ($items as $index => $item) {
    if (strpos($item['title'], 'Advertisement') !== FALSE) {
      unset($items[$index]);
    }
  }
}

/**
 * Implements hook_preprocess_HOOK().
 *
 * By default, Activity Stream displays statements about items. A statement
 * has three parts: an actor ("Morbus Iff"), a verb set for the service in
 * hook_activitystream_services() ("started watching"), and the object
 * ("Emmanuelle in Space"). Together, it reads as "Morbus Iff started watching
 * Emmanuelle in Space", where "Morbus Iff" links to the Drupal user profile
 * and "Emmanuelle in Space" links to the URL of the activity on the service.
 *
 * Sometimes, this isn't exactly right: a service might offer multiple types
 * of activity that a single verb can't fully cover, such as "started
 * watching", "reviewed", "rated", or "recommended" for a movie community.
 *
 * Other times, the service might offer a better choice for the object than
 * just a single link to the original activity. Consider "Morbus Iff won the
 * He Cleans Up Well! achievement in Red Dead Redemption". We might want to
 * link to the service's page about the achievement, the service's page about
 * the video game, as well as my member profile on the service. Or, for Twitter,
 * there might be other usernames and hashtags and embedded URLs that we want
 * linked instead of the default behavior of linking the whole object.
 *
 * Modules can modify the default statement using a theme preprocessor.
 *
 * $variables['statement'] contains arrays of 'actor', 'verb', and
 * 'object'. Modules can tweak these render arrays however they'd like, as
 * long as "print render($statement)" continues to work.
 */
function hook_preprocess_activitystream_item(&$variables) {
  // If our service type is a movie community, look in the body for
  // "Review of ", and if it's there, changed the verb to 'reviewed'.
  if ($variables['service']['type'] == 'movie_example') {
    if (strpos($variables['node']->body[LANGUAGE_NONE][0]['value'], 'Review of ') !== FALSE) {
      $variables['statement']['verb'] = ' ' . t('reviewed') . ' ';
    }
  }

  // For Twitter, usernames, hashtags, and URLs are all separately linked,
  // so we display the formatted node body for the object, not the title.
  if ($variables['service']['type'] == 'twitter') {
    unset($variables['statement']['object']);
    $variables['statement']['object'] = array(
      '#markup' => '"' . preg_replace('/\s*<\/?p>\s*/', '', $variables['node']->body[LANGUAGE_NONE][0]['safe_value']) . '"',
    );
  }

  // Here's a final example where we ignore the actor/verb/object
  // defaults entirely and stick raw HTML into the 'statement' variable.
  if ($variables['service']['type'] == 'image_upload_service') {
    $variables['statement']['#markup'] = '<blockquote>Dick Laurent is dead.</blockquote>';
  }
}

