Module
-------
Views Custom Conditions


CREDITS
--------

This module was created by OSSCube Solutions Pvt Ltd <www dot osscube dot com>
Developed by Bhupendra Singh <bhupendra at osscube dot com>
Developed by Nirbhav Gupta <http://drupal.org/user/2416448>


DESCRIPTION
-------------

  This Index Hint module facilitates the site administrator to inject index
  hint into a views query. ‘Views’ is a powerful and highly flexible module
  that provides website builders with critical list making abilities. It
  generates a dynamic SQL query according to views construction for displaying
  data. So, to implement indexes in these SQL, this module can be really
  helpful.
 
  While using the USE INDEX syntax, administrators/users can tell MySQL which
  index to prefer when it executes a query. This saves time and makes upto 50
  times faster queries.


BENEFITS
---------

 # Faster execution of views queries
 # No need to write separate SQL query, users can do it in views
 # Easy to implement indexes	  

                                                            	 
INSTALLATION
-------------

  # Install the module
  # It will generate a field in views settings “ Index hint”
  # Fill suggestion in this field for index field
  # Then save the views
  # This will inject Index hint


DEPENDENCY
----------
   "views_ui" module
   Downhload URL : http://drupal.org/project/views

