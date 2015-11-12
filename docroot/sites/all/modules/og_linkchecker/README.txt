OG Linkchecker
--------------

Description:

The OG Linkchecker module allows designated OG roles to view group-specific
Linkchecker reports so that group owners (for example) can find and fix broken
links in their own groups.

For users with the "Access group broken links report" OG permission, a menu tab
for a group Linkchecker report is added to the main group entity. This report
behaves exactly like the administrative broken links report, but only shows
links which appear in that group.

Users with the Linkchecker "Access broken links report" permission
automatically gain access both to each group's broken links report and to the
"OG Linkchecker groups summary" report, which provides the following
information:

* The content type of each group as well as a link to the group broken links
  report
* The group content types that are associated with each group (so the site
  admin knows which content types to select in the Linkchecker configuration)
* The group roles and whether or not each role has access to the group broken
  links report
* The number of broken links in each group (not including links in comments)


Required Modules:

1. Link Checker http://drupal.org/project/linkchecker
2. Organic Groups http://drupal.org/project/og


Setup:

Once you have installed, enabled, and configured the required modules, usually
all you will need to do is install and enable the OG Linkchecker module. More
specific information will be available at admin/help/og_linkchecker once the
module is enabled.


Acknowledgements:

The OG Linkchecker module is the result of work done by the USAID-funded
Knowledge for Health (K4Health) Project, which is led by the Johns Hopkins
Bloomberg School of Public Health’s Center for Communication Programs (CCP).
Its contribution to Drupal.org is sponsored by K4Health and USAID.
