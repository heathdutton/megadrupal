<ItemNonInventoryAddRq requestID="p-<?php print $product_id ?>">
  <ItemNonInventoryAdd>
    <Name><?php print $sku ?></Name>
<?php if($parent_sku): ?>
      <ParentRef>
        <FullName><?php print $parent_sku ?></FullName>
      </ParentRef>
<?php endif; ?>
    <SalesOrPurchase>
      <Desc><?php print $title ?></Desc>
      <Price><?php print $price ?></Price>
      <AccountRef>
        <FullName><?php print $accounts['income'] ?></FullName>
      </AccountRef>
    </SalesOrPurchase>
  </ItemNonInventoryAdd>
</ItemNonInventoryAddRq>
