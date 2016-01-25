This module provides a live preview for the Entity Reference autocomplete
widget.  To use it:

1. Create an entity reference field, and set it to use an autocomplete widget
   (the normal one, NOT the tags-style one).

2. Edit the field settings.  In the "Live preview settings" fieldset, select
   the "Enable" checkbox, then select a preview type and a view mode (e.g.
   full, teaser, etc).
   
   2a. If you are previewing the target entity, you can choose one of its view
       modes.  This will be used to render the entity for the live preview.
       This is probably what most people will want.
   
   2b. If you choose to preview the field, you'll be previewing a field of the
       referencing entity, so you'll choose a view mode for that entity
       instead.  You might want to use this if you have a custom formatter for
       that field.

       For example: if you have two content types "Album" and "Track" and the
       Album content type has an entity reference field that can reference
       Track nodes, then you should select a view mode for the Album content
       type.  Then, when editing an album, you will see a preview of the
       references to Track nodes as they will appear when the Album is
       rendered in the selected view mode.

       IMPORTANT!  If you choose a view mode in which your entity reference
       field is hidden, then it will be hidden in the preview too, and nothing
       will be displayed.

3. If you don't want to use a real view mode like "teaser", you can create a
   custom one just for the preview using Display Suite.
