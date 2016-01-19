# Querying Insights events

New Relic Insights allows you to query data in real-time using a SQL-like query
language called NRQL. This module allows you to query Insights in a variety of
ways, exposing Insights data natively to Drupal.

## Drupal Views

[Views](https://drupal.org/project/views) is a powerful, ubiquitous
query-building tool for Drupal. This module gives site builders and savvy site
administrators the ability to query Insights through a GUI (without having to
know the underlying NRQL query language). To do so, you'll need to install and
enable the [EntityFieldQuery Views Backend][] module.

Once this module is [enabled and configured][], and EFQ Views is installed,
creating a View of Insights data is as simple as creating any other View.

Currently, Transactions are best supported, with support for other custom event
types in development. Any custom attribute known to Drupal (see the "Better
Statistics and custom attributes" section of the [transaction decoration][]
documentation for details) can be displayed or used to filter your query.

You can also expose those filters to target users, allowing them to alter the
query made to Insights. It's also possible to use "contextual filters" to filter
the query by contextual information on the request, such as the global user ID,
the piece of content on which the View is embedded, etc.

Like any other View, it can be exported, imported, featurized, etc.

Note that Views of Insights data do not support aggregation beyond a simple
"count." Other aggregation operators are currently unsupported.

## Querying via EntityFieldQuery

Developers can also query Insights directly in code using Drupal core's
EntityFieldQuery class (see [documentation on EFQ][] here). This class provides
a simple interface for querying Insights. Examples follow.

#### Basic EFQ usage
```php
// Instantiate an EFQ instance.
$efq = new EntityFieldQuery();

// Use the "entity_type" entity condition to indicate a query against Insights.
$efq->entityCondition('entity_type', 'insight');

// Different "event types" in Insights are represented as entity bundles in
// Drupal. Here, we're filtering down to Transactions and PageViews
$efq->entityCondition('bundle', array('Transaction', 'PageView'), 'IN');

// Add conditions to the query.
$efq->propertyCondition('name', 'WebTransaction/Uri//index.php');

// Execute the query.
$results = $efq->execute();
```

In the example above, the resulting NRQL would be something like:
`SELECT * FROM Transaction, PageView WHERE name = 'WebTransaction/Uri//index.php'`

The `$results` variable would consist of an array of `Insight` entity objects,
whose properties can be accessed like any other entity, including via the
Entity Metadata Wrapper, like so:

```php
$insight_wrapper = entity_metadata_wrapper('insight', $insight_object);
$web_duration = $insight_wrapper->webDuration->value();
```

#### Filtering by custom attributes

Custom attributes are treated like any other property on the Insight entity.

```php
$efq->propertyCondition('my_customer_segment', $GLOBALS['user']->customer_segment->value());
```

#### "IN" and "NOT IN" operators

The EntityFieldQuery class comes with the ability to query entities using more
traditional "IN" and "NOT IN" operators as found in SQL. You can also use them
on Insights; they'll simply be translated to valid NRQL syntax. For instance:

```php
$efq->propertyCondition('uid', array(5, 27), 'IN');
```

...Would translate to `WHERE (uid = 5 OR uid = 27)`.

While the following...

```php
$efq->propertyCondition('uid', array(5, 27), 'NOT IN');
```

...Would translate to `WHERE (uid != 5 AND uid != 27)`.

#### Time constraints

Using the above as your general framework, you can imagine time constraints on
your NRQL query as another property condition on the property "timestamp," which
takes a unix epoch timestamp as its value.

```php
$efq->propertyCondition('timestamp', strtotime('1 week ago'), '>');
```

The constraint above would result in something like `SINCE 1 week ago` being
applied to your query (although in reality, for precision and consistency, it
will actually translate to `SINCE 604800 seconds ago`).

You can also use the "BETWEEN" operator to easily filter down to a specific
window in time:

```php
$efq->propertyCondition('timestamp', array(
  strtotime('1 week ago'),
  strtotime('2 weeks ago'),
), 'BETWEEN');
```

The constraint above would result in something like `SINCE 2 weeks ago UNTIL 1 week ago`.

Note that it is sufficient to provide a timestamp; no need to run text through
a `strtotime()` call:

```php
$efq->propertyCondition('timestamp', 280281600, '>');
```

#### Limiting query result count

By default, all queries made through EntityFieldQuery will limit themselves to
returning 100 events. You can increase or decrease this by using the `range`
method.

```php
// LIMIT 50
$efq->range(0, 50);

// LIMIT 980
$efq->range(0, 980);

// Note Insights prevents returning more than 1000 events at a time.
// This would result in: LIMIT 1000
$efq->range(0, 2000);
```

Note that Insights doesn't support paging, so the first value in the `range`
method is always ignored.

#### Counts

You can perform simple count queries using `EntityFieldQuery::count()`.

```php
// ...
$efq->count();
$count = $efq->execute();
```

Here, $count would be an integer.


## Executing custom NRQL queries

While EntityFieldQuery and EFQ views are powerful tools that provide handy
shortcuts for querying Insights, they both lack the ability to run aggregation
on your queries, which can be very useful. The reason for this limitation is
that EntityFieldQuery itself doesn't handle aggregation (in Drupal 7).

To work around this, you can inject your NRQL query directly into the EFQ class,
returning the raw API result from Insights.

```php
// Set up your EFQ instance.
$efq = new EntityFieldQuery();
$efq->entityCondition('entity_type', 'insight');

// Inject your NRQL using the "NRQL" pseudo-property.
$nrql = 'SELECT uniqueCount(uid) FROM Transaction SINCE 1 month ago';
$efq->propertyCondition('NRQL', $nrql);

$result = $efq->execute();
```

The structure of `$result` will vary, based on the provided NRQL query.


[EntityFieldQuery Views Backend]: https://drupal.org/project/efq_views
[enabled and configured]: configuring.md
[transaction decoration]: inserting.md
[documentation on EFQ]: https://api.drupal.org/api/drupal/includes%21entity.inc/class/EntityFieldQuery/7
