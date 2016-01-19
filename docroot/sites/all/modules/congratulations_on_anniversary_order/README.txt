Congratulations on Anniversary Order module allows you to congratulate client on
his anniversary order. 
Namely, this module allows you to set anniversary order number at it's admin
page, and when an order with a given number will be completed by one of your
customers, module will show custom checkout pane with congratulations to the
customer. It is also possible to send e-mail with congratulations and additional
data to the client.

To install this module, just download it and copy into your site's modules
folder. Once copied, go to admin/build/modules page and enable Congratulations
on Anniversary Order module. You have to install it's dependencies - Commerce
and Commerce Message Pane - as well.

Now go to module's settings page
(admin/commerce/config/congratulations_on_anniversary_order) and set anniversary
order number (field "Set the anniversary order number"). Alter other settings,
if needed. Afterwards go to checkout settings page
(admin/commerce/config/checkout) and fill custom checkout pane created by
module (it's title is "Anniversary order"). For example, write congratulational
text, or provide a code to get a discount on a next order.

This module provides rules event "After anniversary order was completed".
This event is being called after user have created anniversary order, it will
allow you to trigger arbitrary actions in case anniversary order was completed,
so you can do something non-standard with it.
