CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Features
 * Dependencies
 * Installation & usage
 * Notes


INTRODUCTION
------------
This is a small module that provides the vertical tab contents 
of a node add/edit form as panels content type. In a panel or a 
mini panel though the individual contents of the vertical tab 
are available to be added, but once they are added separately 
they loose the vertical tabs structure.

In Panels when one overrides the default node add/edit form template
the individual components of the vertical tabs like Publishing options,
URL Settings Menu Settings are available as panels content type 
individually. But on adding them to the panels content pane, the 
vertical tab structure is no longer there.

This is where this module kicks in.

FEATURES
--------

1. Rearrange the vertical tab components of the node forms.

2. Break up the default node form vertical tab into a number of
   vertical tabs each with selected components displayed.
   
3. Clean up node forms by hiding/removing unnecessary vertical tab
   components like menu options, url path settings, authoring info.etc.
   
DEPENDENCIES
------------
Ctools
Panels
   
INSTALLATION & USAGE
--------------------

1. Download and Enable the module at Administer >> Site building >> Modules.

2. In any panels "do add more" on any pane.

To Add Vertical Tab components:

  a. Select the "forms" group and select "Node Vertical Tab Elements to Add".

  b. Tick the checkboxes against the components that you want to show.

  c. That's it. :) Drag and drop to any pane and still have the vertical
     tabs structure intact. Hooray!
     
To Remove Vertical Tab components:

  a. Select the "forms" group and select "Node Vertical Tab Elements to Remove".
  
  b. Tick the checkboxes against the components that you want not to show up.
   
NOTES
-----
* Multiple instances of "Node Vertical Tab Elements to Add" can be added each 
  with different components selected.
  
* When using multiple instances of "Node Vertical Tab Elements to Remove" or 
  "Node Vertical Tab Elements to Add" please note that if same component is 
  selected in one and deselected in other it may produce conflicting output.
  