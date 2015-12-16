(function($) {

/**
 * Handles click events on selected links as well as form submits and loads
 * the corresponding pages through Ajax.
 */
Drupal.behaviors.ajaxPages = {
  attach: function (context, settings) {
    // HTML5 window.history only.
    if (!(window.history && window.history.pushState)) {
      return;
    }

    if (context == document && !$(document).data('ajax-pages-processed')) {
      $(document).data('ajax-pages-processed', true);

      var loadPage = function(url) {
        // Create a dummy element because Drupal's Ajax framework needs one.
        var $element = $('<a></a>');
        $element.attr('href', url);

        var element_settings = {};

        // Don't use a progress indicator.
        element_settings.progress = { 'type': 'none' };
        // Send request to the target of the anchor.
        element_settings.url = $element.attr('href');
        // Dummy event because apparently the event property can't be empty.
        element_settings.event = 'neverhappens';
        element_settings.submit = { _ajax_pages: 1 };

        var base = $element.attr('id');
        var ajax = new Drupal.ajax(base, $element[0], element_settings);
        ajax.options.type = 'GET';

        return ajax.eventResponse($(ajax.element));
      };

      window.onpopstate = function() {
        if (!Drupal.ajaxPages.refreshed) {
          loadPage(document.location.href);
        }
      };

      $(document).delegate(settings.ajaxPages.linksSelector, 'click', function(e) {
        if ((this.href.indexOf('#') == -1 || document.location.href.split('#')[0] != this.href.split('#')[0]) &&
          Drupal.ajaxPages.isInternalNonAdminLink(this.href)) {
          // Fire off the title and url to window.history.
          Drupal.ajaxPages.refreshed = false;
          history.pushState(null, $(this).attr('title'), this.href);
          loadPage(this.href);

          // Stop event propagation and prevent the default action.
          e.preventDefault();
          e.stopPropagation();
        }
      });
    }

    // This class means to submit the form to the action using Ajax.
    $('.form-submit:not(.ajax-processed, .ctools-use-ajax)').addClass('ajax-processed').each(function () {
      if (Drupal.ajaxPages.isInternalNonAdminLink(this.form.action)) {
        var element_settings = {};

        // Don't use a progress indicator.
        element_settings.progress = { 'type': 'none' };
        // Ajax submits specified in this manner automatically submit to the
        // normal form action.
        element_settings.url = $(this.form).attr('action');
        // Form submit button clicks need to tell the form what was clicked so
        // it gets passed in the POST request.
        element_settings.setClick = true;
        // Form buttons use the 'click' event rather than mousedown.
        element_settings.event = 'click';
        element_settings.submit = { _ajax_pages: true };

        var base = $(this).attr('id');
        Drupal.ajax[base] = new Drupal.ajax(base, this, element_settings);
        if (this.form.method == 'get') {
          Drupal.ajax[base].options.type = 'GET';
        }
      }
    });
  }
}

// Override parameters for Ajax pages requests to make them via GET.
Drupal.ajaxBeforeSerialize = Drupal.ajax.prototype.beforeSerialize;
Drupal.ajax.prototype.beforeSerialize = function(element, options) {
  if (options.data._ajax_pages) {
    // Allow detaching behaviors to update field values before collecting them.
    // This is only needed when field values are added to the POST data, so only
    // when there is a form such that this.form.ajaxSubmit() is used instead of
    // $.ajax(). When there is no form and $.ajax() is used, beforeSerialize()
    // isn't called, but don't rely on that: explicitly check this.form.
    if (this.form) {
      var settings = this.settings || Drupal.settings;
      Drupal.detachBehaviors(this.form, settings, 'serialize');
    }
  }
  else {
    Drupal.ajaxBeforeSerialize.call(this, element, options);
  }
}

/**
 * Custom Ajax command to set Drupal.settings.
 */
Drupal.ajax.prototype.commands.ajaxPagesSettings = function(ajax, response, status) {
  // Replace Drupal.settings with the supplied settings array while preserving
  // the ajaxPageState object.
  var ajaxPageState = Drupal.settings.ajaxPageState;
  var urlIsAjaxTrusted = Drupal.settings.urlIsAjaxTrusted;
  Drupal.settings = response.settings;
  Drupal.settings.urlIsAjaxTrusted = urlIsAjaxTrusted;
  Drupal.settings.ajaxPageState = ajaxPageState;
}

/**
 * Custom Ajax command to synchronize the page state in case of redirects.
 */
Drupal.ajax.prototype.commands.ajaxPagesSynchronizeState = function(ajax, response, status) {
  if (response.url != document.location.href.split('#')[0]) {
    if (ajax.form) {
      Drupal.ajaxPages.refreshed = false;
      history.pushState(null, null, response.url);
    }
    else {
      history.replaceState(null, null, response.url);
    }
  }
}

/**
 * Custom Ajax command to load files.
 */
Drupal.ajax.prototype.commands.ajaxPagesLoadFiles = function(ajax, response, status) {
  var type = response.type;
  var files = response.files;
  var scope = response.scope || 'header';

  var newFiles = [];
  var absoluteRe = new RegExp('^((f|ht)tps?:)?//');
  for (var key in files) {
    if (!Drupal.settings.ajaxPageState[type][key]) {
      var fileUrl = (absoluteRe.test(files[key]) ? '' : Drupal.settings.basePath) + files[key];
      newFiles.push(fileUrl);
      Drupal.settings.ajaxPageState[type][key] = 1;
    }
  };

  if (newFiles.length > 0) {
    var html = '';

    if (type == 'css') {
      html += '<style>';
      for (var i in newFiles) {
        html += '@import url("' + newFiles[i] + '");\n';
      };
      html += '</style>';
    }
    else if (type == 'js') {
      for (var i in newFiles) {
        html += '<script src="' + newFiles[i] + '"></script>\n';
      };
    }

    if (scope == 'header') {
      ajax.commands.insert(ajax, {
        method: 'prepend',
        selector: 'head',
        data: html
      }, status);
    }
    else {
      ajax.commands.insert(ajax, {
        method: 'append',
        selector: 'body',
        data: html
      }, status);
    }
  }
}

/**
 * Custom Ajax command to set page title.
 */
Drupal.ajax.prototype.commands.ajaxPagesSetTitle = function(ajax, response, status) {
  document.title = response.title;
}

/**
 * Custom Ajax command to set target element's attributes.
 */
Drupal.ajax.prototype.commands.ajaxPagesAttributes = function(ajax, response, status) {
  var $elements = $(response.selector);

  // Store classes that need to be preserved.
  var classes = '';
  $.each(response.preserveClasses, function(i, name) {
    if ($elements.hasClass(name)) {
      classes += ' ' + name;
    }
  });

  // Remove all existing attributes first.
  // See http://stackoverflow.com/a/1870487/1712145
  $elements.each(function() {
    var attributes = $.map(this.attributes, function(item) {
      return item.name;
    });

    var $element = $(this);
    $.each(attributes, function(i, name) {
      $element.removeAttr(name);
    });
  });

  // Add attributes passed to the command.
  $.each(response.attributes, function(name, attribute) {
    $elements.attr(name, attribute);
  });

  // Restore the classes to preserve.
  $elements.addClass(classes);
}

/**
 * Custom Ajax command to set the page content HTML.
 *
 * We need a custom command that a). wouldn't wrap the content in an extra
 * <div> and b). wouldn't run jQuery() on response.data prematurely so as
 * to allow inline scripts reference inserted DOM elements.
 *
 * @see Drupal.ajax.commands.insert
 */
Drupal.ajax.prototype.commands.ajaxPagesPageHtml = function(ajax, response, status) {
  // Get information from the response. If it is not there, default to
  // our presets.
  var wrapper = response.selector ? $(response.selector) : $(ajax.wrapper);

  // Detach behaviors from the wrapper first.
  var settings = response.settings || ajax.settings || Drupal.settings;
  Drupal.detachBehaviors(wrapper, settings);

  // Add the new content to the page.
  wrapper.empty();
  wrapper.html(response.data);

  // Scroll to element if there's an anchor in the URL.
  if (window.location.hash) {
    var $element = $(window.location.hash);
    if ($element.length > 0) {
      $element[0].scrollIntoView(true);
    }
  }

  // Attach all JavaScript behaviors to the new content, if it was successfully
  // added to the page, this if statement allows #ajax['wrapper'] to be
  // optional.
  if (wrapper.contents().length > 0) {
    // Apply any settings from the returned JSON if available.
    var settings = response.settings || ajax.settings || Drupal.settings;
    Drupal.attachBehaviors(wrapper, settings);
    // Make sure stuff like Omega's Equal heights gets executed.
    // @todo This is probably hacky and/or needs to be moved elsewhere.
    $(window).trigger('load');
  }
}

/**
 * Helper object for URL-related functions.
 */
Drupal.ajaxPages = Drupal.ajaxPages || {};

/*
 * Flag indicating whether the most recent page (re)-load was a full
 * page refresh.
 * See http://stackoverflow.com/questions/3700440/html5-onpopstate-on-page-load
 */
Drupal.ajaxPages.refreshed = true;

/**
 * Check if the given link is in the non-administrative section of the site.
 *
 * @see Drupal.overlay.isAdminLink()
 *
 * @param url
 *   The URL to be tested.
 *
 * @return boolean
 *   TRUE if the URL represents an administrative link, FALSE otherwise.
 */
Drupal.ajaxPages.isInternalNonAdminLink = function (url) {
  if (!Drupal.ajaxPages.isInternalLink(url)) {
    return false;
  }

  var path = this.getPath(url);

  // Turn the list of administrative paths into a regular expression.
  if (!this.adminPathRegExp) {
    var prefix = '';
    if (Drupal.settings.ajaxPages.pathPrefixes.length) {
      // Allow path prefixes used for language negatiation followed by slash,
      // and the empty string.
      prefix = '(' + Drupal.settings.ajaxPages.pathPrefixes.join('/|') + '/|)';
    }
    var adminPaths = '^' + prefix + '(' + Drupal.settings.ajaxPages.paths.admin.replace(/\s+/g, '|') + ')$';
    var nonAdminPaths = '^' + prefix + '(' + Drupal.settings.ajaxPages.paths.non_admin.replace(/\s+/g, '|') + ')$';
    adminPaths = adminPaths.replace(/\*/g, '.*');
    nonAdminPaths = nonAdminPaths.replace(/\*/g, '.*');
    this.adminPathRegExp = new RegExp(adminPaths);
    this.nonAdminPathRegExp = new RegExp(nonAdminPaths);
  }

  return !this.adminPathRegExp.exec(path) || this.nonAdminPathRegExp.exec(path);
};

/**
 * Determine whether a link is external to the site.
 *
 * @see Drupal.overlay.isExternalLink()
 *
 * @param url
 *   The URL to be tested.
 *
 * @return boolean
 *   TRUE if the URL is external to the site, FALSE otherwise.
 */
Drupal.ajaxPages.isInternalLink = function (url) {
  var re = RegExp('^(https?:)?//(?=' + window.location.host + ')');
  return re.test(url);
};

/**
 * Helper function to get the (corrected) Drupal path of a link.
 *
 * @see Drupal.ajaxPages.getPath()
 *
 * @param link
 *   Link object or string to get the Drupal path from.
 * @param ignorePathFromQueryString
 *   Boolean whether to ignore path from query string if path appears empty.
 *
 * @return
 *   The Drupal path.
 */
Drupal.ajaxPages.getPath = function (link, ignorePathFromQueryString) {
  if (typeof link == 'string') {
    // Create a native Link object, so we can use its object methods.
    link = $(link.link(link)).get(0);
  }

  var path = link.pathname;
  // Ensure a leading slash on the path, omitted in some browsers.
  if (path.charAt(0) != '/') {
    path = '/' + path;
  }
  path = path.replace(new RegExp(Drupal.settings.basePath + '(?:index.php)?'), '');
  if (path == '' && !ignorePathFromQueryString) {
    // If the path appears empty, it might mean the path is represented in the
    // query string (clean URLs are not used).
    var match = new RegExp('([?&])q=(.+)([&#]|$)').exec(link.search);
    if (match && match.length == 4) {
      path = match[2];
    }
  }

  return path;
};

})(jQuery);
