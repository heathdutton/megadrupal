Module: easyrec for ubercart
Author: David Mann <http://easyrec.org/contact>


Description
===========
Uses the easyrec recommender web service to store user actions and generate recommendations to your website.

Requirements
============

* a free easyrec user account ( http://easyrec.org/ )


Installation
============
* Copy the 'easyrec_for_ubercart' module directory in to your Drupal
sites/all/modules directory as usual.


Usage
=====
In the settings page enter your easyrec API key and tenant name. After that your web shop automatically sends
view and buy actions to easyrec.

To test all recommendations set up a test shop with 2 users:

look at three products with both of them
buy three products with the two users

Now you have to wait until our easyrec.org server starts his cron job to calculate the recommendations. (2:00AM GMT +01)
