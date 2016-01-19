Within this directory create a directory structure consisting of 
FIRST_OPTION.option
  CSS_FILE_1.css
  CSS_FILE_2.css
SECOND_OPTION.option
  CSS_ANOTHER_FILE_1.css
  CSS_ANOTHER_FILE_2.css
  
This will then expose the following select widgets per theme eg.;
  "FIRST_OPTION" with the choices "CSS_FILE_1" and "CSS_FILE_2"
  "SECOND_OPTION" with teh choices "CSS_ANOTHER_FILE_1" and "CSS_ANOTHER_FILE_2"

If the site administrator chooses additional CSS to be included the selected files
will be included in the drupal css.

Review the shipped example, you can place this inside your theme directory to 
have the corresponding options exposed on the respective theme settings page.

