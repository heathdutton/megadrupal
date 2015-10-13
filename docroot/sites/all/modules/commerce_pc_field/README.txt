This simple module for Drupal Commerce allows to store and get access to calculated price (sell price) from database. 

Problem
********************

The problem is that by default Drupal Commerce stores only base price without any sell price calculations so we can see ca price only when it's shown on Product Display page and it's calculated dynamically. The most common problem comes up when we need to filter/sort a such prices. For example, if we have couple of products with prices $10 and $7, and after applying a discount 50% off for first product only we'll see in the view 2 products $5 and $7. But if we start to sort them by price we'll see that $7 goes before $5. Commerce Pre-calculated Field module fixes this problem. When you have calculated price as separate field for each of your product, you can work with it from Views or from you sql queries. So in this example we just need to sort by "calculated price" field to see $5 - $7 order.

Installation & usage
********************

1. Install as usual.
2. Just after installing go to Configure page of module (it will be proposed in message after enabling module). (admin/commerce/config/product-pricing/pre-calculation-field).
3. Here you need to pick Product types where field should be created and submit a form.
4. After you will see "Generate pre-calculated prices" button to fill up empty fields with calculated sell price.
5. Also click on this button every time you add/edit rules that affect on sell price, for example Discount rules.

Thats it! After creating/updating products of selected types the sell price will be generated and ready to use.

author: Alexey Posidelov
https://www.drupal.org/u/radamiel