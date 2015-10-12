WireDocs README
---------------

If you want to share and work with your documents online Google Docs, Zoho, iNetWord, Office 365 or Sharepoint are most likely 
to be - depending on how deep you can dig into your pocket - the solutions of choice. Especially, a large legacy of documents
in proprietary formats, such as MS Word or Excel, may discourage from moving to an online editor. Additionnally, legal issues
might arise if confidential files are hosted by a third party service provider.

WireDocs is a lightweight remote file editing tool. It takes the best of both worlds: Drupal as a CMS being responsible for
hosting files and applications on a operating system (OS) doing the editing part. The approach automatizes a manual process: 
a file is downloaded, edited by a local editor and uploaded to its original remote location again. WireDocs makes this procedure 
completely transparent from a user perspective. The user only watches the application opening the demanded file and uploads are
processed in the background after the file has been saved. WireDocs integrates with Drupal's content structure, namely the Field
API, and currently supports file and image fields.


Client requirements
-------------------

As a Java applet bridges the gap between Drupal and the OS the client must fulfill some prerequisites:

- Java browser plugin running on JRE version 1.7+, download it from http://www.java.com/download
- Proper set up of OS file type associations with editor applications

In this implementation the Java applet delegates the launch of an application for a file type to the operation system. If that's
done correctly it should run on any operating system (supported by Java of course :).


Installation
------------

If the client is properly prepared the server-side installation is straight forward:

1. Enable the module "wiredocs". Additionally you have to enable one field module, e. g. "wiredocs_file" or "wiredocs_image".
2. Set permission "use wiredocs" for privileged user roles

It's recommended that you also enable the module "wiredocs_field_permissions". It makes sure the the menu callbacks for 
downloading and uploading files inherit access permissions from referenced nodes and fields, e. g. defined by field_permissions.


Configuration
--------------

WireDocs doesn't provide an adminstration interface on its own. Depending on which field module (file or image) you have enabled
a dedicated widget called "File Edit" is provided in scope of the content types configuration on admin/structure/types. There are no 
additional configuration options, so just add/edit a field to the content type with the new widget, and that's it!


Usage
-----

Once you have set up your new field, users see an additional operation "Edit" next to field when accessing the node edit form. A simple
click on the button downloads the file temporarily and opens the associated local editor. Now the file can be edited like any other. On
save the file is uploaded in the background to the server. After you are done, just close the application. There's no need to save the
node edit form as the file has already been updated.