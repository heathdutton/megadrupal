/* JavaScript for ext_link_page */

(function ($) {

function extLinkPageModal(url) {
  var title, message, modal, timeoutId;

  // Generate message for external link modal window
  title = Drupal.settings.ext_link_page.messageTitle ? String(Drupal.settings.ext_link_page.messageTitle) : '';
  message = Drupal.settings.ext_link_page.messageBody ? String(Drupal.settings.ext_link_page.messageBody) : '';
  message = message.replace(/\[url\]/gi, url)
    .replace(/\[link\]/gi, '<a href="' + url + '">' + url + '</a>')
    .replace(/\[delay\]/gi, Drupal.settings.ext_link_page.delay)
    .replace(/\[site_name\]/gi, Drupal.settings.ext_link_page.siteName);

  // If directDisable is enabled, then do not automatically redirect.
  directDisable = Drupal.settings.ext_link_page.directDisable ? parseInt(Drupal.settings.ext_link_page.directDisable, 10) : 0;

  // If delay is 0, then redirect immediately (unless directDisable is set).
  delay = Drupal.settings.ext_link_page.delay ? parseInt(Drupal.settings.ext_link_page.delay, 10) : 0;
  if (!directDisable && !delay) {
    window.location.href = url;
    return;
  }

  // Display the message as a jQuery UI modal window
  timeoutId = false;
  modal = $('<div id="extLinkPageModal-content">' + message + '</div>');
  modal.dialog({
    buttons: [
      {
        text: Drupal.t('Cancel'),
        click: function () { $(this).dialog('close'); }
      },
      {
        text: Drupal.t('Continue to this site'),
        click: function () { window.location.href = url; }
      }
    ],
    open: function () {
      if (!directDisable) {
        timeoutId = setTimeout(function () { window.location.href = url; }, delay * 1000);
      }
      $('.ui-widget-overlay').addClass('extLinkPage');
    },
    close: function () {
      if (timeoutId) {
        clearTimeout(timeoutId);
      }
      $('.ui-widget-overlay').removeClass('extLinkPage');
    },
    dialogClass: 'extLinkPage',
    draggable: false,
    modal: true,
    resizable: false,
    title: title,
    width: 400
  });
}

function extLinkPageAttach(context) {
  // Find all links that are directed to the external link page.
  $('a[href^="' + Drupal.settings.ext_link_page.pathLinkPage + '"]', context).each(function () {
    var url = false, urlsource, pos1, pos2;
    try {
      urlsource = this.href;
      pos1 = urlsource.indexOf('?url=');
      if (pos1 > 0) {
        pos2 = urlsource.indexOf('&', pos1 + 1);
        url = decodeURIComponent(urlsource.substring(pos1 + 5, (pos2 == -1) ? urlsource.length : pos2));
      }
    }
    // IE7 throws errors often when dealing with irregular links, such as:
    // <a href="node/10"></a> Empty tags.
    // <a href="http://user:pass@example.com">example</a> User:pass syntax.
    catch (error) {
      return;
    }

    if (Drupal.settings.ext_link_page.extLinkClass && !$(this).find('img').length) {
      // Apply the "ext" class to all links not containing images.
      $(this).addClass(Drupal.settings.ext_link_page.extLinkClass);
      if ($(this).css('display') == 'inline') {
        $(this).after('<span class=' + Drupal.settings.ext_link_page.extLinkClass + '></span>');
      }
    }

    // Attach handler for clicks
    if (url) {
      $(this).click(function (e) {
        extLinkPageModal(url);
        e.preventDefault();
      });
    }
  });

  // Work around for Internet Explorer box model problems.
  if (($.support && $.support.boxModel !== undefined && !$.support.boxModel) || ($.browser.msie && parseInt($.browser.version, 10) <= 7)) {
    $('span.ext, span.mailto').css('display', 'inline-block');
  }
}

Drupal.behaviors.ext_link_page = {attach: extLinkPageAttach};

})(jQuery);
