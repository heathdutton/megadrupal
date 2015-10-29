/**
 * @file
 * contest.js
 * Unobtrusive HTML form validation.
 * Most of this was stolen from validate.js.
 * My humble apologies I can't credit the original author becaue I don't 
 * know where I got this from.
 *
 * On document load, this module scans the document for HTML forms and
 * textfield form elements.  If it finds elements that have a "required" or
 * "pattern" attribute, it adds appropriate event handlers for client-side
 * form validation.
 *
 * If a form element has a "pattern" attribute, the value of that attribute
 * is used as a JavaScript regular expression, and the element is given an
 * onchange event handler that tests the user's input against the pattern.
 * If the input does not match the pattern, the background color of the
 * input element is changed to bring the error to the user's attention.
 * By default, the textfield value must contain some substring that matches
 * the pattern. If you want to require the complete value to match precisely,
 * use the ^ and $ anchors at the beginning and end of the pattern.
 *
 * A form element with a "required" attribute must have a value provided.
 * The presence of "required" is shorthand for pattern="\S".  That is, it
 * simply requires that the value contain a single non-whitespace character
 *
 * If a form element passes validation, its "class" attribute is set to
 * "valid".  And if it fails validation, its class is set to "invalid".
 * In order for this module to be useful, you must use it in conjunction with
 * a CSS stylesheet that defines styles for "invalid" class.  For example:
 *
 *  <!-- attention grabbing orange background for invalid form elements -->
 *  <style>input.invalid { background: #fa0; }</style>
 *
 * When a form is submitted the textfield elements subject to validation are
 * revalidated.  If any fail, the submission is blocked and a dialog box
 * is displayed to the user letting him know that the form is incomplete
 * or incorrect.
 *
 * You may not use this module to validate any form fields or forms on which
 * you define your own onchange or onsubmit event handlers, or any fields
 * for which you define a class attribute.
 *
 * This module places all its code within an anonymous function and does
 * not define any symbols in the global namespace.
 */
 
/**
 * Do everything in this one anonymous function.
 */
(function() {

// When the document finishes loading, call init().

  if (window.addEventListener) {
    window.addEventListener("load", init, false);
  }
  else if (window.attachEvent) {
    window.attachEvent("onload", init);
  }
/**
 * Define event handlers for any forms and form elements that need them.
 */
  function init() {

// Loop through all forms in the document.

    for (var i = 0; i < document.forms.length; i++) {
      var f = document.forms[i];
      var needsValidation = false;

// Now loop through the elements in our form.

      for (j = 0; j < f.elements.length; j++) {
        var e = f.elements[j];

// We're only interested in <input type="text"> textfields.

        if (e.type != "text" && e.type != "checkbox") {
          continue;
        }
// Set the pattern and required attrobutes.

        var pattern = e.getAttribute("pattern");
        var required = e.getAttribute("required") != null;

        if (required && !pattern) {
          pattern = "\\S";
          e.setAttribute("pattern", pattern);
        }
// If this element requires validation, validate the element each time it changes via the onsubmit handler.

        if (pattern) {
          e.onchange = validateOnChange;
          needsValidation = true;
        }
      }
// If at least one of the form elements needed validation, we also need an onsubmit event handler for the form
      if (needsValidation) {
        f.onsubmit = validateOnSubmit;
      }
    }
  }
/**
 * This function is the onchange event handler for textfields that require validation. Remember that we converted the required attribute to a pattern attribute in init().
 */
  function validateOnChange() {
    var field = this;                            // the field
    var pattern = field.getAttribute("pattern"); // the pattern
    var value = this.value;                      // the user's input

// If the value does not match the pattern set the class to "invalid".

    if (value.search(pattern) == -1) {
      field.className = "invalid";
    }
    else {
      field.className = "valid";
    }
  }
/**
 * The onsubmit event handler revalidates the fields and then checks their classNames to see if they are invalid, (failure displays an alert).
 */
  function validateOnSubmit() {
    var invalid = false;

    for (var i = 0; i < this.elements.length; i++) {
      var e = this.elements[i];

// The spam alert.

      if ((e.name == 'opt_in' || e.name == 'optin') && e.checked == false) {
        alert('You must agree to receive promotional efforts from the contest sponsors and promoters.');
        return false;
      }
// If the element is a text field and has our onchange handler invoke the handler to re-validate.

      if (e.type == "text" && e.onchange == validateOnChange) {
        e.onchange();
        if (e.className == "invalid") {
          invalid = true;
        }
      }
    }
// If the form is invalid, alert user and block submission.

    if (invalid) {
      alert("The form is incompletely or incorrectly filled out.\nPlease correct the  highlighted fields and try again.");
      return false;
    }
  }
})();
