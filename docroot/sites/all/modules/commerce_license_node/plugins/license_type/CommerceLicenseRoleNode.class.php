<?php

/**
 * Extends the role license type with the ability to create paid nodes.
 */
class CommerceLicenseRoleNode extends CommerceLicenseRole implements CommerceLicenseBillingUsageInterface  {

  /**
   * Implements CommerceLicenseBillingUsageInterface::usageGroups().
   */
  public function usageGroups() {
    $return['nodes'] = array(
      'title' => 'Created content',
      'type' => 'gauge',
      'product' => $this->wrapper->product->sku->value(),
      'free_quantity' => $this->wrapper->product->cl_node_quota->value(),
      'immediate' => TRUE,
      'not_charged' => TRUE,
    );

    return $return;
  }

  /**
   * Implements CommerceLicenseBillingUsageInterface::usageDetails().
   */
  public function usageDetails() {
    $usage = commerce_license_billing_current_usage($this, 'nodes');
    return t('Created content: @nodes', array('@nodes' => $usage));
  }
}
