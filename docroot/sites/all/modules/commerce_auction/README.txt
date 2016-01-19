Commerce Auction

This module lets you create auction websites using drupal commerce.
It uses eck to create a new entity for commerce auction bids.

The module depends on eck, Entity Reference, Commerce Price field 
and Timeout field.

You should not give "(Add/Edit/Remove) Commerce Auction Bid 
Auction Bid Entities" permissions to your users, instead give them 
'Place bids on auctions' permission so that all the bids go 
through this module and are validated properly. The form on 
node/%node/bid does some validation on bid amount and auction timeout.

The module consists of three sub-modules:
1. commerce_auction_bid: provides core bidding system
2. field_helper: provides a field formatter for entityreference fields 
   which shows the count of referenced entities and a read-only widget
   for commerce_price fields. You can use this formatter to show the
   bids count.
3. commerce_auction_lineitem: creates a new commerce_line_item type, 
   updates auctioned product price and adds the product to the 
   auction winner's cart when the auction is finalized. The auctioned
   products are added to the cart as 'commerce_auction_lineitem' line_items.

After enabling the module, go to 'admin/commerce/auction' and select auction
display content types and configure auction time extensions (default value 
for time extension is to extend auction time for 30 minutes if there is 
a new bid in the final 15 minutes of the auction time). The module will add
the required fields to auction display types automatically. The administration 
interface adds these fields to the selected content types (and removes them if
you deselect an item from the list) :

1. A commerce_price field which contains highest bid value(auction_highest_bid)
2. An entityreference field with unlimited cardinality which holds all the bid
   entities attached to this auction (auction_bid_refs)
3. A commerce_product_reference field (product_field)
4. A timeout field which is used to finalize the auction in the given time.
