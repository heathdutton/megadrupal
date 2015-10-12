<?php

namespace Drupal\commerce_reports_tax\Tests;

use CommerceBaseTestCase;

class CommerceReportsTaxGenerationTests extends CommerceBaseTestCase {

  protected $tax_rates;
  protected $orders;

  public static function getInfo() {
    return array(
      'name' => 'Tax reports',
      'description' => 'Tests if the tax reporting module correctly catches tax information.',
      'group' => 'Commerce Reports Tax',
    );
  }

  /**
   * Overrides CommerceBaseTestCase::permissionBuilder().
   */
  protected function permissionBuilder($set) {
    $permissions = parent::permissionBuilder($set);

    switch ($set) {
      case 'store admin':
      case 'site admin':
        $permissions[] = 'access commerce tax reports';
        $permissions[] = 'configure commerce tax reports';
        break;
    }

    return $permissions;
  }

  protected function generateTax($amount = 1) {
    // Create a tax rate of Salex Type.
    $this->createDummyTaxRate(array('' => ''));

    $taxed = array();

    // Login with normal user.
    $this->drupalLogin($this->normal_user);

    for ($i = 0; $i < $amount; $i ++) {
      // Create a dummy order in cart status.
      $order = $this->createDummyOrder($this->normal_user->uid);

      // Get the checkout url and navigate to it.
      $links = commerce_line_item_summary_links();

      // Checkout order
      $this->drupalPost($links['checkout']['href'], array(
        'customer_profile_billing[commerce_customer_address][und][0][name_line]' => $this->randomName(),
        'customer_profile_billing[commerce_customer_address][und][0][thoroughfare]' => $this->randomName(),
        'customer_profile_billing[commerce_customer_address][und][0][locality]' => $this->randomName(),
      ), t('Continue to next step'));

      $this->drupalPost(NULL, array(
        'commerce_payment[payment_details][name]' => $this->randomName(),
      ), t('Continue to next step'));

      $orders = commerce_order_load_multiple(array($order->order_id), array(), TRUE);
      $order = reset($orders);

      foreach ($order->commerce_order_total[LANGUAGE_NONE][0]['data']['components'] as $component) {
        if (!empty($component['price']['data']['tax_rate'])) {
          if (empty($taxed[$component['price']['data']['tax_rate']['name']])) {
            $taxed[$component['price']['data']['tax_rate']['name']] = 0;
          }

          $taxed[$component['price']['data']['tax_rate']['name']] += $component['price']['amount'] / 100;
        }
      }

      $this->orders[] = $order;
    }

    return $taxed;
  }

  function setUp() {
    $this->resetAll();

    $modules = parent::setUpHelper('all', array('commerce_reports_tax'));
    parent::setUp($modules);

    $this->store_admin = $this->createUserWithPermissionHelper(array('site admin', 'store admin'));
    $this->drupalLogin($this->store_admin);
    $this->drupalGet('admin/commerce/config/tax-reports/generate');

    $this->normal_user = $this->drupalCreateUser(array('access checkout', 'view own commerce_order entities'));

    $this->tax_type = $this->createDummyTaxType();
  }

  /**
   * The following test is unfinished thanks to an apparent bug in SimpleTest.
   */
  function testSingleTaxType() {
    debug($this->generateTax());
    debug(commerce_reports_tax());

    $this->drupalLogin($this->store_admin);

    $this->drupalGet('admin/commerce/reports/tax');
  }

}
