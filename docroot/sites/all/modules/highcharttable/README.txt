
HIGHCHARTTABLE QUICK INSTALL
============================

If you like using Drush, you can download all you need with these 3 commands:

  drush dl highcharttable
  drush en highcharttable
  drush dl-highcharts

If you do not wish to use Drush, follow these steps.

For the bulk of its features, HighchartTable relies on this jQuery library:
http://www.highcharts.com/download. Please download it and uncompress into
sites/all/libraries. Rename the folder to highcharts (lower-case, no version
number).
The highcharts subdirectory /exporting-server contains sample code that may
compromise the security of your site. We suggest you remove it. In fact, all
you need from HighCharts is the file js/highcharts.js

Also required is the HighchartTable add-on. Press "Download Zip" on this page:
https://github.com/highchartTable/jquery-highchartTable-plugin. Unzip the
downloaded folder to sites/all/libraries, renaming it to highcharttable.

Download and enable the HighchartTable module.

Visit admin/reports/status and verify that the HighchartTable section does not
report any warnings.

With that you should be in business! What remains is to visit
/admin/config/content/highcharttable to assign the chart features of your choice
to the tables of your choice.
Or easier still: find a table on your site and hover above it. A cogwheel will
appear and clicking that allows you to insert the chart in situ.

Click on a the legend to temporarily remove a "series" from the chart.

If you wish to change the column used for the horizontal axis you can by
clicking "Configure this chart". For instance, try column 2 on this table
on your system: admin/reports/visitors (enable the core module Statistics).

Enjoy!

FAQ
---
Q: Of the many columns in my table one contains text values only. It causes all
   the other numeric columns not to be displayed. My chart is blank.
A: You can suppress a text column and thereby popping the numeric columns into
   live by clicking its name in the chart legend. If you have editing control
   over the table you can also exclude the column from the chart by adding the
   'data-graph-skip="1"' attribute to the table header (th) cell.

Q: Do I need to enable the core module "Contextual" in order to use the 
   contextual links provided by HighchartTable?
A: No, you may enable or disable the Contextual module. The contextual links of
   HighchartTable will work regardless.

Q: Can I delete all files from the /highcharttable directory except
   jquery.highchartTable.js?
A: Almost. Keep highchartTable.jquery.json. It contains the version number.

FURTHER READING
http://highcharttable.org
http://www.highcharts.com

HIGHCHARTS License and pricing
http://shop.highsoft.com/highcharts.html
