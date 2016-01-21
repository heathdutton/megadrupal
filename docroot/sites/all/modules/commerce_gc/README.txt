INSTALLATION
- Install Commerce Coupon 2.x
- Enable Commerce GC (commerce_gc) for basic giftcard entity support
- Enable Commerce GC Product (commerce_gc_product) to support buying giftcards
  as products.

SPEND A GIFTCARD AT CHECKOUT
- Create a giftcard at admin/commerce/coupons/add/giftcard-coupon
- On the first checkout page after the cart, enter your giftcard code in the 
  “Coupons and giftcards” section.

VIEW PURCHASED GIFTCARDS
- Navigate to user/%/giftcards
- Click on a giftcard to see its transactions
- If you purchase a giftcard for someone else, it will show up on their account, 
  not yours.

VIEW GIFTCARDS AS ADMIN
- Navigate to admin/commerce/coupons/giftcards
- Balance is shown on each giftcard row
- To see a giftcard’s transactions, click on “transactions” in the balance 
  summary of a specific giftcard.

MISC
- Supports both giftcard usage and giftcard purchase integration on the backend.