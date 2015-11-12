Table to Chart
==============

Convert a table of data to a chart with Morris.js

Installation
------------

There are three external libraries required. The libraries should be placed in your "libraries" directory.
See the example make file for more information.

Usage
-----

### Automatic
1. Add the class "table-chart" to the wrapping element around your table
2. Ensure the table_chart library is included
    - drupal_add_library('table_chart', 'tabletochart');
3. You have a chart!

### Views
1. Use the Views to generate your table
2. Add the class "table-chart" to your view
3. Attach the library to the view
    - Easiest way is to use the [library_attach](https://drupal.org/project/library_attach) module.
4. You have a chart!

### Data Attributes

You can control the display of the chart and the columns used using data attributes on your table and columns.
Take any of the following options and preface with 'data-*'.

Ex:

````
<div class="table-chart">
  <table data-morris-type="line" data-morris-colors="red">...</table>
</div>
````


Here are the available options:

#### Chart settings
morris-type
: The type of chart: Options include bar, line, donut and area. Default is line.

morris-colors
: Comma separated list of colors. Hex values (including the # symbol) or a default color (red, green, blue, etc...)

morris-resize
: Boolean value to enable automatic resizing (i.e. responsive charts, requires latest dev version of Morris.js

morris-lineWidth
: Specify line width

morris-pointSize
: Specify the point size

morris-pointFillColors
: Specify point fill colours

morris-pointStrokeColors
: Specify point stroke colours

morris-ymax
: Y max value

morris-ymin
: Y min value

morris-smooth
: Boolean smooth flag

morris-hideHover
: Boolean flag hide hover

morris-hoverCallback
: Hover callback function

morris-parseTime
: Boolean parse time

morris-postUnits
: Post units

morris-preUnits
: Pre units

morris-dateFormat
: Date format

morris-xLabels
: X Labels

morris-xLabelFormat
: X Lable format

morris-yLabelFormat
: Y Lable format

morris-goals
: Goals

morris-goalStrokeWidth
: Goal stroke width

morris-goalLineColors
: Goal line colors

morris-events
: Morris events

morris-eventStrokeWidth
: Event stroke width

morris-eventLineColors
: Event line colors

morris-continuousLine
: Continuous Line

morris-axes
: Axes

morris-grid
: Grid

morris-gridTextColor
: Grid text colour

morris-gridTextSize
: Grid text size

morris-gridTextFamily
: Grid text family

morris-gridTextWeight
: Gride text weight

morris-fillOpacity
: Fill opacity

morris-behaveLikeLine
: Boolean behave like line

morris-formatter
: Formatter

morris-xkey
: A string containing the name of the attribute that contains date (X) values. (See morris js docs for more details). xKey will default to first column if omitted.

morris-ykeys
: A list of strings containing names of attributes that contain Y values (one for each series of data to be plotted). yKeys will default to second column and beyond if omitted.

morris-labels
: A list of strings containing labels for the data series to be plotted (corresponding to the values in the ykeys option). Labels will be automatically generated if omitted.

morris-stacked
: Set to true to draw bars stacked vertically.

#### Data settings
For more details on each option see https://github.com/lightswitch05/table-to-json

tabletojson-ignoreColumns
: Comma separated list of columns to ignore. Use index values starting at zero (0) (e.g. Ignore the first, third and sixth columns - 0,2,5)

tabletojson-onlyColumns
: Comma separated list of columns to include.

tabletojson-ignoreHiddenRows
: Boolean value. Set to "true" or "false" accordingly. (e.g. data-tabletojson-ignoreHiddenRows=true)

tabletojson-headings
: Comma separated list of headings to use. See https://github.com/lightswitch05/table-to-json for more details.