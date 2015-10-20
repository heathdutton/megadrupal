/**
 * @file
 * Adds functions to be used across the Slick slideshow module.
 */

/**
 * Prepares a selector and settings for Slick implementation.
 *
 * @param context
 *  The document context
 *
 * @param string fieldSelector
 *  The element that the slideshow belongs to.
 *
 * @param array settings
 *  The Slick settings and target belonging to the element.
 */
function slickSlideshowPrepare(field, context, behavior, fieldSelector, settings) {
  var $ = jQuery;

  if (fieldSelector.charAt(0) !== '#' && fieldSelector.charAt(0) !== '.') {
    // Selector fallback to id if only the selector name is passed.
    fieldSelector = '#' + fieldSelector;
  }

  var fieldPrepare;
  field.once(behavior, function() {
    var fieldSettings = settings['settings'];

    // Change settings passed as strings to correct data type.
    for (var key in fieldSettings) {
      var field = fieldSettings[key];
      if (typeof field === 'string') {
        // Change string to Function.
        if (field.indexOf('function(') === 0) {
          var params = field.between('function(', ')');
          var functionBody = field.between('{', '}');
          fieldSettings[key] = new Function(params, functionBody);
        }
        // Change string to Object.
        if (field.indexOf('[') === 0 || field.indexOf('{') === 0) {
          if (field.between('[', ']') || field.between('{', '}')) {
            fieldSettings[key] = eval(field);
          }
        }
      }
    }

    if (settings['target']) {
      // Check to see if there's a child target to initialize slick on.
      target = $(this).find(settings['target']).first();
    }
    else {
      // Initialize Slick directly on element with id.
      target = this;
    }

    // Ready the return variable.
    fieldPrepare = {'fieldSettings' : fieldSettings, 'target' : target};
  });

  return fieldPrepare;
}

/*
 * Function that grabs substring between two specified substrings.
 */
String.prototype.between = function(prefix, suffix) {
  s = this;
  var i = s.indexOf(prefix);
  if (i >= 0) {
    s = s.substring(i + prefix.length);
  }
  else {
    return '';
  }
  if (suffix) {
    i = s.indexOf(suffix);
    if (i >= 0) {
      s = s.substring(0, i);
    }
    else {
      return '';
    }
  }
  return s;
}
