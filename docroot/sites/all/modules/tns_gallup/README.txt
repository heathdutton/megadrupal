The official Danish internet traffic statistics from the Association
of Danish Media is handled by TNS Gallup (as of January 1, 2016).

This module provides an easy method for adding the tracking script to
your Drupal site.

You must download the tracking script from
http://www.tns-gallup.dk/iam/dokumenter/spring.js.zip and place it in
a folder named `tns_gallup_spring` in the libraries folder.

You can download and place the script with `drush
tns-gallup-download-libraries`. If you build your site with `drush
make` and add the TNS Gallup module to your make file the script is
downloaded when you run `drush make`.

When you have installed and enabled the module head over to
`admin/config/system/tns-gallup` and configure the Site ID provided to
you by TNS Gallup. If you prefer you can set the Site ID directly in
the variable `tns_gallup_site_id` (i.e. in your settings file).

The tracking script allows you to specify a ContentPath for filtering
and grouping of the statistics (i.e. specifying the site hierarchy).

Since how to set the ContentPath is actually business logic I decided
the module should just set it to the same as your Site ID and then
provide a hook for altering it. This way you can implement your own
business logic by implementing
`hook_tns_gallup_content_path_alter($content_path)`.

The module maintainer is not affiliated with TNS Gallup or the
Association of Danish Media.
