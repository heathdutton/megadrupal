CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Maintainers

# Introduction

**RESTful Panels** provides [Panels](https://www.drupal.org/project/panels)
layout and configuration in response to a [RESTful service]
(https://www.drupal.org/project/restful) call. The response format is controlled
by the RESTful controller, defaulting to JSON.

This module can be used to build progressive decoupled Drupal implementations as
mentioned by Dries in his keynote for DrupalCon Barcelona 2015. Typically,
decoupled implementations cannot use Drupal's powerful presentation capabilities
as only data is exposed. This module allows exposing the entire Panels layout
structure, and content and configuration of each of the panes. This can be
rendered by a front-end application as required. The service endpoint returns
entire data related to a page as well as presentation details in a single
response, allowing for optimum use of resources and ability to use Panels to
control a decoupled implementation.

The layout is built internally using the standard Panels renderer and converted
to an array structure instead of the HTML markup. This may be further
manipulated before your controller returns.

More details can be found at the project page at
https://www.drupal.org/project/restful_panels. Please report issues with this
module at https://www.drupal.org/project/issues/restful_panels.

## Example Endpoint

The module includes a sample RESTful endpoint which takes a Display ID as input
and returns the panels layout and configuration as a JSON response. The resource
name for the endpoint is 'panels_display'. For a Panel display with ID 5, the
URI for this endpoint will be http://example.com/api/v1.0/panels_display/5.

# Requirements

This module requires the following modules:
* [ctools](https://www.drupal.org/project/ctools)
* [restful](https://www.drupal.org/project/restful)
* [panels](https://www.drupal.org/project/panels)

# Recommended Modules

This module also contains a controller to support loading Panelized entities.
Currently, there is no example endpoint to use this. Issues to contribute
examples and documentation are welcome.

# Installation

There are no additional steps to install the module apart from just enabling the
module. Typically, you would write a module extending controller classes
provided by this module. Your module's .info file can mark 'restful_panels' as a
dependency.

# Maintainers

This module was built for a decoupled implementation for [Legacy.com]
(http://www.legacy.com/) by [Axelerant](http://axelerant.com/). It is supported
by Axelerant and maintained by [Hussain Abbas]
(https://www.drupal.org/u/hussainweb). Contributions are welcome. Please create
an [issue](https://www.drupal.org/project/issues/restful_panels) to discuss and
patch.
