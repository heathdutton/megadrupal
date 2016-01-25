- - - - - About this module

Commerce UPS module provides real-time shipping cost estimates for 
United Parcel Service (UPS).


- - - - - Dependencies

Obviously, this module depends on the Commerce module 
(http://www.drupal.org/project/commerce). In addition, the following modules 
are required:

* Commerce Physical - http://www.drupal.org/project/commerce_physical
This module is used to define the physical properties (weight and dimensions) 
of each product. This information is necessary to determine a shipping estimate.

* Commerce Shipping - http://www.drupal.org/project/commerce_shipping
This provides the infrastructure for Commerce UPS to fully integrate with the 
Commerce module.
You must use the commerce_shiping 2.x!


- - - - - Optional modules

* AES - http://www.drupal.org/project/aes
This module is strongly suggested in order to securely store the site's 
UPS credentials. 


- - - - - Installation

1. Install and enable the module and all dependencies (be sure to use the 
latest versions of everything). Add dimensions and weight fields (new field 
types via the Commerce Physical module) to all shippable product types. 
Populate dimensions AND weight fields for all products. IF YOUR PRODUCTS 
DON'T HAVE BOTH PHYSICAL WEIGHT AND DIMENSIONS, A SHIPPING QUOTE WILL NOT BE 
RETURNED - YOU HAVE BEEN WARNED, IF TEXT FILES HAD A <BLINK> TAG, I'D BE 
USING IT HERE TO MAKE SURE THAT THIS IS CRYSTAL CLEAR ;)

2. Configure the "Shipping service" checkout pane so that it is on the 
"Shipping" page. The "Shipping service" checkout pane MUST be on a later page 
than the "Shipping information" pane. (admin/commerce/config/checkout)

3. Configure the UPS settings (admin/commerce/config/shipping/methods/ups/edit). 
You'll need to create UPS account and obtain an access key 
via https://www.ups.com/upsdeveloperkit. 

4. Optionally install and configure the AES encryption module 
(http://www.drupal.org/project/aes). This will help keep your UPS 
credentials secure.


- - - - - Limitations

Eventually, all of these limitations may be addressed. For now, be warned.

1. Single "Ship from" address for all products.

2. "Customer supplied packaging" is the only allowed packaging type.

3. Doesn't ensure product dimensions are less than default package size 
dimensions. In other words, if you have a product that is 1x1x20 (volume=20) 
and your default package size is 5x5x5 (volume=125), even though the product 
won't physically fit in the box, these values will be used to calculate the 
shipping estimate.

4. Doesn't play Tetris. For example, if you have an order with 14 products with 
a combined volume of 50 and your default package size has a volume of 60, the 
shipping estimate will be for a single box regardless of if due to the 
packaging shape they don't actually fit in the box.

5. Doesn't limit the weight of packages. If you're trying to ship a box full of 
lead that weighs 600lbs, this module will let you (instead of breaking the 
order into more packages).

6. Doesn't account for packing material. If you need to account for packing 
material, then you may want to adjust product dimensions accordingly.


- - - - - Methodology

Calculating estimated shipping costs is a tricky business, and it can get 
really complicated really quickly. Knowing this, we purposely designed this 
module with simplicity in mind. Here's how it works:

1. Every order must contain at least one package.

2. The number of packages is determined by calculating the total volume of all 
products in the order, dividing by the volume of the default package size, 
and rounding up.

3. The weight of each package is determined by dividing the total weight of all 
products in the order by the number of packages.

If you need custom functionality, you have several options:

1. Determine if it is something that can be generalized to suit a number of 
users and submit it via the issue queue as a suggestion for inclusion in 
this module.

2. Hire one of the maintainers to create a custom module that interfaces with 
Commerce UPS to add your custom functionality.

3. Break open a text editor and start coding your own custom module.


- - - - - Authors/Maintainers/Contributors

Frank Lakatos - http://drupal.org/user/834502
Chris Calip - http://drupal.org/user/210499
Ryan Szrama - http://drupal.org/user/49344
Andrew Riley - http://drupal.org/user/98079
Michael Anello - http://drupal.org/user/51132

A good portion of this module was originally written during the 
DrupalCamp Atlanta 2011 code sprint. Thanks to all who participated 
including Brent Ratliff and Tom Geller. 
