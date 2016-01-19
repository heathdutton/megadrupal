
Module: Tether Stats Charts
Author: Rustin Zantolas <http://drupal.org/user/2745073>


================================================================================
Introduction
================================================================================

This is an optional module which provides supporting classes and methods for
generating charts based on the data collected by Tether Stats.


================================================================================
Requirements
================================================================================

The Charts module is required (http://www.drupal.org/project/charts). 


================================================================================
Classes
================================================================================

Tether Stats Charts contains a collection of classes which can be organized
into two different groups described as follows:


1) Schema Classes (tether_stats_charts.schema.inc)

The schema classes do not query or output any data. Instead they build an
object which describes the kind of chart you want to build. A schema class,
for example, allows you to specify which columns of data should appear and
in what order. When building a chart, the first thing you will need to do
is construct a Schema object.

Tether Stats Charts presently provides schema classes for pie charts and
combo charts. See the "Examples" section below.

TetherStatsChartsPieChartSchema

- This is the pie chart schema class where you can add the data slices you
wish to appear. 

TetherStatsChartsComboChartSchema

- This is the combo chart schema class. A combo chart is basically a 
vertical column chart but you can also add special aggregate line series to 
be displayed over the data columns.


2) Chart Data Classes (tether_stats_charts.data.inc)

These classes are responsible for actually querying and building the data 
matrix. The Schema object describes which data to query for, but not over 
which time period. So, constructing these classes requires both a schema 
object and start time.

In this way, schema classes can be reused to generate the same chart over
different time periods like you would for iterative charts.

The primary method of these classes is the getRenderable() method which
will generate a renderable array for the chart. This array can be modified
or rendered directly but the rendering task is accomplished by the
Charts module 

Once you have a Schema class, you can instantiate one of these classes
below to build a chart data object:


TetherStatsChartsPieChart

- Builds a Pie chart based on a TetherStatsChartsPieChartSchema object.


TetherStatsChartsComboChart

- Builds a Combo chart based on a TetherStatsChartsComboChartSchema object.


================================================================================
Security And Iteration
================================================================================

The iterable charts use a JSON call back to the server to load more data.
Users without the 'view tether stats charts' permission will not be able to
use this callback. In short, charts will not iterate without this permission
set.

A user with this permission, however, may use the callback to extract data at
any point in history. The type of data loaded, however, is limited to the 
schema used by the iterative chart.


================================================================================
Examples
================================================================================

Below are some examples for generating charts from Tether Stats data. You can
also refer to the tether_stats_charts.examples.inc file for some real data
examples of charts rendered for the Chart Examples page.

Building a Pie Chart:

In this example, we will compare the number of hits on three different pages.
We have already discovered the element Ids of these three pages and have them
stored in an array. You can discover elids for specific pages by using the
element finder tool (admin/config/system/tether_stats/elements) or by
querying the tether_stats_element table.

@code
// An associative array of elid to page title.
$page_elids = array(
  12 => 'Page A',
  46 => 'Page B',
  32 => 'Page C',
);

// Choose a time period over the last seven days.
$period_interval = new DateInterval('P7D');
$period_starting = new DateTime();
$period_starting->sub($period_interval);

// Load the charts include.
module_load_include('inc', 'tether_stats_charts', 'tether_stats_charts.data');

// Construct a Schema to describe the chart we wish to generate.
$schema = new TetherStatsChartsPieChartSchema('my_pie_chart_id',
  period_interval);

foreach ($page_elids as $elid => $title) {

  $schema->addElementSlice($elid, 'hit', $title);
}

// Construct the chart data object.
$chart = new TetherStatsChartsPieChart($schema, $period_starting,
  tether_stats_set_db());

// Generate the output for the Pie Chart.
$chart_renderable = $chart->getRenderable();

// Add a title to the chart.
$chart_renderable['#title'] = t('A comparison of three pages over the ' .
  'last week.');
@endcode


Building a Combo Chart:

For this chart, we can show data for the same pages as the previous example,
but for each day over the course of the week. Let us also add a summation line
series to show the total number of hits across all three.

@code
// An associative array of elid to page title.
$page_elids = array(
  12 => 'Page A',
  46 => 'Page B',
  32 => 'Page C',
);

// Choose a time period over the last seven days.
$period_starting = new DateTime();

// Reduce the $period_starting to the beginning of the current day.
TetherStatsChartsSteppedChartSchema::normalizeDate('day', $period_starting);

// Create an end time as the beginning of the current day.
$period_ending = clone $period_starting;

// Now reduce our start time to the beginning of the day seven days ago.
$period_starting->sub(new DateInterval('P7D'));

// Load the google charts include.
module_load_include('inc', 'tether_stats_charts', 'tether_stats_charts.data');

// Construct a Schema to describe the chart we wish to generate.
$schema = new TetherStatsChartsComboChartSchema('my_combo_chart_id');

// This chart is more complex then the Pie chart as we have to specify a domain
// and step size. In this case, we would like our step size to be a 'day' which
// will show one column for each day within our one week domain.

// We could specify the $domain_ticks as 7 and the $domain_step as 'day' within
// the Schema constructor. However, the base class
// TetherStatsChartsSteppedChartSchema class provides a method for specifying a
// "best fit" domain and domain step given a start and end date.
$schema->calcDomainStep($period_starting, $period_ending);

// Add the page elements as columns
foreach ($page_elids as $elid => $title) {

  $schema->addElementColumn($elid, 'hit', $title);
}

// Add a summation line series
$schema->addSummationLineSeries('Total Hits');

// Construct the chart data object.
$chart = new TetherStatsChartsComboChart($schema, $period_starting);

// Generate the output for the Pie Chart.
$chart_renderable = $chart->getRenderable();

// Add a title to the chart.
$chart_renderable['#title'] = t('Page hits over the last week with a ' .
  'summation line series.');
@endcode
