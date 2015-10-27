# Entity XLIFF FTP [![Build Status](https://travis-ci.org/tableau-mkt/entity_xliff_ftp.svg?branch=7.x-1.x)](https://travis-ci.org/tableau-mkt/entity_xliff_ftp) [![Code Climate](https://codeclimate.com/github/tableau-mkt/entity_xliff_ftp/badges/gpa.svg)](https://codeclimate.com/github/tableau-mkt/entity_xliff_ftp) [![Test Coverage](https://codeclimate.com/github/tableau-mkt/entity_xliff_ftp/badges/coverage.svg)](https://codeclimate.com/github/tableau-mkt/entity_xliff_ftp/coverage) [![Dependency Status](https://gemnasium.com/tableau-mkt/entity_xliff_ftp.svg)](https://gemnasium.com/tableau-mkt/entity_xliff_ftp)

Entity XLIFF FTP is a Drupal extension that introduces a localization workflow
whereby editors or administrators can push XLIFF files to a remote server via
SFTP, then pull down processed XLIFF files when they're ready (either manually,
or automatically on cron).

This extension was designed specifically to work with [SDL WorldServer][], but
can theoretically be used with any FTP server or FTP-enabled translation
management system.

## Installation
1. Install this module and its dependency, [Composer Manager][], via drush:
  `drush dl entity_xliff_ftp composer_manager`
1. Enable Composer Manager: `drush en composer_manager`
1. Then enable this module: `drush en entity_xliff_ftp`
1. Composer Manager may automatically download and enable requisite PHP
   libraries, but if not, run `drush composer-manager install` or
   `drush composer-manager update`.

More information on [installing and using Composer Manager][] is available on
GitHub.

## Configuration
1. To access configuration pages, grant the "administer entity xliff" permission
   to all roles appropriate for your site and use-case.
1. Configure FTP credentials at `/admin/config/services/entity-xliff-ftp`.
1. On the same page, configure the "target root path" and the "source root path"
   1. The __target root path__ is the path on the remote FTP server where files
      will be pushed from Drupal. Within this folder, XLIFF files will be pushed
      into sub-folders whose naming convention is like so: `en_to_de`,
      where `en` is the source language and `de` is the target language. Both
      are the language codes associated with the language in Drupal.
   1. The __source root path__ is the path on the remote FTP server where this
      module will assume processed or translated XLIFFs are placed. On cron, or
      when manually triggered through the UI, this module will search this root
      within sub-folders whose naming convention is like so: `de`, where
      `de` is the target language, also the language code associated with the
      language in Drupal.

#### For example
![remote-ftp-structure](https://cloud.githubusercontent.com/assets/3496491/7213577/80b8852e-e538-11e4-8cf0-ea9aab33d75c.png)

A remote server set up with a folder structure above would yield the following
configurations:

- __Target root path__: `Clients/Marketing/Drupal/target`
- __Source root path__: `Clients/Marketing/Drupal/source`

## Usage

#### Pushing content to the remote server
Entity XLIFF, a dependency of this module, creates an "XLIFF" local task on all
entities that are known to be translatable (for example at `/node/1/xliff`). By
default, XLIFF files can be downloaded or uploaded individually for each
language.

![entity-xliff-ui](https://cloud.githubusercontent.com/assets/3496491/7192582/ca2c6de4-e44b-11e4-8040-90d04703d8c2.png)

This module adds a fieldset to this page called "Remote file sync integration,"
which shows a select list of target languages. Choose the languages you would
like this entity translated into, then press "push to remote server" to upload
XLIFF files to your configured remote server.

In the future, this module may introduce Actions and/or Rules integration along
with Views Bulk Operations integration to perform the same task on a large
number of entities simultaneously.

#### Pulling translated content from the remote server
This module exposes a simple admin UI that exposes "pending" and "processed"
XLIFFs, located at `/admin/config/regional/translate/entity-xliff-ftp`.

![remote-file-sync-ui](https://cloud.githubusercontent.com/assets/3496491/7192737/1c6875fc-e44d-11e4-9991-696db3589a5b.png)

__Pending projects__ are sets of XLIFFs representing an entity that are ready on
the remote server, but have not yet been pulled into Drupal (meaning, the
translated XLIFFs are sitting in the "source root path" on the remote server.

__Processed projects__ are XLIFFs that have recently been pulled into Drupal.
This module knows which XLIFFs have already been imported because it moves
imported files on the remote server from the configured "source root path" to
a sub-directory called "processed."

If you're actively working on a project or need to import a project immediately,
you can select pending projects in the aforementioned admin UI and click the
"process selected projects" button to run a manual import.

##### Automation
This module also provides two methods to automate syncing/importing processed
XLIFF files from the remote server to Drupal.

__On cron__: By default, this module will attempt to pull all available XLIFFs
from the remote server on cron. If you wish to control how often this task is
run, independent of how often you run your Drupal cron task, use a module like
[Elysia Cron][].

__Web service call__: For more advanced workflows, this module opens up a web
service endpoint that can be hit remotely in order to trigger, on an ad-hoc
basis, the same process that's triggered on cron. For example, in your
translation management system, you could add a workflow action to the end of a
process that pings this web service endpoint, immediately pulling in content to
Drupal. Endpoint details follow:

`POST /sync-xliffs-from-remote?key=[cron_key]`

Responses:
- `200 OK`: Processing occurred without issue.
- `403 Forbidden`: Either no `key` was provided, or the key provided is invalid.
- `405 Method Not Allowed`: The request used an unsupported method. Use POST.
- `500 Internal Server Error`: An error occurred while processing XLIFFs.

## Please note
This module and its underlying dependencies are still under active development.
Use at your own risk with the intention of finding and filing bugs.

[SDL WorldServer]: http://www.sdl.com/cxc/language/translation-management/worldserver/
[Composer Manager]: https://www.drupal.org/project/composer_manager
[installing and using Composer Manager]: https://github.com/cpliakas/composer-manager-docs/blob/master/README.md#installation
[Elysia Cron]: https://www.drupal.org/project/elysia_cron
