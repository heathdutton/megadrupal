
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation


INTRODUCTION
------------

Current Maintainer: Troels Lenda <troelslenda@gmail.com

There's a lot of breadcrumb modules out there, but none of them build
true breadcrumbs.

Cookie Crumbs, makes use of cookies (hence the name), to provide
history based breadcrumbs, and not the hierarchical standard ones.

Example of use:

For instance your site have top level menu items with a few sub pages. The user
visit the first one (Products) and go down one level to view (01101 Product A).
Then the user looks for the company phone number in top level menu item
(About Company) and then down a level to view (Location) until he finds
(Contact) in the footer. The breadcrumb trail would then be

(Products) - (01101 Product A) - (About) - (Location) - (Contact).

When on the phone the customer can easily refer to the product id from the
breadcrumbs and quickly and convenient navigate back to that page.

It's is essentially a representation of your browsers history. But some target
groups can have a hard time using the browsers back button.

The breadcrumbs are provided through a block with the option to configure how
deep the breadcrumb trail should be. The library, exposed in the block will
append the title of the current page to an unordered list and keep track of
them with cookies.

INSTALLATION
------------

Install the module like with other modules. It depends on Libraries API
and a library called henselsrevenge.
Place jquery.hanselsrevenge.js in sites/all/libraries/hanselsrevenge.
You can download the library from https://github.com/soitgoes/hanselsrevenge
Once the module is installed and library downloaded, you can visit
admin/reports/status on your site, to see whether the library is placed
properly or not.
