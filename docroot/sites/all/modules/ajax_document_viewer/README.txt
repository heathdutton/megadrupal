-- INTRODUCTION --

   The Ajax Document viewer module allows to provide a document viewer for
 the uploaded documents in the drupal.To view the document, the user no
 need to download the document into his machine.Using ajax document viewer
 the documents of type pdf, doc, xls and many more can be seen online.

   The Ajax Document viewer module enables you to link the documents
 on your servers to the viewer on "http://www.ajaxdocumentviewer.com/" server.

   No installation is required for viewing the documents in the same page or in
 new tab.This is very helpful if we need to show any documents which are
 available for read only but not for download

   This module will work with private urls as well. It is not allowing to
download the file directly, instead it is showing the document in a viewer.

-- INSTALLATION --

1. Download and unpack the module.
2. Place the module in your modules folder ("sites/all/modules/").
3. Enable the module under admin/modules
4. If you install the lightbox2 module,
   you can view the documents in lightbox view also.

-- USAGE --

After installation,
  configure at "admin/config/user-interface/ajaxdocumentviewer".

Note: With the free key only following options will work:
      viewer-height,
      viewer-width,
      toolbarcolor,
      toolbarcolor.

With the personal or commercial key all options will work.
Options :-
Key            : is the key provided by Adeptol obtained by filling the form at
                 "http://www.ajaxdocumentviewer.com/cloudconnect-api.asp".
                 Key will be sent to your mail address.

viewer-height  : is the display height of the viewer.
                 You can choose any +ve number for this.

viewer-width   : is the display width of the viewer.
                 You can choose any +ve number for this.

toolbarcolor   : allows to set color of the toolbar.
                 This should be an Hexadecimal Color Code.

toolbarheight  : allows to set the height of buttons on toolbar.
                 (Default Value=32). Can be any numeric number.

saveButton     : is the ability to switch save document button on and off
                 (Values: Yes/No) (Default: Yes)

printButton    : is the ability to switch print document button on and off
                 (Values: Yes/No) (Default: Yes)

copytextButton : the ability to switch copying of text to Clipboard on and off
                 (Values: Yes/No) (Default: Yes)
                 (Default Copying of Text is allowed)

logoimage      : is the logo of your company that will appear when
                 viewer is loading (Default: AJAXDocumentViewer Logo)

quality        : is the fidelity of the document being loaded.
                 You can control the quality and resolution of document
                 loaded in viewer.
                 (Value: high/medium/low) (Default: medium).
                 A medium quality is recommended for optimized load times and
                 better resolution. A higher quality would mean more rendering
                 time but higher resolution.
                 Quality value if not passed defaults to medium.

After configurations :  

Once you are done with the configurations, you will find three new display 
formats added to the file type fields.

Display formats added : 
  - Ajax document new window view
  - Ajax document inline view
  - Ajax document lightbox view

For file fields you can use these display formats from the manage display tab
in content types settings or from 'display formats' in the views.
