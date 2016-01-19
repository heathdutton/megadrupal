<?php

namespace Drupal\commerce_reports\Tests;

/**
 * Class CommerceReportsUITestCase
 */
class CommerceReportsUITestCase extends CommerceReportsBaseTestCase {

  public static function getInfo() {
    return array(
      'name' => 'Reports user interface',
      'description' => 'Test the reports user interface.',
      'group' => 'Commerce Reports',
    );
  }

  protected function _getLinks($label) {
    $links = $this->xpath('//a[normalize-space(text())=:label]', array(':label' => $label));

    return $links;
  }

  public function testMenuIntegration() {
    $paths = array(
      'admin/commerce/reports',
      'admin/commerce/reports/products',
      'admin/commerce/reports/customers',
      'admin/commerce/reports/sales',
      'admin/commerce/reports/payment-methods',
    );
    $this->drupalLogin($this->store_admin);
    foreach ($paths as $path) {
      $this->drupalGet($path);
      $this->assertResponse(200, t('Store admin can access report: @path.', array('@path' => $path)));
    }

    $this->drupalLogin($this->createStoreCustomer());
    foreach ($paths as $path) {
      $this->drupalGet($path);
      $this->assertResponse(403, t('Customers cannot access report: @path.', array('@path' => $path)));
    }
  }

}
