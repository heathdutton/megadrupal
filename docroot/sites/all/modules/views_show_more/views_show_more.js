/**
 * @file
 * Handles the AJAX for the view_show_more plugin.
 */

(function ($) {

  // Provide a series of commands that the server can request the client perform.
  Drupal.ajax.prototype.commands.viewsShowMore = function (ajax, response, status) {
    // Get information from the response. If it is not there, default to
    // our presets.
    var wrapper = response.selector ? $(response.selector) : $(ajax.wrapper);
    var method = response.method || ajax.method;
    var append_at = response.append_at || '';
    var effect = ajax.getEffect(response);
    var pager_selector = response.options.pager_selector ? response.options.pager_selector : '.pager-show-more';

    // We don't know what response.data contains: it might be a string of text
    // without HTML, so don't rely on jQuery correctly iterpreting
    // $(response.data) as new HTML rather than a CSS selector. Also, if
    // response.data contains top-level text nodes, they get lost with either
    // $(response.data) or $('<div></div>').replaceWith(response.data).
    var new_content_wrapped = $('<div></div>').html(response.data);
    var new_content = new_content_wrapped.contents();

    // For legacy reasons, the effects processing code assumes that new_content
    // consists of a single top-level element. Also, it has not been
    // sufficiently tested whether attachBehaviors() can be successfully called
    // with a context object that includes top-level text nodes. However, to
    // give developers full control of the HTML appearing in the page, and to
    // enable Ajax content to be inserted in places where DIV elements are not
    // allowed (e.g., within TABLE, TR, and SPAN parents), we check if the new
    // content satisfies the requirement of a single top-level element, and
    // only use the container DIV created above when it doesn't. For more
    // information, please see http://drupal.org/node/736066.
    if (new_content.length != 1 || new_content.get(0).nodeType != 1) {
      new_content = new_content_wrapped;
    }
    // If removing content from the wrapper, detach behaviors first.
    var settings = response.settings || ajax.settings || Drupal.settings;
    Drupal.detachBehaviors(wrapper, settings);

    // Set up our default query options. This is for advance users that might
    // change there views layout classes. This allows them to write there own
    // jquery selector to replace the content with.
    // Provide sensible defaults for unordered list, ordered list and table
    // view styles.
    var content_query = append_at && !response.options.content_selector ? '> .view-content ' + append_at : response.options.content_selector || '> .view-content';

    // Immediately hide the new content if we're using any effects.
    if (effect.showEffect != 'show' && effect.showEffect !== 'scrollToggle') {
      new_content.find(content_query).children().hide();
    }

    // Scrolling effect.
    if(effect.showEffect === 'scroll_fadeToggle' || effect.showEffect === 'scrollToggle') {
      // Get old contenter height.
      var old_height = wrapper.find(content_query).addClass('clearfix').height();

      // Get content count.
      var old_items = wrapper.find(content_query).children().length;
      var new_items = new_content.find(content_query).children().length;

      // Calculate initial new height.
      var new_height = old_height + Math.ceil(old_height / old_items * new_items);

      // Set initial new height for scrolling.
      if(effect.showEffect === 'scroll_fadeToggle') {
        wrapper.find(content_query).height(new_height);
      }

      // Get offset top for scroll.
      var position_top = wrapper.find(content_query).offset().top + old_height - 50;

      // Finally Scroll.
      $('html, body').animate({ scrollTop: position_top}, effect.showSpeed);
    }

    // Update the pager
    // Find both for the wrapper as the newly loaded content the direct child
    // .item-list in case of nested pagers.
    wrapper.find(pager_selector).replaceWith(new_content.find(pager_selector));

    // Add the new content to the page.
    wrapper.find(content_query)[method](new_content.find(content_query).children());

    // Re-class the loaded content.
    wrapper.find(content_query).children()
      .removeClass('views-row-first views-row-last views-row-odd views-row-even')
      .filter(':first')
        .addClass('views-row-first')
        .end()
      .filter(':last')
        .addClass('views-row-last')
        .end()
      .filter(':even')
        .addClass('views-row-odd')
        .end()
      .filter(':odd')
        .addClass('views-row-even')
        .end();

    if (effect.showEffect != 'show' && effect.showEffect !== 'scrollToggle') {
      var delay_speed = (effect.showSpeed == 'slow') ? '600' : effect.showSpeed;
      delay_speed = (effect.showSpeed == 'fast') ? '200' : effect.showSpeed;
      if(effect.showEffect === 'scroll_fadeToggle') {
        effect.showEffect = 'fadeIn';
      }
      wrapper.find(content_query).children(':not(:visible)')[effect.showEffect](effect.showSpeed);
      wrapper.find(content_query).queue(function(next) {
        $(this).css('height', 'auto');
        next();
      });
    }

    // Additional processing over new content.
    wrapper.trigger('views_show_more.new_content', new_content.clone());

    // Attach all JavaScript behaviors to the new content
    // Remove the Jquery once Class, TODO: There needs to be a better
    // way of doing this, look at .removeOnce().
    var classes = wrapper.attr('class');
    var onceClass = classes.match(/jquery-once-[0-9]*-[a-z]*/);
    wrapper.removeClass(onceClass[0]);
    settings = response.settings || ajax.settings || Drupal.settings;
    Drupal.attachBehaviors(wrapper, settings);
  };
})(jQuery);
