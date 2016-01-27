$Id 

-- SUMMARY --

Gallery Summary allows to display content for each argument in a summary view.

Project page:
  http://drupal.org/project/gallery_summary
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/issues/gallery_summary


-- REQUIREMENTS --

* Views module <http://drupal.org/project/views>


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- USAGE --

  1. Create a view as usually and add the argument you'll use for summary.
  2. In the argument options, set the "Action to take if argument is not 
     present" to "Summary, sorted ascending" or "Summary, sorted descending"
  3. In summary style options choose "Gallery Summary"
  4. Choose the display you want to use for making preview of the content, 
     take in account that the display you will use here need to have the same
	 argument defined.
  5. Save the view.


-- TROUBLESHOOTING --

* If the summary of arguments is displayed in the view output but the display
  you attached is not appearing, check the argument configuration or 
  display settings. 
  
  Make sure that the argument you set for summary is actually working, for
  doing this, set the argument as a default summary (list or unformatted)
  and go check the output to make sure that the view is actually working. 
  If everything is OK, when you click on one of the summary results 
  you should see a list of elements for that argument.
  
  Also make sure that the display you choose for using as "gallery summary"
  has an argument set and that the argument is the same as the parent display.


-- CONTACT --

This project is maintained by Javier Reartes (javierreartes)

Significant design guidelines by Nathan Haug (quicksketch)
