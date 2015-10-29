README
================================================================================

This module converts node bodies and titles written with valid(!) HTML markup
to the XLIFF (XML Localization Interchange File Format) and back to HTML. You 
can use Computer Aided Translation (CAT) tools to support your content 
translation process.

Note that this module is in the very early stages, and there are several
limitations. Testing and improvements (patches) are very much welcome!

More information on XLIFF:

 - http://www.xliff.org/
 - http://xliff-tools.freedesktop.org/wiki/
 - https://sourceforge.net/projects/xliffroundtrip/

USAGE
================================================================================

1. Copy the module files to sites/all/modules/xliff (or your site
   specific directory).
2. Enable the module on the module list interface.
3. Hand out xliff export/import permission to desired user roles.
4. Look for the 'XLIFF Tools' tab on nodes, where you can export the
   given node as XLIFF, and import an XLIFF file, overwriting current
   contents of the node.

SUGGESTED XLIFF TOOL
================================================================================

As a proof of concept, I have tested the translation of the
generated XLIFF content in

(A) Sun's Open Language Tools XLIFF Editor 1.2.7
    (https://open-language-tools.dev.java.net/) and
(B) Heartsome XLIFF Translation Editor 6.2-10
    (http://www.heartsome.net/EN/xlfedit.html) - 30 day evaluation version.

My experience showed that (A) chokes on attributes in namespaces it does not
know (and it does remove "foreign" xmlns declarations on loading a file),
interestingly it did not offer the first translation unit for translation,
it did not show inline formatting in the source text, and it did not preserve
inline formatting in the translated text. Finally it did add the same
attributes multiple times to some elements, which easily invalidated the XML.
Therefore I would not recommend you using (A).

(B) however sports a much nicer look, and it worked with the generated XLIFF
documents nicely, without the above oddities. Unfortunately it is not a free
tool.

dwaynebailey suggested (http://drupal.org/node/204642) these two tools as well:
  Translate Toolkit: http://translate.sourceforge.net/wiki/toolkit/index
  Pootle: http://translate.sourceforge.net/wiki/pootle/index

LIMITATIONS, POSSIBLE ISSUES
================================================================================

- Only the node title and body are exported (Fields are not supported yet).
- The body should be valid HTML (other types of markup is not supported).
- The imported XLIFF is converted to HTML and imported as-is, without
  checking it against some input format.

CONTRIBUTORS
================================================================================

The XSL sheets used to convert HTML to/from XLIFF are originally created by
Bryan Schnabel <bryan.schnabel (AT) bschnabel.com> for the
https://sourceforge.net/projects/xliffroundtrip/ project, generously released
under the GNU GPL license, so I can reuse them.
