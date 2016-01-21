
-- SUMMARY --

Easychart provides a way to create charts based on the powerful Highcharts library (http://www.highcharts.com/products/highcharts).

What makes this a really awesome module:

* Easychart defines a new Content Type named 'Chart' that allows you to add one or more charts to your website.
* Support for Highcharts 4.1.9
* Covered chart types: line, spline, column, bar, area, areaspline, arearange, areasplinerange, pie, boxplot, bubble, columnrange, errorbar, funnel, scatter, gauge
* Charts can be used and reused in other content types, panels, pages...
* Easychart integrates with the WYSIWYG module and provides an editor button that allows you to add charts to your WYSIWYG content (CKeditor and TinyMCE are supported).
* Easychart gives you an easy interface to configure your charts through the Easychart jQuery plugin.
* Easychart is built on top of the Highcharts-Configuration-Options (http://api.highcharts.com/highcharts/option/dump.json).
* A brand spanking new UI, customizable through JSON-object. Absolutely awesome!
* Copy-paste data from libreoffice/numbers/excel/csv-file and manage in easy-to-use table.
* In-page help through tooltips for every option in the configuration form.

Attention:

Highcharts is free for personal, school or non-profit projects under the Creative Commons Attribution - Non Commercial 3.0 License.
For commercial and governmental websites and projects, you need to buy a license. (But they're absolutely worth every penny.) See http://shop.highsoft.com/highcharts.html.

-- INSTALLATION --
through Drush:
1. download and install the Easychart module: drush en easychart -y
2. download and install the required javascript libraries: drush ec-dependencies -y

traditional:
1. Download and install the Easychart module: https://drupal.org/project/easychart
2. Download the Highcharts plugin (v4.1.9) at http://www.highcharts.com/download and place it in /sites/all/libraries/highcharts.
3. Download the Easychart plugin (v2.2.1) at https://github.com/bestuurszaken/easychart and place it in /sites/all/libraries/easychart.


-- WYSIWYG PLUGIN ---

To use the WYSIWYG plugin, you will need to do the following:

1. Download and install the WYSIWYG module at https://drupal.org/project/wysiwyg. Follow the install instructions to add the CKeditor (v3.6.6.2) or TinyMCE (v3.5.11) editors.

2. Enable the Easychart button:
   Go to http://YOUR_SITE/admin/config/content/wysiwyg and click edit next to the text format(s) that you want to allow a chart to be added.
   Check 'Easychart' under 'Buttons and plugins' and save.

3. Enable the Easychart filter:
   Go to http://YOUR_SITE/admin/config/content/formats and click configure next to the Text Format(s) from step 2.
   Check 'Insert Easychart charts' and save.
   Note: if you don't do this, you will see [[chart-nid:x]] whe viewing your node. The filter will replace that placeholder with the actual Chart.

4. Make sure that DIV tags are allowed in your Text Format.
   Go to http://YOUR_SITE/admin/config/content/formats and click configure next to the Text Format(s) from step 2.
   If 'Limit allowed HTML tags' is checked, than add 'div' to the allowed tags.


-- POSSIBLE ISSUES --

1. I get a javascript error: 'uncaught exception: Highcharts error #13: www.highcharts.com/errors/13'
   The chart needs a div to be printed in. If your Text Format does not allow DIV's to be printed, you will get the error above, and the chart will not be printed.
   See step 4 under 'WYSIWYG PLUGIN' for the solution.

2. I see [[chart-nid:x]] instead of the actual chart.
   See step 3 under 'WYSIWYG PLUGIN' for the solution.



This project is sponsored by Bestuurszaken, Vlaamse Overheid: http://www.bestuurszaken.be

