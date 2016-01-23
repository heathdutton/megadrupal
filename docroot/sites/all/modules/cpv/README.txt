Overview
========

Code per Views Display (CPV) creates a display extender plugin for Views that allows Views administrators the ability to add custom CSS and Javascript into any display. The CSS and JS code is stored in the display and served as as either inline code or from a file.

CPV is intended for use by Views (i.e. site) administrators.

Features
========

- Add custom CSS or JS code to each display.
- Serve code either as inline or from file.

Use Cases
========

CPV is useful in cases where:

- you want to allow views admins to theme a particular display without granting them access to theme files;
- you want to apply CSS and JS code to a display across all themes;
- you want to apply CSS and JS code to a display without modifying an existing theme or creating a sub-theme.

Requirements
========

CPV requires Drupal 7 and Views 3. It also requires the Views UI module to be enabled, as CPV has no effect otherwise.

Usage
========

Install and enable the CPV module, then go to "admin/structure/views/settings/advanced" to enable the display extender, "Code per Display". In each views display, a "CSS" and "JS" field will allow custom code to be added under the "Advanced" fieldset in the display settings.

The code can be configured to be served inline (default) embedded into the HTML or served from a file. Go to "admin/structure/views/settings/cpv" to configure that option and the file path if that option is selected.

Caveats
========

- Do not include the <style> and <script> tags for CSS and JS code, respectively.
- JS code written with jQuery must be wrapped with: jQuery(document).ready(function($){ /* INSERT CODE HERE */ });.
- If multiple views are displayed in the same page, take care to ensure the CSS and JS code from a CPV display doesn't also select elements from the other view(s). As a precaution, prefix all selectors with the view ID or display ID (e.g. "view-myview" or "view-display-id-cpv_page_1").

Example Code Snippets
========

CSS
--------
The following CSS highlights the first row of the view with a red background.

.views-row-1 {
  background-color: #F00;
}

Javascript
--------

The following JS uses jQuery. For the view this snippet is inserted into, clicking the view's first row will prompt the user with an alert window.

jQuery(document).ready(function($){
  $('.views-row-1').click(function() {
    alert('Handler for .click() called.');
  });
});