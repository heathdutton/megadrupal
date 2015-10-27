// $Id:$

# Detector

This Drupal module supports the inclusion of Dave Olsen's Detector library in 
Drupal, and allows Drupal to use the provided $ua variable to detect browser 
capabilities.

[https://github.com/dmolsen/Detector](https://github.com/dmolsen/Detector)  
[http://detector.dmolsen.com/](http://detector.dmolsen.com/)

This module was developed by Geoffrey Roberts at 
[Cross(Functional)](http://crossfunctional.net).

[geoffrey@crossfunctional.net](mailto:geoffrey@crossfunctional.net)  
[g.roberts@blackicemedia.com](mailto:g.roberts@blackicemedia.com)

## Introduction

> Detector is a simple, PHP- and JavaScript-based browser- and 
> feature-detection library that can adapt to new devices & browsers on its own 
> without the need to pull from a central database of browser information.

> Detector dynamically creates profiles using a browser's (mainly) unique 
> user-agent string as a key. Using Modernizr it records the HTML5 & CSS3 
> features a requesting browser may or may not support. ua-parser-php is used 
> to collect and record any useful information (like OS or device name) the 
> user-agent string may contain.

> With Detector a developer can serve the appropriate markup, stylesheets, 
> and JavaScript to a requesting browser without being completely dependent 
> on a front-end-only resource loader nor a browser-detection library being 
> up-to-date.

- _[Detector README](https://github.com/dmolsen/Detector#readme)_

This Drupal module includes the Detector library, and sets it up to run tests 
on the browser in such a way that custom Drupal modules are able to detect 
browser capabilities.

For a demonstration of the sorts of browser capabilities that Detector 
can detect, see the [official Detector site](http://detector.dmolsen.com/).

## Installation

See [http://drupal.org/documentation/install/modules-themes][drupal-modules] 
for a general guide on installing and updating modules.

[drupal-modules]: http://drupal.org/documentation/install/modules-themes

This module requires the [Libraries](http://drupal.org/project/libraries) 
module in order to work.  It also requires that you install the Detector 
library into your site's Libraries directory.

For most sites, your Libraries directory will be  
<code>sites/all/libraries</code>

If you are using an installation profile, the Libraries directory will be  
<code>profile/profile_name/libraries</code>

### Installing Detector library

The easiest way to install the Detector libraries is by using Git.

To clone the Github repository into your site, go to your Libraries directory 
and type:  
<code>git clone https://github.com/dmolsen/Detector.git</code>

On the other hand, if your site is already in Git and you'd like a bit more 
control over dependent repositories, you may be better off adding it as a 
submodule.  To do that, run the following from the root directory: 
<code>git submodule add https://github.com/dmolsen/Detector.git path/to/libraries/Detector
git add .gitmodules</code>

More information about Git submodules can be found in the 
[GIT SCM Manual.][gitmanual]

[gitmanual]: http://git-scm.com/book/en/Git-Tools-Submodules

Once you've finished copying the Detector library to its destination, follow 
the [installation procedure][detectorinstall].  This includes 
setting up its config.ini file and changing permissions on some subdirectories.

[detectorinstall]: https://github.com/dmolsen/Detector/wiki/Adding-&-Using-Detector-With-Your-Application

### Extra Tests

This module provides a few additional, optional tests that detect Flash and 
Java.  See the [Extras](#extras) section for more details.

## Usage

There are multiple ways to use Detector's abilities.

### Reference to global $ua object

Detector publishes a global variable called **$ua**, that contains the relevant 
client details straight from the source

<code>global $ua;</code>

For more detailed information on the usage of the $ua object, see the 
[Detector documentation](https://github.com/dmolsen/Detector#readme).

### DetectorProperty

There is also a class called DetectorProperty that exposes functions that will 
help you obtain details from the $ua object without having to reference it 
directly.  It exposes a couple of static functions:

* **DetectorProperty::getProperty($id)**  
  Fetches the contents of the property matching key $id.  If the property you 
  are looking for is nested, use an arrow to join them.  
  For example, **"screenattributes->colorDepth"** will fetch the colour depth 
  of the screen attached to the client.
* **DetectorProperty::listProperties()**  
  Fetches a list of all Detector properties in a key/value array.

### Rules

Detector offers its properties up to Rules. You can also use "when Detector 
properties are initialised" as a Rules action.

### Context

Detector also offers up its properties to Context module, so that site contexts 
can be set on the value of a particular property.  To use it on a set of 
properties, define multiple Contexts and combine them into another Context, 
using a Context action.

## Extras

This module provides a few additional, optional tests that detect Flash and 
Java.  To use these tests, copy them from the **tests/perrequest** directory 
into Detector's **lib/Detector/tests/perrequest** directory.

### Requirements

These tests require additional JavaScript to be included via CDN, 
specifically:

* Java detection: [deployJava.js][deployjava] from java.com
* Flash detection: [swfobject.js][swfobject] from ajax.googleapis.com

[deployJava]: http://java.com/js/deployJava.js
[swfobject]: http://ajax.googleapis.com/ajax/libs/swfobject/2.2/swfobject.js

The module supports copies of these libraries at custom-defined sources, and 
allows for them to be hosted anywhere you like, whether within your Drupal site 
itself, or on a CDN you control.

Once you've installed these files where you want, go to the Settings page 
(admin/config/development/detector) and set the URL (if an external file) or 
the internal path (if local) into the 
**Path to deployJava script** and **Path to swfobject script** fields.

## TODO

* If Modernizr module is installed, use its copy of Modernizr to prevent 
  library overloading.
