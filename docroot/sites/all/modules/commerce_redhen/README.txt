Commerce Redhen
=================

Integration between Redhen CRM and Drupal Commerce. For now, the following
 has been implemented:

- Redhen Memberships - enable Commerce products to grant Redhen Memberships
  when they are purchased.


INSTALLATION
----------------

For this module to work, you need at least one RedHen Contact Type and one
RedHen Membership Type created. You will be given a warning if this is not
the case.

Configure which Product Types will be able to grant RedHen Memberships by
going to /admin/commerce/config/redhen.

If you want to grant memberships to organizations, you must select the
appropriate product type. A custom line item type for that will be created
with the same name.  Ensure that the add to cart form widget settings are
set to use the new line item type.
