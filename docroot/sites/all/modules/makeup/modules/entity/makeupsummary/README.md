MakeUp Summary
==============

Maintainer: w3wfr (<drupal@w3w.fr>)

MakeUp Summary defines a new Summary formatter.

### Notice: 

 * The function that actually shorten the text is shared in the MakeUp module. Only the formatter is in the makeupsummary.module file.

### Features

Drupal Core `text_summary()` from the *text.module* file shortens the raw text to the end of the last complete sentence. Depending on how long is the last incomplete sentence, the string length can be very different from what you ask for in the settings.

MakeUp Summary use `makeup_text_summary()` function that firstly strip tags: this make sure only text length is counted. `<img>`, `<a>` and other tags will never shorten the text.

`<!--break-->` delemiter is still available.

String is cut at last space: no broken word.

If the string finish with: 

 * `...` or `.`: no change.
 * In other case: `...` will be added.

Requirements
------------

 * Drupal 7.x

### Modules

 * Field
 * MakeUp

Installation
------------

Ordinary installation.
[http://drupal.org/documentation/install/modules-themes/modules-7](http://drupal.org/documentation/install/modules-themes/modules-7)
