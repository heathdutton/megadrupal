AudioRecorderField.module
--------------------------------------------------------------------------------
The AudioRecorderField.module provides a new CCK field that enables both the recording and playing of .wav audio files directly from the web browser. 

The new field is based on AudioField module and records audio via the Nanogong applet (http://gong.ust.hk/nanogong) or 
Soundcloud flash recorder (https://github.com/jwagener/recorder.js). 
Note that Nanogong applet is limited to 20 mins of audio recording per file.

In order to convert the new audio files to .mp3, please refer to the AudioConverter module (http://www.drupal.org/project/audioconverter/).


Installation

1. Extract AudioRecorderField to your sites/all/modules directory. Make sure you have installed CCK, FileField and AudioField modules.
2. Download Nanogong applet recorder from the following link: http://gong.ust.hk/nanogong/downloads_form.html
   or download Soundcloud flash recorder from the following link: https://github.com/jwagener/recorder.js
   Both recorders record in .wav format.
3. If using Nanogong then copy nanogong.jar file to sites/all/modules/audiorecorderfield/recorders
   If using Soundcloud then copy recorder.swf to sites/all/modules/audiorecorderfield/recorders
   If you copied both recorders you will have option to choose between them at admin/settings/audiofield under "Audio Recorders"
4. Enable the AudioRecorderField module in admin/build/modules.
5. Choose any content type from admin/content/types and go to "manage fields".
6. Add a new field, select File as its field type and Audio Recorder as the widget.
7. Save.
8. Create new content and you will see the Audio Recorder!

Player configuration

Additionally this module provides wav players. You can choose between Nanogong and Wav Player. Note that Nanogong is not compatible with all versions of .wav 
files. If you intend to use Soundcloud as recorder we recommend to use Wav Player as default .wav player.

In order to choose the .wav player directly from AudioFields, you should:
1. Go to admin/content/types and choose the content type that includes the audio field you would like to play with Nanogong
2. Go to "display fields"
3. Choose "Audio" for either the "teaser" or the "full node" display modes
4. Go to admin/config/audiofield and select the default player for .wav files under "wav Audio Players"
5. Now you should be all set!


FileField Sources support

This module also adds new "recorder" uploading method for FileField Sources module. Its possible to create one field with multiple uploading choices.
1. Download and install FileField Sources module.
2. Change your Audio Recorder widget to File Upload widget.
3. Under File Sources fieldset you will see various uploading methods (Audio recorder method is added by this module).

Java Fallback (only for Nanogong)

What do do if Java is not enabled at the client? There are two options that are configurable through admin/settings/audiorecorderfield
( ) Replace recorder for file upload option (DEFAULT)
( ) Remind user to download and set up Java for their computer

---
The AudioRecorderField module has been originally developed by Leo Burd and Tamer Zoubi under the sponsorship of the MIT Center for Future Civic Media (http://civic.mit.edu).