Media 23Video

Creates a 23Video PHP Stream Wrapper for Resource and implements the various
formatter and file listing hooks in the Media module.

Prerequisites
-------------
The media_23video requires the media and media_internet to be installed.

Installation
------------
To install, copy the media_23video directory and all its contents to your
modules directory.

Configuration
-------------
To enable this module, visit administer -> modules, and enable media_23video.

Usage
-------------
## File fields

- Add a new "file" type field to your content type or entity. Choose the widget
  type "Multimedia browser". You can also select an existing file field.
- While setting up the field (or after selecting "edit" on an existing field)
  enable:
    - Enabled browser plugins: "Web"
    - Allowed remote media types: "Video"
    - Allowed URI schemes: "media-23video:// (23video videos)"

- On "Manage display" for the file field's content or entity type, choose
  "Rendered file" and a view mode.
- Set up 23video video formatter options for each view mode in Structure ->
  File types -> Manage file display. This is where you can choose size, autoplay
  and etc.
- When using the file field while creating or editing content, paste a 23video
  video url into the Web tab.


## WYSIWYG inserts

- Enable the Media module "Media insert" button on your WYSIWYG profile.
- Enable "Convert Media tags to markup" filter in the appropriate text formats.
- Configure any desired settings in Configuration -> Media -> "Media browser
  settings"
- Set up 23video video formatter options in Structure -> File types -> Manage
  file display. **Note:** for any view mode that will be used in a WYSIWYG,
  enable both the 23video video and preview image formatter. Arrange the Video
  formatter on top. This allows the video to be used when the content is viewed,
  and the preview when the content is being edited.

- When editing a text area with your WYSIWYG, click the "Media insert" button,
  and paste a 23video video url into the Web tab of the media browser.
