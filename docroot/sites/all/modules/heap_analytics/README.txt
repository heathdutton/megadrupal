Module: Heap Analytics
Author: Iztok Smolič <http://drupal.org/user/123987>


Description
===========
Adds the Heap Analytics tracking system to your website.

Requirements
============
* Heap Analytics user account that you get on their website heapanalytics.com.


Installation
============
* Copy the 'heap_analytics' module directory in to your Drupal
sites/all/modules directory as usual.


Usage
=====
In the settings page enter your Heap Analytics account number.

The "Page specific tracking" area comes with an interface that
copies Drupal's block visibility settings.

The default is set to "Add to every page except the listed pages". By
default the following pages are listed for exclusion:

admin
admin/*
batch
node/add*
node/*/*
user/*/*

These defaults are changeable by the website administrator or any other
user with 'administer heap analytics' permission.

Like the blocks visibility settings in Drupal core, there is now a
choice for "Add if the following PHP code returns TRUE." Sample PHP snippets
that can be used in this textarea can be found on the handbook page
"Overview-approach to block visibility" at http://drupal.org/node/64135.
