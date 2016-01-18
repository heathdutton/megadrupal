UBERCART ORDERS ACCESS README

This module allow site administrators to allow access to view, edit, delete or
change status of Ubercart orders only by certain roles and only if order has a
certain status. 
Permissions are set for individual statuses of orders not globaly.

INSTALATION
- Install this module normally (like you do with any other module - copy module into your site modules directory and enable it).
- Go to site permission configuration page and allow Order Access: View orders for roles that should view at least orders from one or more statuses (for example shipped or completed.
- Go to admin/store/settings/orders/access and configure order permission for each order status and role.
- Go to any order views you have and change Access from Permission | View all orders to Permission | View orders
- That should be it, now users can see orders based on order status and user role.
