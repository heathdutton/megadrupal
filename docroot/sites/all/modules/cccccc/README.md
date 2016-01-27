# Clear Cached CiviCRM Credit Cards (CCCCCC) #

Author: Chris Burgess, [Giant Robot Ltd](http://www.giantrobot.co.nz).

This module can be added to a Drupal/CiviCRM site which has an onsite payment
processor. It protects against a data caching issue which causes credit card
details to be retained in CiviCRM's civicrm_cache table.

This module is not required in the following instances -

* for CiviCRM sites v4.1.3 or later, or
* for sites using an offsite (hosted) processor, or
* for sites which do not process transactions.

If possible, you should upgrade to the current CiviCRM release (v4.1.3 or
later). If that is not an option, you should consider this module (or switching
to an offsite payment processor, which is A Good Idea anyway).

## Cron implementation ##

If installed and enabled, the module will clear cached data up to half an hour
ago whenever Drupal's cron is called.

## Drush command. ##

This Drush command will flush the table of matching entries. It accepts an
argument so you can retain a certain number of seconds of cache entries (so
as to avoid causing any payments currently in process to fail).

This allows sites to configure cache clearing aggressively, without the need
to call Drupal cron frequently.

Usage:

* `drush @example.org civicrm-cccccc` clears data up to half an hour ago
* `drush @example.org civicrm-cccccc 120` clears data up to 120s ago

## Further approaches ##

There are two ways in which this solution can be improved upon.

### Implementation of hook_civicrm_postProcess(). ###

This is intended to prune entries as soon as they are created. It looks for
more specific paths than the Drush command, so it may need to be expanded
to include other CiviCRM contribution interfaces. The initial version
covers only "regular" contribution pages, not event regos or membership
renewals.

This approach is currently disabled because I found that CiviCRM seemed to
be storing the cached data *after* hook_civicrm_postProcess() was called. It
may be that hook_civicrm_postProcess() needs to register a shutdown function
to clear the cached data, but for the initial release I've decided to drop
this approach and simply match the fix in CiviCRM.

### Process transactions without caching the data ###

With some changes CiviCRM could not cache this data. The current default
form setup does require caching the data, because we present the user with
a confirm screen after requiring their CC details.

Caching the data can be fixed two ways.

We can drop the need for caching if the CC form transacts immediately and
doesn't store the data locally. The simplest method to do this is to drop
the confirm screen from the process. The current CiviCRM forms implementation
will still cache the data even the Confirm screen is removed.

Removing the confirm screen is now an option in 4.2, and can be done in earlier
versions via https://github.com/emotive/CiviCRM-No-Confirmation ... but from
early tests it seems that neither of these approaches will prevent CiviCRM
from caching the data on submission. Hopefully we can resolve this for 4.2
series.

We can offload the data to the transacting authority if the payment processor
requests an authorisation hold on the first submission, and then settles the
transaction or removes the authorisation once the confirm screen is complete)
