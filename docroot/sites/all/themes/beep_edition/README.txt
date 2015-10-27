
ABOUT BEEP EDITION
-----------

The Beep Edition theme was developed as a mobile-first responsive base theme. It does require a few modules to function, but only ones that make HTML 5 and better semantic markup possible.

REQUIRED MODULES
- Block Class
- Fences
- HTML5 Tools
- Menu Block
- Semantic Views

REQUIRED LIBRARIES
- Font Awesome  (http://fontawesome.io)
  (place in sites/all/libraries/font-awesome)

REQUIRED SASS BITS
- Sass          (http://sass-lang.com)
- Compass       (http://compass-style.org)
- Zen-grids     (http://zengrids.com)
- Breakpoint    (http://breakpoint-sass.com)

TO USE THE NAVIGATION
---------------------

To use the drop-down/off-canvas main menu system, make sure you use the 'Main menu' default menu, and create a Menu Block.

Once you create and save the menu block, click the link to 'configure' the block. Make sure you select 'Unlimited' for menu depth, check the box to 'Expand all children' and then add these classes (you must have the Block Class module enabled): 

main-navigation block-nav

You can now place the block you created in whichever region you select.

The 'main-navigation' class ties in the main styles

The 'block-nav' class ensures that the block will be rendered with the 'nav' HTML element rather than a 'div', and adds the ARIA role of 'navigation' as well.


BLOCK CONFIGURATION
-------------------

The block template file has a bit of logic in it to determine what HTML element to use as the main wrapping tag. Simply by adding a class to the configuration screen of any block (enabled with the Block Class module) you can determine what HTML element will be used to wrap the content of the block.

To render it as a <nav> element use:

  block-nav
  

To render it as an <aside> element use:

  block-aside


To render it as a <section> element use:

  block-section
  
  
If you do not add a 'block-x' class (or one not listed above), it will be rendered as a <div>.
