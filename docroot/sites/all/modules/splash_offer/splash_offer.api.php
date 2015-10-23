<?php
/**
 * @file
 * Hooks provided by this module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implements hook_splash_offer_no_show_paths().
 *
 * The advantage of this hook over hook_splash_offer_show_alter is that these
   paths are displayed in the admin UI so it may be more clear for administrators
 *
 * @param SplashOfferEntity $entity
 *
 * @return string
 *   A string of paths separated by "\n" for which the $entity page should never
     show
 *
 */
function hook_splash_offer_no_show_paths($entity) {
  // Notice that we don't care about the entity in this invokation
  // Not on admin pages, if a module really wants a splash page on admin
  // they can use their own hook_splash_offer_show_alter
  $auto_pages = "admin*\n";

  // Service endpoints break if the splash page loads, so register them
  if (module_exists('services')) {
    $endpoints = services_endpoint_load_all();
    foreach ($endpoints as $endpoint) {
      $auto_pages .= $endpoint->path . "*\n";
    }
  }
  return $auto_pages;
}

/**
 * Implements hook_splash_offer_no_show_paths().
 *
 * @param string &$pages
 *
 * @return NULL
 */
function hook_splash_offer_no_show_paths_alter(&$pages) {
  // I do want splash to appear on admin pages!
  $pages = str_replace("admin*\n", '', $pages);
}

/**
 * Implements hook_splash_offer_show_alter().
 *
 * Alter the page visibility as set by the UI
 *
 * @param int $oid
 *   The SplashOfferEntity id to be displayed; set this to 0 to hide all offers
 * @param string
 *   The current path; same as $_GET['q']
 * @param array
 *   An array keyed by ids of all SplashOfferEntities
 *
 * @return NULL
 *
 * @see splash_offer_flush_caches()
 */
function hook_splash_offer_show_alter(&$oid, $path, $offers) {
  // Don't show it until after March 12, 2013
  // Note: by checking $oid first, this will be faster, in this example it's
  // somewhat trivial, but you need to be lean with this hook as it's called on
  // every page load.
  if ($oid && time() < 1363132020) {
    $oid = 0;
  }
}

/**
 * Acts on apps being loaded from the database.
 *
 * This hook is invoked during app loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of app entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_splash_offer_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a app is inserted.
 *
 * This hook is invoked after the app is inserted into the database.
 *
 * @param SplashOfferEntity $entity
 *   The app that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_splash_offer_insert(SplashOfferEntity $entity) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('splash_offer', $entity),
      'extra' => print_r($entity, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a app being inserted or updated.
 *
 * This hook is invoked before the app is saved to the database.
 *
 * @param SplashOfferEntity $entity
 *   The app that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_splash_offer_presave(SplashOfferEntity $entity) {
  $entity->name = 'foo';
}

/**
 * Responds to a app being updated.
 *
 * This hook is invoked after the app has been updated in the database.
 *
 * @param SplashOfferEntity $entity
 *   The app that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_splash_offer_update(SplashOfferEntity $entity) {
  db_update('mytable')
    ->fields(array('extra' => print_r($entity, TRUE)))
    ->condition('id', entity_id('splash_offer', $entity))
    ->execute();
}

/**
 * Responds to app deletion.
 *
 * This hook is invoked after the app has been removed from the database.
 *
 * @param SplashOfferEntity $entity
 *   The app that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_splash_offer_delete(SplashOfferEntity $entity) {
  db_delete('mytable')
    ->condition('pid', entity_id('splash_offer', $entity))
    ->execute();
}

/**
 * Alter app forms.
 *
 * Modules may alter the {ENTITY} entity form by making use of this hook or
 * the entity bundle specific hook_form_{ENTITY_ID}_edit_BUNDLE_form_alter().
 * #entity_builders may be used in order to copy the values of added form
 * elements to the entity, just as documented for
 * entity_form_submit_build_entity().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function hook_form_splash_offer_form_alter(&$form, &$form_state) {
  // Your alterations.
}

/**
 * @}
 */
