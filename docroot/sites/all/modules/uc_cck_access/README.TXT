This module extends Ubercart product features and is compatible with Ubercart Marketplace.

It allows customers to buy access to various content items on your site when they purchase specified Ubercart products.

Access can be configured to start immediately, a fixed period after the purchase, or on a preset date, and it can be given either for indefinite time, until a preset date or for a limited period based on the feature's settings.

Currently it includes a handler for nodes and fields (it requires ACL and Content Access modules).

With future addition of more handler modules, it can grant access to other items (i.e. Taxonomy, Roles).

Module name does not truly reflect that it can control access to other items (I'm open for new name suggestions).

FEATURES
* Extensible with new access handlers. Node & Fields handler is included.
* Each product can have many access items (added as product features), each item can have its own access handler. This allows for example, to create a product that gives access to a field on a node AND taxonomy access to a set of nodes AND a new role.
* Each access product feature can have its own schedule - when to grant access, and when to expire the access.
* User account view gets a new tab - "Premium Content", which lists all items that are granted or will be granted.
* Nodes get a new section on "Access Control" tab (visible to admins only) that lists all users that have access or will have access.


REQUIREMENTS
Ubercart (Store, Order, Product), Rules for main module
ACL, Content Access for node & fields access handler module.

USAGE
* Install the module - copy to your sites/all/modules directory, enable "UC CCK Access" module and "UC CCK Access ACL" handler module on admin/build/modules page.
* Configure few defaults on the admin/store/settings/products page (under "CCK access settings" section).
* Create product features for existing products or when creating new products (on node/***/edit/features page) - select "CCK access" and click add.
* Configure the feature - select access handler, settings provided by the handler, and access schedule

MAKING IT WORK
There are few things that are not self-evident - access to nodes and fields can be very complicated to figure out. Many modules cooperate with (or butt against) each other. If one module gives access, another module cannot take it back.

Here are few tips for included "UC CCK Access ACL" access handler module:

Common configuration
1. Create content type for your nodes and fields (follow instructions for fields, add your fields to the content type)
2. Go to admin/structure/types/manage/%type/access (where %type is your content type from previous step)
3. Enable "per content node access control settings" and save
4. Create content of %type from above (add body, upload files to the fields, etc.)
5. Go to node/%nid/access (where %nid is the node id that you created in previous step)

Next steps depend on whether you want to have a preview, or hide the whole node from users that did not pay for it.

To sell access for a full node (no preview)
Usually access is given to the whole node with all fields in it (hide whole node until paid for).

6A. Disable "view any content" for all user roles on node/%nid/access page and save
    Test that nobody can view that node but admin or other supervisor roles


To sell access for a specific field on a given node (with preview of other fields)
An example can be a node with two file fields - one is a video preview, another is a full video. The node and video preview is accessible to everyone using content_access module. A product feature then gives access only to the file field with a full video to users who purchased the product.

6B. Enable "view any content" on node/%nid/access page for user roles you want to access the node (it will be limited to preview fields only) and save
    Test that everybody can view that node and all fields (including the premium ones)


Continue common configuration
7. Create ubercart product (follow instructions in Ubercart)
8. Go to node/%nid_product/edit/features (where %nid_product is the product node from previous step)
9. Select "CCK access" in "Add a new feature" drop-down box and click "Add" button
10. Enter your %nid (node to sell access to) into "Node id"
11. For option 6B check the fields you want to be sold. Any unchecked fields will have access regardless of payment.
Note that if you had chosen 6A, the whole node and any of its fields won't be shown until the node is paid for. Typically you leave all fields unchecked for option 6A.
12. Fill in all remaining options and "Save feature"
    Test that everybody can view that node and all fields except the fields you checked in the product feature above.

Now users will have a product that they can buy to view premium fields. They can view non-premium fields before purchase (option 6B), but they have to buy the product to view premium fields. Fields can be anything. E.g. you can have file field for videos.


BACKGROUND
This module is a complete redesign and rewrite of uc_node_access with a lot of additional features. A lot of issues with uc_node_access were fixed in the new architecture. Clean separation of functionality between this module and access handler modules was performed, with API between them that opens possibility for other extensions.
The code was ported to Drupal 7 and further improved.

This module is different from modules:
  http://drupal.org/project/nodeaccess
  http://drupal.org/project/uc_node_access
Although Nodeaccess can be used with Ubercart to sell access to nodes, and UC Node
Access is a native Ubercart solution for selling access to nodes, neither of those
provide the granularity that UC CCK Access does. In other words, with
Nodeaccess/UC Node Access you either give access to the node (all the fields) or
totally deny it, and with UC CCK Access you can have fine access granularity per
each field.

Bugs from uc_node_access that are fixed in uc_cck_access:
- http://drupal.org/node/1057330 Manually revoking a permission does not clean up database
- http://drupal.org/node/1204394, http://drupal.org/node/430280  Manually grant node access for a user, User account area for purchased nodes (http://drupal.org/files/issues/manually_grant_access-1204394-1.patch)


OTHER
I am not planning to develop any new access handlers at this time, but co-maintainers that are willing to do so are welcome.


