<ItemInventoryAddRq requestID="p-<?php print $product_id ?>">
  <ItemInventoryAdd>
    <Name><?php print $sku ?></Name>
<?php if($parent_sku): ?>
    <ParentRef>
      <FullName><?php print $parent_sku ?></FullName>
    </ParentRef>
<?php endif; ?>
    <SalesDesc><?php print $title ?></SalesDesc>
    <SalesPrice><?php print $price ?></SalesPrice>
    <IncomeAccountRef>
      <FullName><?php print $accounts['income'] ?></FullName>
    </IncomeAccountRef>
    <COGSAccountRef>
      <FullName><?php print $accounts['cogs'] ?></FullName>
    </COGSAccountRef>
    <AssetAccountRef>
      <FullName><?php print $accounts['assets'] ?></FullName>
    </AssetAccountRef>
  </ItemInventoryAdd>
</ItemInventoryAddRq>
