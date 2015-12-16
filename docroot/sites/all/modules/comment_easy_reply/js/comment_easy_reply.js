/**
 * @file
 * Contains Comment Easy reply frontend javascript functions.
 */

(function ($) {
  Drupal.behaviors.comment_easy_reply = {
    attach: function (context, settings) {
      var comment_form = $("#comment-form #edit-comment-body");
      var comment_subject_form = $("#comment-form #edit-subject");
      $('.comment-easy-reply-number-link-wrapper', context).once('comment-easy-reply').each(function () {
        var tips = $(this).children('.comment-easy-reply-number-link-tips');
        var link = $(this).children('.comment-easy-reply-number-link');
        if (Drupal.settings.comment_easy_reply != 'undefined'
          && comment_form.length > 0
          && Drupal.settings.comment_easy_reply.user_can_reply
          && Drupal.settings.comment_easy_reply.reply_from_numlink) {
          link.click(function (e) {
            var scrollto = false;
            var ajax_reply_enabled = false;
            var ideal_comments_enabled = false;
            var b2_nice_comments_enabled = false;
            if (Drupal.settings.comment_easy_reply.scrollform_enabled) {
              scrollto = true;
            }
            if (Drupal.settings.comment_easy_reply.ajax_reply_enabled) {
              ajax_reply_enabled = true;
            }
            if (Drupal.settings.comment_easy_reply.ideal_comments_enabled) {
              ideal_comments_enabled = true;
            }
            if (Drupal.settings.comment_easy_reply.b2_nice_comments_enabled) {
              b2_nice_comments_enabled = true;
            }
            var commentnumber = Drupal.comment_easy_reply.get_number_from_class(this, 'comment-easy-reply-linknum-');
            if (commentnumber != 'undefined' && ajax_reply_enabled) {
              var throbber = $(Drupal.theme('comment_easy_reply_throbber'));
              link.append(throbber);
              var nodenumber = Drupal.comment_easy_reply.get_number_from_class(this, 'comment-easy-reply-node-');
              var cidnumber = Drupal.comment_easy_reply.get_number_from_class(this, 'comment-easy-reply-cid-');
              var url = Drupal.comment_easy_reply.get_full_url('ajax/comment-easy-reply/reply/' + nodenumber + '/' + cidnumber);
              if (b2_nice_comments_enabled) {
                $('.b2-nice-comment-textarea').trigger('click');
              }
              $.getJSON(
                url,
                {
                  format: "json"
                },
                function(data) {
                  Drupal.comment_easy_reply.insert_title(data.comment.subject, false);
                  Drupal.comment_easy_reply.insert_content(data.comment.body + ' ');
                  if(scrollto){
                    Drupal.comment_easy_reply.scroll_to_comment_form();
                  }
                  throbber.remove();
                }
               );
            }
            else if (ideal_comments_enabled) {
               $(this).closest('.comment').find('.comment-form').slideToggle("slow");
            }
            e.preventDefault();
          });
        }
        if (Drupal.settings.comment_easy_reply != 'undefined'
          && comment_form.length > 0
          && Drupal.settings.comment_easy_reply.user_can_reply
          && Drupal.settings.comment_easy_reply.replytip_activated) {
          var scrollto = false;
          var ajax_reply_enabled = false;
          var ideal_comments_enabled = false;
          var b2_nice_comments_enabled = false;
          if (Drupal.settings.comment_easy_reply.scrollform_enabled) {
            scrollto = true;
          }
          if (Drupal.settings.comment_easy_reply.ajax_reply_enabled) {
            ajax_reply_enabled = true;
          }
          if (Drupal.settings.comment_easy_reply.ideal_comments_enabled) {
            ideal_comments_enabled = true;
          }
          if (Drupal.settings.comment_easy_reply.b2_nice_comments_enabled) {
            b2_nice_comments_enabled = true;
          }
          var tips_link = tips.children('a');
          tips_link.click(function (e) {
            var commentnumber = Drupal.comment_easy_reply.get_number_from_class($(this).parent(), 'comment-easy-reply-number-link-tips-');
            if (commentnumber != 'undefined' && ajax_reply_enabled) {
              var throbber = $(Drupal.theme('comment_easy_reply_throbber'));
              tips_link.append(throbber);
              var url = tips_link.attr('href').replace('comment/reply', 'ajax/comment-easy-reply/reply');
              if (b2_nice_comments_enabled) {
                $('.b2-nice-comment-textarea').trigger('click');
              }
              $.getJSON(
                url,
                {
                  format: "json"
                },
                function(data) {
                  Drupal.comment_easy_reply.insert_title(data.comment.subject, false);
                  Drupal.comment_easy_reply.insert_content(data.comment.body + ' ');
                  if(scrollto){
                    Drupal.comment_easy_reply.scroll_to_comment_form();
                  }
                  throbber.remove();
                }
              );
              e.preventDefault();
            }
            else if (ideal_comments_enabled) {
              $(this).closest('.comment').find('.comment-form').slideToggle("slow");
              e.preventDefault();
            }
          });
        }
      });
      $('.comment-easy-reply-referrer-link-wrapper', context).once('comment-easy-reply').each(function () {
        $(this).find('.comment-easy-reply-number-link').each(function () {
          if (Drupal.settings.comment_easy_reply != 'undefined'
            && (Drupal.settings.comment_easy_reply.in_preview
            || Drupal.settings.comment_easy_reply.reply_page)) {
            $(this).click(function (e) {
              e.preventDefault();
              return false;
            });
          }
        });
      });
      if (Drupal.settings.comment_easy_reply != 'undefined'
        && Drupal.settings.comment_easy_reply.user_can_reply) {
        $('.comment-reply', context).once('comment-easy-reply').each(function () {
          var scrollto = false;
          var ajax_reply_enabled = false;
          if (Drupal.settings.comment_easy_reply.scrollform_enabled) {
            scrollto = true;
          }
          if (Drupal.settings.comment_easy_reply.ajax_reply_enabled) {
            ajax_reply_enabled = true;
          }
          if($(this).hasClass('views-field-comment-easy-reply-replyto-comment')){
            var link = $(this).find('.comment-easy-reply-views-reply-link');
          }
          else {
            var link = $(this).children('a');
          }
          link.click(function (e) {
            var commentnumber = Drupal.comment_easy_reply.get_number_from_class(this, 'comment-easy-reply-number-');
            if (commentnumber != 'undefined' && ajax_reply_enabled) {
              var throbber = $(Drupal.theme('comment_easy_reply_throbber'));
              var b2_nice_comments_enabled = false;
              if (Drupal.settings.comment_easy_reply.b2_nice_comments_enabled) {
                b2_nice_comments_enabled = true;
              }
              link.append(throbber);
              var url = link.attr('href').replace('comment/reply', 'ajax/comment-easy-reply/reply');
              if (b2_nice_comments_enabled) {
                $('.b2-nice-comment-textarea').trigger('click');
              }
              $.getJSON(
                url,
                {
                  format: "json"
                },
                function(data) {
                  Drupal.comment_easy_reply.insert_title(data.comment.subject, false);
                  Drupal.comment_easy_reply.insert_content(data.comment.body + ' ');
                  if(scrollto){
                    Drupal.comment_easy_reply.scroll_to_comment_form();
                  }
                  throbber.remove();
                }
              );
              e.preventDefault();
            }
          });
        });
      }
      if (Drupal.settings.comment_easy_reply != 'undefined'
        && comment_form.length > 0
        && Drupal.settings.comment_easy_reply.quote_enabled) {
        $('.comment .links .quote, .comment-easy-reply-views-quote-link', context).once('comment-easy-reply').each(function () {
          if ($(this).hasClass('comment-easy-reply-views-quote-link')) {
            var link = $(this);
          } else {
            var link = $(this).children('a');
          }
          var scrollto = false;
          if (Drupal.settings.comment_easy_reply.scrollform_enabled) {
            scrollto = true;
          }
          link.click(function (e) {
            var throbber = $(Drupal.theme('comment_easy_reply_throbber'));
            link.append(throbber);
            var url = link.attr('href').replace('comment/reply', 'ajax/comment-easy-reply/quote');
            $.getJSON(
                url,
                {
                  format: "json"
                },
                function(data) {
                  if (Drupal.settings.comment_easy_reply.ideal_comments_enabled) {
                    var form = $(link).closest('.comment').find('.comment-form');
                    if (form != 'undefined' && form.length > 0) {
                      var textarea = $(form).find('textarea');
                      if (textarea != 'undefined' && textarea.length > 0) {
                        if ($(form).is(":hidden")) {
                          $(form).slideDown("slow");
                        }
                        if (!$(form).hasClass('quoted')) {
                          Drupal.comment_easy_reply.insert_title(data.comment.subject, false);
                          Drupal.comment_easy_reply.insert_content(data.comment.body, textarea.attr('id'));
                          $(form).addClass('quoted');
                          if(scrollto){
                            Drupal.comment_easy_reply.scroll_to_comment_form();
                          }
                        }
                      }
                    }
                    else {
                      Drupal.comment_easy_reply.insert_title(data.comment.subject, false);
                      Drupal.comment_easy_reply.insert_content(data.comment.body);
                      if(scrollto){
                        Drupal.comment_easy_reply.scroll_to_comment_form();
                      }
                    }
                  }
                  else {
                    Drupal.comment_easy_reply.insert_title(data.comment.subject, false);
                    Drupal.comment_easy_reply.insert_content(data.comment.body);
                    if(scrollto){
                      Drupal.comment_easy_reply.scroll_to_comment_form();
                    }
                  }
                  throbber.remove();
                }
            );
            e.preventDefault();
          });
        });
      }
      if (Drupal.settings && Drupal.settings.views && Drupal.settings.views.ajaxViews) {
        $.each(Drupal.settings.views.ajaxViews, function(i, settings) {
          var selector = '.view-dom-id-' + settings.view_dom_id;
          var $comment_easy_reply_view = $(selector);
          $comment_easy_reply_view.find('.comment-easy-reply-number-link').bind('click', function(e) {
            e.preventDefault();
          });
        });
      }
    }
  };
  Drupal.comment_easy_reply = {
    insert_content: function(newvalue, textareaid){
      if (!textareaid) {
        var textareaid = $('#edit-comment-body textarea').attr('id');
      }
      // WYSIWYG support.
      if (Drupal.wysiwyg && Drupal.wysiwyg.activeId) {
        Drupal.wysiwyg.instances[Drupal.wysiwyg.activeId].insert(newvalue)
      }
      // CKeditor module support.
      else if (typeof(CKEDITOR) != 'undefined' && CKEDITOR.instances[textareaid]) {
        CKEDITOR.instances[textareaid].insertHtml(newvalue);
      }
      // No editor found, go for plain textarea.
      else {
        var editor = document.getElementById(textareaid);
        var oldvalue = editor.value;
        if (document.selection) {
          editor.focus();
          sel = document.selection.createRange();
          sel.text = newvalue;
          editor.focus();
        }
        else if (editor.selectionStart || editor.selectionStart == '0') {
          var startPos = editor.selectionStart;
          var endPos = editor.selectionEnd;
          var scrollTop = editor.scrollTop;
          editor.value = editor.value.substring(0, startPos) + newvalue + editor.value.substring(endPos,editor.value.length);
          editor.focus();
          editor.selectionStart = startPos + newvalue.length;
          editor.selectionEnd = startPos + newvalue.length;
          editor.scrollTop = scrollTop;
        } else {
          editor.value += newvalue;
          editor.focus();
        }
      }
    },
    insert_title: function(newvalue, override){
      var subject = $('#comment-form #edit-subject');
      if (subject.length > 0 && subject.val() == '') {
        subject.val(newvalue);
        subject.focus();
      }
    },
    init_tips: function(link,tips){
      var link = $(link);
      var tips = $(tips);
      if ($(window).width() < tips.outerWidth() * 1.5) {
        tips.css('max-width', $(window).width() / 2);
      }
      else {
        tips.css({'min-width':'200px', 'max-width':'340px'});
      }
      var pos_left = link.offset().left + (link.outerWidth() / 2) - (tips.outerWidth() / 2);
      var pos_top = link.offset().top - tips.outerHeight();
      if (pos_left < 0) {
        pos_left = link.offset().left + link.outerWidth() / 2 - 20;
      }
      if (pos_left + tips.outerWidth() > $(window).width()) {
        pos_left = link.offset().left - tips.outerWidth() + link.outerWidth() / 2 + 20;
      }
      else {
        if (pos_top < 0) {
          var pos_top = link.offset().top + link.outerHeight();
        }
        tips.css({'left': pos_left, 'top': pos_top - 10}).offset({'left': pos_left, 'top': pos_top - 10});
      }
    },
    get_full_url: function(path){
      if(Drupal.comment_easy_reply.clean_url == 'undefined' || !Drupal.comment_easy_reply.clean_url){
        var path = '?q=' + path;
      }
      var url = Drupal.settings.basePath + path;      
      return location.protocol + '//' + location.host + url;
    },
    get_number_from_class: function(link, definedclass){
      var classes = $(link).attr('class').split(/\s+/);
      var commentnumber = 0;
      if (definedclass == 'undefined') {
        definedclass = 'comment-easy-reply-number-';
      }
      for (i = 0; i < classes.length; i++) {
        if(classes[i].length > 0 && classes[i].substr(0,definedclass.length) == definedclass){
          commentnumber = classes[i].substr(definedclass.length,classes[i].length);
          break;
        }
      }
      return commentnumber;
    },
    get_add_class: function(link){
      var classes = $(link).attr('class').split(/\s+/);
      var add_class = '';
      definedclass = 'comment-easy-reply-add-';
      for (i = 0; i < classes.length; i++) {
        if(classes[i].length > 0 && classes[i].substr(0,definedclass.length) == definedclass){
          add_class = classes[i].substr(definedclass.length,classes[i].length);
          break;
        }
      }
      return add_class;
    },
    is_native_tooltip_enabled: function(link){
      if(Drupal.settings.comment_easy_reply.tooltip_native_enabled == 'undefined') {
        return true;
      }
      if(Drupal.settings.comment_easy_reply.tooltip_native_enabled == true) {
        return true;
      }
      return false;
    },
    scroll_to_comment_form: function(){
      var comment_form = $("#comment-form #edit-comment-body");
      var comment_subject_form = $("#comment-form #edit-subject");
      var padding = 10;
      if ($('body.toolbar').length > 0){
        padding += parseInt($('body.toolbar').css('padding-top'));
      }
      if (comment_subject_form.length > 0) {
        $('html,body').animate({scrollTop: comment_subject_form.offset().top - padding});
      }
      else {
        $('html,body').animate({scrollTop: comment_form.parent().offset().top});
      }
    }
  };
  Drupal.theme.prototype.comment_easy_reply_throbber = function () {
    return '<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>';
  };
})(jQuery);
