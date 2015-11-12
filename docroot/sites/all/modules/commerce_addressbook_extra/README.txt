This module for Drupal Commerce extends Commerce Addressbook module and allows to add customer profiles from Register form. Also it allows to setup what field should be used to show the values for ADDRESSES ON FILE select-list on Checkout page.

Features
********
1. Ability to expose customer profile on Register form and after signin up have access to it on AddressBook tab.
2. Ability to setup default field for showing options for ADDRESSES ON FILE on Checkout page. It fixes bug with empty options when Customer profile doesn't contain "Postal address" field type.
3. Abilitiy to check "Enable the Address Book" box for every customer profile on same form that is very convinient.

Installation & usage
********************

1. Install as usual.
2. Just after installing go to Configure page of the module (it will be proposed in message after enabling module). (admin/commerce/customer-profiles/addressbook-extra).
3. Here you need to pick Customer profiles that should be exposed to Register form and submit a form.
4. You can also setup or override field that is shown in ADDRESSES ON FILE select-list on Checkout page.

Dependencies
************

Commerce Addressbook

Another Commerce modules
************************
https://www.drupal.org/project/commerce_pc_field
https://www.drupal.org/project/commerce_panes_manager

author: Alexey Posidelov
https://www.drupal.org/u/radamiel