README
HTML IMPORT
24 January 2015

Main feature
============
- Allows user to specify heading levels at which the HTML document is divided and imported. For example, if H3 is selected under "Heading level depth", each H1, H2 and H3 will become a separate page in the imported book.
- Imports images. The module scans and fixes the paths of the images referenced by imported pages.
- Respects the heading level hierarchy of the source document by reconstructing the same book hierarchy
- Scans and re-creates reference links. Reference links in the source HTML may be divided into different book pages after import. The module scans and re-links the reference links to maintain the integrity of the document.
- Scans and moves footnotes/endnotes. If footnotes/endnotes are well-formatted, the module scans and moves the footnotes/endnotes to the sections where they are referenced for easy reading.
- Meets WCAG accessibility requirements. The module preserves accessibility properties of the source HTML such as ALT text.
- Cleans up undesirable Word characters such as smart quotes in titles for clean URL creation

Installation and configuration
==============================
https://www.drupal.org/project/html_import