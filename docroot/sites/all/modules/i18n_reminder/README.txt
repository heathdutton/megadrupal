# Translation Reminder (i18n)

Offers helpful reminder messages to aid users with edit permissions on
translated content. This helps ensure consistency and quality for a process
which can seen confusing and cumbersome to users, particularly those who
don't use it every day.

---------------------------------------

### Setup

The admin settings for this module are within the Node Options of the i18n
module found here: admin/config/regional/i18n/node

Visibility of messages is set to it's own permission because you probably
only want to show this to site administrators. You may also want to hide the
messages for yourself, though it's good info.

---------------------------------------

### Requirements

* Variable (7.x-2.x): https://drupal.org/project/variable
* i18n Variable: https://drupal.org/project/i18n

---------------------------------------

### Known Limitations

This modules works with node translations, not yet entity translations.
That's a different paradigm which aims for field-by-field translations.

---------------------------------------

### Reporting Issues and Contributing

Any issues should be reported to the drupal.org issue queue for this module:
https://drupal.org/project/issues/i18n_reminder