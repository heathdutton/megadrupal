For more information about this module please visit http://drupal.org/project/transform

Contents of this file
---------------------

 * About Drupal
 * jqTransform plugin
 * About Transform module
 * Use Transform
   - Install Transform and libraries
   - Apply styling to forms
   - Examples
   - Form element settings
 * Notes
   - Compatibility
   - Known issues


*** About Drupal
---------------------------------------
Drupal is an open source content management platform supporting a variety of
websites ranging from personal weblogs to large community-driven websites. For
more information, see the Drupal website at http://drupal.org/, and join the
Drupal community at http://drupal.org/community.

Legal information about Drupal:
 * Know your rights when using Drupal:
   See LICENSE.txt in the same directory as this document.
 * Learn about the Drupal trademark and logo policy:
   http://drupal.com/trademark


*** jqTransform plugin
---------------------------------------
The jqTransform plugin is att jQuery styling plugin which allows you to skin
form elements.
For more information please visit http://www.dfc-e.com/metiers/multimedia/opensource/jqtransform


*** About Transform module
---------------------------------------
The Transform module uses the jqTransform plugin to style forms in Drupal.
It requires the jqTransform library to work.


*** Use Transform
---------------------------------------
- Install Transform module and libraries
  1. Download Transform module and extract the files into your Drupal
     module folder.
  2. Go to Administer > Site building > Modules and enable the Transform module.
  3. Download the jqTransform library from
     http://www.dfc-e.com/metiers/multimedia/opensource/jqtransform
     and extract the files.
  4. Copy the folder 'jqtransformplugin' that you find among the extracted files
     into the folder 'sites/all/libraries/' in your site.

- Apply styling to forms
  1. Go to Administer > Site configuration > Transform.
  2. Check 'Enable transformation of form elements' if you want to transform
     Drupal forms.
  3. If you want to transform only a specific form, write the class that
     identifies the form in 'Form classes'. Input one class per row if you to
     transform different forms with different classes. If you leave the
     'Form classes' empty, than all forms will be transformed.

- Examples
  1. Go to Administer > Site configuration > Transform.
  2. If you have three forms like this

       <form class="myform1 myform"></form>
       <form class="myform2 myform"></form>
       <form id="form3" class="myform3 myform"></form>

     and you want to transform only 'myform1' and 'myform3', set "How to apply
     the transformation" to "By form class" and write the following in
     "Form list" under "Forms to transform":
       
       myform1
       myform3

     if you want to transform all the forms with the class "myform", but exclude
     the form id "form3":

     - set "How to apply the transformation" to "By form class"
     - input "myform" in the "Form list" under "Forms to transform"
     - under "Forms that shouldn't get transformed", set "By form id"
     - input "form3" under "Form list"


  3. If you leave the textarea under "Forms to transform" empty then all the
     three forms will be like all other forms in your site as well.


- Form element settings
    You can use these settings to fix some layout problems with
     - Selectboxes.
     - Textfields.
    These settings gets ONLY numbers that represents the number of pixels for
    each form element.


*** Notes
---------------------------------------
- Compatibility
  Chrome 15+
  Firefox 7+
  IE 7+
  Opera 11.5+
  Safari 5.1+

- Known issues
  IE7: Sometimes the textfields get a strange appearance.

 
---------------------------------------
by Dhavyd Vanderlei - http://www.dhavyd.com