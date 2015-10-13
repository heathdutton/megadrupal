
Welcome to Views Popup 3. Please see the advanced help for more information.

If you're having trouble installing this module, please ensure that your
tar program is not flattening the directory tree, truncating filenames
or losing files.

Installing Views Popup:

Place the entirety of this directory in sites/all/modules/views_popup
You must also install the Views module (http://www.drupal.org/project/views) to use Views Popup.

Using Views Popup:

Start by creating a view with fields. 
Add some fields. It is best if you should have at least two. More is better for experimentation.
Add a Global Views Popup field. 
 Select one field to attach the popup window to. You can also select the row.
 Select the type of layout
 Select one or more fields to place into the popup window. 
 Other items are optional.

Popup rows and fields cannot be mixed. 
If a row is first then all others will be ignored.
If a field is first then all rows will be ignored. 


Notes:
Click on/off mode can have additional close buttons in the popup window. 
Include views-popup-window-custom-close-button in the class of the element to be clicked. 
Use CSS to hide the default button if desired. 
For example, add this to the footer to include a Close button. 

<input class='views-popup-window-custom-close-button' type="button" value="Close">

Custom.css
The module will load images/custom.css if the file exists allowing for customization 
that is independent of the module installation. 

