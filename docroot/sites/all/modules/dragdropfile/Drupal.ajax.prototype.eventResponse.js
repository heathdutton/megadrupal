
/**
 * Handle an event that triggers an Ajax response.
 *
 * When an event that triggers an Ajax response happens, this method will
 * perform the actual Ajax call. It is bound to the event using
 * bind() in the constructor, and it uses the options specified on the
 * ajax object.
 */

Drupal.ajax && (Drupal.ajax.prototype.eventResponse = function (element, event) {
  // Create a synonym for this to reduce code confusion.
  var ajax = this;

  // Do not perform another ajax command if one is already in progress.
  if (ajax.ajaxing) {
    return false;
  }

  try {
    if (ajax.form) {
      // If setClick is set, we must set this to ensure that the button's
      // value is passed.
      if (ajax.setClick) {
        // Mark the clicked button. 'form.clk' is a special variable for
        // ajaxSubmit that tells the system which element got clicked to
        // trigger the submit. Without it there would be no 'op' or
        // equivalent.
        element.form.clk = element;
      }

      if ( 'FormData' in window  && jQuery(element).hasClass('i-am-dragdropfile') ) {
        // Get form state crap from Drupal.
        ajax.beforeSerialize(ajax.element, ajax.options);

        // Stole this nasty shit from jquery.form.js. I expected a nice, reusable function but NO OF COURSE NOT.
        var i = [];
        for (g in ajax.options.data)
          if (ajax.options.data[g] instanceof Array)
            for (var j in ajax.options.data[g])
              i.push({name: g,value: ajax.options.data[g][j]});
          else
            h = ajax.options.data[g], h = jQuery.isFunction(h) ? h() : h, i.push({name: g,value: h})
        var serialized = jQuery.param(i);

        // Build form data + ajax state crap.
        var fd = new FormData(ajax.form[0]);
        jQuery.each(serialized.split('&'), function(i, d) {
          var x = d.split('='),
            name = decodeURIComponent(x.shift()),
            value = decodeURIComponent(x.join('='));
          fd.append(name, value);
        });

        // Change request parameters.
        ajax.options.data = fd;
        ajax.options.processData = false;
        ajax.options.contentType = false;

        // Let jQuery handle it CORRECTLY. Damn jQuery, you a boss!
        jQuery.ajax(ajax.options);
      }
      else {
        ajax.form.ajaxSubmit(ajax.options);
      }
    }
    else {
      ajax.beforeSerialize(ajax.element, ajax.options);
      jQuery.ajax(ajax.options);
    }
  }
  catch (e) {
    // Unset the ajax.ajaxing flag here because it won't be unset during
    // the complete response.
    ajax.ajaxing = false;
    alert("An error occurred while attempting to process " + ajax.options.url + ": " + e.message);
  }

  // For radio/checkbox, allow the default event. On IE, this means letting
  // it actually check the box.
  if (typeof element.type != 'undefined' && (element.type == 'checkbox' || element.type == 'radio')) {
    return true;
  }
  else {
    return false;
  }

});
