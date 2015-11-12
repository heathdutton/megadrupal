<?php
/**
 * @file
 * Module Entity functions.
 */

/**
 * Entity Abstractions.
 */

/**
 * Abstraction function for create ChargeBee entity.
 *
 * @param string $type
 *   Entity type.
 * @param array $values
 *   Entity property values.
 *
 * @return bool
 *   Create status.
 */
function _chargebee_entity_create($type, $values) {
  $entity = entity_create('chargebee_' . $type, $values);
  $entity->save();
  return $entity;
}

/**
 * Abstraction function for load ChargeBee entity.
 *
 * @param string $type
 *   Entity type.
 * @param int $uid
 *   User identifier.
 *
 * @return mixed
 *   Entity object or FALSE.
 */
function _chargebee_entity_load($type, $uid) {
  $entities = entity_load('chargebee_' . $type, array($uid));
  return isset($entities[$uid]) ? $entities[$uid] : FALSE;
}

/**
 * Abstraction function for multiple load ChargeBee entities.
 *
 * @param string $type
 *   Entity type.
 * @param array|bool $ids
 *   ID array or FALSE.
 *
 * @return array|bool
 *   Entities array or FALSE.
 */
function _chargebee_entity_load_multiple($type, $ids) {
  return entity_load('chargebee_' . $type, $ids);
}

/**
 * Abstraction function for update entity values.
 *
 * @param object $entity
 *   Entity object.
 * @param array $values
 *   New values.
 *
 * @return object|bool
 *   Updated entity or FALSE.
 */
function _chargebee_entity_update($entity, $values) {
  foreach ($entity as $key => $value) {
    if ($key == 'uid') {
      continue;
    }
    $entity->$key = isset($values[$key]) ? $values[$key] : $entity->$key;
  }
  try {
    $entity->save();
    return $entity;
  }
  catch (Exception $e) {
    watchdog('ChargeBee', 'Error update entity', array(), WATCHDOG_ERROR);
    return FALSE;
  }
}

/**
 * Abstraction function for delete ChargeBee entity.
 *
 * @param string $type
 *   Entity type.
 * @param int $uid
 *   User identifier.
 *
 * @return bool
 *   Delete status.
 */
function _chargebee_entity_delete($type, $uid) {
  return entity_delete('chargebee_' . $type, $uid);
}

/**
 * Subscription Entity.
 */

/**
 * Subscription Entity create function.
 */
function chargebee_subscription_create($values) {
  global $user;
  if (!isset($values['uid'])) {
    $values['uid'] = $user->uid;
  }
  if (is_string($values['plan_id'])) {
    $plan = chargebee_plan_load($values['plan_id']);
    $values['plan_id'] = $plan->id;
  }
  $subscription = _chargebee_entity_create('subscription', $values);
  if (is_object($subscription)) {
    chargebee_change_user_role($user, $subscription);
  }
  return $subscription;
}

/**
 * Subscription Entity load function.
 */
function chargebee_subscription_load($uid) {
  return _chargebee_entity_load('subscription', $uid);
}

/**
 * Subscription Entity multiple load function.
 */
function chargebee_subscription_load_multiple($ids = FALSE) {
  return _chargebee_entity_load_multiple('subscription', $ids);
}

/**
 * Subscription Entity update values.
 */
function chargebee_subscription_update($subscription, $values) {
  $user = user_load($subscription->uid);
  chargebee_change_user_role($user, $subscription);
  if (is_string($values['plan_id'])) {
    $plan = chargebee_plan_load($values['plan_id']);
    $values['plan_id'] = $plan->id;
  }
  return _chargebee_entity_update($subscription, $values);
}

/**
 * Subscription Entity delete function.
 */
function chargebee_subscription_delete($uid) {
  return _chargebee_entity_delete('subscription', $uid);
}

/**
 * Customer Entity.
 */

/**
 * Customer Entity create function.
 */
function chargebee_customer_create($values) {
  global $user;
  if (!isset($values['uid'])) {
    $values['uid'] = $user->uid;
  }
  return _chargebee_entity_create('customer', $values);
}

/**
 * Customer Entity load function.
 */
function chargebee_customer_load($uid) {
  return _chargebee_entity_load('customer', $uid);
}

/**
 * Customer Entity multiple load function.
 */
function chargebee_customer_load_multiple($ids = FALSE) {
  return _chargebee_entity_load_multiple('customer', $ids);
}

/**
 * Customer Entity update values.
 */
function chargebee_customer_update($customer, $values) {
  return _chargebee_entity_update($customer, $values);
}

/**
 * Customer Entity delete function.
 */
function chargebee_customer_delete($uid) {
  return _chargebee_entity_delete('customer', $uid);
}

/**
 * Card Entity.
 */

/**
 * Card Entity create function.
 */
function chargebee_card_create($values) {
  if (!isset($values['uid'])) {
    global $user;
    $values['uid'] = $user->uid;
  }
  return _chargebee_entity_create('card', $values);
}

/**
 * Card Entity load function.
 */
function chargebee_card_load($uid) {
  return _chargebee_entity_load('card', $uid);
}

/**
 * Card Entity multiple load function.
 */
function chargebee_card_load_multiple($ids = FALSE) {
  return _chargebee_entity_load_multiple('card', $ids);
}

/**
 * Card Entity update values.
 */
function chargebee_card_update($card, $values) {
  return _chargebee_entity_update($card, $values);
}

/**
 * Card Entity delete function.
 */
function chargebee_card_delete($uid) {
  return _chargebee_entity_delete('card', $uid);
}

/**
 * Plan Entity.
 */

/**
 * Plan Entity create function.
 */
function chargebee_plan_create($values) {
  return _chargebee_entity_create('plan', $values);
}

/**
 * Plan Entity load function.
 */
function chargebee_plan_load($plan_id) {
  if (is_numeric($plan_id)) {
    return _chargebee_entity_load('plan', $plan_id);
  }
  return _chargebee_plan_load_by_name($plan_id);
}

/**
 * Plan Entity multiple load function.
 */
function chargebee_plan_load_multiple($ids = FALSE) {
  return _chargebee_entity_load_multiple('plan', $ids);
}

/**
 * Plan Entity update values.
 */
function chargebee_plan_update($plan, $values) {
  $values['id'] = $plan->id;
  return _chargebee_entity_update($plan, $values);
}

/**
 * Plan Entity delete function.
 */
function chargebee_plan_delete($plan_id) {
  return _chargebee_entity_delete('plan', $plan_id);
}

/**
 * Plan Entity load functions by name (plan id).
 */
function _chargebee_plan_load_by_name($plan_id) {
  $plan = entity_load('chargebee_plan', FALSE, array('plan_id' => $plan_id));
  return reset($plan);
}
