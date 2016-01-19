<?php
/**
 * @file
 * Subs theme functions.
 */

/**
 * Theme function.
 * Renders the contents of the "Create new subscription" block.
 *
 * @see subs_block_view()
 * @see subs_theme()
 */
function theme_subs_block_create() {
  $types = subs_get_types();
  $count = count($types);

  if ($count === 0) {
    if (user_access('administer subs')) {
      return t("You haven't created any !types yet!", array('!types' => l(t('Subscription Types'), 'admin/structure/subscriptions')));
    }
    else {
      return t('There are no subscription types at the moment.');
    }
  }
  else {
    $content = array();

    foreach ($types as $type) {
      // Create dummy subscription to check access against.
      $subscription = subs_create(array('type' => $type));

      // Only display link for types the user has access to create.
      if (subs_access('create', $subscription)) {
        $content[] = l($type->label, "subscription/add/{$type->name}");
      }
    }

    // Return an empty string if the user doesn't have access to create
    // subscriptions of any type.
    return !empty ($content)
      ? theme('item_list', array('items' => $content))
      : '';
  }
}

/**
 * Theme function.
 * Renders the contents of the "Subscription status" block.
 */
function theme_subs_block_status() {
  $account = user_uid_optional_load();
  $subscriptions = subs_load_by_user($account);

  if (empty($subscriptions)) {
    return t("You don't have any subscriptions.");
  }
  else {
    $header = array(
      'Type',
      'Status',
      'View',
    );

    $rows = array();
    $status_options = subs_status_options_list();

    foreach ($subscriptions as $sub) {
      $url = entity_uri('subs', $sub);

      $rows[] = array(
        $sub->type,
        $status_options[$sub->status],
        l(t('view'), $url['path']),
      );
    }

    return theme('table', array('header' => $header, 'rows' => $rows));
  }
}