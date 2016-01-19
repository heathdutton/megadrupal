# Configuring New Relic Insights Integration

Once you've installed and enabled this Drupal extension, you must configure the
module so that Drupal can communicate with your Insights account.

All configurations for this module are available under the "web services" area
of the admin configuration section: /admin/config/services/new-relic-insights

### Configuration through the UI

__New Relic Account ID__ - This is a required field. You can find it by logging
in to your Insights account and observing the URL. Your account number is the
number following `/accounts/`.

__Insert API Key__ - This is a required field. You can register a key by logging
in to Insights and navigating to the "API Keys" tab on the "manage data" pane.
For details, see New Relic's [documentation on registering Insert API keys][].

__Query Key__ - This field is only required if you are using this module to
query Insights from Drupal. You can register a key in the same way as outlined
above. See New Relic's [documentation on registering Query keys][] for details.

__Insights query data expiry time__ - If you provide a query key, a selection
will appear allowing you to configure an expiry time. In order to reduce the
number of queries / amount of data returned by Insights, some data is cached
locally. This expiry time defines the period for which this data will be stored
in your application database. It's recommended you keep this relatively low.

__Enable watchdog integration__ - This checkbox toggles watchdog integration.
Check it to send Drupal log (watchdog) events to Insights. Uncheck it to disable
this behavior. Note that this will happen in addition to your existing watchdog
integration (including database logging or logging via syslog). Adjust those
implementations accordingly.

__Enable accesslog integration__ - If you have the [Better Statistics][] module
enabled, this checkbox will appear. Check it to enable Transaction decoration.
Uncheck it to disable this behavior. Note that, by default, this decoration will
happen in addition to the database logging performed by the Statistics module.
Visit the Statistics configuration page to disable database logging.

For further details on watchdog event insertion and Transaction decoration, see
the [insert documentation](inserting.md) page.

### Advanced configuration

All of the above can also be configured in your settings.php file like any other
variable stored in Drupal. In addition to those, you may wish to configure some
additional variables, explained below:

__CURL timeout__ - By default, all web service calls to Insights will time out
after 5 seconds. You can configure a higher or lower timeout value based on your
own needs. To do so, add the following line to settings.php:
```php
// Cap Insights web service calls at 10 seconds.
$conf['new_relic_insights_curl_timeout'] = 10;
```

__Insights API endpoint bases__ - In rare cases, you may wish to configure
different endpoints for Insights event insertion and querying (perhaps to take
advantage of a newer API release, etc). To do so, add one or both of the
following to your settings.php:
```php
$conf['new_relic_insights_query_baseurl'] = 'https://insights-api.newrelic.com/vN';
$conf['new_relic_insights_baseurl'] = 'https://example-alternate.insights-collector.newrelic.com/v1';
```


[documentation on registering Insert API keys]: http://docs.newrelic.com/docs/insights/new-relic-insights/adding-and-querying-data/inserting-custom-events#register
[documentation on registering Query keys]: http://docs.newrelic.com/docs/insights/new-relic-insights/adding-and-querying-data/querying-your-data-remotely#register
[Better Statistics]: https://drupal.org/project/better_statistics
