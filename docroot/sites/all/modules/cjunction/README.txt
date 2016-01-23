CJunction is an implementation of the Commission Junction's API for Drupal.

Using this Drupal module you can easily promote products from the huge
database of Commission Junction's (CJ) Affiliate and Merchant Network.
The module takes data from CJ and put it to the nodes.

Required:
---------
curl
openssl for php;


Getting Started:
-----------------
1. Upload and enable the Commission Junction API module. It will require enabling
Commission Junction API Fields module that is part of Commission Junction API module.

2. Go to Administer -> Commission Junction -> Settings and enter required fields.

3. Go to Structure -> Taxonomy then select CJ vocabulary and to add new items.
Here you can fill one or few fields that will be used in the query for getting
products from CJ.
In the field Keywords can be used next combination, for example: "kitchen
sink" - any product with the word "kitchen" or "sink"; "+kitchen +sink" -
any product with the words "kitchen" and "sink"; "+kitchen -sink" -
any product with "kitchen" and without "sink"; "kitchen +sink" - all the
products with the word "sink" and if they also contain "kitchen", it
increases the product's relevancy.
For more information on these parameters, please see the API documentation -
http://help.cj.com/en/web_services/product_catalog_search_service_rest.htm
After saving new products appeared in /admin/content/node.

4. Now is few ways of using the collected data:

  1) to use node view as usually - before, is needed to go to
/admin/structure/types/manage/cjproduct/display (Structure -> Content types ->
CJ Product -> Manage display) and in column Format to change for needed items the
format from Hidden to some other.

  2) you can use Views module for visualization. As an example exist a demo Views 'Commission
  Junction' with list of products and available here /commission-junction .
  The module Taxonomy Menu can be used for creating menu.

  3) go to Configuration -> Content authoring -> Text formats  and select some
text format. Then enable a filter Commission Junction. Now you can
use codeshortcuts like example: [cjunction keywords="Sony notebook" count="5"],
where "Sony notebook" is a taxonomy item name from an item 3 of this instruction.


Some tips
---------
- If you don't want for showing some products (if duplicate) just go to node edit and
in "Publishing options" to disable "Published". In that case product will be updated
on cron but not showed.

- If you need some fields where data will not be change with crone update, just add new
ones and use them.


Trouble solving
-----------------
Trouble: All is done but no one product is showing.
Solution: Firstly check that enabled php extensions Curl and Openssl. Then check the
  settings data /admin/config/cjunction/api . And finally try change settings described
  in an item 3 of Getting Started section of that instruction.

