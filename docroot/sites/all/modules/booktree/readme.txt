Booktree Module:


Description
-----------

BookTree is simple and flexible module.
The module allow site administrator to draw a simple 
tree presentation of one or more book.


Requirements
------------

This module requires Drupal 6.x or a later version.


Installation 
------------

1) Copy/upload the folders to the modules/ directory of your
   Drupal installation.
2) Enable the modules in Drupal (administer -> modules).
3) Configure the root of main tree in admin/config/booktree/booktree


Advanced use 
------------

If you want draw many book or different part of the same book you can use 
this path sintax:

/booktree/ID of root node/[deep]/[max length]




Ex:
  http://mysite.com/?q=booktree/1834/20/50
  http://mysite.com/?q=booktree/1834/20/
  http://mysite.com/?q=booktree/1834/
or with Clean Urls
  http://mysite.com/booktree/1834/20/50
  http://mysite.com/booktree/1834/20/
  http://mysite.com/booktree/1834/


Author
------------
<Uccio> http://www.drupalitalia.org/user/226




