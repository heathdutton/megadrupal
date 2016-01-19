## Installation

Install as you would with any other Drupal module.

Install composer_manager and list_predefined_options modules and optionally the
queue_ui module.

Use Drush or other method to set the following variables for connection to
GovDelivery.

```
drush vset govdelivery_username 'YOUR USERNAME'
drush vset govdelivery_password 'YOUR PASSWORD'
drush vset govdelivery_account_key 'GOVDELIVERY ACCOUNT KEY'
drush vset govdelivery_url 'GOVDELIVERY URL'
```

For testing you can use the govdelivery_url of https://stage-api.govdelivery.com/api/account/ACCOUNT_KEY/.
Make sure to include the trailing slash!

Finally, none of the queue processing in this module is setup to run Drupal's
cron. It is important that a external cron be run to process the queues. For
example, install the following crontab entry. You can adjust the frequency
depending on need.

```
*/5 * * * * drush queue-run govdelivery_topic_submissions
```
