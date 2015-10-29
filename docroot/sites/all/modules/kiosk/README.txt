$Id: README.txt,v 1.3.2.1 2010/03/05 20:50:05 tylerwalts Exp $

This module allows an administrator to put selected computers into "Kiosk Mode".

"Kiosk mode" simply means that a given computer will have an extra flag set
for template files, which allows a themer to exclude certain page elements, such
as off-site navigation.  

A computer may be put into "Kiosk mode" in two ways.  First, an IP address can
be set to always go into Kiosk mode.  Secondly, a given computer can have a
cookie set by the administrator that will persist for one year or until cookies
are cleared.  Either has the same effect.

Note that this module is not compatible with aggressive page caching, as it 
needs to maintain a separate kiosk page cache table.  That page cache will be
cleared on cron, regardless of whether the main page cache has been cleared
or not.  That may result in a delay before updates appear on kiosk computers.

INSTALLATION

Upload and enable the kiosk module following the normal process for
contributed Drupal modules.  

The theme author can then put conditionals into the template like so:

<?php if (!$kiosk) : ?>
  <?php echo $header_that_you_don't_want_on_kiosk_machines; ?>
<?php endif; ?>
