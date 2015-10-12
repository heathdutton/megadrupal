
Pictured cart block provides some advantages in contrast with standart
Ubercart block.

 * Show product image (or default icon).
 * Vertical or horisontal ("large icons") orientation.
 * Sort by name, quantity or price.
 * User can decrease or increase quantity of product right in block
   (and remove in vertical orientation).
 * Optional show item's description in vertical orientation (e.g. selected
   attributes, content of product kit, etc).
 * Optional scrolling.
 * Items count in summary is number of icons, not summa of
   products quantity (optional).

UC pic cart block was written by Dmitriy "Vetkhy" Kosolapov,
kosolapov.d@gmail.com, http://vetkhytest.ucoz.ru/ for russian readers.

Dependencies
------------
 * Ubercart :) - http://drupal.org/project/ubercart
 * ImageCache - http://drupal.org/project/imagecache
 * ImageField - http://drupal.org/project/imagefield

Our module recognizes "Restrict qty" product feature
(http://drupal.org/project/uc_restrict_qty).

"Ajax driven cart" is not supported.

Install
-------
Installation is typical: extract archive to the modules folder of your
Drupal site (...sites/all/modules/ in most cases), enable it in admin panel
(Administer -> Site building -> Modules, /admin/build/modules). After that
place and set up pictured cart block (Administer -> Site building -> Blocks,
admin/build/block).

IMPORTANT: if you are using caching of blocks or pages, cart block works
only for registered users. In this case guests are seeing message and links
to register, login and cart page or nothing (optional).

Note the standart cart block is not compatible with caching too.

Designers can customize block via CSS and changing graphics used ("img" subdir).
Note the filenames must be equal if you don't want to modify code :)

Misc
----
English is foreign language for me, so I'm so sorry for possible grammatical
mistakes and I'll be very glad if you can help me to correct them.