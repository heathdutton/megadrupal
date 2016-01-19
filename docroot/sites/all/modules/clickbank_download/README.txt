Getting Started
===============

1. Enable the module
2. Navigate to admin/config/media/clickbank
3. Enter your secret key from your ClickBank account
4. Upload your product images
  (admin/structure/taxonomy/clickbank_download_images/add)
5. Upload the digital goods
  (admin/structure/taxonomy/clickbank_download_files/add)
6. Create product nodes combining ClickBank product ids and the previously 
  uploaded images and files (node/add/clickbank-download-product)
7. Set the "ClickBank Download Block" in the region it should appear
  (admin/structure/block)
8. Change the "Thank You page" in ClickBank to a URL where the "ClickBank 
  Download Block" will appear for each product
9. Uncheck 'Debug Mode'
10. Test that downloads work successfully


Testing Products
================

Before changing all the "Thank You" pages in ClickBank, I recommend that you
create one dummy product inside ClickBank that customers will not be able to
see. Next, create a test credit card inside ClickBank. Then create a new product
node inside Drupal with the dummy product ID. Uncheck "Debug Mode." Set the 
Thank You page in ClickBank to the download page in Drupal. Logout out of Drupal
and try to complete a purchase of your dummy product using the test credit card.
If all works, you should see the product image and all download links for that
product. Do click the download link(s) to ensure (1) downloading works, and (2)
the correct file is downloaded.


No ClickBank Account
--------------------

If you do not have access to a Clickbank account, go to the module configuration
page and check the "Debug Mode" checkbox. Then go to the url with your download
block enabled, and add a query string like this:

    ?item=<product_id>

For example, to test a product with an id of 1 at example.com/downloads, point
your browser to:

    http://example.com/downloads?item=1

You should see the title, product image, and the download links. Clicking the
download links should trigger an immediate download.

*NOTE:* Using this setting turns off purchase validation, (allowing anyone to
download files without purchasing them). So remember to uncheck that box before
trying to sell anything.  


If Downloads Work...
--------------------

Set the products' "Thank You" pages to point to the node with the ClickBank
block enabled. Make sure the images and digital goods match what you described
in your "Pitch Pages." You should be all set! 


If Something Goes Wrong...
-------------------------

+ Make sure that your secret code in configuration page
    (admin/config/media/clickbank) matches the value set inside ClickBank.
+ Make sure that the product id numbers match the numbers inside ClickBank
+ Make sure that the "Thank You" page listed in ClickBank is correct.
+ Check that all roles can access the ClickBank block inside the configuration
  page 
+ When in doubt....clear the Drupal cache?

If these steps do not fix your option, search the issue queue for a similar
problem to your own. Feel free to ask for help AFTER checking the issue queue.
