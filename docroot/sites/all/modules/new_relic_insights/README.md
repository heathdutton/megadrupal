# New Relic Insights Integration

This Drupal extension deeply integrates your Drupal application with the New
Relic Insights service.


### Features

* Write data to your Insights database
  * Insert native Drupal log events (watchdog)
  * Decorate application transactions with custom attributes
  * API for custom event / event type insertion
* Query data in your Insights database
  * Create Drupal Views of your Insights events
  * API for advanced querying techniques within Drupal


### Requirements

* A [New Relic Insights](https://newrelic.com/insights) account
* PHP >= 5.3 with [cURL](https://php.net/manual/en/book.curl.php)
* [Remote Entity API](https://drupal.org/project/remote_entity) and its
  dependencies
* [Better Statistics](https://drupal.org/project/better_statistics) and
  [EntityFieldQuery Views Backend](https://drupal.org/project/efq_views) are
  highly recommended.


### Installation and configuration

The best way to install this extension is to use
[drush](https://github.com/drush-ops/drush)!

```sh
drush dl new_relic_insights
drush en new_relic_insights
```

To get started, you'll want to note your Insights account ID and register API
keys for inserting and querying Insights data (according to your needs).

Configurations can be made through the UI at
yoursite.com/admin/config/services/new-relic-insights.

See the [module configuration](doc/configuring.md) documentation for further
details.


### Usage

This extension works on the principle that it's advantageous to treat analytics
data--stored in New Relic Insights--as if it were native application data.

This module provides a Drupal entity called an "Insight" whose data just happens
to be stored externally. Piggybacking off of the entity system allows us to
provide a Drupal-native interface to Insights that feels familiar to site
administrators and developers alike.

The speed and scalability of the Insights database, combined with the power and
flexibility of Drupal's entity system, provide tantalizing use-cases:

* Offload high-volume, high-value transactional data from your application
  database and into Insights while maintaining fast access within your app.
* Provide analytics to site administrators _in context_ by querying Insights
  data keyed off of the content or account they're viewing.
* Personalize end-user experiences in real-time by using contextual details at
  render-time to query the wealth of historical data stored in Insights.

Get started inserting and querying your data below:

#### Writing data to Insights

Although Insights provides a wealth of performance data in your Insights
database out of the box, you will likely want to insert custom events or add
custom data attributes to existing events.

Currently, this module supports:
* Writing watchdog events to Insights as custom events of type "watchdog."
* Decorating Transactions with user-defined attributes.
* Sending other custom events with arbitrary data.

See the [inserting Insights data](doc/inserting.md) documentation for further
details.


#### Querying Insights data

New Relic Insights allows you to query data in real-time using a SQL-like query
language called NRQL. This module allows you to query Insights in a variety of
ways, exposing Insights data natively to Drupal.

Currently, this module supports:
* Creating Drupal Views that use Insights as their data source.
* Querying Insights using Drupal's native EntityFieldQuery query builder.
* Executing custom NRQL queries and processing the returned data.

See the [querying Insights](doc/querying.md) documentation for further details.
