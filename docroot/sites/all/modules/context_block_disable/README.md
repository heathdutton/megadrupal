# Introduction

This is a [Context][1] reaction plugin that allows you to select blocks to disable based on the conditions specified in Context. It will work for any blocks that make use of the Drupal hook and theme system.

## Requirements

 * [Context][1]

## Installation

Install as usual, see [this guide][2] for further information.

### Notes

If this module is loaded before other modules that alter blocks then those
changes will still be made. In order to work around this you must make sure that
this module is loaded after other modules.

If you run into this issue then a workaround is either adjusting the weight of
this module in the system table manually, or by using a module that allows
adjusting of module weights through an interface.

Two such modules are:

 * [modules_weight][5]
 * [Util][6]

## Related

Pairs well with [Context Mobile Detect][3]

## Usage

Usage of this plugin is fairly straightforward. Enable it as you would any other module. A new reaction should be available within the Context UI. The form it provides is the same form as the stock Context Block reaction block selection form.

## This project has been sponsored by

**McMurry/TMG**

> McMurry/TMG is a world-leading, results-focused content marketing
> firm. We leverage the power of world-class content — in the form of
> the broad categories of video, websites, print and mobile — to keep
> our clients’ brands top of mind with their customers.  Visit
> [http://www.mcmurrytmg.com][4] for more information.


  [1]: http://drupal.org/project/context
  [2]: http://drupal.org/documentation/install/modules-themes/modules-7
  [3]: http://drupal.org/project/context_mobile_detect
  [4]: http://www.mcmurrytmg.com
  [5]: https://www.drupal.org/project/modules_weight
  [6]: https://www.drupal.org/project/util
