Setting up BluePay
------------------

You'll need your numeric Account ID and Secret Key. You can find these after 
logging in to the [BluePay account area][baa] and clicking on Admin -> 
Accounts -> List. Your Account ID and Secret Key will be listed on the right
side of the page.


Development Notes
-----------------

This library uses the API that is documented at the following URL:

https://www.bluepay.com/sites/default/files/documentation/BluePay_bp20post/Bluepay20post.txt

This API varies from other APIs that BluePay offers, so please ensure that you
are looking at and using the correct API documentation when writing patches.


Test Transactions
-----------------

When using the test transactions, you can use the standard test card info:

Card: 4111111111111111
Expr. Date: 10/20 (or any date in the future)
C2V: (any three digits)

**Note: Order whose whole dollar value that are _even_ will fail and _odd_ 
amounts will be successful. e.g. $9.99 and $11.50 will succeed because the whole
dollar amount is odd (9 and 11 respectively). However, $10.99 and $22.00 will
fail because their amounts are even (10 and 22 respectively).**

  [baa]: https://secure.bluepay.com/
