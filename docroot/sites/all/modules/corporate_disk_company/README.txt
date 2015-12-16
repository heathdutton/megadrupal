
Corporate Disk Fulfillment module for Drupal 7.x.
This module adds the ability to automatically fulfill orders from Drupal Commerce

REQUIREMENTS
------------
* Commerce
* Commerce Shipping
* Account at Disk.com and your customer number

INSTALLATION INSTRUCTIONS
-------------------------
1.  Copy the files included in the tarball into a directory named "corporate_disk_fulfillment" in
    your Drupal sites/all/modules/ directory.
2.  Login as site administrator.
3.  Enable the Disk module on the Administer -> Modules page 
    (Under the "Commerce (shipping)" category).
4.  Fill in required settings on the Administer -> Configuration -> 
    Web Services -> Corporate Disk Fulfillment Service page.
5.  Fill in the available shipping services Disk.com offers on Administer -> 
    Configuration -> Web Services -> Corporate Disk Fulfillment Shipping Services
6.  Configure default and shipping services mappings on Administer -> 
    Configuration -> Web Services ->
7.  Enjoy.

SKU Manipulation
-------------------------
This module allows for a lot of manipulation of the SKUs being processed.  The current options
are based on use cases of running two different eCommerce sites against a single Disk.com account.
You can have SKUs stored in Drupal Commerce products that differ from Disk.com but only differ in a
repeatable pattern that can be manipulated by a string replacement, more below.

There are three portions to the SKU setup of this module:

First is the base SKU setup.  
This field must be populated with the SKU's as they exist in your Disk.com
account.

Second is the global SKU replacment.  
This is an optional field but allows you to have SKUs that differ
in Drupal Commerce from those in Disk.com.  But they must differ in a repeatable fashion that can be
handled by a string replacement.  Example:
In Drupal Commerce we have the following SKUs:  1000-1 2325-1 6789-1
In Disk.com the SKUs are: 1000-5 2325-5 6789-5
These differ by only the two characters at the end of the numbers.  This is the type of SKU replacement this
option in the module supports.

Last option is bundled SKUs.
This allows you to create an SKU in Drupal Commerce that may represent more than one product fulfillment.
The input of this variable must match the SKUs stored at Disk.com for subsitution.
Example:
Product SKU 8976 in Drupal Commerce when sold is fulfilled by SKUs 100, 105, and 106
The setup of the bundle option would be
8976|100 105 106
