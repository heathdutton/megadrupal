Readme file for Routing Debug module
--------------------------------------------------------------------------------

Visit project [page](https://www.drupal.org/project/routing_debug)
on Drupal.org, for more information about the project.

# Overview

Routing Debug is a simple module that provides menu routing table with
additional information like:

- name of the callback function,
- filename and path of file where the callback is defined,
- line number of function definition.

Intention of the module is to help developer to identify potential menu
overrides. Module interface is accessible on _devel/routing_debug_ URL.

# Features

- Display registered routes, module the are coming from,
- callback function handling each route/path,
- file and line number of each callback function,
- display raw information provided by _menu_names()_, _system_menus()_
  and _menu_router()_ functions.
- Module also provides block with the debug information for current path.

# Installation

Download latest stable version the module - either from Drupal.org project page,
or by running

```drush en -y routing_debug```

which downloads and enables the module.

Only dependency of the module is `Devel` module.

# Tutorial

After module is installed, the usage is very simple. Module registers it's main
path (`devel/rounting_debug`), which directly shows available menu routing
information in a table. Two additional tabs are available on the page: raw devel
formatted menu data and settings page. Settings page allows _'debug log'_ toggle
to be enabled, which logs additional information to watchdog, in case of _'magic
Reflection'_ failure.

On the main routing debug page, each routing page links both to the actual page
(if it's a valid path) and 'devel/menu/item' page for given route.

Module also provides a block with the same routing information. Add the block
to your preferred region to always show routing debug information
for current path.

## Module paths

* If you have Administration menu module enabled, you will find the module pages
  under 'Development' as 'Routing debug', along with other tools provided by
  Devel module. Path is `devel/routing_debug`.
* You also find the main module page under 'Reports' _(Administration -> Reports
  -> Routing debug)_. Path is `admin/reports/routing_debug`.
* Module configuration page is under _Administration -> Configuration ->
  Development -> Routing Debug settings_. Path of the configuration page is
  `admin/config/development/routing_debug`.
* To get to all pages at once follow this path: _Administration -> Index
  (`admin/index`) -> Routing Debug.

## Permissions

Module defines one permission: `Access routing information`. Grant this
permission to roles, who should be able to see the routing information and the
debug block as well. If they should see the detailed debug provided by Devel
module, they well need to be granted permissions provided by Devel as well.

## Requirements and dependencies

Module uses some functions from [Devel](https://www.drupal.org/project/devel)
and therefore is dependent on it.

---

### _About author_

_[David Lukac](https://about.me/davidlukac) is a senior IT consultant
and analyst with experience and background in large enterprise Java/J2EE
projects in telecommunication sector, system integration and CRM.
He's been working with Drupal since 2009 as CTO of
[Mogdesign.eu](http://mogdesign.eu), joining
[iKOS Digital](http://i-kos.com) in 2014 as senior Technical Consultant.
David has finally found time to do more development due to his position
change to less executive and more technical :-). Author is also
co-founder of [DrupalFund](http://drupalfund.us) - unique crowdfunding
platform for Drupal related projects._
