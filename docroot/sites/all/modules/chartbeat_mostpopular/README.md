# Chartbeat Most Popular

## Introduction
This module is designed to be used with Chartbeat. If you do not have a
Chartbeat account, it will not work. Please configure the module first before
you enable the Chartbeat - Most Popular block. It needs to be properly
configured so that it can access the Chartbeat API.

## Requirements
You need to get your Chartbeat API key at https://chartbeat.com/apikeys/
The key MUST have toppages access in order to work. This is usually the default
API key that comes with a Chartbeat account so you can use that.

## Recommended modules
Chartbeat -- https://www.drupal.org/project/chartbeat
It is recommended but not required. Be aware that if you use this module without
using Chartbeat, you will need to appropriately limit the usage of Chartbeat
Javascript to ONLY your publicly accessible nodes. While this module tries to
verify the page title for every URL, it can't do that for URLs that don't belong
to nodes. Instead, it will fallback on the page title that is recorded at
Chartbeat.

## Installation
Install as you would normally install a contributed Drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information.

## Configuration
After you enable this module, please go to
admin/config/services/chartbeat_mostpopular to configure it.

You will see multiple fields there that you will have to fill in. These fields
are for configuring the module so that it can pull in the list of most popular
items from Chartbeat as well as append Google Analytics campaign code to the
links in the block.

## Troubleshooting
## FAQ
## Maintainers
 * Allan Benamer
 * Darryl Norris
