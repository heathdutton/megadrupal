
WHOS.AMUNG.US FOR DRUPAL 7.x
----------------------------

CONTENTS OF THIS README
-----------------------

   * Description
   * Dependencies / Integration
   * Installation
   * Support
   * Credits


DESCRIPTION
-----------

This module allows to create and manage Whos.amung.us widgets via
blocks without writing HTML/JS code, just configuring an option form.

You can create as many widgets as you want, configuring default options
for all of them, or overriding them as you want.

The main module allows to manage configurable core Blocks, but this
module comes with four submodules if that is not enough:

  * Bean: Integration with Bean module.
  * Boxes: Integration with Boxes module.
  * CTools: Integration with CTools.
    - It defines a configurable Content Type plugin.
  * Views: Integration with Views.
    - It defines a configurable View Handler Area.
    - View Handler Areas can be placed in:
      # View's Header.
      # View's Footer.
      # View's Empty area.

API key is detected at installation time (if possible) or it can be
configured/changed via administration.

This module is integrated with Features when using core blocks.
If you are using widgets via submodules, they are also natively exportable.
Default options are system variables, so they can be exported with Strongarm.


DEPENDENCIES
------------

Base Whos.amung.us module has not dependencies at all.
Whos.amung.us Bean depends on Bean module.
Whos.amung.us Boxes depends on Boxes module.
Whos.amung.us CTools depends on CTools module.
Whos.amung.us Views depends on Views module.


INSTALLATION
------------

1. Just download and enable the module.
2. Assign 'administer whos_amung_us blocks' permission to desired
   roles. This roles can now create/edit/delete blocks, so be careful.
3. Go to settings page via admin/config/services/whos-amung-us
   or Configuration -> Web services -> Whos.amung.us and configure the
   API key (if not auto-generated) and the default options.
4. Create a whos.amung.us block via:
   admin/structure/block/add-whos-amung-us-block
   or Structure -> Blocks -> Add whos.amung.us block. You can create as
   many as you desire. Use the default options you configured before, or
   override them.
5. If you need another kind of widgets (not a block) enable the desired
   submodule (Whos.amung.us: Bean, Boxes, CTools, Views), so you can use
   those widgets instead of core blocks.


SUPPORT
-------

Donation is possible by contacting me via grisendo@gmail.com


CREDITS
-------

7.x-1.x Developed and maintained by grisendo
