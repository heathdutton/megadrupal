Introduction
============
Context as Reaction is a small utility module that provides a context reaction where a name/value pair can be set by context_set().  This can become useful if you want context to set additional information that can be used later in the rendering processing (for example:  Use this reaction to set additional bits of information for the theme.)

The use case this module solved is:
  - On the SETI site, we use flags to feature content in the carousels. There are three different carousels for the three different "Homepage" variants.
  - Administrators will assign content to each defined flag.
  - Using context, when one of the three Homepages are triggered, set a name/value pair in context to identify which flag to use. For example, on the main homepage, set fid = 3.
  - Then we are able to generate the custom javascript carousel based on the flag.

Installation
============
Download and enable the module.