Drupal details element module:
_____________________________

Maintainer:
  Kurt Madel
  
Overview:
________
The Details Element module provides a field formatter for the core text_with_summary field type.  
It is similar to the 'Text with summary' module (http://drupal.org/project/text_with_summary), 
but rather than just display both the summary and the full text vaue of the text_with_summary field 
the Details Element modules uses the HTML5 details element (http://www.w3.org/TR/html5/interactive-elements.html#the-details-element).
The summary is placed in the seemly very appropriate summary element, and if there is on summary, 
then it fallsback to 'Details. The actual full value of the text_with_summary field is the hidden 
and placed after the summary element as a child of the details element.  

Currenlty Chrome 12 is the only browser that supports this element. However, the module includes 
JavaScript that detects browser support for this element and if it is not support, a jQuery function 
provides this functionality.

Usage:
-----
All you need to do is select the 'HTML5 Details element' format for the text_with_summary field via 
the 'manage display' page of the content type containing that field.

You may also want to specify the 'HTML5 details element' widget on 'manage fields' page for that field.

This module would be good for a 'question and answer' or 'faq' type field where you don't want to 
manage each faq as an individule node, but rather as a multi-valued field.

To Do:
-----
Add a format setting that allows you to specify if the details should be open or closed initially 
and/or possibly add it to the widget so it may be decided on an individual field by field basis.