<?php

/**
 * @file
 * Salsa API mock implementation for tests.
 */

class SalsaAPIMockCommerceEvent extends SalsaAPIMock {

  /**
   * Overrides SalsaAPIMock::query().
   */
  public function query($script, $query) {
    $return = parent::query($script, $query);
    $query_args = explode('&', $query);
    if (($query_args[0] == 'object=event_fee') && isset($query_args[1]) && $query_args[1] == 'condition=event_KEY=1') {
      $return = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="1">
  <event_fee>
    <item>
      <event_fee_KEY>1</event_fee_KEY>
      <organization_KEY>1</organization_KEY>
      <chapter_KEY/>
      <event_KEY>1</event_KEY>
      <event_fee_group_KEY/>
      <_order/>
      <Category>Normal</Category>
      <amount>50.00</amount>
      <membership_level_KEY>0</membership_level_KEY>
      <Reference_Name>Normal</Reference_Name>
      <Description>Just a normal ticket</Description>
      <Status>Active</Status>
      <Discount_Code/>
      <key>1</key>
      <object>event_fee</object>
    </item>
    <item>
      <event_fee_KEY>2</event_fee_KEY>
      <organization_KEY>1</organization_KEY>
      <chapter_KEY/>
      <event_KEY>1</event_KEY>
      <event_fee_group_KEY/>
      <_order/>
      <Category>Early Bird</Category>
      <amount>100.00</amount>
      <membership_level_KEY>0</membership_level_KEY>
      <Reference_Name>Early</Reference_Name>
      <Description>More Expensive Ticket.</Description>
      <Status>Active</Status>
      <Discount_Code/>
      <key>2</key>
      <object>event_fee</object>
    </item>
    <count>2</count>
  </event_fee>
</data>
EOF;
    }
    if (($query_args[0] == 'object=event_fee') && isset($query_args[1]) && $query_args[1] == 'condition=event_fee_KEY=1') {
      $return = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="1">
  <event_fee>
    <item>
      <event_fee_KEY>1</event_fee_KEY>
      <organization_KEY>1</organization_KEY>
      <chapter_KEY/>
      <event_KEY>1</event_KEY>
      <event_fee_group_KEY/>
      <_order/>
      <Category>Normal</Category>
      <amount>50.00</amount>
      <membership_level_KEY>0</membership_level_KEY>
      <Reference_Name>Normal</Reference_Name>
      <Description>Just a normal ticket</Description>
      <Status>Active</Status>
      <Discount_Code/>
      <key>1</key>
      <object>event_fee</object>
    </item>
    <count>1</count>
  </event_fee>
</data>
EOF;
    }
    if (empty($return)) {
      //debug($query, $script);
    }
    return $return;
  }
}
