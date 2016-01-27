README.txt
==========

THIS MODULE IS ONLY USEFUL FOR AFFILIATES WITH THE AVANTLINK NETWORK.

It has the same capabilities for affiliates as the AvantLink Wordpress Add-on.
Included functionality allows the use of the AvantLink Affiliate Link Encoder(ALE),
Product Search, and the display of Related Products.

The module creates a new content type called "Avant". This is where content needs to be created
to be associated with the Product Search and Related Products display. The ALE tool is available to
all content types if it's enabled except the front page. The idea is to allow some content to be created
that is free of the commercially related tools.

3 fields have been added to the Avant content type so that when you create content
you have the option of adding keywords to be used in the Related Products display. Also you can opt out
of using Related Products on an individual Avant content type. There is an option to add negative keywords
also on a per Avant content type. basis.

Settings in the Related Products configuration are designed to give fall back global keywords
for content that doesn't have keywords set on a per Avant content type basis.

ALE configuration allows associating your ALE subscription at avantlink.com with the AvantLinker module.
Also you can turn it off completely here.

This module configures 2 blocks that are available for positioning within your theme. One is for the
Product Search form and the other is for the display of Related Products.

INSTALLATION NOTES
=================

1) After normal installation go to Structure >> Blocks and position the 2 blocks labeled "AvantLinker Product Search"
and "AvantLinker Related Products". These are containers for the 2 features described in their titles. I like
them in the Sidebar second region.

2) Next go to "configure" for each of the Related Products block and restrict it's display Visibility setting
to the Avant content type. This will leave Article and other content types free of the ads.

3) You can use more content types, but may want to have some content without Product Search or Related Products.

The ALE feature is coded in the module to be configured only for Page, Article, Avant content types'
and the front page. It must be enabled in the admin configuration. The JavaScript it adds should be visible in the source
for the head area of your content.

BE AWARE: Uninstalling Avantlinker will remove the data that makes Related Products work. It can be disabled
without uninstalling and data remains in the database. Posts with the Avant content type will not be uninstalled.