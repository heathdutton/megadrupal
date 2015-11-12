<?php

/**
 * @file
 * Class Extension form Mandrill API wrapper class.
 */

/**
 * Class MandrillSubaccount.
 */
class MandrillSubaccount extends Mandrill {

  /**
   * Retrive subaccount list.
   *
   * @link https://mandrillapp.com/api/docs/subaccounts.JSON.html
   *
   * @return array|MandrillException
   *   List of current subaccounts
   */
  public function subaccountsList($q) {
    return $this->request('subaccounts/list', compact('q'));
  }

  /**
   * Create a new subaccount.
   *
   * @link https://mandrillapp.com/api/docs/subaccounts.JSON.html
   *
   * @return array|MandrillException
   *   The created subaccount.
   */
  public function subaccountsAdd($id) {
    return $this->request('subaccounts/add', array(
      'id' => $id,
    ));
  }
}
