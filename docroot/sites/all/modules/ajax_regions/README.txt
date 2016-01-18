INTRODUCTION
------------
This module allows to configure flexibly the internal links of your site, which
will update the appointed regions using Ajax, instead of full page refresh.
Using this module you can configure the usual ajax-menu without writing a line
of code.

All the options are collected on one page of module configuration.
(admin/config/system/ajax_regions)

You have to set json-object with list of regions (column AJAX-REGIONS) to links
with certain CSS-selectors (column LINK), - which have to be updated by click.
The json-object is specified as a:
{"region_name1": "region-selector-1", "region_name2": "region-selector-2" ... }

Furthermore, you can view the following additional parameters in the options
block:
SET LOADING-INDICATOR - imitation of indicator of loading the page by the
browser
UPDATE DOCUMENT TITLE - title page refresh
UPDATE CURRENT ADDRESS - the name speaks for itself
UPDATE ACTIVE LINKS - setting to new link class = "active" (this class is used
in menu of the theme appearance Bartik)

Besides, you have got the possibility to indicate your own js-functions
Drupal.ajax_regions.before and Drupal.ajax_regions.after, that will be executed
correspondingly - before and after loading of ajax-content. Clear these fields,
if you don't want the functions to be generated on the pages of your site.


CONFIGURATION
-------------
* Customize the module settings in
  Administration > Configuration > Ajax Regions
  - Choose at least one CSS-selector for links which should ajaxify
    e.g "#main-menu-links a" without the quotes (Bartik theme)
  - Set json-object with list of regions which have to be updated by click
    e.g. (Bartik theme):
    {"content": ".region-content", "sidebar_second": ".region-sidebar-second"}
  - Set additional optional parameters (see INTRODUCTION)
* Set permission "Administer Ajax Regions configuration page" in
  Administration > People > Permissions
  - PLEASE NOTE THAT ANYONE GRANTED THIS PERMISSION CAN POST INJECT ARBITRARY
    JAVASCRIPT (INCLUDING XSS EXPLOITS). THERE IS NO WAY OF BLOCKING XSS
    EXPLOITS (INSERTED VIA THE ADMIN GUI) WITHOUT CRIPPLING THE MODULE.
