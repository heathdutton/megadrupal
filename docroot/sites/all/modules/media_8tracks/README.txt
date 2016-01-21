# Media: 8Tracks

Media: 8Tracks integrates with the Media module to make 8Tracks audio files
available as file entities. Users can insert 8Tracks audio files with file
fields or directly into into WYSIWYG text areas with the Media module insert
button.


## File fields

- Add a new "file" type field to your content type or entity. Choose the widget
  type "Multimedia browser". You can also select an existing file field.
- While setting up the field (or after selecting "edit" on an existing field)
  enable:
    - Enabled browser plugins: "Web"
    - Allowed remote media types: "Audio"
    - Allowed URI schemes: "8tracks:// (8Tracks audio files)"

- On "Manage display" for the file field's content or entity type, choose
  "Rendered file" and a view mode.
- Set up 8Tracks audio formatter options for each view mode in Structure ->
  File types -> Manage file display. This is where you can choose size, autoplay,
  appearance, and special JS API integration options.
- When using the file field while creating or editing content, paste an 8Tracks
  audio url into the Web tab.

ProTip: You can use multiple providers (e.g., Media: 8Tracks and Media: YouTube)
on the same file field.


## WYSIWYG inserts

- Enable the Media module "Media insert" button on your WYSIWYG profile.
- Enable "Convert Media tags to markup" filter in the appropriate text formats.
- Configure any desired settings in Configuration -> Media -> "Media browser
  settings"
- Set up 8Tracks audio formatter options in Structure -> File types -> Manage
  file display. **Note:** for any view mode that will be used in a WYSIWYG,
  enable both the 8Tracks audio and preview image formatter. Arrange the Audio
  formatter on top. This allows the audio to be used when the content is viewed,
  and the preview when the content is being edited.

- When editing a text area with your WYSIWYG, click the "Media insert" button,
  and paste a 8Tracks audio url into the Web tab of the media browser.


## Further Reading

- Media 2.x Overview, including file entities and view modes:
  http://drupal.stackexchange.com/questions/40229/how-to-set-media-styles-in-media-7-1-2-media-7-2-x/40685#40685
- Media 2.x Walkthrough: http://drupal.org/node/1699054
