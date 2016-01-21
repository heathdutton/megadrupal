RESPONSIVE TABLES FILTER
------------------------
Enable the "Make tables responsive" text format filter to display field tables responsively using the [Tablesaw Library](https://www.filamentgroup.com/lab/tablesaw.html).

 * For a full description of the module, visit the project page:
   https://drupal.org/project/responsive_tables_filter

REQUIREMENTS / DEPENDENCIES
---------------------------
Filter module (core)
[Libraries API](https://www.drupal.org/project/libraries)
jQuery version 1.7 or higher. Consider using [jQuery Update module](https://www.drupal.org/project/jquery_update)
[Tablesaw v 1.x or 2.x](https://www.filamentgroup.com/lab/tablesaw.html)

INSTALLATION
------------
1. Extract this module into your modules directory (ex: sites/all/modules/contrib)
and enable it.

2. Clone or download the Tablesaw JS library at https://github.com/filamentgroup/tablesaw
and save it in sites/all/libraries/tablesaw (if you downloaded, you will need
to change the folder name from "tablesaw-master" to "tablesaw").

USAGE
-----
0. If you want to make Views tables responsive, enable this at /admin/config/content/responsive_tables_filter.
1. Go to admin/config/content/formats.
2. On any text formats for which you want to make tables responsive (e.g., Filtered HTML),
Enable the filter "Make tables responsive" .
3. Verify the text format(s) allow HTML table tags (admin/config/content/formats
> “Limit HTML tags”). All of the following should be allowed:
<table> <th> <tr> <td> <thead> <tbody> <tfoot>

Any fields that use the text format(s) which have tables in them will now be
responsive.

FAQ
---
Q: Does this make Drupal Views tables responsive?

A: Yes. Enable this at /admin/config/content/responsive_tables_filter

Q: Can I override the tablesaw CSS?

A: Yes, but any CSS you add needs to include the tablesaw naming patterns so that the Javascript can find elements.

Q: What if I can't use jQuery update due to other requirements?

A: Comment out the dependency on jQuery Update in this module’s .info file, and try using an older version of the Tablesaw library.

Q: Can I target specific tables within nodes?

A: Yes. The Drupal variable 'responsive_tables_filter_table_xpath' accepts an xpath query. See _tablesaw_filter().

MAINTAINERS
-----------
Current maintainers:
 * Mark Fullmer (mark_fullmer) - https://www.drupal.org/u/mark_fullmer
