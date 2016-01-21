/**
 * @file

 *  Creates a tab by the scrollbar for feedback and suggestions.
 */
(function ($) {

  Drupal.behaviors.feedbacktab = {
    attach: function(context, settings) {
	  // @todo Make these settings extensible and configurable with an
	  // admin/settings/feedbacktab UI.
	  var distance = 300;
	  var bounceOptions = {
	    direction: 'left',
	    distance: distance / 2,
	    times: 3
	  };
	  var dialogOptions = {
	    modal: true,
	    minWidth: 700,
	    width: 900,
	    minHeight: 500,
	    height: 580,
	    resizable: false,
	    draggable: false,
	    open: function() {
	      tab.hide();
	    },
	    close: function() {
	      tab.show();
	    }
	  }

	  // Add the tab to the DOM.
	  var tab = Drupal.theme('feedbacktab').appendTo('body', context);

	  // Attach an event handler to clicks on the tab.
	  tab.click(function() {
	    Drupal.theme('feedbacktabIframe').dialog(dialogOptions);
	  });

	  // Bounce the tab a short while after it has been made visible.
	  // @todo Make these settings extensible and configurable with an
	  // admin/settings/feedbacktab UI.
	  setTimeout(function() {
	    if ($.browser.mozilla) {
	      // Bounce on position:fixed elements only works in FF.
	      tab.effect('bounce', bounceOptions, 500, attachHoverCallbacks);
	    }
	    else if (!$.browser.safari) {
	      // Slide out and in in other browsers, except safari.
	      tab.addClass('hover', 80, function() {
	        setTimeout(function() {
	          tab.removeClass('hover', 500, attachHoverCallbacks);
	        }, 500);
	      });
	    }
	    else {
	      // Neither method works well in safari.
	      attachHoverCallbacks();
	    }
	  // Wait 1 second after the tab has loaded before bouncing it.
	  }, 1000);

	  // Attaches the hover callbacks.
	  var attachHoverCallbacks = function() {
	    tab.hover(function() {
	      // Slide the tab out quickly.
	      tab.addClass('hover', 80);
	    }, function() {
	      // Hide it away slowly.
	      tab.removeClass('hover', 500);
	    });
	  };
    }
  }

})(jQuery);

// Theme function for the tab.
Drupal.theme.prototype.feedbacktab = function() {
  var detail = Drupal.t('We are interviewing our readers.  Please take a moment to <strong>complete our two-minute questionaire</strong>.  Thank you! :)');
  var content = '<span class="title">' + Drupal.t('Feedback') + '</span>';
  content += '<span class="content">' + detail + '</span>';
  return jQuery('<a class="feedbacktab feedbacktab-right">' + content + '</a>');
};

// Theme function for the dialog box.
Drupal.theme.prototype.feedbacktabIframe = function() {
  // jQuery.dialog() sets the width and height on <iframe>.
  return jQuery('<iframe class="feedbacktab-iframe" src="' + Drupal.settings.feedbacktab.iframeUri + '"> </iframe>');
};
