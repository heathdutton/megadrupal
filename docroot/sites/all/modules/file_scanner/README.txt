
-- SUMMARY --

The File Scanner module is a utility module that allows to analyse files
locally recursively inside a folder.
During the parsing, the module opens each file - that matches a user-defined
allowed file extension -, opens it as an HTML document and tries to find a
user-defined HTML element (tag, attribute or even text) matching a CSS3
selector with the help of QueryPath library.
Each match is stored in the database grouped per analysis. Each analysis can
then be viewed through the interface, providing a statistics summary.

This tool can be used for any use case where you have a number of files to
analyse.
Probably the most common use case is when migrating an old website where
pages are stored in HTML files.


-- REQUIREMENTS --

* Libraries API module
    https://drupal.org/project/libraries

* QueryPath library
    http://querypath.org/


-- CONTACT --

Current maintainers:
* Ludovic Ramarojaona (dolu) - https://drupal.org/user/374670

This project has been sponsored by:
* Blue4You
  Visit http://www.blue4you.be for more information.
