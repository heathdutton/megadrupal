********************************************************************
                G E N T L E  S E L E C T  W I D G E T
********************************************************************
Original Author: Varun Mishra
Current Maintainers: Varun Mishra
Email: varunmishra2006@gmail.com

********************************************************************
DESCRIPTION:

   Gentle Select Widget implements gentleSelect jQuery plugin for 
   transforming select boxes into a skinnable alternative. The 
   selection list can be rendered with multiple columns/rows to 
   present larger datasets in a more manageable format. It 
   recognizes the multiple attribute on select boxes and does the 
   right thing automatically.

   This module works with list, list_text, list_number, node_reference,
   taxonomy_term_reference, user_reference and entityreference fields.

   This module provide various setting to configure the widget like 
   title, number of columns or row, item width, open effect, 
   close effect, open speed, close speed, hide on mouse out, maximum 
   number of display.


********************************************************************

INSTALLATION:

1. Place the entire gentleselect directory into sites modules 
  directory (eg sites/all/modules).

2. Enable this module by navigating to:

     Administration > Modules

3) This module have dependency on libraries module.

4) Download gentleSelect library from 
   http://shawnchin.github.io/jquery-gentleSelect/ . Install it in
   sites/all/libraries directory, and rename the directory to gentle_select.
   The library should be available at a path like
   sites/all/libraries/gentle_select/jquery-gentleSelect.js.

5) Please read the step by step instructions as an example to use this
   module below:-

a) Install the module.

b) Go to admin/structure page. Click on manage fields of any content type.

c) Add a new field and select List(text) or Term Reference or User Reference or
   Node Reference or Entity Reference field. 

d) Select Gentle Select from widget list box and save.

e) Configure the various options available for widget setting.
