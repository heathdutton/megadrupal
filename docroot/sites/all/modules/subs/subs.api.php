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
 * Acts on Subscriptions being loaded from the database.
 *
 * This hook is invoked during Subscription loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $subscriptions
 *   An array of Subscription entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_subs_load(array $subscriptions) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a Subscription is inserted.
 *
 * This hook is invoked after the Subscription is inserted into the database.
 *
 * @param Subs $subscription
 *   The Subscription that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_subs_insert(Subs $subscription) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('subs', $subscription),
      'extra' => print_r($subscription, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a Subscription being inserted or updated.
 *
 * This hook is invoked before the Subscription is saved to the database.
 *
 * @param Subs $subscription
 *   The Subscription that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_subs_presave(Subs $subscription) {
  $subscription->name = 'foo';
}

/**
 * Responds to a Subscription being updated.
 *
 * This hook is invoked after the Subscription has been updated in the database.
 *
 * @param Subs $subscription
 *   The Subscription that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_subs_update(Subs $subscription) {
  db_update('mytable')
    ->fields(array('extra' => print_r($subscription, TRUE)))
    ->condition('id', entity_id('subs', $subscription))
    ->execute();
}

/**
 * Responds to Subscription deletion.
 *
 * This hook is invoked after the Subscription has been removed from the database.
 *
 * @param Subs $subscription
 *   The Subscription that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_subs_delete(Subs $subscription) {
  db_delete('mytable')
    ->condition('pid', entity_id('subs', $subscription))
    ->execute();
}

/**
 * Act on a Subscription that is being assembled before rendering.
 *
 * @param $subscription
 *   The Subscription entity.
 * @param $view_mode
 *   The view mode the Subscription is rendered in.
 * @param $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $Subscription->content prior to rendering. The
 * structure of $Subscription->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 */
function hook_subs_view($subscription, $view_mode, $langcode) {
  $subscription->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_view() for Subscriptions.
 *
 * @param $build
 *   A renderable array representing the Subscription content.
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * Subscription content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the Subscription rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_Subscription().
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @see hook_entity_view_alter()
 */
function hook_subscription_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Define default Subscription configurations.
 *
 * @return
 *   An array of default Subscriptions, keyed by machine names.
 *
 * @see hook_default_subs_alter()
 */
function hook_default_subs() {
  $defaults['main'] = entity_create('subs', array(
    // …
    ));
  return $defaults;
}

/**
 * Alter default Subscription configurations.
 *
 * @param array $defaults
 *   An array of default Subscriptions, keyed by machine names.
 *
 * @see hook_default_subs()
 */
function hook_default_subs_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}

/**
 * Act after rebuilding default Subscriptions.
 *
 * This hook is invoked by the entity module after default Subscriptions
 * have been rebuilt; i.e. defaults have been saved to the database.
 *
 * @param $subscriptions
 *   The array of default Subscriptions which have been inserted or
 *   updated, keyed by name.
 * @param $originals
 *   An array of original Subscriptions keyed by name; i.e. the Subscriptions
 *   before the current defaults have been applied. For inserted
 *   Subscriptions no original is available.
 *
 * @see hook_default_subs()
 * @see entity_defaults_rebuild()
 */
function hook_subs_defaults_rebuild($subscriptions, $originals) {

}

/**
 * Alter Subscription forms.
 *
 * Modules may alter the Subscription entity form by making use of this hook or
 * the entity bundle specific hook_form_subs_edit_BUNDLE_form_alter().
 * #entity_builders may be used in order to copy the values of added form
 * elements to the entity, just as documented for
 * entity_form_submit_build_entity().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
//function hook_form_subs_form_alter(&$form, &$form_state) {
// Your alterations.
//}

/**
 * Acts on Subscription Types being loaded from the database.
 *
 * This hook is invoked during Subscription Type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $subscription_types
 *   An array of Subscription Type entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_subs_type_load(array $subscription_types) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a Subscription Type is inserted.
 *
 * This hook is invoked after the Subscription Type is inserted into the database.
 *
 * @param SubsType $subscription_type
 *   The Subscription Type that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_subs_type_insert(SubsType $subscription_type) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('subs_type', $subscription_type),
      'extra' => print_r($subscription_type, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a Subscription Type being inserted or updated.
 *
 * This hook is invoked before the Subscription Type is saved to the database.
 *
 * @param SubsType $subscription_type
 *   The Subscription Type that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_subs_type_presave(SubsType $subscription_type) {
  $subscription_type->name = 'foo';
}

/**
 * Responds to a Subscription Type being updated.
 *
 * This hook is invoked after the Subscription Type has been updated in the database.
 *
 * @param SubsType $subscription_type
 *   The Subscription Type that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_subs_type_update(SubsType $subscription_type) {
  db_update('mytable')
    ->fields(array('extra' => print_r($subscription_type, TRUE)))
    ->condition('id', entity_id('subs_type', $subscription_type))
    ->execute();
}

/**
 * Responds to Subscription Type deletion.
 *
 * This hook is invoked after the Subscription Type has been removed from the database.
 *
 * @param SubsType $subscription_type
 *   The Subscription Type that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_subs_type_delete(SubsType $subscription_type) {
  db_delete('mytable')
    ->condition('pid', entity_id('subs_type', $subscription_type))
    ->execute();
}

/**
 * Define default Subscription Type configurations.
 *
 * @return
 *   An array of default Subscription Types, keyed by machine names.
 *
 * @see hook_default_subs_type_alter()
 */
function hook_default_subs_type() {
  $defaults['main'] = entity_create('subs_type', array(
    // …
    ));
  return $defaults;
}

/**
 * Alter default Subscription Type configurations.
 *
 * @param array $defaults
 *   An array of default Subscription Types, keyed by machine names.
 *
 * @see hook_default_subs()
 */
function hook_default_subs_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}

/**
 * Act after rebuilding default Subscription Types.
 *
 * This hook is invoked by the entity module after default Subscription Types
 * have been rebuilt; i.e. defaults have been saved to the database.
 *
 * @param $subscription_types
 *   The array of default Subscription Types which have been inserted or
 *   updated, keyed by name.
 * @param $originals
 *   An array of original Subscription Types keyed by name; i.e. the Subscription Types
 *   before the current defaults have been applied. For inserted
 *   Subscription Types no original is available.
 *
 * @see hook_default_subs()
 * @see entity_defaults_rebuild()
 */
function hook_subs_type_defaults_rebuild($subscription_types, $originals) {

}

/**
* Alter Subscription Type forms.
*
* Modules may alter the Subscription Type entity form by making use of this hook or
* the entity bundle specific hook_form_subs_type_edit_BUNDLE_form_alter().
* #entity_builders may be used in order to copy the values of added form
* elements to the entity, just as documented for
* entity_form_submit_build_entity().
*
* @param $form
*   Nested array of form elements that comprise the form.
* @param $form_state
*   A keyed array containing the current state of the form.
*/
function hook_form_subs_type_form_alter(&$form, &$form_state) {
//   Your alterations.
}

/**
 * Control access to subscriptions.
 *
 * Modules may implement this hook if they want to have a say in whether or not
 * a given user has access to perform a given operation on a subscription.
 *
 * @param $op
 *   The operation being performed. One of 'view', 'edit' (being the same as
 *   'create' or 'update') and 'delete'.
 * @param $subscription
 *   (optional) A subscription to check access for. If nothing is given, access for
 *   all subscriptions is determined.
 * @param $account
 *   (optional) The user to check for. If no account is passed, access is
 *   determined for the global user.
 * @return boolean
 *   Return TRUE to grant access, FALSE to explicitly deny access. Return NULL
 *   or nothing to not affect the operation.
 *   Access is granted as soon as a module grants access and no one denies
 *   access. Thus if no module explicitly grants access, access will be denied.
 *
 * @see subs_access()
 */
function hook_subs_access($op, $subscription = NULL, $account = NULL) {
  if (isset($subscription)) {
    // Explicitly deny access for a 'secret' subscription type.
    if ($subscription->type == 'secret' && !user_access('custom permission')) {
      return FALSE;
    }
    // For subscriptions other than the default subscription grant access.
    if ($subscription->type != 'standard' && user_access('custom permission')) {
      return TRUE;
    }
    // In other cases do not alter access.
  }
}

/**
* @}
*/