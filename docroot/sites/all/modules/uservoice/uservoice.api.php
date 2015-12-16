<?php

/**
 * @file
 * Hooks provided by the UserVoice module.
 */

/**
 * Modify UserVoice widget script identity traits array.
 *
 * This hook allows you to modify the identity array that is passed to the
 * UserVoice script constructor for a given user account. This forms the
 * basis for sending Drupal user information to UserVoice. You should flush
 * the cache_uservoice_script table after implementing this hook.
 *
 * @param array $identity
 *   An array of UserVoice identity traits containing:
 *   - email: User’s unique email address.
 *   - name: User’s real name.
 *   - created_at: Unix timestamp (epoch seconds) for the date the user first
 *     signed up for your service. Also useful for autoprompting users. (If not
 *     set, UserVoice will assume the first time we see the user is when they
 *     were created at.)
 *   - id: Unique id of the user (if set, this should not be changed).
 *   - type: Segment your users by type. For example, "Designer", "Developer",
 *   "Teacher".
 * @param object $account
 *   The user account object that forms the basis for this identity.
 *
 * @see _uservoice_get_identity()
 * @see uservoice_user_update()
 */
function hook_uservoice_identity_alter(&$identity, $account) {
  // Example: push identity "type" based on specific user role priority.
  $roles = $account->roles;
  if (in_array('administrator', $roles)) {
    $type = 'administrator';
  }
  elseif (in_array('designer', $roles)) {
    $type = 'designer';
  }
  elseif (in_array('forum moderator', $roles)) {
    $type = 'forum moderator';
  }
  elseif (in_array('surrogate uploader', $roles)) {
    $type = 'surrogate uploader';
  }
  elseif (in_array('creative team', $roles)) {
    $type = 'creative team';
  }
  elseif (in_array('cu subscriber', $roles)) {
    $type = 'cu subscriber';
  }
  elseif (in_array('pu subscriber', $roles)) {
    $type = 'pu subscriber';
  }
  elseif (in_array('donator', $roles)) {
    $type = 'donator';
  }
  elseif (in_array('scrapper', $roles)) {
    $type = 'scrapper';
  }
  elseif (in_array('new user', $roles)) {
    $type = 'new user';
  }
  elseif (in_array('first login', $roles)) {
    $type = 'first login';
  }
  elseif (in_array('just registered', $roles)) {
    $type = 'just registered';
  }
  elseif (in_array('authenticated user', $roles)) {
    $type = 'authenticated user';
  }

  $identity['type'] = $type;
}

/**
 * Modify UserVoice widget script account traits array.
 *
 * This hook allows you to modify the account traits array that is passed to the
 * UserVoice script constructor for a given user account. Account traits are
 * used to track things like subscription plan, monthly rate, and total lifetime
 * value. Normally only applicable to sites with monetized users; can be used to
 * to entegrate ecommerce data from modules like Membership Suite, Ubercart,
 * Drupal Commerce, etc. You should flush the cache_uservoice table after
 * implementing this hook. Also take care to flush cache_uservoice_script for
 * the relevant user whenever their account traits are likely to change.
 *
 * @param array $account_traits
 *   An array of UserVoice account traits containing:
 *   - id: Unique id of the account. Allows you to associate multiple users with
 *     a single account. If not set, other account traits will be attributed to
 *     the single user.
 *   - name: Account name.
 *   - created_at: The date the account was created.
 *   - monthly rate: Monthly rate of the account.
 *   - ltv: Lifetime value of the account.
 *   - plan: Plan name for the account.
 * @param object $account
 *   The user account object that forms the basis for these account traits.
 *
 * @see _uservoice_get_account_traits()
 * @see uservoice_ms_products_payment()
 */
function hook_uservoice_account_traits_alter(&$account_traits, $account) {
  // Example: integration with Membership Suite's ms_products module (this
  // integration is already included in uservoice.module).
  if (module_exists('ms_products')) {
    $uid = $account->uid;

    $sql = "SELECT *
            FROM {ms_products_purchases} mspp
            LEFT JOIN {ms_orders} mo
            ON mspp.oid = mo.oid
            WHERE mspp.uid = :uid
            ORDER BY mspp.start_date DESC";

    $result = db_query($sql, array(':uid' => $uid))->fetchAll();

    if ($result) {
      $first_result = $result[0];

      // If more than one order, add up total for all orders to get lifetime
      // value.
      if (count($result) > 1) {
        $lifetime_value = 0;
        foreach ($result as $r) {
          $lifetime_value += $r->total;
        }
        $first_result->total = $lifetime_value;
      }

      // Tack expiration status onto sku if plan has been canceled/expired.
      if ($first_result->status == 'expired' || $first_result->status == 'cancelled') {
        $first_result->sku = $first_result->sku . ' - ' . $first_result->status;
      }

      if (isset($first_result->start_date) && isset($first_result->amount) && isset($first_result->total) && isset($first_result->sku)) {
        $account_traits['created_at'] = $first_result->start_date;
        $account_traits['monthly_rate'] = $first_result->amount;
        $account_traits['ltv'] = $first_result->total;
        $account_traits['plan'] = $first_result->sku;
      }
    }
  }
}

/**
 * Modify uservoice SSO token user data.
 *
 * This hook allows you to modify the user data that is passed to UserVoice
 * in the single-sign on (sso) token, such as display name, email, and avatar.
 *
 * @param array $user_data
 *   An array of UserVoice sso data for the given user. For a list of potential
 *   keys see
 *   https://developer.uservoice.com/docs/single-sign-on/single-sign-on/.
 * @param object $account
 *   The user account object that forms the basis for the sso data.
 *
 * @see uservoice_get_sso_token()
 * @see http://feedback.uservoice.com/knowledgebase/articles/81480-set-up-sso-single-sign-on
 */
function hook_uservoice_sso_data_alter(&$user_data, $account) {
  // Example: hard code the same avatar url for all users. This avatar will then
  // be seen for all users in the UserVoice web portal.
  $user_data["avatar_url"] = "http://some-domain.com/some-image.jpg";
}

/**
 * Modify order history information used by the order history gadget.
 *
 * This hook allows you to modify the order history array that is used by the
 * order history gadget to show a user's order history within the UserVoice
 * admin console when viewing a ticket from the given user.
 *
 * You can use this hook to integrate with contributed ecommerce modules, or
 * with custom ecommerce solution.
 *
 * @param array $orders
 *   An array of orders by the given user. Each order is an array containing:
 *   - date: order timestamp.
 *   - sku: order sku.
 *   - status: order status.
 *   - gateway: order gateway (PayPal, Authorize.net, etc.)
 *   - amount: order amount ($ value).
 *   - link: internal Drupal path for more information about the given order.
 * @param int $uid
 *   Drupal user id associated with the given order history.
 *
 * @see _uservoice_get_order_history()
 */
function hook_uservoice_order_history_alter(&$orders, $uid) {
  // Example: integration with Membership Suite's ms_products module (this
  // integration is already included in uservoice.module).
  if (module_exists('ms_products')) {
    $sql = "SELECT mso.order_key, mso.status, mso.gateway, mso.total, mso.created, mspp.sku
            FROM {ms_orders} mso
            LEFT JOIN {ms_products_purchases} mspp
            ON mso.oid = mspp.oid
            WHERE mso.uid = :uid
            ORDER BY mso.created DESC";

    $results = db_query($sql, array(':uid' => $uid), array('target' => 'slave'))->fetchAll();

    if (!empty($results)) {
      foreach ($results as $result) {
        $orders[] = array(
          'date' => $result->created,
          'sku' => $result->sku,
          'status' => $result->status,
          'gateway' => $result->gateway,
          'amount' => $result->total,
          'link' => "admin/moneyscripts/orders/view/$result->order_key",
        );
      }
    }
  }
}
