(function($) {
if (typeof(Drupal) == "undefined" || !Drupal.tribune) {
  Drupal.tribune = {
    notification: false,
    notification_original_title: document.title
  };
}

$.expr[":"].textequals = function(obj, index, meta, stack) {
  return (obj.textContent || obj.innerText || $(obj).text() || "") == meta[3];
}

Drupal.behaviors.tribune = {
  attach: function(context) {
    $('.tribune-wrapper', context).each(function(index, element) {
      var tribune = $(this);

      var settings = Drupal.settings.tribune['tribune-' + tribune.data('nid').toString()];

      // Make it easier to get the tribune's settings.
      tribune.data('tribunesettings', settings);

      // Setup favicon or title notifications.
      Drupal.tribune.setupNotifications(tribune);

      // Setup a few writing aids.
      if (settings.buttons) {
        if (!Drupal.tribune.smallScreen()) {
          Drupal.tribune.attachWysiwygPreview(tribune);
        }

        Drupal.tribune.addWysiwygButtons(tribune);
      }

      // Change the button's label depending on textfield value.
      tribune.find('form.tribune-post-form .tribune-form-message')
        .bind('keydown keyup', (function(submit) {return function() {
          if ($(this).val()) {submit.val(Drupal.t('Post'));}
          else {submit.val(Drupal.t('Reload'));}
      };})(tribune.find('form.tribune-post-form .form-submit'))).keydown();

      // Attach handler for new posts.
      tribune.bind('tribunereloaded', (function(tribune) {return function(e, nodes) {
        Drupal.tribune.onNewPosts(tribune, nodes);
      };})(tribune));
      Drupal.tribune.onNewPosts(tribune, tribune.find('li.tribune-post'), true);

      // Attach handler for AJAX posting.
      tribune.find('form.tribune-post-form').submit(Drupal.tribune.postHandler);

      // Start reloading the tribune.
      if (undefined != $.fn.appear) {
        // If the jquery-appear library is loaded, use it.
        tribune.appear((function(tribune) {
          return function() {
            tribune.timer = setInterval(function() {Drupal.tribune.startReloading(tribune);}, settings.period);
          };
        })(tribune));
      } else {
        tribune.timer = setInterval(function() {Drupal.tribune.startReloading(tribune);}, settings.period);
      }

      // Cache the list of tribune clocks, to make highlighting faster.
      settings.clocks = tribune.find('li.tribune-post span.tribune-message span.tribune-post-clock');
    })

    // Should be enough for now, but I'll do it better once I have
    // fixed all the rest of the code to be a bit cleaner.
    $('.tribune-scrollback .tribune-posts', context)
      .attr('scrollTop', $('.tribune-scrollback .tribune-posts', context).attr('scrollHeight'));

    $('.tribune-scrollback .tribune-posts', context).scroll(function() {
      if ($(this).scrollTop() < 30) {
        // Load older posts if we are near the top of the list.
        var tribune = $(this).parent();
        var firstId = tribune.find('li:first').data('postid').toString();
        Drupal.prependHistory(tribune, firstId, 30);
      }
    });
  }
};

Drupal.tribune.settings = function(tribune, key, value) {
  var nid = tribune.data('nid').toString();
  if (undefined == Drupal.settings.tribune['tribune-' + nid]) {
    Drupal.settings.tribune['tribune-' + nid] = {};
  }

  if (undefined == key) {
    return Drupal.settings.tribune['tribune-' + nid];
  } else if (undefined == value) {
    return Drupal.settings.tribune['tribune-' + nid][key];
  } else {
    Drupal.settings.tribune['tribune-' + nid][key] = value;
    return Drupal.settings.tribune['tribune-' + nid][key];
  }
};

Drupal.tribune.setupNotifications = function(tribune) {
  var favicon = $('link[rel="shortcut icon"]');
  Drupal.tribune.settings(tribune, 'old_favicon', favicon.attr('href'));

  $(document).mousemove(function() {Drupal.tribune.resetNotification(tribune);});
  $(window).focus(function() {Drupal.tribune.resetNotification(tribune);});
};

Drupal.prependHistory = function(tribune, untilId, count) {
  tribune.addClass('tribune-loading-history');

  $.ajax({
    url: tribune.data('tribunesettings').historyurl,
    data: {count: count, until: untilId},
    dataType: 'json',
    cache: false,
    type: 'POST',
    success: function(data) {Drupal.tribune.prependPosts(tribune, data.posts)},
    complete: function() {tribune.removeClass('tribune-loading-history');},
  });
}

Drupal.tribune.prependPosts = function(tribune, posts) {
  var post_ids = [];

  for (post_id in posts) {
    if (post_id === 'length' || !posts.hasOwnProperty(post_id)) continue;
    post_ids.push(post_id);
  }

  // Sort them from latest to oldest.
  post_ids.sort(function(a, b) {
    return b - a;
  });

  var new_posts = $();
  var height = 0;

  for (i in post_ids) {
    var post = $(posts[post_ids[i]]);
    tribune.find('ol').prepend(post);
    height += post.outerHeight();
    new_posts = new_posts.add(post);
  }

  tribune.find('.tribune-posts')[0].scrollTop += height;

  tribune.trigger('tribunereloaded', [new_posts]);
}

Drupal.tribune.onNewPosts = function(tribune, posts, no_notifications) {
  posts.each(function() {
    Drupal.tribune.findClocks($(this));
    Drupal.tribune.findUsernames($(this));
    Drupal.tribune.attachClockHandlers(tribune, $(this));
    Drupal.tribune.attachUsernameHandlers(tribune, $(this));

    if (Drupal.tribune.settings(tribune, 'permissions') && Drupal.tribune.settings(tribune, 'permissions').mod) {
      Drupal.tribune.attachModHandlers(tribune, $(this));
    }
  });

  Drupal.tribune.updatePostClasses(tribune, posts);
  Drupal.tribune.settings(tribune, 'clocks', tribune.find('li.tribune-post span.tribune-message span.tribune-post-clock'));

  if (undefined == no_notifications) {
    Drupal.tribune.setNotification(tribune, posts);
  }
};

Drupal.tribune.smallScreen = function() {
  return (window.screen.width <= 640 || window.screen.height <= 640);
}

Drupal.tribune.attachWysiwygPreview = function(tribune) {
  var nid = tribune.data('nid');

  var input = tribune.find('div.form-item-message .tribune-form-message');
  var preview = $('<div class="tribune-message-preview tribune-post"></div>');
  preview.data('tribune', nid);
  preview.css('width', tribune.css('width'));
  tribune.prepend(preview);
  preview.hide();
  input.bind('keyup change focus mousedown', (function(preview, tribune) {
    return function() {
      var val = Drupal.tribune.getCurrentMessage(tribune);

      clearTimeout(Drupal.settings.tribune['tribune-' + nid].wysiwygTimeout);

      if (val.length > 0) {
        var message = Drupal.tribune.sanitize(val);
        preview.html(message);
        preview.slideDown(100);
        Drupal.settings.tribune['tribune-' + nid].wysiwygTimeout = setTimeout(
          (function(tribune) {
            return function() {
              tribune.find('.tribune-message-preview').slideUp(100);
            }
          })(tribune), 2000
        );
      } else {
        preview.html('');
        preview.slideUp(100);
      }
    };
  })(preview, tribune));

}

Drupal.tribune.addWysiwygButtons = function(tribune) {
  var buttons = $('<button class="tribune-wysiwyg" value="b" type="button" name="bold"><b>b</b></button><button class="tribune-wysiwyg" value="i" type="button" name="italic"><i>i</i></button><button class="tribune-wysiwyg" value="u" type="button" name="underline"><u>u</u></button>');
  var wrap = $('<div class="form-item"></div>');
  wrap.append(buttons);
  tribune.find('div.form-item-message').before(wrap);

  buttons.each(function() {
    $(this).click((function(tribune) {
      return function() {
        var tag = $(this).val();
        Drupal.tribune.insertTag(tribune, '<' + tag + '>', '</' + tag + '>');
        return false;
      };
    })(tribune));
  });
};

Drupal.tribune.setNotification = function(tribune, nodes) {
  nodes.each(function() {
    if (!$(this).hasClass("tribune-own-post")) {
      if ($(this).hasClass("tribune-answer")) {
        document.title = "# " + Drupal.tribune.notification_original_title;
        Drupal.tribune.notification = true;
        Drupal.tribune.setNotificationFavicon(tribune, Drupal.tribune.settings(tribune, 'favicons').answer);
        return;
      } else if (!Drupal.tribune.notification) {
        document.title = "* " + Drupal.tribune.notification_original_title;
        Drupal.tribune.notification = true;
        Drupal.tribune.setNotificationFavicon(tribune, Drupal.tribune.settings(tribune, 'favicons').new_post);
      }
    }
  });
}

Drupal.tribune.setNotificationFavicon = function(tribune, url) {
  // Browsers only take into account *new* favicons.

  if (Drupal.tribune.settings(tribune, 'favicon_notification')) {
    if (!url) {
      url = Drupal.tribune.settings(tribune, 'old_favicon');
    }

    var favicon = $('link[rel="shortcut icon"]');

    favicon.remove();
    favicon.attr('href', url);
    $('head').append(favicon);
  }
}

Drupal.tribune.resetNotification = function(tribune) {
  if (Drupal.tribune.notification) {
    document.title = Drupal.tribune.notification_original_title;
    Drupal.tribune.notification = false;
    Drupal.tribune.setNotificationFavicon(tribune, null);
  }
};

Drupal.tribune.getCurrentMessage = function(tribune) {
  var wysiwyg = tribune.find('span.tribune-form-message');
  if (wysiwyg.size() == 1) {
    return wysiwyg.get(0).innerHTML;
  } else {
    return tribune.find('.tribune-form-message').val();
  }
};

Drupal.tribune.setCurrentMessage = function(tribune, message) {
  var wysiwyg = tribune.find('span.tribune-form-message');
  if (wysiwyg.size() == 1) {
    wysiwyg.get(0).innerHTML = message;
  } else {
    tribune.find('.tribune-form-message').val(message);
  }
};

Drupal.tribune.postHandler = function() {
  var tribune = $(this).parents('.tribune-wrapper:first');
  var error = false;

  tribune.find('form.tribune-post-form .required').each(function() {
    if ($(this).val() == '') {
      $(this).addClass('error');
      error = true;
    } else {
      $(this).removeClass('error');
    }
  });

  if (error) {
    return false;
  }

  var last_id = 0;
  var last_message = tribune.find('li:last');
  if (last_message) {
	  var data = last_message.data('postid');
	  if (data) {
		  last_id = data.toString();
	  }
  }

  tribune.find('form.tribune-post-form .tribune-form-message').get(0).setAttribute('disabled', 'disabled');
  tribune.addClass('tribune-posting');
  var data = {
    message: Drupal.tribune.getCurrentMessage(tribune),
    last: last_id
  };
  tribune.trigger('beforepost', [data]);
  $.ajax({
    url: Drupal.tribune.settings(tribune, 'posturl'),
    data: data,
    dataType: 'json',
    cache: false,
    type: 'POST',
    success: function(data, textStatus, jqXHR) {Drupal.tribune.postSuccess(data, tribune)},
    error: function(jqXHR, textStatus, errorThrown) {Drupal.tribune.postError(tribune)},
    complete: function(jqXHR, textStatus) {Drupal.tribune.postComplete(tribune)},
  });
  return false;
};

Drupal.tribune.postSuccess = function(data, tribune) {
  Drupal.tribune.setCurrentMessage(tribune, '');
  tribune.removeClass('tribune-post-error');
  Drupal.tribune.reloadSuccess(data, tribune);
};

Drupal.tribune.postError = function(tribune) {
  tribune.addClass('tribune-post-error');
};

Drupal.tribune.postComplete = function(tribune) {
  tribune.find('form.tribune-post-form .tribune-form-message').get(0).removeAttribute('disabled');
  tribune.removeClass('tribune-posting');
};

Drupal.tribune.attachModHandlers = function(tribune, post) {
  var del = $('<a></a>').attr('class', 'tribune-delete-post').attr('title', Drupal.t('Delete this post')).attr('href', '#').text('✖');
  var post_id = post.data('postid').toString();
  del.click(function() {
    $.ajax({
      url: Drupal.tribune.settings(tribune, 'controlurl'),
      data: {'delete': post_id},
      cache: false,
      method: 'post',
      dataType: 'json',
      success: function(data, textStatus, jqXHR) {Drupal.tribune.postDelete(tribune, post_id)},
    });
    return false;
  });

  var ban = $('<a></a>').attr('class', 'tribune-ban-user').attr('title', Drupal.t('Ban this user')).attr('href', '#').text('#');
  ban.click(function() {
    $.ajax({
      url: Drupal.tribune.settings(tribune, 'controlurl'),
      data: {'ban': post_id},
      cache: false,
      method: 'post',
      dataType: 'json',
      success: (function(link) {return function(data, textStatus, jqXHR) {link.css('visibility', 'hidden');}})(ban),
    });
    return false;
  });

  var delban = $('<a></a>').attr('class', 'tribune-ban-delete-posts').attr('title', Drupal.t('Ban and delete all posts')).attr('href', '#').text('#!');
  delban.click(function() {
    if (window.confirm(Drupal.t('Are you sure you want to ban this user and delete all his or her posts?'))) {
      $.ajax({
        url: Drupal.tribune.settings(tribune, 'controlurl'),
        data: {'delete-ban': post_id},
        cache: false,
        method: 'post',
        dataType: 'json',
        success: function(data, textStatus, jqXHR) {Drupal.tribune.postDeleteBan(tribune, post_id)},
      });
    }
    return false;
  });

  post.find('.tribune-date').after($('<span class="tribune-mod-actions"></span>').append(ban).append(del).append(delban));
}

Drupal.tribune.postDelete = function(tribune, post_id) {
  tribune.find('li[data-postid=' + post_id + ']').slideUp();
};

Drupal.tribune.attachUsernameHandlers = function(tribune, post) {
  post.find('.tribune-message span.tribune-post-user').hover(
    function(e) {Drupal.tribune.postUsernameHoverIn(tribune, $(this));},
    function(e) {Drupal.tribune.unHighlightPosts(tribune);}
  );
  post.find('.tribune-user').click(function() {Drupal.tribune.usernameClick(tribune, $(this));});
};

Drupal.tribune.usernameClick = function(tribune, usertag) {
  var username = usertag.text();
  Drupal.tribune.insertText(tribune, username.replace(/ /, ' ') + '< ');
};

Drupal.tribune.attachClockHandlers = function(tribune, post) {
  post.find('.tribune-message span.tribune-post-clock').hover(
    function(e) {Drupal.tribune.postClockHoverIn(tribune, $(this));},
    function(e) {Drupal.tribune.unHighlightPosts(tribune);}
  );
  post.find('.tribune-message span.tribune-post-clock').click(function() {
    var reference = Drupal.tribune.findClockReference(tribune, $(this));

    if (undefined != reference && reference.size() > 0) {
      Drupal.tribune.scrollToClock(tribune, reference);
    }
  });
  post.find('.tribune-clock').hover(
    function(e) {Drupal.tribune.highlightPost(tribune, $(this).parent(), true);},
    function(e) {Drupal.tribune.unHighlightPosts(tribune);}
  );
  post.find('.tribune-clock').click(function() {Drupal.tribune.clockClick(tribune, $(this));});
};

Drupal.tribune.scrollToClock = function(tribune, clock) {
  var position = clock.position().top;
  $('html, body').animate({
    scrollTop: position
  }, 200);
};

Drupal.tribune.clockClick = function(tribune, clock) {
  var post = clock.parents('.tribune-post:first');
  var nid = post.data('tribune').toString();
  var same_time = post.siblings('[data-tribune=' + nid + '][data-timestamp=' + post.data('timestamp').toString() + ']').andSelf();
  if (same_time.size() > 1) {
    var index = same_time.index(post) + 1;
    if (index < 10) {
      var map = {1: '¹', 2: '²', 3: '³', 4: '⁴', 5: '⁵', 6: '⁶', 7: '⁷', 8: '⁸', 9: '⁹'};
      var suffix = map[index];
    } else {
      var suffix = ':' + index;
    }
  } else {
    var suffix = '';
  }
  Drupal.tribune.insertText(tribune, clock.text() + suffix + ' ');
};

Drupal.tribune.insertText = function(tribune, text) {
  var input = tribune.find('form.tribune-post-form .tribune-form-message');
  var range = Drupal.tribune.getSelectionRange(input.get(0));
  var originalText = Drupal.tribune.getCurrentMessage(tribune);
  input.get(0).focus();
  Drupal.tribune.setCurrentMessage(tribune, originalText.substring(0, range[0]) + text + originalText.substring(range[1], originalText.length));
  Drupal.tribune.setSelectionRange(input.get(0), range[0] + text.length, range[0] + text.length);
  input.change();
};

Drupal.tribune.insertTag = function(tribune, open, close) {
  var input = tribune.find('form.tribune-post-form .tribune-form-message');
  var range = Drupal.tribune.getSelectionRange(input.get(0));
  var originalText = Drupal.tribune.getCurrentMessage(tribune);
  input.get(0).focus();
  Drupal.tribune.setCurrentMessage(tribune, originalText.substring(0, range[0]) + open + originalText.substring(range[0], range[1]) + close + originalText.substring(range[1], originalText.length));
  Drupal.tribune.setSelectionRange(input.get(0), range[1] + open.length, range[1] + open.length);
  input.change();
};

Drupal.tribune.getSelectionRange = function(field) {
  if (field.setSelectionRange) {
    return [field.selectionStart, field.selectionEnd];
  } else if (field.createTextRange) {
    var range = document.selection.createRange();

    if (range.parentElement() == field) {
      var range2 = field.createTextRange();
      range2.collapse(true);
      range2.setEndPoint('EndToEnd', range);
      return [range2.text.length - range.text.length, range2.text.length];
    }
  }

  return [field.value.length, field.value.length];
}

Drupal.tribune.setSelectionRange = function(field, start, end) {
  if (field.setSelectionRange) {
    field.setSelectionRange(start, end);
  } else if (field.createTextRange) {
    var range = field.createTextRange();
    range.collapse(true);
    range.moveStart('character', start);
    range.moveEnd('character', end - start);
    range.select();
  }
}

Drupal.tribune.getReferencesTo = function(tribune, post) {
  var nid = post.data('tribune').toString();
  var timestamp_date = post.data('timestamp').toString();
  var same_time = post.siblings('[data-tribune=' + nid + '][data-timestamp=' + timestamp_date + ']').andSelf();
  if (same_time.size() > 1) {
    var index = same_time.index(post) + 1;
  } else {
    var index = 0;
  }

  var timestamp_full = timestamp_date.substr(-6, 6);
  var timestamp_short = timestamp_date.substr(-6, 4);
  var clocks = tribune.find('li.tribune-post[data-tribune=' + nid + '] span.tribune-message span.tribune-post-clock');
  var referencing = clocks.filter('[data-timestamp=' + timestamp_full + ']');
  referencing = referencing.add(clocks.filter('[data-timestamp=' + timestamp_short + ']'));
  referencing = referencing.add(clocks.filter('[data-timestamp=' + timestamp_date + ']'));

  if (index > 0) {
    var timestamp_with_index = timestamp_date.substr(-6, 6) + ':' + index;
    var timestamp_with_index_and_date = post.data('timestamp') + ':' + index;
    referencing = referencing.add(clocks.filter('[data-timestamp=' + timestamp_with_index + ']'));
    referencing = referencing.add(clocks.filter('[data-timestamp=' + timestamp_with_index_and_date + ']'));

    if (index < 10) {
      var map = {1: '¹', 2: '²', 3: '³', 4: '⁴', 5: '⁵', 6: '⁶', 7: '⁷', 8: '⁸', 9: '⁹'};
      var timestamp_with_superscript = timestamp_date.substr(-6, 6) + map[index];
      var timestamp_with_superscript_and_date = post.data('timestamp') + map[index];
      referencing = referencing.add(clocks.filter('[data-timestamp=' + timestamp_with_superscript + ']', tribune));
      referencing = referencing.add(clocks.filter('[data-timestamp=' + timestamp_with_superscript_and_date + ']', tribune));
    }
  }

  return referencing;
};

Drupal.tribune.highlightPost = function(tribune, post, hide_reference, hide_highlight, referencing) {
  if (!hide_highlight) {
    post.addClass('tribune-highlighted');
  }

  if (undefined == referencing) {
    referencing = Drupal.tribune.getReferencesTo(tribune, post);
  }

  if (!hide_highlight) {
    referencing.addClass('tribune-highlighted');
  }

  var list = post.parent().parent();

  referencing.each(function() {
    var clock = post.find('.tribune-clock');
    var half_height = clock.height() / 2;

    var left = post.position().left - 20;
    var from = post.position().top + half_height;
    var height = $(this).parent().position().top - from + half_height - 4;
    var segment = document.createElement('div');
    segment.setAttribute('class', 'tribune-answer-segment');
    segment.style.top = from + 'px';
    segment.style.left = left + 'px';
    segment.style.height = height + 'px';
    list.prepend(segment);
  });

  if (!hide_reference) {
    Drupal.tribune.showReferencePost(tribune, post);
  }
};

Drupal.tribune.unHighlightPosts = function(tribune) {
  tribune.find('.tribune-highlighted').removeClass('tribune-highlighted');
  $('.tribune-answer-segment').remove();
  Drupal.tribune.hideReferencePost(tribune);
};

Drupal.tribune.postUsernameHoverIn = function(tribune, username) {
  username.addClass('tribune-highlighted');

  setTimeout((function(tribune, username) {
    return function() {
      if (username.hasClass('tribune-highlighted')) {
        var username_text = username.data('username').toString().replace(/ /, ' ');
        var reference = tribune.find('li.tribune-post[data-username=' + username_text + ']');
        reference = reference.add(tribune.find('li.tribune-post[data-user=0] span.tribune-user:textequals("' + username_text + '")').parent());
        if (reference.size()) {
          Drupal.tribune.highlightPost(tribune, reference, true);
        }
      }
    }
  })(tribune, username), 50);
};

Drupal.tribune.findClockReference = function(tribune, clock) {
  var nid = clock.parents('li:first').data('tribune').toString();
  var post_id = clock.parents('li:first').data('postid').toString();
  var timestamp = clock.data('timestamp').toString();
  if (timestamp.length == 4) {
    var reference = tribune.find('li[data-postid!=' + post_id + '][data-tribune=' + nid + '].tribune-post[data-timestamp*=' + timestamp + ']');
  } else if (timestamp.length == 6 || timestamp.length == 14) {
    var reference = tribune.find('li[data-postid!=' + post_id + '][data-tribune=' + nid + '].tribune-post[data-timestamp$=' + timestamp + ']');
  } else if (timestamp.length == 7 || timestamp.length == 15) {
    var timestamp_full = timestamp.substr(0, 6);
    var index = timestamp.substr(6, 1);
    var map = {'¹': 1, '²': 2, '³': 3, '⁴': 4, '⁵': 5, '⁶': 6, '⁷': 7, '⁸': 8, '⁹': 9};
    var reference = tribune.find('li[data-postid!=' + post_id + '][data-tribune=' + nid + '].tribune-post[data-timestamp$=' + timestamp_full + ']');
    if (undefined != map[index]) {
      index = map[index] - 1;
      reference = reference.eq(index);
    }
  } else if ((timestamp.length > 7 && timestamp.length < 14) || (timestamp.length > 15)) {
    var timestamp_full = timestamp.substr(0, 6);
    var index = timestamp.substr(7) - 1;
    var reference = tribune.find('li[data-postid!=' + post_id + '][data-tribune=' + nid + '].tribune-post[data-timestamp$=' + timestamp_full + ']');
    reference = reference.eq(index);
  }

  return reference;
}

Drupal.tribune.postClockHoverIn = function(tribune, clock, preview, highlight) {
  if (undefined == highlight || highlight == true) {
    clock.addClass('tribune-highlighted');
  }

  var reference = Drupal.tribune.findClockReference(tribune, clock);

  if (undefined == preview) {
    preview = true;
  }

  if (undefined == highlight) {
    highlight = true;
  }

  if (undefined == reference || reference.size() == 0) {
    Drupal.tribune.showPostAjax(tribune, clock);
  } else {
    Drupal.tribune.highlightPost(tribune, reference, !preview, !highlight, preview ? undefined : clock);

    reference.find('.tribune-post-clock').each(function() {
      Drupal.tribune.postClockHoverIn(tribune, $(this), false, false);
    });
  }
};

Drupal.tribune.showPostAjax = function(tribune, clock) {
  var post = clock.parents('li.tribune-post:first');

  $.ajax({
    url: Drupal.tribune.settings(tribune, 'referencesearchurl'),
    data: {
      timestamp: clock.data('timestamp').toString(),
      from: post.data('postid').toString(),
      tribune: post.data('tribune').toString()
    },
    cache: false,
    method: 'post',
    dataType: 'json',
    success: (function(tribune, clock) {
      return function(data, textStatus, jqXHR) {
        if (clock.hasClass('tribune-highlighted')) {
          for (i in data.posts) {
            var post = $(data.posts[i]);
            Drupal.tribune.showReferencePost(tribune, post);
            var references = Drupal.tribune.getReferencesTo(tribune, post);
            references.addClass('tribune-highlighted');
            break;
          }
        }
      }
    })(tribune, clock)
  });
};

Drupal.tribune.showReferencePost = function(tribune, post) {
  var wrapper = $('<div></div>');
  var attributes = post.get(0).attributes;
  if (attributes != undefined) {
    var i;
    for (i = 0 ; i < attributes.length ; i++) {
      wrapper.attr(attributes[i].name, attributes[i].value);
    }
  }
  wrapper.addClass('tribune-post-preview');
  wrapper.addClass('tribune-post');
  wrapper.removeClass('tribune-highlighted');
  wrapper.css('width', tribune.css('width'));
  wrapper.html(post.html());
  wrapper.hide();
  tribune.prepend(wrapper);
  wrapper.slideDown(100);
};

Drupal.tribune.hideReferencePost = function(tribune) {
  tribune.find('.tribune-post-preview').slideUp(100, function() {$(this).remove();});
};

Drupal.tribune.findUsernames = function(post) {
  var text = post.find('.tribune-message').html().replace(/(([^ ]+)&lt;)/g, "<span class='tribune-post-user' data-username='\$2'>\$1</span>");
  post.find('.tribune-message').html(text);
};

Drupal.tribune.findClocks = function(post) {
  var text = post
    .find('.tribune-message')
    .html()
    .replace(
    //  |--------------------------------$1------------------------------|
    //  ||-----------------------------$2------------------------------| | |---------------------------------------------$11---------------------------------------------|
    //  ||           |--------$4-------| |--------------$7------------|| | ||---------$12---------|             |-----$16-----|                                          |
    //  |||---$3---| ||--$5--| |--$6--|| ||--$8--| |----$9---| |-$10-||| | |||---$13----| |-$14--|| |----$15---|| |---$17----|| |---------$18----------| |-----$19-----| |
       /((([0-9]{4})-((0[1-9])|(1[0-2]))-((0[1-9])|([12][0-9])|(3[01])))#)?((([01]?[0-9])|(2[0-3])):([0-5][0-9])(:([0-5][0-9]))?([:\^][0-9]|[¹²³⁴⁵⁶⁷⁸⁹])?(@[0-9A-Za-z]+)?)/g
     , "<span class='tribune-post-clock' data-timestamp='$3$4$7$12$15$17$18'>\$1\$11</span>"
    );
  post.find('.tribune-message').html(text);
  post.find('.tribune-post-clock').each(function() {
    var clock = $(this).html();
    if (clock) {
      $(this).html(clock.replace(/#/, '<span class="tribune-clock-sign">#</span>'));
    }
  });
};

Drupal.tribune.startReloading = function(tribune, callback) {
  var e = jQuery.Event('tribunereloading');
  tribune.trigger(e);
  if (e.isDefaultPrevented()) {
    return;
  }

  clearInterval(tribune.timer);
  tribune.addClass('tribune-reloading');
  tribune.find('form.tribune-post-form .tribune-form-message').addClass('form-autocomplete').addClass('throbbing');

  var request = {
    url: tribune.data('tribunesettings').staticurl,
    data: {last: tribune.find('li:last').data('postid').toString()},
    dataType: 'json',
    success: function(data) {Drupal.tribune.reloadSuccess(data, tribune); if (undefined != callback) {callback(true);}},
    error: function() {Drupal.tribune.reloadError(tribune); if (undefined != callback) {callback(false);}},
    complete: function() {Drupal.tribune.reloadComplete(tribune);},
  }
  if (undefined == request.url) {
    request.url = tribune.data('tribunesettings').reloadurl;
    request.cache = false;
  }

  $.ajax(request);
};

Drupal.tribune.reloadSuccess = function(data, tribune) {
  tribune.removeClass('tribune-reload-error');

  if (data) {
    if (data.moderated) {
      Drupal.tribune.removeModeratedPosts(tribune, data.moderated);
    }

    if (data.posts) {
      Drupal.tribune.addNewPosts(tribune, data.posts);
    }
  }
}

Drupal.tribune.removeModeratedPosts = function(tribune, moderated) {
  for (post_id in moderated) {
    tribune.find('ol li[value=' + moderated[post_id] + ']').each(function() {
      $(this).slideUp(function() {$(this).remove()});
    });
  }
};

Drupal.tribune.addNewPosts = function(tribune, posts) {
  var last_id = 0;
  var last_message = tribune.find('li:last');
  if (last_message) {
	  var data = last_message.data('postid');
	  if (data) {
		  last_id = data.toString();
	  }
  }

  // When a tribune has no post, this will of course return
  // an undefined value, hence the following conditional check.
  if (!last_id) {
    last_id = 0;
  }

  var new_posts = $();

  for (post_id in posts) {
    if (post_id > last_id) {
      post = $(posts[post_id]);

      tribune.find('ol').append(post);

      if (!tribune.data('tribunesettings').scrollback &&
          tribune.find('li.tribune-post').size() > Drupal.tribune.settings(tribune, 'count')) {
        tribune.find('li.tribune-post:first').remove();
      }

      new_posts = new_posts.add(post);
    }
  }

  tribune.trigger('tribunereloaded', [new_posts]);

  // Should be enough for now, but I'll do it better once I have
  // fixed all the rest of the code to be a bit cleaner.
  tribune.find('.tribune-posts')
    .attr('scrollTop', tribune.find('.tribune-posts').attr('scrollHeight'));

  Drupal.attachBehaviors(new_posts);
};

Drupal.tribune.updatePostClasses = function(tribune, posts) {
  if (Drupal.tribune.settings(tribune, 'uid') != 0) {
    $('li[data-user=' + Drupal.tribune.settings(tribune, 'uid') + ']', tribune).each(function() {
      $(this).addClass('tribune-own-post');
      var referencing = Drupal.tribune.getReferencesTo(tribune, $(this));
      referencing.each(function() {
        $(this).parents('li:first').addClass('tribune-answer');
      });
    });
  }
  if (Drupal.tribune.settings(tribune, 'username')) {
    $('li[data-username=' + Drupal.tribune.settings(tribune, 'username') + ']', tribune).each(function() {
      $(this).addClass('tribune-own-post');
      var referencing = Drupal.tribune.getReferencesTo(tribune, $(this));
      referencing.each(function() {
        $(this).parents('li:first').addClass('tribune-answer');
      });
    });
  }
};

Drupal.tribune.reloadError = function(tribune) {
  tribune.addClass('tribune-reload-error');
};

Drupal.tribune.reloadComplete = function(tribune) {
  tribune.removeClass('tribune-reloading');
  tribune.find('form.tribune-post-form .tribune-form-message').removeClass('form-autocomplete').removeClass('throbbing');
  tribune.timer = setInterval(function() {
    Drupal.tribune.startReloading(tribune);
  }, tribune.data('tribunesettings').period);
};

Drupal.tribune.sanitize = function(message) {
  message = message.replace(/<(m|s|u|b|i|tt|code)>(.*?)<\/\1>/g, Drupal.tribune.sanitizeCallback);

  message = message.replace(/&/g, '&amp;');
  message = message.replace(/</g, '&lt;');
  message = message.replace(/>/g, '&gt;');

  message = message.replace(/\032/g, '<');
  message = message.replace(/\033/g, '>');

  message = message.replace(/((https?|ftp|gopher|file|mms|rtsp|rtmp):\/\/.*?)((,|\.|\)|\]|\})?(<| | |"|$))/,
    function(match, url, protocol, cruft, punctuation, after) {
      var string = '<a href="' + url + '">[url]</a>';
      if (undefined != punctuation) {
        string += punctuation;
      }
      if (undefined != after) {
        string += after;
      }
	  return string;
    }
  );

  return message;
};

Drupal.tribune.sanitizeCallback = function(match, tag, text) {
  text = text.replace(/<(m|s|u|b|i|tt|code)>(.*?)<\/\1>/g, Drupal.tribune.sanitizeCallback);

  return '\032' + tag + '\033' + text + '\032/' + tag + '\033';
};

})(jQuery);
