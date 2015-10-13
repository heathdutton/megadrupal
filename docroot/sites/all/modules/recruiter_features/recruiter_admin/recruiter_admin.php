<?php

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allows module to add administration links to the user view.
 *
 * @param $items
 *   The list items that are going to be listed.
 *
 * @param $account
 *   The user account on which the items are listed.
 */
function hook_recruiter_admin_links_alter(&$items, $account) {
  if (in_array('applicant', $account->roles)) {
    $items[] = l(t('Resume'), 'resume/' . $account->uid);
  }
}


/**
 * @}
 */
