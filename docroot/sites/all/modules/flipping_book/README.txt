
-- SUMMARY --

This module provides an integration point between Drupal7 and FlippingBook 
(http://flippingbook.com/). Flippingbook is a very used software to make digital 
publication from any document you have (.doc, .pdf...).

This module handles your flippingbook file, giving you a comfortably control
panel in which you can upload the classic .zip file exported by Flippingbook
software after digitalization. Every file imported will be unpacked into a
public://flipping_book directory in you files folder
(usually sites/default/files).

Enabling the submodule flipping_book_reference, in bundle in this package, 
you can add a flippingbook reference field to your fieldable entity 
(nodes, users, taxonomy,...) choosing the field widget you prefer and what
kind of display you want of this field. Colorbox is just supported.
  
-- REQUIREMENTS --

Views
Transliteration (optional)
Colorbox (optional) 


-- GOALS AND LIMITATIONS -- 
  
-- Available Field widget type

1) Checkboxes
2) Select list
3) Textfield with autocomplete

-- Available Field Display formats

1) Title with or without link to the flippingbook
2) Flippingbook ID
3) URL as plain text
4) Colorbox link (you need colorbox module enabled and "colorbox_load"
variable checked)

-- CONTACT --

Current maintainers:

* bmeme.com - http://www.bmeme.com
