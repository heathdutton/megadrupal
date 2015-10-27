<?php

/**
 * Class ZuoraSubscription.
 */
class ZuoraSubscription {

  /**
   * Gets a subscription from the API.
   *
   * @param $subscription_key
   *
   * @return ZuoraSubscriptionObject
   */
  public static function get($subscription_key) {
    $data = ZuoraRest::instance()->get('/subscriptions/' . $subscription_key)->data();
    if ($data['success']) {
      return new ZuoraSubscriptionObject($data);
    }

    return NULL;
  }

  /**
   * Creates a subscription in the API.
   *
   * @param ZuoraSubscriptionObject $data
   * @param array $rate_plans
   *
   * @return mixed
   */
  public static function create(ZuoraSubscriptionObject $data, array $rate_plans) {
    $subscription_data = $data->toArray();
    $subscription_data['subscribeToRatePlans'] = $rate_plans;
    return ZuoraRest::instance()->post('/subscriptions/', $subscription_data)->data();
  }

  /**
   * Updates a subscription in the API.
   *
   * @param $account_key
   * @param ZuoraSubscriptionObject $data
   *
   * @return mixed
   */
  public static function update($account_key, ZuoraSubscriptionObject $data) {
    return ZuoraRest::instance()->put('/subscriptions/' . $account_key, $data->toArray())->data();
  }
}
