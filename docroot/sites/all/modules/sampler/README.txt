*******************************************************
    README.txt for sampler.module for Drupal
*******************************************************

This module was co-developed by Chad Phillips (hunmonk), Derek Wright (dww)
and Michael Prasuhn (mikey_p).  Initial funding was provided by the Drupal
Association as part of the 2010 drupal.org redesign contract work through
3281d Consulting (3281d.com).

The Sampler API allows modules to easily collect and store calculated pieces
of data.  It is designed primarily to assist in collection, storage, and
display of metrics.

Modules provide the API with a metric to track (eg. 'Number of comments added
to a node') a listing of objects to be tracked (for example, you may only want
to track that for certain nodes), a sampling strategy (eg. 'every 3 days'),
and a method that returns the calculated values for the metric (eg. 'give me
the values that you want to store for the period of January 1st midnight to
January 4th midnight').

Based on the provided data, the framework:
  - Performs calculations to determine the how to sample for the metrics.
  - Calls the necessary functions to gather the calculated values.
  - Handles all storage of the metrics.
  - Provides default views of the metrics (if stored in the Drupal database).

Metrics are made available to the API via the CTools plugin system
(http://drupal.org/project/ctools), and are responsible for handling the
calculations that the API tracks.  This architecture allows for a large amount
of flexibility in what can be collected and stored -- there are thousand of
things that could be measured, and the API makes no attempt to decide which
ones matter.

Metrics can take arbitrary options to assist in making the calculation
functionality even more flexible.  Furthermore, sampling strategies,
adjustments to sample sets and sample results, and storage of the calcuated
values are all pluggable.

The API is primarily immplemented as a series of plugins.  The plugin types
that come with the module are:

  storage:
    Handles storage of collected samples.  This can be any storage method you
    desire (database, file, S3, etc.).  The API comes with two Drupal database
    plugins, one which stores all metrics in one table, one which stores each
    metric in its own table.  The single table plugin allows for multiple
    values per sample.  It also comes with a proof-of-concept file storage
    plugin which saves sample data in CSV format, and two MongoDB plugins.
  method:
    Handles building a set of 'sample points'.  This set is then passed on to
    the computation layer.  The API comes with the 'periodic' method plugin,
    which generates a set of sample points according to a regular interval, eg.
    'every 2 weeks'.  It also comes with a proof-of-concept 'random' method
    plugin, which takes a single sample of a random number of object IDs.
  adjustment:
    Handles making adjustments during various phases of the sampling cycle.
    Currently plugins can be written that adjust both the sample points (prior
    to calculation, and the sample results (after calculation).  The API comes
    with several proof-of-concept adjustment plugins -- see the plugins
    themselves for details.


Plugins can take options -- any options that are passed to the sampling engine
are made available to the various plugins, computation and object tracking
functions when they are called.

See the sampler_example.module included with this project for examples
of how to leverage the APIs.

See sampler.api.php for API documentation.

See sampler.drush.inc for command-line awesomeness.

INSTALLATION:

Put the module in your modules directory.
Enable it.
Enable any other modules that advertise metrics via the API.

TRIGGERING METRICS CALCULATION:

The project provides a Drush (http://drupal.org/project/drush) command line
implementation to trigger calculation of the metrics data (which is highly
configurable, and the preferred method of triggering samples), and a simple
menu callback that processes all metrics using default options.

CAVEATS:

 - As part of the schema management of the module, metrics table names are
   programatically generated for each individual metric being tracked.  The
   naming convention for a table is sampler_MODULE_METRIC, where MODULE is the
   module name (eg. 'sampler'), and METRIC is the internal name of the metric
   as provided by hook_sampler_metrics().

   Due to table name length restrictions in both MySQL (64 characters), and
   PostgreSQL (63 characters by default), caution must be used when naming
   your metrics so that the table name does not exceed the maximum allowed
   length for the database.

 - The API handles creation of the necessary schemas as needed for storage of
   metrics, using the necessary storage plugin.  To prevent unintended data
   loss, the API makes no effort to remove this stored data should the modules
   providing any metrics cease supporting them -- any storage in this state
   will need to be manually removed, including the entry in the
   {sampler_metric_state} table.

 - The internal metric name and data_type attributes are used by the storage
   plugins to create a storage schema to store the metric's data.  The API
   internally trackes the data_type and metric name in the
   {sampler_metric_state} table from the time of schema creation -- to change
   these in the hook and prevent data corruption you'll need to handle updating
   that data store and the schema table for the metric in an update function.

 - To prevent data corruption, the API never overwrites existing samples.  If
   existing samples need to be updated, they must be manually removed from the
   data store and re-sampled.

