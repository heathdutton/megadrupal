Integrate Profile2 with XML sitemap. This module generate XML Sitemap links with
the profile2 profiles.

Comparing to the xmlsitemap_user module that comes with the xmlsitemap module,
this module generates links with the Profile2 profiles instead of user profiles.
A user without a Profile2 profile does not get indexed by XML Sitemap with this module.


Installing
----------
See http://drupal.org/getting-started/install-contrib for instructions on
how to install or update Drupal modules.


Usage
-----
Once Profile2 XMLSitemap is installed and enabled, a Profile tab would appear in
admin/config/search/xmlsitemap/settings. Adjust the XML Sitemap settings for
each Profile2 profiles as you would for other items that support XML Sitemap.

Alternatively, a XML Sitemap fieldset is added to the settings page of each
Profile2 profile under admin/structure/profiles/manage/[profile].
This is similar to XML Sitemap fieldset on the node edit form which you can
adjust XML Sitemap settings for the particular Profile2 profile you are editing.
