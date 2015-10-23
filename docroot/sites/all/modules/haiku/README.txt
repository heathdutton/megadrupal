-- SUMMARY --

This module serves primarily as a Haiku API of sorts and provides useful
functionality mostly to developers (if anyone), but offers something for
Drupalers of all levels.


-- INSTALLATION --

* Enable the module.
* Try out the standalone form at http://your-site.com/parse-haikus
* Apply the "Haiku" field formatter to any field in any entity.


-- FOR END USERS --

After installing this module, you'll have a page with a basic form which will
return haikus parsed from text entered by the user.

To try it out, go to http://your-site.com/parse-haikus


-- FOR DEVELOPERS AND SITE BUILDERS --

After installing this module, in addition to the page above, you'll notice
several additions within the Drupal interface as well as within the code.

* This module introduces a new field format for text fields (long text and text
  with summary) called "Haiku." When a field's display is set to use this
  format, rather than the text of the field, it displays a haiku parsed from the
  field text. What use this is is beyond me, but it's available.
* For developers, this module introduces two classes: HaikuSource and Haiku
  which come with useful methods for creating/parsing/returning Haikus from any
  text you throw at it. Details on this are available below in the API section.


-- FOR ALGORITHM DESIGN ENTHUSIASTS --

After installing this module, you'll have access to the HaikuSourceInterface and
HaikuInterface interfaces; they may be useful for implementing your own haiku
parsing algorithms. Either implement them in your code, or contribute back to
this project and the default implementations it provides.


-- "HOW TO" FOR DEVELOPERS --

This module provides two classes: Haiku and HaikuSource. Use them in a way
similar to the following to parse haikus within your own modules.

* First, create a new HaikuSource object by passing in a string.
  $haiku_text = new HaikuSource('This can be any body of text.');

* Instantiating it automatically parses the text for haikus. You can use the
  getHaikus method to return a specified number of haikus like so:
  $haikus = $haiku_text->getHaikus(1);

* The getHaikus method will return an array of Haiku objects. If you run
  getHaikus on the HaikuSource without any arguments, it will return an array of
  all parsed haikus.

* The Haiku objects getHaikus returns have two useful return types:
  - toString($delimiter) will return the Haiku as a string delimited by the 
    delimiter you pass in. Calling toString without a delimiter will return
    haikus delimited by a newline character.
  - toArray() will return the Haiku as an array where each element in the array
    is a line of the haiku.

A full example would be something like the following. Don't be afraid to poke
around in this module's .module file to see how it's used.

  $haiku_text = new HaikuSource($node->body[LANGUAGE_NONE][0]['value']);
  $haikus = $haiku_text->getHaikus();
  echo $haikus[0]->toString('<br />');
