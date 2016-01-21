<?php

/**
 * A condition that can be applied to select queries.
 */
interface menupoly_MenuTreeSource_MenuLinks_RootCondition_Interface {

  /**
   * Apply the condition to a select query.
   *
   * @param SelectQueryInterface $q
   *   The select query to modify.
   */
  function apply($q);

  /**
   * Apply the final condition to a select query.
   *
   * @param SelectQueryInterface $q
   *   The select query to modify.
   * @param array $settings
   *   Settings array.
   * @param array $trail_mlids
   *   Mlids in the active trail, in case of MENUPOLY_EXPAND_ACTIVE.
   */
  function applyFinal($q, $settings, $trail_mlids);

  /**
   * @return int
   *   Mlid of the root item, or 0 if there is none.
   */
  function getRootMlid();
}
