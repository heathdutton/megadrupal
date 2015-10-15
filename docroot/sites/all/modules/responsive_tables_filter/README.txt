RESPONSIVE TABLES FILTER
------------------------
Enable the "Make tables responsive" text format filter to display field tables responsively using the [Tablesaw Library](https://www.filamentgroup.com/lab/tablesaw.html).

 * For a full description of the module, visit the project page:
   https://drupal.org/project/responsive_tables_filter

REQUIREMENTS / DEPENDENCIES
---------------------------
Filter module (core)
[Libraries API](https://www.drupal.org/project/libraries)
[jQuery Update module](https://www.drupal.org/project/jquery_update)
[Tablesaw v 1.0.4](https://www.filamentgroup.com/lab/tablesaw.html)

INSTALLATION
------------
1. Extract this module into your modules directory (ex: sites/all/modules/contrib)
and enable it.

2. Clone or download the Tablesaw JS library at https://github.com/filamentgroup/tablesaw
and save it in sites/all/libraries/tablesaw (if you downloaded, you will need
to change the folder name from "tablesaw-master" to "tablesaw").

USAGE
-----
1. Go to admin/config/content/formats.
2. Enable the filter "Make tables responsive" on any text formats for which you
want to make tables responsive (e.g., Filtered HTML).
3. Verify the text format(s) allow HTML table tags (admin/config/content/formats
> “Limit HTML tags”). All of the following should be allowed:
<table> <th> <tr> <td> <thead> <tbody> <tfoot>

Any fields that use the text format(s) which have tables in them will now be
responsive.

FAQ
---
Q: Can I override the table CSS?

A: Yes, but any CSS you add it needs to include the tablesaw naming patterns so that the Javascript can find elements.
 
Q: What if I can’t use jQuery update due to other requirements?

A: Try using an older version of the Tablesaw library.


MAINTAINERS
-----------
Current maintainers:
 * Mark Fullmer (mark_fullmer) - https://www.drupal.org/u/mark_fullmer
