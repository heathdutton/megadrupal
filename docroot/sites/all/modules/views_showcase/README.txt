Description
===========
This module has been created to fulfill the needs of a good looking, function and easy to use swhocase for Drupal
This module is, in some way, a replacement for the Front Page Slideshow module. The biggetst advantages above FPS is that this module is entirely build on free javascript libraries.


Dependencies
============
This module depends on Views to work.

Installation
============
To install this module follow the basic steps:

* Download de module
* Uncompress it on sites/all/modules folder
* Access the module administration area of your site (admin/build/modules)
* Enable this module

Configuration
============
You'll need to create a view that will get the data to be used as a showcase. This module require at least 3 fields to display a proper showcase:

Image - An image that will be shown on the main area of the showcase
Title - A title to be used as primary text and also as the mini-list selection area
Teaser - A short text that will be shown under the title (if you are using the default template and css)

So, create a view with at least these 3 fields and in Style choose the tipe Views Showcase. Then, choose how this showcase will behave.
One other step is to set the Row style as Views Showcase. Once you've done with that you'll have to choose wich field in your view will be used as Title, Image and teaser.

You can creat a view that displays both a block or a page. The block display, of course, is much more flexible then a page, once you can place it whathever you want

Author
=====

Rafael Ferreira silva

http://rafaelsilva.net
http://drupal-br.org

I'm the creator and mantainer of Brazilian Drupal comunnity website.
