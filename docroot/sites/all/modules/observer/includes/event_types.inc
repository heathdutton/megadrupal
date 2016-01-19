<?php

/**
 * @file
 * Contains ObserverEventTypes class.
 */

/**
 * Class ObserverEventTypes
 *
 * Contains a list of typical events commonly triggered in inside the global
 * Observable instance. This list is not exhaustive as the system dynamically
 * generates types for each of the entities existing inside Drupal.
 *
 * @see observer_entity_insert().
 * @see observer_entity_update().
 * @see observer_entity_delete().
 */
class ObserverEventTypes {

  // Generic Entity Events
  const ENTITY_INSERTED = 'entity:inserted';
  const ENTITY_UPDATED = 'entity:updated';
  const ENTITY_DELETED = 'entity:deleted';

  // User Events
  const USER_INSERTED = 'user:inserted';
  const USER_UPDATED = 'user:updated';
  const USER_DELETED = 'user:deleted';
  const USER_LOGIN = 'user:login';
  const USER_LOGOUT = 'user:logout';

  // Node Events
  const NODE_INSERTED = 'node:inserted';
  const NODE_UPDATED = 'node:updated';
  const NODE_DELETED = 'node:deleted';

  // Comment Events
  const COMMENT_INSERTED = 'comment:inserted';
  const COMMENT_UPDATED = 'comment:updated';
  const COMMENT_DELETED = 'comment:deleted';

  // Flag Events
  const FLAG_FLAG = 'flag:flag';
  const FLAG_UNFLAG = 'flag:unflag';

  // Organic Groups Events
  const OG_SUBSCRIBE = 'og_membership:subscribe';
  const OG_UNSUBSCRIBE = 'og_membership:unsubscribe';

}
