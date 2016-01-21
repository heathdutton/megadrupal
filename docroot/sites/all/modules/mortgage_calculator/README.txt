Real Estate Mortgage Calculator module is a simple mortgage calculator.
It is designed to operate with the Drupal Real Estate module
( http://drupal.org/project/real_estate ), but also can be used on
any other Drupal sites.

For calculation used a mortgage algorithm -
http://www.moneychimp.com/articles/finworks/fmmortgage.htm
Payment Frequency -
http://www.gailvazoxlade.com/articles/home_sweet_home/payment_frequency.html

For now exist two types of the blocks:
    Mortgage Calculator - used a submit form
    Mortgage Calculator JS - used a JS calculation


INSTALLATION:

1. Download and enable the Real Estate Mortgage Calculator module.
2. Visit the /mortgage-calculator page
3. Go to /admin/structure/block for managing an enabled Mortgage Calculator
block or enabling Mortgage Calculator JS block.


MORTGAGE CALCULATOR BLOCK - configuration

For the Mortgage Calculator block can be selected payment frequency methods: Monthly,
Yearly, Semi-monthly, Bi-weekly, Weekly, Accelerated Bi-weekly, Accelerated Weekly.
That can be done from the block configuration
/admin/structure/block/manage/mortgage_calculator/mortgage_calculator_1/configure

For populating the forms' values can be used tokens. For showing it should be enabled the
Token module https://drupal.org/project/token .
The tokens should be placed in appropriated fields of "Default values" section
of the block configuration
/admin/structure/block/manage/mortgage_calculator/mortgage_calculator_1/configure


MORTGAGE CALCULATOR JS BLOCK - configuration

For the Mortgage Calculator JS block can be selected payment frequency methods: Monthly,
Yearly, Semi-monthly, Bi-weekly, Weekly, Accelerated Bi-weekly, Accelerated Weekly.
That can be done from a block configuration
/admin/structure/block/manage/mortgage_calculator/mortgage_calculator_js_1/configure
By default selected - Monthly. (In that case will not shown a checkbox for selecting
a payment frequency method.)

For embedding Mortgage Calculator JS somewhere in a text, may be used filters:
  [mortgage_calculator_js:PRICE;RATE;YEARS]
  (where values: PRICE,RATE,YEARS must be replaced by appropriate values. For
  example: [mortgage_calculator_js:10000;4.5;10])

For enabling them, go to admin/config/content/formats, select appropriate format
and enable 'Set values for Mortgage Calculator'
More details https://drupal.org/node/778976

For populating the forms' values can be used tokens. For showing it should be enabled the
Token module https://drupal.org/project/token .
The tokens should be placed in appropriated fields of "Default values" section
of the block configuration
/admin/structure/block/manage/mortgage_calculator/mortgage_calculator_js_1/configure

