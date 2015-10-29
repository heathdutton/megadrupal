
Millennium Input Filter README.txt

ABOUT
=====
Millennium Input Filter embeds book information in nodes when if finds special token that includes a record number or URL (e.g. 
{{millennium|b123456}}, {{millennium|i123456}}, 
{{millennium|http://example.org/record=b123456}}) with book information from a 
Millennium (III) WebOPAC.

For instance, typing the following text in a node body would insert a single 
item record from the New York Public Library:
  _____________________________________________________________________________
    This is a record for an awesome book! Please read it now!

      {{millennium|http://catalog.nypl.org/record=b17496311~S1}}
  _____________________________________________________________________________

When you view the node, it would produce something like this:
  _____________________________________________________________________________
    This is a record for an awesome book! Please read it now!
    
    +-----------+------------------------------------------+
    |Title      | Harry Potter and the philosopher's stone |
    |Item type  | Book                                     |
    |Author(s)  | Rowling, J. K                            |
    |Imprint    | London Bloomsbury Pub., 1997             |
    |ISBN       | 0747532745; 0747532699                   |
    |Language   | English                                  |
    |URL        | Link to original record                  |
    +-----------+------------------------------------------+
  _____________________________________________________________________________

You also can configure what fields to show (see "INSTALLATION AND USAGE").
  
NOTE: Remember to pick the proper Input Format before saving the node!
  
INSTALLATION AND USAGE
======================
1. This module requires the Millennium module to be enabled.
2. Enable the new module "Millennium Filter" ad admin/build/modules, under the
    "Millennium" fieldset.
3. Go to admin/settings/filters and click "configure" beside one of the enabled 
    input formats.
4. Find the checkbox for "Millennium Filter", and click "Save configuration".
5. Optionally, you can go to the "Configure" tab for the Input Filter and change
    the settings under the "Millennium Records Filter" fieldset.
6. Create a new node.
7. Select the just-modified input format under the body field.
8. Use any of these syntaxes to add a record display inside the body:

    {{millennium|b123456}}
    {{millennium|i123456}}
    {{millennium|http://example.org/record=b123456}}
    {{millennium|http://example.org/any/link/to/a/single/record/in/WebOPAC}}

Note the first two examples would try to fetch the record information from
the currently-configured WebOPAC in the Millennium module settings 
(admin/settings/millennium).
  
Note the URL syntax allows any URL as long as it leads to a single record. For
example, these are equivalent:

  {{millennium|http://catalog.nypl.org/record=b17496311~S1}}

  {{millennium|http://catalog.nypl.org/search~S1?/Xharry+potter+and+the
  +philosopher%27s&searchscope=1&SORT=D/Xharry+potter+and+the+philosoph
  er%27s&searchscope=1&SORT=D&SUBKEY=harry%20potter%20and%20the%20philo
  sopher%27s/1%2C20%2C20%2CB/frameset&FF=Xharry+potter+and+the+philosop
  her%27s&searchscope=1&SORT=D&1%2C1%2C}}

TROUBLESHOOTING AND SUPPORT
===========================
Make sure you enable error reporting before reporting problems (see step 5 
above).

Need support? Want to contribute? Have an idea?
Please visit the main project page:
  http://drupal.org/project/millennium
