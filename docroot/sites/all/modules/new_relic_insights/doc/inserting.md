# Insights Transaction decoration and other events

Although Insights provides a wealth of performance data in your Insights
database out of the box, you will likely want to insert custom events or add
custom data attributes to your Transactions.


## Transaction decoration

Transaction decoration is the process of adding custom attributes (like a user
ID or product price) to the Transaction corresponding to the current request.
Although the New Relic PHP extension provides a function to add arbitrary
attributes to a given request, we recommend doing so in a more structured way,
described below.

This structure provides useful metadata to other Drupal systems, providing
seamless integration with the Entity subsystem, Views filters and handlers, as
well as access to a wealth of preexisting attributes and data.

To get started with Transaction decoration, you'll need to install the
[Better Statistics](https://drupal.org/project/better_statistics) module.

### Better Statistics and custom attributes

The Better Statistics module provides a framework for defining analytics
attributes and storage backends. By default, Better Statistics defines and
stores data in the Drupal DB; this module exposes Insights as an alternate
backend (and allows you to disable the default DB backend).

Custom attributes declared through Better Statistics' framework can be enabled
and disabled at-will through the user interface (on the Statistics configuration
page) or via drush. Full details are available in the
[Better Statistics documentation][].

Better Statistics also provides a number of custom attributes out of the box,
including some on behalf of Drupal core modules:

* __title__: Page Title for the given transaction
* __path__: Internal Drupal path for the given request, e.g. node/1
* __url__: The http referer for the given transaction
* __hostname__: The end-user's IP address for the given transaction
* __uid__: The Drupal user ID for the given transaction
* __sid__: The PHP Session ID for the given transaction
* __timer__: The length of time, in seconds, the given transaction took to
  generate from Drupal's perspective, probably redundant in the context of a New
  Relic Transaction.
* __user_agent__: The full user-agent string for the given transaction, a bit
  redundant but at least more verbose than what is provided by Insights
* __peak_memory__: The peak memory, in bytes, used by PHP for the given
  transaction
* __cache__: Drupal page cache hit/miss stat for the given transaction (one of:
  hit, miss, or null for uncacheable requests)

Better Statistics also provides custom attributes on behalf of Drupal core
modules. For instance, if the current request corresponds to a node, you could
decorate the transaction with the node ID, node type, node author ID, etc.

### Declaring your own custom attributes

Better Statistics provides a hook-based API for declaring custom analytics
attributes. Full details are available in the [Better Stats API docs][], but a
simple example follows:

```php
/**
 * Implements hook_better_statistics_fields().
 */
function my_module_better_statistics_fields() {
  $attributes['my_customer_segment'] =  array(
    'schema' => array(
    'type' => 'varchar',
    'length' => 128,
    'not null' => FALSE,
    'description' => 'The customer segment of the user on this request.',
    ),
    'callback' => 'my_module_get_attributes',
      'views_field' => array(
      'title' => t('Customer segment'),
      'help' => t('The customer segment of the user on this request.'),
    ),
  );

  return $attributes;
}

/**
 * Return the value of a given attribute for this request.
 * @param string $attribute
 *   The desired attribute.
 * @return mixed
 *   The scalar value of the given attribute.
 */
function my_module_get_attributes($attribute) {
  switch ($attribute) {
    case 'my_customer_segment':
      return $GLOBALS['user']->customer_segment->value();
      break;
  }
}
```

Once enabled (via drush or through the Statistics admin config UI), all
Transactions would be decorated with the attribute "my_customer_segment," whose
value would be pulled from the callback you defined.

Note that the schema and help text you provide for your custom attributes
inform the Entity API about what attributes are available on Insight entities.
This, in turn, informs Views and EntityFieldQuery about what you can use to
filter your Insights queries.


## Watchdog integration

Watchdog is Drupal's native logging interface. It keeps track of events of
note, including critical alerts, debug messages, and other useful details.

Watchdog integration can be configured at the main configuration page for this
module. Watchdog integration is achieved by queueing messages logged to watchdog
in the Drupal Queue. Queued messages are processed automatically on cron but
can also be processed manually in the following ways:

* If the [Drush Queue][] extension is installed, you can run
  ``` drush queue-run new_relic_insights ```
* You can also make a manual HTTP request to the following callback. When you do
  so, ensure your actual cron key is appended:
  ``` https://example.com/new-relic-insights-runner/your-cron-key-here```

Because it's unlikely that you run cron very frequently, if you require
real-time, or near-real time watchdog analysis, you'll want to manually trigger
queue processing in one of the aforementioned ways at a high frequency (e.g.
once every minute or every 30 seconds).

When the queue is processed, events are sent through New Relic's Insert API as
events of type "watchdog."

The following fields are added to all watchdog events in Insights:

* __type__: The type of watchdog message being logged (often the module
  responsible for generating the message).
* __user__: The e-mail address of the user for whom the watchdog event was
  logged. For anonymous users, this field will be blank.
* __uid__: The user ID of the user for whom the watchdog event was logged. For
  anonymous users, the value of this field will be 0.
* __request_uri__: The request URI associated with the given watchdog event.
* __referer__: The URI associated with the referring page prior to this request.
* __ip__: The IP address from where the request for this page came.
* __timestamp__: The UNIX timestamp of the date/time the event occurred.
* __severity__: An integer representing the severity of this message (e.g. an
  alert vs. a notice vs. a warning vs. an emergency, see below for details).
* __fullMessage__: The fully processed message that you would normally see when
  analyzing watchdog messages and other notices/errors.
* __message__: The raw, unprocessed message as it was passed to watchdog in
  code. This may include replacement tokens.

The following fields are optional and may or may not be passed along with events
in Insights:

* __links__: If a link/operation is associated with this watchdog event, it will
  be included here as a full HTML anchor.
* Variables: Some messages may include tokenized variables that are added when
  processing the associated message. The names and contents of these variables
  vary widely depending on the watchdog event being logged, but each token is
  included as a separate field/column in Insights. The name used for the field
  is the name of the token, prefixed with "_var_." For instance, in the
  following raw message, the subsequent tokenized variables would be included:
  ``` The node with nid %nid was edited by user with id !uid ```
    * _var_nid: Would contain the associated node ID.
    * _var_uid: Would contain the associated user ID.

Note that queueing of watchdog events for inserting into Insights does not
interfere with the core dblog or syslog modules. It may be used in tandem with
or independently of the aforementioned modules, depending on your use-case.


## Inserting custom events

A full API for this functionality is still in development. For now, you can call
`new_relic_insights_post_event()` with a single argument, containing an
associative array of event data. When you do so, be sure to include an
`eventType` key, representing your custom event. For complete details, read the
[Insert API documentation][].

Note that this call is synchronous.


[drush queue]: https://drupal.org/project/drush_queue
[better statistics]: https://drupal.org/project/better_statistics
[Better Statistics documentation]: http://cgit.drupalcode.org/better_statistics/plain/README.txt?h=7.x-1.x
[Better Stats API docs]: http://cgit.drupalcode.org/better_statistics/tree/better_statistics.api.php?h=7.x-1.x
[Insert API documentation]: http://docs.newrelic.com/docs/insights/new-relic-insights/adding-and-querying-data/inserting-custom-events#json-format
