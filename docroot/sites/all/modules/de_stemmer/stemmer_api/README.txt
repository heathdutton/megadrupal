README

The "stemmer_api" module aims to
* support administration 
* and help check functionality
of stemmers, based on hook_search_preprocess.

Currently available stemmers as there are
* dutchstemmer
* porterstemmer
* swedishstemmer
* spanishstemmer
all have no settings menu. The reason is that the complete 
functionality including the list of stop words and exceptions is 
hard coded and therefore not changable. The stemmers have to be 
used "as is" and no local adjustments are possible. And since 
stemmers work in conjunction with search routines, result and 
quality of the stemming process is not under control of the user.

Some efforts were made to circumvent some of the issues 
mentioned above. The module stemmer_api in its current state
offers the following functionalities:

When the stemmer is patched to have its own (even empty) settings 
menu the Module stemmer_api 
* shows a list of available stemmers. 
* those stemmers which already have a menu entry in "admin/setting" 
  are linked to this menu. 
* a checkbox indicates wether the list of stop words / exceptions 
  is currently managed
* all documentation files like README.txt and so on can be viewed in 
  separate textareas
* phrases can be typed in to see the result of stemming immedeately
* the list of stop words / exceptions can be managed

When the stemmer is patched to use some stemmmer_api functions like 
"stemmer_api_stopwords_read" (see INSTALL.txt) then
* the stemming process will be affected by using these lists
 
Pitfalls:
* large documentation files need some time to be displayed

