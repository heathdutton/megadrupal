The Research Annotator module provides you with the ability to annotate html elements
of node body content through the field_body field.

Installation
------------
Follow the standard contributed module installation process:
http://drupal.org/documentation/install/modules-themes/modules-7

Requirements
------------
Dependencies include the following modules.
- Block
- ctools
- entity

How it Works
------------
Once configured, this "block" allows people to annotate node content. Each annotation is associated with a node revision rather
than the node itself so if you want to revert node revisions, annotations are preserved. Once a new node revision is created, new annotations
can be associated with the new revision. 

Configuration
-------------
1.  Go to any content type and modify its body field (for example, go to example.com/admin/structure/types/manage/page/fields/body).
    Configure the Research Annotator settings. NOTE, it is also recommended you set the default output filter of the body field to
    "Annotation Safe Output".
2.  Configure CRUD permissions regarding research annotations.

IMPORTANT Recommendations
-------------------------
1.  It is STRONGLY encouraged to create a new node revision if you are significantly editing body content, E.G. removing paragraphs. 
    Not doing so will keep existing annotations associated with the current node revision and the annotations will not make sense in the 
    context of the content.
2.  Use the "Annotation Safe Output" filter on body content that is being annotated.
3.  Use a LARGE block region for displaying annotations. This block region should also be to the left or right of the content being annotated.
4.  When modifying tpl files in your theme, do not remove or modify existing element attributes. Adding new attributes is safe.
    The reason for this is the heavy use of jQuery for this module to function correctly.

Limitations
-----------
1.  This module is currently only operating on the field_body field and no other textarea field.
2.  You can only annotate the FIRST value of a body field.
3.  Currently, there is no multilingual support for annotation text.
  