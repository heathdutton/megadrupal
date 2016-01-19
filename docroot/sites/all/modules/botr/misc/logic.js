(function ($) {

botr = {
  // Perform search call after user has stopped typing for this many milliseconds.
  search_timeout : 1000,

  // Poll server every given number of milliseconds for upload progress info.
  upload_poll_interval : 2000,

  // The chunk size for resumable uploads.
  upload_chunk_size : 2 * 1024 * 1024,

  // Poll API every given number of milliseconds for thumbnail status.
  thumb_poll_interval : 5000,

  // Width of video thumbnails.
  thumb_width : 40,

  // Timers.
  search_timer_id : null,
  thumb_timer_id : null,

  // Apparently, there's no built-in javascript method to escape html entities.
  html_escape : function (str) {
    return $('<div/>').text(str).html();
  },

  // Test if a string starts with a given prefix.
  starts_with : function (str, prefix) {
    return str.substr(0, prefix.length) === prefix;
  },

  // Strip a given prefix from a string.
  lstrip : function (str, prefix) {
    if (botr.starts_with(str, prefix)) {
      return str.substr(prefix.length);
    }
    else {
      return str;
    }
  },

  // Simple function for building html tags.
  tag : function (name, content) {
    return '<' + name + '>' + botr.html_escape(content) + '</' + name + '>';
  },

  // Construct a thumbnail url for a given video.
  make_thumb_url : function (video_hash, width) {
    if (width === undefined) {
      width = botr.thumb_width;
    }
    return 'http://' + Drupal.settings.botr.content_mask + '/thumbs/' + video_hash + '-' + width + '.jpg';
  },

  // Insert a string into the post editor at the current position of the cursor.
  insert_editor_content : function insertAtCaret(text) {
    // If the target is a textarea with summary, don't use the summary
    // If the target is the textarea itself, use it
    var target = $('.botr_widget_target textarea.text_full, textarea.botr_widget_target');
    if (target.length == 0) {
      // If there is no summary, just use the last textarea in the target
      var target = $('.botr_widget_target textarea');
    }
    if (target.length == 0) {
      // If all textareas fail, let's try an input field
      var target = $('.botr_widget_target input:text, input.botr_widget_target:text');
    }
    if (target.length > 0)
    {
      var txtarea = target.last().get(0);
      var scrollPos = txtarea.scrollTop;
      var strPos = 0;
      var br = ((txtarea.selectionStart || txtarea.selectionStart == '0') ? "ff" : (document.selection ? "ie" : false));

      if (br == "ie") {
        txtarea.focus();
        var range = document.selection.createRange();
        range.moveStart('character', -txtarea.value.length);
        strPos = range.text.length;
      }
      else if (br == "ff") {
        strPos = txtarea.selectionStart;
      }

      var front = (txtarea.value).substring(0, strPos);
      var back = (txtarea.value).substring(strPos, txtarea.value.length);

      txtarea.value = front + text + back;
      strPos = strPos + text.length;

      if (br == "ie") {
        txtarea.focus();
        var range = document.selection.createRange();
        range.moveStart('character', -txtarea.value.length);
        range.moveStart('character', strPos);
        range.moveEnd('character', 0);
        range.select();
      }
      else if (br == "ff") {
        txtarea.selectionStart = strPos;
        txtarea.selectionEnd = strPos;
        txtarea.focus();
      }

      txtarea.scrollTop = scrollPos;
    }
  },

  // Insert the quicktag into the editor box.
  insert_quicktag : function (video_hash) {
    var quicktag = '[bitsontherun ' + video_hash + ']';
    botr.insert_editor_content(quicktag);
    return false;
  },

  /* Make a list item for a video.
   * The `video` parameter must be a dict as returned by the /videos/list call.
   */
  make_video_list_item : function (video) {
    var thumb_url, js, make_quicktag;
    var css_class = botr.widgets.list.children().length % 2 ? 'botr-odd' : 'botr-even';
    if (video.status == 'ready') {
      thumb_url = botr.make_thumb_url(video.key);
      make_quicktag = function(video_key) {
        return function() {
          botr.insert_quicktag(video_key);
        }
      }(video.key);
    }
    else if (video.status == 'processing') {
      thumb_url = Drupal.settings.botr.plugin_url + '/misc/processing-' + botr.thumb_width + '.gif';
      make_quicktag = function(video_key) {
        return function() {
          botr.insert_quicktag(video_key);
        }
      }(video.key);
      css_class += ' botr-processing';
    }
    else if (video.status == 'failed') {
      thumb_url = Drupal.settings.botr.plugin_url + '/misc/video-error-' + botr.thumb_width + '.gif';
      make_quicktag = null;
      css_class += ' botr-failed';
    }

    // Create the list item
    var elt = $('<li>').attr('id', 'botr-video-' + video.key);
    elt.addClass(css_class);
    elt.css('background-image', 'url(' + thumb_url + ')');
    if (make_quicktag) {
      // If we can embed, add the functionality to the item
      elt.click(make_quicktag);
    }
    $('<p>').text(video.title).appendTo(elt);

    return elt;
  },

  make_channel_list_item : function (channel) {
    var thumb_url, js, make_quicktag;
    var css_class = botr.widgets.list.children().length % 2 ? 'botr-odd' : 'botr-even';
    thumb_url = Drupal.settings.botr.plugin_url + '/misc/channel-' + botr.thumb_width + '.png';
    make_quicktag = function(video_key) {
      return function() {
        botr.insert_quicktag(video_key);
      }
    }(channel.key);

    // Create the list item
    var elt = $('<li>').attr('id', 'botr-channel-' + channel.key);
    elt.addClass(css_class);
    elt.css('background-image', 'url(' + thumb_url + ')');
    if (make_quicktag) {
      // If we can embed, add the functionality to the item
      elt.click(make_quicktag);
    }
    $('<p>').text(channel.title + ' ').append($('<em>').text(Drupal.t('(playlist)'))).appendTo(elt);

    return elt;
  },

  show_wait_cursor : function () {
    botr.widgets.box.addClass('botr-busy');
  },

  show_normal_cursor : function () {
    botr.widgets.box.removeClass('botr-busy');
  },

  /* List the most recently uploaded videos. If query is supplied, we will only show
   * those that match the given string.
   */
  list_videos : function (query, nr_videos, callback) {
    botr.show_wait_cursor();

    if (query === undefined || query == 'Search videos') {
      query = '';
    }

    if (nr_videos === undefined) {
      nr_videos = Drupal.settings.botr.nr_videos;
    }

    var params = {
      method : '/videos/list',
      result_limit : nr_videos,
      order_by : 'date:desc',
      random : Math.random()
    }

    if (query != '') {
      params['text'] = query;
    }

    $.ajax({
      type : 'GET',
      url : botr.api_proxy,
      data : params,
      dataType : 'json',
      success : function (data) {
        if (data && data.status == 'ok') {
          if (data.videos.length) {
            for (var i = 0; i < data.videos.length; i += 1) {
              var elt = botr.make_video_list_item(data.videos[i]);
              botr.widgets.list.append(elt);
            }

            if (botr.thumb_timer_id == null) {
              botr.thumb_timer_id = window.setInterval(botr.poll_thumb_progress, botr.thumb_poll_interval);
            }
          }

          if (callback !== undefined) {
            callback(data.videos.length);
          }
        }
        else {
          var msg = data ? 'API error: ' + data.message : 'No response from API.';
          botr.widgets.list.html(botr.tag('p', msg));
        }

        botr.show_normal_cursor();
      },
      error : function (request, message, error) {
        botr.widgets.list.html(botr.tag('p', 'AJAX error: ' + message));
        botr.show_normal_cursor();
      }
    });
  },

  list_channels : function (query, nr_videos, callback) {
    botr.show_wait_cursor();

    if (query === undefined || query == 'Search videos') {
      query = '';
    }

    if (nr_videos === undefined) {
      nr_videos = Drupal.settings.botr.nr_videos;
    }

    var params = {
      method : '/channels/list',
      result_limit : nr_videos,
      random : Math.random()
    }

    if (query != '') {
      params['text'] = query;
    }

    $.ajax({
      type : 'GET',
      url : botr.api_proxy,
      data : params,
      dataType : 'json',
      success : function (data) {
        if (data && data.status == 'ok') {
          if (data.channels.length) {
            for (var i = 0; i < data.channels.length; i += 1) {
              var elt = botr.make_channel_list_item(data.channels[i]);
              botr.widgets.list.append(elt);
            }
          }

          if (callback !== undefined) {
            callback(data.channels.length);
          }
        }
        else {
          var msg = data ? 'API error: ' + data.message : 'No response from API.';
          botr.widgets.list.html(botr.tag('p', msg));
        }

        botr.show_normal_cursor();
      },
      error : function (request, message, error) {
        botr.widgets.list.html(botr.tag('p', 'AJAX error: ' + message));
        botr.show_normal_cursor();
      }
    });
  },

  list: function(query, channels, videos, nr_videos) {
    if (query === undefined) {
      query = $.trim(botr.widgets.search.val());
    }
    if (nr_videos === undefined) {
      nr_videos = Drupal.settings.botr.nr_videos;
    }
    if (channels === undefined) {
      channels = true;
    }
    if (videos === undefined) {
      videos = true;
    }
    // Handle the "playlist:" syntax
    var m;
    if (m = query.match(/(playlist|channel):(.*)/)) {
      videos = false;
      channels = true;
      query = m[2];
    }

    botr.widgets.list.empty();

    var doDescribeEmpty = function()
    {
      if (botr.widgets.list.children().length == 0)
      {
        if (channels && videos) {
          botr.widgets.list.html('No playlists or videos have been found.');
        } else if (channels) {
          botr.widgets.list.html('No playlists have been found.');
        } else if (videos) {
          botr.widgets.list.html('No videos have been found.');
        } else {
          botr.widgets.list.html('Please search for videos or playlists.');
        }
      }
    };
    var doChannels = function(num) {
      if (num < nr_videos) {
        botr.list_channels(query, nr_videos - num, doDescribeEmpty);
      }
    };
    var doVideos = function(num) {
      if (num < nr_videos) {
        botr.list_videos(query, nr_videos - num, doChannels);
      }
    };
    if (videos) {
      doVideos(0);
    } else if (channels) {
      doChannels(0);
    } else {
      doDescribeEmpty();
    }
  },

  // Poll API for status of thumbnails.
  poll_thumb_progress : function () {
    var processing = botr.widgets.list.children('li.botr-processing');

    if (processing.length) {
      processing.each(function () {
        var item = $(this);
        var video_key = botr.lstrip(item.attr('id'), 'botr-video-');

        $.ajax({
          type : 'GET',
          url : botr.api_proxy,
          data : {
            method : '/videos/thumbnails/show',
            video_key : video_key
          },
          dataType : 'json',
          success : function (data) {
            if (data && data.status == 'ok') {
              switch (data.thumbnail.status) {
                case 'ready':
                  var thumb_url = botr.make_thumb_url(video_key);
                  break;

                case 'failed':
                  var thumb_url = Drupal.settings.botr.plugin_url + '/misc/thumb-error-' + botr.thumb_width + '.gif';
                  break;

                case 'not build':
                case 'processing':
                default:
                  // Don't update thumb.
                  var thumb_url = null;
                  break;
              }

              if (thumb_url) {
                item.removeClass('botr-processing');
                item.css('background-image', 'url(' + thumb_url + ')');
              }
            }
          },
          error : function () {}
        });
      });
    }
    else {
      window.clearTimeout(botr.thumb_timer_id);
      botr.thumb_timer_id = null;
    }
  },

  // Open a small window for file uploads
  open_upload_window : function() {
    var win = $('<div>')
      .addClass('botr-upload-window')
      .appendTo('body')
      .html(
        '<form action="" method="post" enctype="multipart/form-data">\
           <fieldset>\
             <legend>\
               <span class="fieldset-legend">' + Drupal.t('Bits on the Run Video Upload') + '</span>\
             </legend>\
             <p>\
               <label>' + Drupal.t('Title (optional)') + '</label>\
               <input type="text" class="botr-upload-title" name="title">\
             </p>\
             <p>\
               <label>' + Drupal.t('Video file') + '</label>\
               <input type="file" class="botr-upload-file" name="file">\
             </p>\
             <input type="submit" class="botr-upload-submit form-submit" disabled="disabled" value="' + Drupal.t('Upload') + '">\
             <div class="botr-message"></div>\
             <div class="botr-progress-bar">\
               <div class="botr-progress"></div>\
             </div>\
           </fieldset>\
           <div class="botr-close">X</div>\
         </form>');
    win.find('form')
      .submit(function(e) {
        botr.upload_video(win);
        return false;
      })
      .find('.botr-upload-file').change(function() {
        $(this).parents(':eq(1)').find('.botr-upload-submit').removeAttr('disabled');
      });
    win.draggable({handle: '.fieldset-legend'});
    // Close button behavior
    win.find('.botr-close').click(function() {
      var upload = win.data('upload');
      if(upload) {
        upload.cancel();
        if(!upload.isResumable()) {
          $(upload.getIframe()).remove();
        }
      }
      win.remove();
    });
    return false;
  },

  // Reset upload timer and widgets.
  reset_upload : function (win) {
    win.find('.botr-upload-title').val('').removeAttr('disabled');
    win.find('.botr-upload-file').val('').removeAttr('disabled');
    win.find('.botr-upload-submit').show();
    win.find('.botr-pause').remove();

    win.removeClass('botr-busy');
  },

  // Upload a new video. First, we do a /videos/create call, then we start uploading.
  upload_video : function (win) {
    var title = $(win.find('input').get(0));
    win.addClass('botr-busy');

    if(!$.browser.msie) {
      // IE will not submit the form if even one property of the file input has changed.
      win.find('input').attr('disabled', 'disabled');
    }
    else {
      win.find('input[type!="file"]').attr('disabled', 'disabled');
    }

    win.find('.botr-message').text("");

    var data = {
      method : '/videos/create',
      // IE tends to cache too much
      random : Math.random()
    };
    if (BotrUpload.resumeSupported()) {
      data.resumable = 'true';
    }
    title = $.trim(title.val());

    if (title != '') {
      data.title = title;
    }

    $.ajax({
      type : 'GET',
      url : botr.api_proxy,
      data : data,
      dataType : 'json',
      success : function (data) {
        if (data && data.status == 'ok') {
          var upload = new BotrUpload(data.link, data.session_id);
          win.data('upload', upload);
          upload.useForm(win.find('.botr-upload-file').get(0));
          win.append(upload.getIframe());
          upload.pollInterval = botr.upload_poll_interval;
          upload.chunkSize = botr.upload_chunk_size;
          upload.onProgress = function(bytes, total) {
            var ratio = bytes / total;
            var pct = Math.round(ratio * 1000) / 10;
            var txt = "Uploading: " + pct + "%";
            if(!upload._running) {
              txt += " (paused)";
            }
            win.find('.botr-message').text(txt);
            var progress = win.find('.botr-progress');
            progress.stop().animate({'width': (progress.parent().width() * ratio)}, 400);
          }
          upload.onError = function(msg) {
            win.find('.botr-message').text('Upload failed: ' + msg);
            botr.reset_upload(win);
          }
          upload.onCompleted = function() {
            win.remove();
            botr.list();
          }
          win.find('.botr-message').text(Drupal.t('Uploading...'));
          win.find('.botr-progress-bar').show();

          // Add the pause / resume button
          if (data.session_id) {
            var pause = $('<input>').attr('type', 'button').addClass('botr-pause form-submit').val(Drupal.t('Pause'));
            pause.click(function() {
              if(!upload._completed) {
                if(upload._running) {
                  upload.pause();
                  win.removeClass('botr-busy');
                  pause.val(Drupal.t('Resume'));
                }
                else {
                  upload.start();
                  win.addClass('botr-busy');
                  pause.val(Drupal.t('Pause'));
                }
              }
              return false;
            });
            win.find('.botr-upload-submit').hide().after(pause);
          }

          setTimeout(function() { upload.start() }, 0);
        }
        else {
          var msg = data ? 'API error: ' + data.message : 'No response from API.';
          win.find('.botr-message').text(msg);
          botr.reset_upload(win);
        }
      },
      error : function (request, message, error) {
        win.find('.botr-message').text("AJAX error: " + message);
        botr.reset_upload(win);
      }
    });
    return false;
  }
};

$(function() {
  botr.api_proxy = Drupal.settings.botr.plugin_url + '/proxy.php';

  botr.widgets = {
    box : $('#botr-video-box'),
    search : $('#botr-search-box'),
    list : $('#botr-video-list'),
    button : $('#botr-upload-button')
  };

  if (botr.widgets.box.length == 0) {
    return;
  }

  botr.widgets.search.click(function () {
    var query = $.trim($(this).val());

    if (query == 'Search videos') {
      $(this).val('');
    }

    $(this).select();
  });

  botr.widgets.search.keydown(function(e) {
    // Ignore enter, but immediately submit
    if(e.keyCode == 13) {
      var query = $.trim($(this).val());
      if (botr.search_timer_id !== null) {
        window.clearTimeout(botr.search_timer_id);
      }
      botr.list(query);
      return false;
    }
  });

  botr.widgets.search.keyup(function(e) {
    if(e.keyCode != 13) {
      var query = $.trim($(this).val());

      if (botr.search_timer_id !== null) {
        window.clearTimeout(botr.search_timer_id);
      }

      botr.search_timer_id = window.setTimeout(function () {
        botr.search_timer_id = null;
        botr.list(query);
      }, botr.search_timeout);
    }
  });

  botr.widgets.search.blur(function() {
    var query = $.trim($(this).val());

    if (query == '') {
      $(this).val('Search videos');
    }
  });

  botr.widgets.button.click(botr.open_upload_window);

  botr.list();
});

})(jQuery);
