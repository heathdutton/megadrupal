A simple module that allows to display login, register
and password reset pages using overlay.

Anonymous visitors must have access to use the overlay module.
There ARE security concers about it, don't say you weren't warned.
Please see at the bottom of this file for more info.

Upon activation the module createsi a new menu in the drupal menu list
(admin/structure/menu) and three new links are made available to the user:
 overlay-login
 overlay-register
 overlay-password
that are to be used in custom links.
Any call to these links will be displayed using overlay.

If you need to display custom pages using overlay please check out
the Overlay Paths module: http://drupal.org/project/overlay_paths

SECURITY CONCERS
As described above, the are security concerns about giving access to overlay
to anonymous users.
While these are just concers and I haven't found anything that works by the
time I'm writing this readme (17/10/2012 - Drupal 7.15) it is only logical
to inform potentional users of this module.
The only exploit I could find is bullet 3.1 from here
http://exploitsdownload.com/exploit/na/drupal-cms-712-cross-site-request-forgery
and it doesn't apply to Drupal 7.15
