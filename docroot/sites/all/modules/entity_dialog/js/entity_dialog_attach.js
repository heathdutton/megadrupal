(function($) {
/**
 * @file
 * Adds the entity-dialog path and class to matched links.
 */
Drupal.behaviors.entity_dialog = {
  attach: function (context, settings) {
    var basePath = Drupal.settings.basePath,
        crudPaths = Drupal.settings.entityDialog,
        anchors = $('a', context);
    
    // Step through each link on the page that hasn't been rewritten.
    anchors.once('entity-dialog-setup', function (i) {
      // Not all anchor tags have hrefs but we only want to deal with
      // ones that do.    
      if (typeof($(this).attr('href')) !== 'undefined') {
        checkAndAttach($(this), crudPaths);
      }
    });
  }
};


/**
 * Decide whether to have entity_dialog intercept the link click, for the a tag
 * that is passed to us.
 *
 * @param e
 *   The <a> element we are checking and possibly intercepting.
 */
function checkAndAttach(e, crudPaths) {
  $.each(crudPaths, function(i, crudPath) {    
    dialogHref = getDialogHref(e, crudPath);
    
    // If match then add class and bodgy the path.
    if (dialogHref) {
      e.attr('href', dialogHref).addClass('use-ajax entity-dialog');

      // Since we just added .use-ajax to an element, we now need to attach
      // behaviours to it.
      Drupal.attachBehaviors(e);
    }
  });
}


/**
 * Get the dialog href. Return a modified href if the passed href matched the
 * passed CRUD path. Otherwise returns false.
 *
 * Links could take any of the following formats:
 *     
 *     %basepath%crud-path (basePath is something either just / or /directory/ )
 *     %basepath%crud-path/
 *     %basepath%crud-path?query-string
 *
 * We will convert the following to something like:
 *
 *     %basepath%entity-dialog/crud-pat
 *     %basepath%entity-dialog/crud-path/
 *     %basepath%entity-dialog/crud-path nb. query string will not be retained (eg ?destination=blah/boo).
 */
function getDialogHref(e, crudPath) {
  var href = e.attr('href'),
      basePath = Drupal.settings.basePath,
      basePathFound = (href.indexOf(basePath) === 0);

  // Don't continue if the href doesn't start with the basepath. ie it must begin 
  // with either a single / or a basepath which looks like /directory/
  if (!basePathFound) {
    return false;
  } 
  
  // Strip the basepath off the href: 
  // If the basepath is only a / then just remove the first character from the href
  var basePathLength = basePath.length;
      noBasePath = href.substring(basePathLength),
      strippedHref = noBasePath.split('?')[0],
      hrefBits = strippedHref.split('/'),
      crudBits = crudPath.split('/'),
  
      matchFound = pathMatch(hrefBits, crudBits);  
  
  if (!matchFound) return false;
  
  // We have determined that the href was found to match the given CRUD path,
  // so this is an href we want to intercept. 
  return basePath + 'entity-dialog/' + strippedHref; 
}



/**
 * Work out how many parts of the supplied URL match the CRUD url.
 */
function pathMatch(hrefBits, crudBits) {
  if (crudBits.length != hrefBits.length) return false; // No match if this CRUD path has differnt length of pieces than the URL.

  var result = true;
  $.each(crudBits, function(i, crudBit) {
    // If this crud arg is a %something% placeholder, we don't care what the 
    // corresponding hrefBit is.
    if (crudBit.substr(0, 1) == '%') return true; // Continue to the next item.
  
    if (hrefBits[i] !== crudBit) {
      result = false;
      return false; // Break out of the $.each entirely.
    }
  });
  
  return result; // Everything matched!
}


})(jQuery);